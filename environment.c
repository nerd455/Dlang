#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

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
// header contents end----------------------------------------------------------

/* A data structure that maps variable names (i.e. strings) to offsets
 * (integers) in the current stack frame.
 */

Environment *environment_new() {
    Environment *env = malloc(sizeof(Environment));
    env->size = 0;
    env->items = NULL;

    return env;
}

void environment_set_offset(Environment *env, char *var_name, int offset) {
    env->size++;
    env->items = realloc(env->items, env->size * sizeof(VarWithOffset));

    VarWithOffset *vwo = &env->items[env->size - 1];
    // TODO: use a copy of the string instead
    vwo->var_name = var_name;
    vwo->offset = offset;
}

/* Return the offset from %ebp of variable VAR_NAME.
 */
int environment_get_offset(Environment *env, char *var_name) {
    VarWithOffset vwo;
    for (size_t i = 0; i < env->size; i++) {
        vwo = env->items[i];

        if (strcmp(vwo.var_name, var_name) == 0) {
            return vwo.offset;
        }
    }

    warnx("Could not find %s in environment", var_name);
    return -1;
}

void environment_free(Environment *env) {
    if (env != NULL) {
        free(env->items);
        free(env);
    }
}
