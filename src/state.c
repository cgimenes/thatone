#include "state.h"
#include <raylib.h>

void initialize_state(State *state) {
  TraceLog(LOG_DEBUG, "Initializing state");

  initialize_registers(state);
  initialize_mmu(state);

  TraceLog(LOG_DEBUG, "State initialized");
}
