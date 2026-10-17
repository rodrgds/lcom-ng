/* Legacy score updates must commit to the same files that stdio opened. */
#include <lcom/paths.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "path check failed at line %d: %s (errno=%d)\n", __LINE__, #c, errno); return 1; } } while (0)
static int write_text(const char *path, const char *text) {
  FILE *file = fopen(path, "w");
  if (!file) return -1;
  int result = fputs(text, file) < 0;
  return fclose(file) || result ? -1 : 0;
}
static int has_text(const char *path, const char *expected) {
  FILE *file = fopen(path, "r");
  if (!file) return 0;
  char text[128] = {0};
  size_t count = fread(text, 1, sizeof(text) - 1, file);
  int error = ferror(file);
  fclose(file);
  return !error && count == strlen(expected) && !strcmp(text, expected);
}
int main(void) {
  char temporary[] = "/tmp/lcom-paths-XXXXXX";
  char *root = mkdtemp(temporary);
  CHECK(root != NULL && chdir(root) == 0);
  CHECK(mkdir("records", 0700) == 0);
  CHECK(setenv("LCOM_COMPAT_PROJECT_ROOT", root, 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "1", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_FROM", "/home/lcom/labs/save-test", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_TO", "records", 1) == 0);
  const char *saved = "/home/lcom/labs/save-test/record.txt";
  const char *pending = "/home/lcom/labs/save-test/pending.txt";
  CHECK(write_text("records/record.txt", "previous\n") == 0);
  CHECK(has_text(saved, "previous\n"));
  /* Original bitmap constructors probe the same aliased file before opening. */
  CHECK(access(saved, F_OK) == 0);
  CHECK(access(saved, R_OK | W_OK) == 0);
  CHECK(access(saved, X_OK) == -1 && errno == EACCES);
  CHECK(access("//home//lcom//labs//save-test//record.txt", F_OK) == 0);
  CHECK(access("/home/lcom/labs/save-test-sibling/missing.txt", F_OK) == -1 && errno == ENOENT);
  /* POSIX bitmap readers use open+pread, while score writers pass O_CREAT's mode. */
  int fd = open(saved, O_RDONLY);
  CHECK(fd >= 0);
  char partial[4] = {0};
  CHECK(pread(fd, partial, 3, 1) == 3 && !strcmp(partial, "rev"));
  CHECK(lseek(fd, 0, SEEK_CUR) == 0 && close(fd) == 0);
  mode_t old_mask = umask(0);
  fd = open("/home/lcom/labs/save-test/fd-pending.txt", O_WRONLY | O_CREAT | O_EXCL, 0640);
  CHECK(fd >= 0);
  struct stat created;
  CHECK(fstat(fd, &created) == 0 && (created.st_mode & 0777) == 0640);
  CHECK(write(fd, "descriptor save\n", 16) == 16 && close(fd) == 0);
  umask(old_mask);
  CHECK(open("/home/lcom/labs/save-test/fd-pending.txt", O_WRONLY | O_CREAT | O_EXCL, 0600) == -1 && errno == EEXIST);
  CHECK(rename("/home/lcom/labs/save-test/fd-pending.txt", "/home/lcom/labs/save-test/fd-saved.txt") == 0);
  CHECK(has_text("records/fd-saved.txt", "descriptor save\n"));
  CHECK(remove("/home/lcom/labs/save-test/fd-saved.txt") == 0);
  CHECK(open("/home/lcom/labs/save-test-sibling/missing.txt", O_RDONLY) == -1 && errno == ENOENT);
  /* Original Sudoku paths have repeated separators in every component. */
  CHECK(has_text("//home//lcom//labs//save-test//record.txt", "previous\n"));
  CHECK(write_text("///home///lcom/labs//save-test///double.txt", "separator\n") == 0);
  CHECK(has_text("records/double.txt", "separator\n"));
  CHECK(rename("//home//lcom//labs//save-test//double.txt",
               "//home/lcom/labs//save-test//renamed.txt") == 0);
  CHECK(has_text("records/renamed.txt", "separator\n"));
  CHECK(remove("//home/lcom//labs/save-test///renamed.txt") == 0);
  CHECK(fopen("//home/lcom/labs/save-test-sibling/record.txt", "r") == NULL);

  CHECK(write_text(pending, "winner\nprevious\n") == 0);
  /* The original Memory Quest save_winner uses this exact commit sequence. */
  CHECK(remove(saved) == 0);
  CHECK(rename(pending, saved) == 0);
  CHECK(has_text("records/record.txt", "winner\nprevious\n"));
  CHECK(access("records/pending.txt", F_OK) != 0 && errno == ENOENT);

  /* Default Minix rebasing also commits two missing absolute names. */
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "0", 1) == 0);
  CHECK(write_text("records/pending.txt", "default\n") == 0);
  CHECK(rename("/home/lcom/labs/records/pending.txt", "/home/lcom/labs/records/record.txt") == 0);
  CHECK(has_text("records/record.txt", "default\n"));
  CHECK(remove("/home/lcom/labs/records/record.txt") == 0);

  /* A relative source can be moved to an explicitly aliased destination. */
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "1", 1) == 0);
  CHECK(write_text("native-source.txt", "mixed\n") == 0);
  CHECK(rename("native-source.txt", saved) == 0);
  CHECK(has_text("records/record.txt", "mixed\n"));

  /* Existing real paths take precedence over matching configured aliases. */
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_FROM", root, 1) == 0);
  CHECK(write_text("original.txt", "real\n") == 0);
  CHECK(write_text("destination.txt", "old real\n") == 0);
  CHECK(write_text("records/original.txt", "mapped source\n") == 0);
  CHECK(write_text("records/destination.txt", "mapped destination\n") == 0);
  char original[512], destination[512];
  snprintf(original, sizeof(original), "%s/original.txt", root);
  snprintf(destination, sizeof(destination), "%s/destination.txt", root);
  CHECK(rename(original, destination) == 0);
  CHECK(has_text(destination, "real\n"));
  CHECK(has_text("records/original.txt", "mapped source\n"));
  CHECK(has_text("records/destination.txt", "mapped destination\n"));
  CHECK(remove(destination) == 0);
  CHECK(has_text("records/destination.txt", "mapped destination\n"));
  CHECK(rename("missing-relative", "other-relative") == -1 && errno == ENOENT);
  CHECK(remove("missing-relative") == -1 && errno == ENOENT);

  /* A dangling destination symlink is an existing name, not an alias miss. */
  CHECK(symlink("missing-target", "link.txt") == 0);
  struct stat link_info;
  CHECK(lstat("link.txt", &link_info) == 0 && S_ISLNK(link_info.st_mode));
  CHECK(write_text("records/link.txt", "mapped link\n") == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_1_FROM", "/legacy/save", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_1_TO", "records", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "2", 1) == 0);
  CHECK(write_text("records/pending.txt", "linked result\n") == 0);
  snprintf(destination, sizeof(destination), "%s/link.txt", root);
  CHECK(rename("/legacy/save/pending.txt", destination) == 0);
  CHECK(has_text("link.txt", "linked result\n"));
  CHECK(has_text("records/link.txt", "mapped link\n"));

  /* Explicit relative prefixes use the same read/write/commit resolver. */
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "1", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_FROM", "home/lcom", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_TO", "records", 1) == 0);
  CHECK(write_text("records/resource.txt", "asset\n") == 0);
  CHECK(has_text("home/lcom/resource.txt", "asset\n"));
  CHECK(access("home/lcom/resource.txt", R_OK) == 0);
  CHECK(access("home//lcom///resource.txt", F_OK) == 0);
  CHECK(access("home/lcom2/resource.txt", F_OK) == -1 && errno == ENOENT);
  fd = open("home/lcom/resource.txt", O_RDONLY);
  CHECK(fd >= 0 && read(fd, partial, 3) == 3 && !memcmp(partial, "ass", 3) && close(fd) == 0);
  CHECK(has_text("home//lcom///resource.txt", "asset\n"));
  CHECK(write_text("home/lcom/pending.txt", "relative save\n") == 0);
  CHECK(rename("home/lcom/pending.txt", "home/lcom/saved.txt") == 0);
  CHECK(has_text("records/saved.txt", "relative save\n"));
  CHECK(remove("home/lcom/saved.txt") == 0);
  CHECK(fopen("home/lcom2/resource.txt", "r") == NULL && errno == ENOENT);
  CHECK(fopen("other/resource.txt", "r") == NULL && errno == ENOENT);
  CHECK(mkdir("home", 0700) == 0 && mkdir("home/lcom", 0700) == 0);
  CHECK(write_text("home/lcom/resource.txt", "real relative\n") == 0);
  CHECK(has_text("home/lcom/resource.txt", "real relative\n"));
  CHECK(rename("records/resource.txt", "records/hidden.txt") == 0);
  CHECK(access("home/lcom/resource.txt", R_OK) == 0);
  CHECK(rename("records/hidden.txt", "records/resource.txt") == 0);
  fd = open("home/lcom/resource.txt", O_RDONLY);
  CHECK(fd >= 0 && read(fd, partial, 3) == 3 && !memcmp(partial, "rea", 3) && close(fd) == 0);
  CHECK(has_text("home///lcom//resource.txt", "real relative\n"));
  CHECK(has_text("records/resource.txt", "asset\n"));
  CHECK(remove("home/lcom/resource.txt") == 0);
  CHECK(has_text("records/resource.txt", "asset\n"));
  CHECK(rmdir("home/lcom") == 0 && rmdir("home") == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "2", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_1_FROM", "home/lcom/nested", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_1_TO", "records/deep", 1) == 0);
  CHECK(mkdir("records/deep", 0700) == 0);
  CHECK(write_text("records/deep/resource.txt", "longest\n") == 0);
  CHECK(has_text("home/lcom/nested/resource.txt", "longest\n"));
  CHECK(has_text("home//lcom///nested//resource.txt", "longest\n"));
  CHECK(remove("home/lcom/nested/resource.txt") == 0);
  CHECK(rmdir("records/deep") == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "0", 1) == 0);
  CHECK(fopen("home/lcom/resource.txt", "r") == NULL && errno == ENOENT);
  CHECK(remove("records/resource.txt") == 0);

  /* Android's /tmp can be searchable but not writable by the app UID.
   * An explicit alias supplies a private writable location without changing
   * successful normal paths or granting implicit/default rebasing on EACCES. */
  CHECK(mkdir("denied", 0700) == 0);
  CHECK(write_text("denied/read.txt", "real accessible\n") == 0);
  CHECK(write_text("records/read.txt", "private mapped\n") == 0);
  char denied[512], denied_read[512], denied_pending[512], denied_saved[512];
  snprintf(denied, sizeof(denied), "%s/denied", root);
  snprintf(denied_read, sizeof(denied_read), "%s/denied/read.txt", root);
  snprintf(denied_pending, sizeof(denied_pending), "%s/denied/pending.txt", root);
  snprintf(denied_saved, sizeof(denied_saved), "%s/denied/saved.txt", root);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "1", 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_FROM", denied, 1) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_0_TO", "records", 1) == 0);
  CHECK(has_text(denied_read, "real accessible\n"));
  CHECK(chmod("denied", 0000) == 0);
  CHECK(has_text(denied_read, "private mapped\n"));
  CHECK(access(denied_read, R_OK) == 0);
  fd = open(denied_read, O_RDONLY);
  CHECK(fd >= 0 && pread(fd, partial, 3, 0) == 3 && !memcmp(partial, "pri", 3) && close(fd) == 0);
  CHECK(write_text(denied_pending, "private write\n") == 0);
  CHECK(has_text("records/pending.txt", "private write\n"));
  CHECK(rename(denied_pending, denied_saved) == 0);
  CHECK(has_text(denied_saved, "private write\n"));
  CHECK(remove(denied_saved) == 0);
  CHECK(write_text("mixed-real.txt", "mixed private commit\n") == 0);
  CHECK(rename("mixed-real.txt", denied_saved) == 0);
  CHECK(has_text("records/saved.txt", "mixed private commit\n"));
  CHECK(remove(denied_saved) == 0);
  CHECK(fopen(denied_saved, "r") == NULL && errno == EACCES);
  CHECK(open(denied_saved, O_RDONLY) == -1 && errno == EACCES);
  CHECK(remove(denied_saved) == -1 && errno == EACCES);
  CHECK(rename(denied_pending, denied_saved) == -1 && errno == EACCES);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "0", 1) == 0);
  CHECK(fopen(denied_read, "r") == NULL && errno == EACCES);
  CHECK(open(denied_read, O_RDONLY) == -1 && errno == EACCES);
  CHECK(access(denied_read, F_OK) == -1 && errno == EACCES);
  CHECK(remove(denied_read) == -1 && errno == EACCES);
  CHECK(rename(denied_read, denied_saved) == -1 && errno == EACCES);
  /* A searchable, non-writable directory matches the measured Android /tmp
   * permissions: metadata lookup succeeds, but creating a child is denied. */
  CHECK(chmod("denied", 0111) == 0);
  CHECK(setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "1", 1) == 0);
  CHECK(has_text(denied_read, "real accessible\n"));
  CHECK(write_text(denied_pending, "searchable parent\n") == 0);
  CHECK(rename(denied_pending, denied_saved) == 0);
  CHECK(has_text("records/saved.txt", "searchable parent\n"));
  CHECK(remove(denied_saved) == 0);
  CHECK(chmod("denied", 0700) == 0);
  CHECK(has_text("denied/read.txt", "real accessible\n"));
  CHECK(remove("denied/read.txt") == 0 && rmdir("denied") == 0);
  CHECK(remove("records/read.txt") == 0);

  CHECK(remove("link.txt") == 0);
  CHECK(remove("records/record.txt") == 0);
  CHECK(remove("records/original.txt") == 0);
  CHECK(remove("records/destination.txt") == 0);
  CHECK(remove("records/link.txt") == 0);
  CHECK(rmdir("records") == 0);
  CHECK(chdir("/") == 0 && rmdir(root) == 0);
  puts("Compatibility save-file commit paths passed");
  return 0;
}
