#ifndef MACHINE_LAB_CLI_FILE_OPS_HPP
#define MACHINE_LAB_CLI_FILE_OPS_HPP

#include <filesystem>
#include <string>

namespace lcom::cli {

// Resolve the executable selected by a direct invocation or PATH, following
// symlinks so installed resources are found beside the actual binary.
std::filesystem::path resolveExecutablePath(const std::filesystem::path &program,
                                            std::error_code &error);

bool copyIfExists(const std::filesystem::path &from,
                  const std::filesystem::path &to,
                  std::string &error);
bool copyTreeIfExists(const std::filesystem::path &from,
                      const std::filesystem::path &to,
                      std::string &error);
bool makeExecutable(const std::filesystem::path &path, std::string &error);
std::string shellQuote(const std::filesystem::path &path);
bool writeTextFile(const std::filesystem::path &path,
                   const std::string &text,
                   bool force,
                   std::string &error);

} // namespace lcom::cli

#endif
