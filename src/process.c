#include "process.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stddef.h>
#include <stdio.h>

State init() {
  SetTraceLogLevel(LOG_DEBUG);

  TraceLog(LOG_DEBUG, "Initializing");

  State state;
  Registers registers;
  state.registers = registers;
  registers.A = 0x00;
  registers.B = 0x00;
  registers.D = 0x00;
  registers.H = 0x00;
  registers.F = 0x0000;
  registers.C = 0x0000;
  registers.E = 0x0000;
  registers.L = 0x0000;
  registers.SP = 0x0000;
  registers.PC = 0x0000;

  int screenWidth = 800;
  int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "ThatOne");

  SetTargetFPS(60);

  TraceLog(LOG_DEBUG, "Initialized");

  return state;
}

void LD8(byte *reg, byte value) { *reg = value; }

void LD16(word *reg, word value) { *reg = value; }

byte fetch() {
  // Simulate fetching an opcode from memory.
  // In a real implementation, this would read from the memory mapped to the
  // CPU.
  return 0x00; // Placeholder
}

word fetch16() {
  // Simulate fetching a 16-bit value from memory.
  // In a real implementation, this would read from the memory mapped to the
  // CPU.
  return 0x0000; // Placeholder
}

void process(State *state) {
  // Fetch the next opcode.
  byte b = fetch();
  // Decode the fetched opcode.
  switch (b) {
  case 0x06:
    LD8(&state->registers.B, fetch());
    break;
  case 0x0E:
    LD8(&state->registers.C, fetch());
    break;
  case 0x16:
    LD8(&state->registers.D, fetch());
    break;
  case 0x1E:
    LD8(&state->registers.E, fetch());
    break;
  case 0x26:
    LD8(&state->registers.H, fetch());
    break;
  case 0x2E:
    LD8(&state->registers.L, fetch());
    break;
  case 0x3E:
    LD8(&state->registers.A, fetch());
    break;
    /* case 0x20 -> JRcc(FlagKind.Z, (byte) 0x0, fetch()); */
    /* case 0x28 -> JRcc(FlagKind.Z, (byte) 0x1, fetch()); */
    /* case 0x30 -> JRcc(FlagKind.C, (byte) 0x0, fetch()); */
    /* case 0x38 -> JRcc(FlagKind.C, (byte) 0x1, fetch()); */
  case 0x01:
    LD16(&state->registers.BC, fetch16());
    break;
  case 0x11:
    LD16(&state->registers.DE, fetch16());
    break;
  case 0x21:
    LD16(&state->registers.HL, fetch16());
    break;
  case 0x31:
    LD16(&state->registers.SP, fetch16());
    break;
  /* case 0x32 -> LDD16n(); */
  /* case 0x4F -> LDnA(registers.C); */
  /* case 0x77 -> LDnA(registers.HL); */
  /* case 0x1A -> LDAn(registers.DE); */
  /* case 0x05 -> DEC(registers.B); */
  /* case 0x0B -> DEC(registers.BC); */
  /* case 0x1B -> DEC(registers.DE); */
  /* case 0x2B -> DEC(registers.HL); */
  /* case 0x3B -> DEC(registers.SP); */
  /* case 0x0C -> INC(registers.C); */
  /* case 0x17 -> RLA(); */
  /* case (byte) 0xC1 -> POP(registers.BC); */
  /* case (byte) 0xC5 -> PUSH(registers.BC); */
  /* case (byte) 0xCD -> CALL(); */
  /* case (byte) 0xA8 -> XORn(registers.B); */
  /* case (byte) 0xA9 -> XORn(registers.C); */
  /* case (byte) 0xAA -> XORn(registers.D); */
  /* case (byte) 0xAB -> XORn(registers.E); */
  /* case (byte) 0xAC -> XORn(registers.H); */
  /* case (byte) 0xAD -> XORn(registers.L); */
  /* case (byte) 0xAF -> XORn(registers.A); */
  /* case (byte) 0xCB -> CB(); */
  /* case (byte) 0xE0 -> LDHnA(); */
  /* case (byte) 0xE2 -> LDcA(); */
  default:
    printf("Unknown opcode: 0x%02X\n", b);
    printf("PC: 0x%04X\n", state->registers.PC);
  }

  // draw
  BeginDrawing();
  {
    ClearBackground(BLACK);
    DrawRectangle(50, 50, 50, 50, RED);
  }
  EndDrawing();
}
