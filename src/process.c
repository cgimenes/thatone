#include "process.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void loadFile(const char *filename, State *state) {
  FILE *fp;
  long filesize;
  byte *buffer;

  TraceLog(LOG_DEBUG, "Opening file: %s", filename);
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "cannot open input file\n");
    exit(1);
  }
  TraceLog(LOG_DEBUG, "File opened");

  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);

  TraceLog(LOG_DEBUG, "File size: %ld bytes", filesize);

  TraceLog(LOG_DEBUG, "Allocating memory");
  if (!(buffer = malloc(filesize))) {
    fprintf(stderr, "Failed to allocate memory\n");
    exit(1);
  }

  TraceLog(LOG_DEBUG, "Reading file into memory");
  rewind(fp);
  long read = fread(buffer, 1, filesize, fp);
  fclose(fp);

  state->instruction_count = filesize;
  state->instructions = buffer;

  TraceLog(LOG_DEBUG, "File loaded into memory: %ld bytes read", read);
}

void initializeRegisters(State *state) {
  TraceLog(LOG_DEBUG, "Initializing registers");

  Registers registers;
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

  state->registers = registers;

  TraceLog(LOG_DEBUG, "Registers initialized");
}

State init() {
  SetTraceLogLevel(LOG_DEBUG);

  TraceLog(LOG_DEBUG, "Initializing state");

  State state;
  initializeRegisters(&state);
  loadFile("dmg_boot.bin", &state);

  TraceLog(LOG_DEBUG, "Initializing window");

  int screenWidth = 800;
  int screenHeight = 800;
  InitWindow(screenWidth, screenHeight, "ThatOne");
  SetTargetFPS(60);

  TraceLog(LOG_DEBUG, "Window initialized");

  return state;
}

byte fetch(State *state) {
  if (state->registers.PC >= state->instruction_count) {
    fprintf(stderr, "PC out of bounds: 0x%04X\n", state->registers.PC);
    exit(1);
  }

  byte b = state->instructions[state->registers.PC];

  TraceLog(LOG_DEBUG, "Fetched opcode: 0x%02X - PC: 0x%04X", b,
           state->registers.PC);

  state->registers.PC++;
  return b;
}

word fetch16(State *state) {
  byte low = fetch(state);
  byte high = fetch(state);
  return (high << 8) | low;
}

void LD8(State *state, byte *reg) {
  byte value = fetch(state);
  *reg = value;
}

void LD16(State *state, word *reg) {
  word value = fetch16(state);
  *reg = value;
}

void XORn(State *state, byte *reg) {
  byte xor = state->registers.A ^ *reg;
  state->registers.A = xor;
  state->registers.F = (xor== 0) ? 0x80 : 0x00; // Set Z flag if zero
}

void process(State *state) {
  // Fetch the next opcode.
  byte b = fetch(state);

  // Decode the fetched opcode.
  switch (b) {
  case 0x06:
    LD8(state, &state->registers.B);
    break;
  case 0x0E:
    LD8(state, &state->registers.C);
    break;
  case 0x16:
    LD8(state, &state->registers.D);
    break;
  case 0x1E:
    LD8(state, &state->registers.E);
    break;
  case 0x26:
    LD8(state, &state->registers.H);
    break;
  case 0x2E:
    LD8(state, &state->registers.L);
    break;
  case 0x3E:
    LD8(state, &state->registers.A);
    break;
    /* case 0x20 -> JRcc(FlagKind.Z, (byte) 0x0, fetch()); */
    /* case 0x28 -> JRcc(FlagKind.Z, (byte) 0x1, fetch()); */
    /* case 0x30 -> JRcc(FlagKind.C, (byte) 0x0, fetch()); */
    /* case 0x38 -> JRcc(FlagKind.C, (byte) 0x1, fetch()); */
  case 0x01:
    LD16(state, &state->registers.BC);
    break;
  case 0x11:
    LD16(state, &state->registers.DE);
    break;
  case 0x21:
    LD16(state, &state->registers.HL);
    break;
  case 0x31:
    LD16(state, &state->registers.SP);
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
  case 0xA8:
    XORn(state, &state->registers.B);
    break;
  case 0xA9:
    XORn(state, &state->registers.C);
    break;
  case 0xAA:
    XORn(state, &state->registers.D);
    break;
  case 0xAB:
    XORn(state, &state->registers.E);
    break;
  case 0xAC:
    XORn(state, &state->registers.H);
    break;
  case 0xAD:
    XORn(state, &state->registers.L);
    break;
  case 0xAF:
    XORn(state, &state->registers.A);
    break;
  /* case (byte) 0xCB -> CB(); */
  /* case (byte) 0xE0 -> LDHnA(); */
  /* case (byte) 0xE2 -> LDcA(); */
  default:
    TraceLog(LOG_ERROR, "Unknown opcode: 0x%02X - PC: 0x%04X", b,
             state->registers.PC - 1);
    exit(1);
  }

  // draw
  BeginDrawing();
  {
    ClearBackground(BLACK);
    DrawRectangle(50, 50, 50, 50, RED);
  }
  EndDrawing();
}
