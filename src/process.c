#include "process.h"
#include "state.h"

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

  state->mmu.cartridge = buffer;
  state->mmu.cartridge_size = filesize;

  TraceLog(LOG_DEBUG, "File loaded into memory: %ld bytes read", read);
}

State init() {
  SetTraceLogLevel(LOG_DEBUG);

  State state;
  initialize_state(&state);

  loadFile("dmg_boot.bin", &state);

  TraceLog(LOG_DEBUG, "Initializing window");

  int screenWidth = 800;
  int screenHeight = 800;
  InitWindow(screenWidth, screenHeight, "ThatOne");
  SetTargetFPS(60);

  TraceLog(LOG_DEBUG, "Window initialized");

  return state;
}

byte fetch_byte(State *state) {
  byte b = read_byte_from_mmu(&state->mmu, state->registers.PC);

  state->registers.PC++;
  return b;
}

word fetch_word(State *state) {
  byte low = fetch_byte(state);
  byte high = fetch_byte(state);
  return (high << 8) | low;
}

void LDb(byte *reg, byte value) { *reg = value; }

void LDw(word *reg, word value) { *reg = value; }

void XORn(State *state, byte *reg) {
  byte xor = state->registers.A ^ *reg;
  state->registers.A = xor;
  clear_flags(state);
  if (xor== 0) {
    set_flag(state, Z_FLAG);
  }
}

void DECw(State *state, word *reg) {
  // TODO
  // if ((register.get() & 0x0f) == 0x0f) {
  //     registers.Flags.set(FlagKind.H);
  // }
  *reg -= 1;
  if (*reg == 0) {
    set_flag(state, Z_FLAG);
  } else {
    clear_flag(state, Z_FLAG);
  }
  set_flag(state, N_FLAG);
}

void DECb(State *state, byte *reg) {
  // TODO
  // if ((register.get() & 0x0f) == 0x0f) {
  //     registers.Flags.set(FlagKind.H);
  // }
  *reg -= 1;
  if (*reg == 0) {
    set_flag(state, Z_FLAG);
  } else {
    clear_flag(state, Z_FLAG);
  }
  set_flag(state, N_FLAG);
}

void LDDwn(State *state) {
  write_byte_to_mmu(&state->mmu, state->registers.HL, state->registers.A);
  DECw(state, &state->registers.HL);
}

void BIT(State *state, byte reg, byte bit) {
  if ((reg & (1 << bit)) == 0) {
    set_flag(state, Z_FLAG);
  } else {
    clear_flag(state, Z_FLAG);
  }
  clear_flag(state, N_FLAG);
  set_flag(state, H_FLAG);
}

void JRcc(State *state, byte flag, byte condition, byte offset) {
  if (get_flag(state, flag) == condition) {
    state->registers.PC += offset;
  }
}

void CB(State *state) {
  byte op = fetch_byte(state);

  TraceLog(LOG_DEBUG, "Fetched opcode: 0x%02X - PC: 0x%04X", op,
           state->registers.PC - 1);

  switch (op) {
  case 0x40:
    BIT(state, state->registers.B, 0x0);
    break;
  case 0x41:
    BIT(state, state->registers.C, 0x0);
    break;
  case 0x42:
    BIT(state, state->registers.D, 0x0);
    break;
  case 0x43:
    BIT(state, state->registers.E, 0x0);
    break;
  case 0x44:
    BIT(state, state->registers.H, 0x0);
    break;
  case 0x45:
    BIT(state, state->registers.L, 0x0);
    break;
  case 0x47:
    BIT(state, state->registers.A, 0x0);
    break;
  case 0x48:
    BIT(state, state->registers.B, 0x1);
    break;
  case 0x49:
    BIT(state, state->registers.C, 0x1);
    break;
  case 0x4A:
    BIT(state, state->registers.D, 0x1);
    break;
  case 0x4B:
    BIT(state, state->registers.E, 0x1);
    break;
  case 0x4C:
    BIT(state, state->registers.H, 0x1);
    break;
  case 0x4D:
    BIT(state, state->registers.L, 0x1);
    break;
  case 0x4F:
    BIT(state, state->registers.A, 0x1);
    break;
  case 0x50:
    BIT(state, state->registers.B, 0x2);
    break;
  case 0x51:
    BIT(state, state->registers.C, 0x2);
    break;
  case 0x52:
    BIT(state, state->registers.D, 0x2);
    break;
  case 0x53:
    BIT(state, state->registers.E, 0x2);
    break;
  case 0x54:
    BIT(state, state->registers.H, 0x2);
    break;
  case 0x55:
    BIT(state, state->registers.L, 0x2);
    break;
  case 0x57:
    BIT(state, state->registers.A, 0x2);
    break;
  case 0x58:
    BIT(state, state->registers.B, 0x3);
    break;
  case 0x59:
    BIT(state, state->registers.C, 0x3);
    break;
  case 0x5A:
    BIT(state, state->registers.D, 0x3);
    break;
  case 0x5B:
    BIT(state, state->registers.E, 0x3);
    break;
  case 0x5C:
    BIT(state, state->registers.H, 0x3);
    break;
  case 0x5D:
    BIT(state, state->registers.L, 0x3);
    break;
  case 0x5F:
    BIT(state, state->registers.A, 0x3);
    break;
  case 0x60:
    BIT(state, state->registers.B, 0x4);
    break;
  case 0x61:
    BIT(state, state->registers.C, 0x4);
    break;
  case 0x62:
    BIT(state, state->registers.D, 0x4);
    break;
  case 0x63:
    BIT(state, state->registers.E, 0x4);
    break;
  case 0x64:
    BIT(state, state->registers.H, 0x4);
    break;
  case 0x65:
    BIT(state, state->registers.L, 0x4);
    break;
  case 0x67:
    BIT(state, state->registers.A, 0x4);
    break;
  case 0x68:
    BIT(state, state->registers.B, 0x5);
    break;
  case 0x69:
    BIT(state, state->registers.C, 0x5);
    break;
  case 0x6A:
    BIT(state, state->registers.D, 0x5);
    break;
  case 0x6B:
    BIT(state, state->registers.E, 0x5);
    break;
  case 0x6C:
    BIT(state, state->registers.H, 0x5);
    break;
  case 0x6D:
    BIT(state, state->registers.L, 0x5);
    break;
  case 0x6F:
    BIT(state, state->registers.A, 0x5);
    break;
  case 0x70:
    BIT(state, state->registers.B, 0x6);
    break;
  case 0x71:
    BIT(state, state->registers.C, 0x6);
    break;
  case 0x72:
    BIT(state, state->registers.D, 0x6);
    break;
  case 0x73:
    BIT(state, state->registers.E, 0x6);
    break;
  case 0x74:
    BIT(state, state->registers.H, 0x6);
    break;
  case 0x75:
    BIT(state, state->registers.L, 0x6);
    break;
  case 0x77:
    BIT(state, state->registers.A, 0x6);
    break;
  case 0x78:
    BIT(state, state->registers.B, 0x7);
    break;
  case 0x79:
    BIT(state, state->registers.C, 0x7);
    break;
  case 0x7A:
    BIT(state, state->registers.D, 0x7);
    break;
  case 0x7B:
    BIT(state, state->registers.E, 0x7);
    break;
  case 0x7C:
    BIT(state, state->registers.H, 0x7);
    break;
  case 0x7D:
    BIT(state, state->registers.L, 0x7);
    break;
  case 0x7F:
    BIT(state, state->registers.A, 0x7);
    break;
  default:
    TraceLog(LOG_ERROR, "Unknown opcode: 0x%02X - PC: 0x%04X", op,
             state->registers.PC - 1);
    exit(1);
  }
}

void process(State *state) {
  // Fetch the next opcode.
  byte op = fetch_byte(state);

  TraceLog(LOG_DEBUG, "Fetched opcode: 0x%02X - PC: 0x%04X", op,
           state->registers.PC - 1);

  // Decode the fetched opcode.
  switch (op) {
  case 0x06:
    LDb(&state->registers.B, fetch_byte(state));
    break;
  case 0x0E:
    LDb(&state->registers.C, fetch_byte(state));
    break;
  case 0x16:
    LDb(&state->registers.D, fetch_byte(state));
    break;
  case 0x1E:
    LDb(&state->registers.E, fetch_byte(state));
    break;
  case 0x26:
    LDb(&state->registers.H, fetch_byte(state));
    break;
  case 0x2E:
    LDb(&state->registers.L, fetch_byte(state));
    break;
  case 0x3E:
    LDb(&state->registers.A, fetch_byte(state));
    break;
  case 0x20:
    JRcc(state, Z_FLAG, 0x0, fetch_byte(state));
    break;
  case 0x28:
    JRcc(state, Z_FLAG, 0x1, fetch_byte(state));
    break;
  case 0x30:
    JRcc(state, C_FLAG, 0x0, fetch_byte(state));
    break;
  case 0x38:
    JRcc(state, C_FLAG, 0x1, fetch_byte(state));
    break;
  case 0x01:
    LDw(&state->registers.BC, fetch_word(state));
    break;
  case 0x11:
    LDw(&state->registers.DE, fetch_word(state));
    break;
  case 0x21:
    LDw(&state->registers.HL, fetch_word(state));
    break;
  case 0x31:
    LDw(&state->registers.SP, fetch_word(state));
    break;
  case 0x32:
    LDDwn(state);
    break;
  /* case 0x4F -> LDnA(registers.C); */
  /* case 0x77 -> LDnA(registers.HL); */
  /* case 0x1A -> LDAn(registers.DE); */
  case 0x05:
    DECb(state, &state->registers.B);
    break;
  case 0x0B:
    DECw(state, &state->registers.BC);
    break;
  case 0x1B:
    DECw(state, &state->registers.DE);
    break;
  case 0x2B:
    DECw(state, &state->registers.HL);
    break;
  case 0x3B:
    DECw(state, &state->registers.SP);
    break;
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
  case 0xCB:
    CB(state);
    break;
  /* case (byte) 0xE0 -> LDHnA(); */
  /* case (byte) 0xE2 -> LDcA(); */
  default:
    TraceLog(LOG_ERROR, "Unknown opcode: 0x%02X - PC: 0x%04X", op,
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
