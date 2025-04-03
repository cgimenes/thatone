#ifndef PROCESS_H_
#define PROCESS_H_

#include "state.h"
#include <raylib.h>

typedef void (*process_t)(State *state);

typedef State (*init_t)();

#endif // PROCESS_H_
