// Common SDL app entry point. The exporter supplies student code and GameConfig.
#include "Host.hpp"
#include "SdlHost.hpp"
#include "GameConfig.hpp"
#include <lcom/lcom.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <filesystem>
#include <cstdio>
#include <cstdlib>
#include <vector>
#ifdef __ANDROID__
#include <android/log.h>
#include <unistd.h>
#include <thread>
#endif

extern "C" int lcom_student_main(int argc, char **argv);

int main(int argc, char **argv) {
#ifdef __ANDROID__
  // Android discards ordinary stdout. Preserve student diagnostics in logcat.
  int descriptors[2];
  if (pipe(descriptors) == 0) {
    dup2(descriptors[1], STDOUT_FILENO); dup2(descriptors[1], STDERR_FILENO);
    close(descriptors[1]);
    std::thread([fd=descriptors[0]] {
      FILE *input = fdopen(fd,"r");
      char line[2048];
      if (input) { while (fgets(line,sizeof(line),input)) __android_log_write(ANDROID_LOG_INFO,"lcom-ng",line); fclose(input); }
    }).detach();
    setvbuf(stdout,nullptr,_IOLBF,0); setvbuf(stderr,nullptr,_IOLBF,0);
  }
#endif
  try {
    lcom::embedded::configureControls(game::controls);
    std::filesystem::path root = argc > 1 ? argv[1] : ".";
    root = std::filesystem::absolute(root);
    for (const auto &[name,value] : game::environment) setenv(name,value,1);
    setenv("LCOM_COMPAT_PROJECT_ROOT",root.c_str(),1);
    std::filesystem::create_directories(root / game::workingDirectory);
    std::filesystem::current_path(root / game::workingDirectory);
    std::vector<char *> arguments;
    for (const char *argument : game::arguments) arguments.push_back(const_cast<char *>(argument));
    arguments.push_back(nullptr);
    int result = lcom_student_main(static_cast<int>(arguments.size()-1),arguments.data());
    std::printf("lcom-ng: game exited with status %d\n", result);
    lcom_exit();
    return result;
  } catch (const lcom::embedded::QuitRequested &) {
    lcom_exit();
    return 0;
  } catch (const std::exception &error) {
    std::fprintf(stderr,"lcom-ng: %s\n",error.what());
    lcom_exit();
    return 1;
  }
}
