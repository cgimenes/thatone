#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <raylib.h>

#include <dlfcn.h>

#include "process.h"

init_t init = NULL;
process_t process = NULL;
const char *libprocess_file_name = "libprocess.so";
const char *libprocess_file_path = "build/libprocess.so";
time_t libprocess_file_mod_time = 0;
void *libprocess = NULL;

bool reload_lib() {
  if (libprocess != NULL)
    dlclose(libprocess);

  libprocess = dlopen(libprocess_file_name, RTLD_NOW);
  if (libprocess == NULL) {
    fprintf(stderr, "ERROR: could not load %s: %s", libprocess_file_name,
            dlerror());
    return false;
  }

  init = dlsym(libprocess, "init");
  if (init == NULL) {
    fprintf(stderr, "ERROR: could not find init symbol in %s: %s",
            libprocess_file_name, dlerror());
    return 1;
  }

  process = dlsym(libprocess, "process");
  if (process == NULL) {
    fprintf(stderr, "ERROR: could not find process symbol in %s: %s",
            libprocess_file_name, dlerror());
    return false;
  }
  libprocess_file_mod_time = GetFileModTime(libprocess_file_path);

  return true;
}

bool hot_reload() {
  time_t current_libprocess_file_mod_time =
      GetFileModTime(libprocess_file_path);
  if (current_libprocess_file_mod_time != libprocess_file_mod_time) {
    // mod time is changed quicker than the compilation, so we have an empty
    // file for an while. I may change it for a "trigger file" in the future.
    // Creating a specific file after compilation finishes and checking its mod
    // time here
    TraceLog(LOG_INFO, "Reloading libprocess");
    sleep(1);
    return reload_lib();
  }
  return true;
}

int main() {
  if (!reload_lib())
    return 1;

  State state = init();

  while (!WindowShouldClose()) {
    if (!hot_reload())
      return 1;

    process(&state);
  }

  CloseWindow();
  return 0;
}
