#include "state.h"
#include <raylib.h>

void initialize_registers(State *state) {
  TraceLog(LOG_DEBUG, "Initializing registers");

  Registers registers;
  registers.A = 0x00;
  registers.B = 0x00;
  registers.D = 0x00;
  registers.H = 0x00;
  registers.F = 0x00;
  registers.C = 0x00;
  registers.E = 0x00;
  registers.L = 0x00;
  registers.SP = 0x0000;
  registers.PC = 0x0000;

  state->registers = registers;

  TraceLog(LOG_DEBUG, "Registers initialized");
}

byte get_flag(State *state, byte flag) { return state->registers.F & flag; }

void set_flag(State *state, byte flag) { state->registers.F |= flag; }

void clear_flag(State *state, byte flag) { state->registers.F &= ~flag; }

void clear_flags(State *state) { state->registers.F = 0x00; }
