#include "render.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stddef.h>

State init() {
  SetTraceLogLevel(LOG_DEBUG);

  TraceLog(LOG_DEBUG, "Initializing");

  State state;

  int screenWidth = 800;
  int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "ThatOne");

  SetTargetFPS(60);

  TraceLog(LOG_DEBUG, "Initialized");

  return state;
}

void render(State *state) {
  // update
  // --

  // draw
  BeginDrawing();
  {
    ClearBackground(BLACK);
    DrawRectangle(50, 50, 50, 50, RED);
  }
  EndDrawing();
}
