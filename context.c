// header contents start--------------------------------------------------------
// #include "environment.h"
#include <stdlib.h>

#ifndef BABYC_ENV_HEADER
#define BABYC_ENV_HEADER

typedef struct VarWithOffset {
    char *var_name;
    int offset;
} VarWithOffset;

typedef struct Environment {
    size_t size;
    VarWithOffset *items;
} Environment;

Environment *environment_new();

void environment_set_offset(Environment *env, char *var_name, int offset);

int environment_get_offset(Environment *env, char *var_name);

void environment_free(Environment *env);

#endif
// #include "context.h"
typedef struct Context {
    int stack_offset;
    Environment *env;
    int label_count;
} Context;

void new_scope(Context *ctx);

Context *new_context();

void context_free(Context *ctx);
// header contents end----------------------------------------------------------

// TODO: this is duplicated with assembly.c.
static const int WORD_SIZE = 4;

void new_scope(Context *ctx) {
    // Each function needs a fresh set of local variables (we
    // don't support globals yet).
    environment_free(ctx->env);
    ctx->env = environment_new();

    ctx->stack_offset = -1 * WORD_SIZE;
}

Context *new_context() {
    Context *ctx = malloc(sizeof(Context));
    ctx->stack_offset = 0;
    ctx->env = NULL;
    ctx->label_count = 0;

    return ctx;
}

void context_free(Context *ctx) {
    environment_free(ctx->env);
    free(ctx);
}
