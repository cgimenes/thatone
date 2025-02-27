#ifndef RENDER_H_
#define RENDER_H_

#include <raylib.h>

#define MAX_FILEPATH_SIZE 2048

typedef struct State {
} State;

typedef void (*render_t)(State *state);

typedef State (*init_t)();

#endif // RENDER_H_
