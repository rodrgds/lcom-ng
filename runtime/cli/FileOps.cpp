#include "FileOps.hpp"

#include <cstdlib>
#include <fstream>
#if !defined(_WIN32)
#include <unistd.h>
#endif

namespace lcom::cli {

std::filesystem::path resolveExecutablePath(const std::filesystem::path &program,
                                            std::error_code &error) {
  namespace fs = std::filesystem;
  error.clear();
  if (program.has_parent_path()) return fs::canonical(program, error);
  const char *environment = std::getenv("PATH");
  if (environment != nullptr && !program.empty()) {
    const std::string search(environment);
#if defined(_WIN32)
    constexpr char separator = ';';
#else
    constexpr char separator = ':';
#endif
    size_t start = 0;
    for (;;) {
      size_t end = search.find(separator, start);
      fs::path directory = search.substr(start, end == std::string::npos ? end : end - start);
      fs::path candidate = (directory.empty() ? fs::path(".") : directory) / program;
      fs::path resolved = fs::canonical(candidate, error);
      if (!error && fs::is_regular_file(resolved, error)) {
#if !defined(_WIN32)
        if (access(resolved.c_str(), X_OK) == 0)
#endif
          return resolved;
      }
      if (end == std::string::npos) break;
      start = end + 1;
    }
  }
  error = std::make_error_code(std::errc::no_such_file_or_directory);
  return {};
}


bool copyIfExists(const std::filesystem::path &from,
                  const std::filesystem::path &to,
                  std::string &error) {
  std::error_code ec;
  if (!std::filesystem::exists(from, ec)) return true;
  std::filesystem::create_directories(to.parent_path(), ec);
  if (ec) {
    error = ec.message();
    return false;
  }
  std::filesystem::copy_file(from, to, std::filesystem::copy_options::overwrite_existing, ec);
  if (ec) {
    error = ec.message();
    return false;
  }
  return true;
}

bool copyTreeIfExists(const std::filesystem::path &from,
                      const std::filesystem::path &to,
                      std::string &error) {
  std::error_code ec;
  if (!std::filesystem::exists(from, ec)) return true;
  std::filesystem::create_directories(to, ec);
  if (ec) {
    error = ec.message();
    return false;
  }
  std::filesystem::copy(from, to,
                        std::filesystem::copy_options::recursive |
                            std::filesystem::copy_options::overwrite_existing,
                        ec);
  if (ec) {
    error = ec.message();
    return false;
  }
  return true;
}

bool makeExecutable(const std::filesystem::path &path, std::string &error) {
  std::error_code ec;
  std::filesystem::permissions(path,
                               std::filesystem::perms::owner_exec |
                                   std::filesystem::perms::group_exec |
                                   std::filesystem::perms::others_exec,
                               std::filesystem::perm_options::add,
                               ec);
  if (ec) {
    error = "could not make " + path.string() + " executable: " + ec.message();
    return false;
  }
  return true;
}

std::string shellQuote(const std::filesystem::path &path) {
  std::string s = path.string();
  std::string out = "'";
  for (char c : s) {
    if (c == '\'') {
      out += "'\\''";
    } else {
      out.push_back(c);
    }
  }
  out.push_back('\'');
  return out;
}

bool writeTextFile(const std::filesystem::path &path,
                   const std::string &text,
                   bool force,
                   std::string &error) {
  namespace fs = std::filesystem;
  std::error_code ec;
  if (!force && fs::exists(path, ec)) return true;
  fs::create_directories(path.parent_path(), ec);
  if (ec) {
    error = ec.message();
    return false;
  }
  std::ofstream out(path);
  if (!out.is_open()) {
    error = "could not open " + path.string();
    return false;
  }
  out << text;
  out.flush();
  if (!out) {
    error = "could not write " + path.string();
    return false;
  }
  return true;
}

} // namespace lcom::cli
