#ifndef LCOM_CHILD_STATUS_HPP
#define LCOM_CHILD_STATUS_HPP

#include <csignal>
#include <cerrno>
#include <ostream>
#include <string>
#include <sys/wait.h>

namespace lcom {

inline void stopChildForHostTimeout(int &pid, bool &running) {
  if (!running || pid <= 0) return;
  kill(pid, SIGKILL);
  while (waitpid(pid, nullptr, 0) < 0 && errno == EINTR) {}
  running = false;
  pid = -1;
}

inline int reportChildStatus(std::ostream &out, int status, const std::string &prefix = "") {
  if (WIFEXITED(status)) {
    out << prefix << "Program exited with status " << WEXITSTATUS(status) << '\n';
    return WEXITSTATUS(status);
  }
  if (!WIFSIGNALED(status)) return 1;
  int signal = WTERMSIG(status);
  const char *name = nullptr;
  switch (signal) {
  case SIGSEGV: name = "SIGSEGV, invalid memory access"; break;
  case SIGBUS: name = "SIGBUS, invalid memory access or alignment"; break;
  case SIGABRT: name = "SIGABRT, program aborted"; break;
  case SIGFPE: name = "SIGFPE, arithmetic fault"; break;
  case SIGILL: name = "SIGILL, illegal instruction"; break;
  case SIGTRAP: name = "SIGTRAP, trap instruction"; break;
  default: break;
  }
  out << prefix << "Program terminated by signal " << signal;
  if (name) out << " (" << name << ')';
  out << '\n';
  if (name) {
    out << prefix << "For student C projects, rebuild with: lcom-ng compat build <project> --debug\n"
        << prefix << "Use --sanitize undefined for additional source diagnostics, and --trace <file> for device activity.\n";
  }
  return 128 + signal;
}

} // namespace lcom

#endif
