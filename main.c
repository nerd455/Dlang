#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <err.h>

#include "build/y.tab.h"
#include "syntax.h"

// header include start---------------------------------------------------------
// #include<stack.h>
#include <stdbool.h>

#ifndef BABYC_STACK_HEADER
#define BABYC_STACK_HEADER

typedef struct Stack {
    int size;
    void **content;
} Stack;

Stack *stack_new();

void stack_free(Stack *stack);
void stack_push(Stack *stack, void *item);
void *stack_pop(Stack *stack);
void *stack_peek(Stack *stack);
bool stack_empty(Stack *stack);

#endif
// #include<assembly.h>
#include <stdio.h>
#include "syntax.h"

#ifndef BABYC_ASSEMBLY_HEADER
#define BABYC_ASSEMBLY_HEADER

void emit_header(FILE *out, char *name);

void emit_insn(FILE *out, char *insn);

void write_header(FILE *out);

void write_footer(FILE *out);

void write_syntax(FILE *out, Syntax *syntax);

void write_assembly(Syntax *syntax);

#endif
// header include end-----------------------------------------------------------

void print_help() {
    printf("\nDLang is a very basic C compiler.\n");
    printf("To compile and to execute a file:\n");
    printf("    $ dlang foo.c\n");
    printf("    $ ./link\n");
}

extern Stack *syntax_stack;

extern int yyparse(void);
extern FILE *yyin;

typedef enum {
    MACRO_EXPAND,
    PARSE,
    EMIT_ASM,
} stage_t;

int main(int argc, char *argv[]) {
    ++argv, --argc; /* Skip over program name. */

    stage_t terminate_at = EMIT_ASM;

    char *file_name;
    if (argc == 1) {
        file_name = argv[0];
    } else {
        print_help();
        return 1;
    }

    int result;

    // TODO: create a proper temporary file from the preprocessor.
    char command[1024] = {0};
    snprintf(command, 1024, "gcc -E %s > .expanded.c", file_name);
    result = system(command);
    if (result != 0) {
        puts("Macro expansion failed!");
        return result;
    }

    yyin = fopen(".expanded.c", "r");

    if (terminate_at == MACRO_EXPAND) {
        int c;
        while ((c = getc(yyin)) != EOF) {
            putchar(c);
        }
        goto cleanup_file;
    }

    if (yyin == NULL) {
        // TODO: work out what the error was.
        // TODO: Unit test this.
        printf("Could not open file: '%s'\n", file_name);
        result = 2;
        goto cleanup_file;
    }

    syntax_stack = stack_new();

    result = yyparse();
    if (result != 0) {
        printf("\n");
        goto cleanup_syntax;
    }

    Syntax *complete_syntax = stack_pop(syntax_stack);
    if (syntax_stack->size > 0) {
        warnx("Did not consume the whole syntax stack during parsing! Remaining:");

        while(syntax_stack->size > 0) {
            fprintf(stderr, "%s", syntax_type_name(stack_pop(syntax_stack)));
        }
    }

    if (terminate_at == PARSE) {
        print_syntax(complete_syntax);
    } else {
        write_assembly(complete_syntax);
        syntax_free(complete_syntax);
        printf("Written out.s.\n");
    }

cleanup_syntax:
    /* TODO: if we exit early from syntactically invalid code, we will
       need to free multiple Syntax structs on this stack.
     */
    stack_free(syntax_stack);
cleanup_file:
    if (yyin != NULL) {
        fclose(yyin);
    }

    unlink(".expanded.c");

    return result;
}
