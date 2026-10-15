/* Legacy score updates must commit to the same files that stdio opened. */
#include <lcom/paths.h>
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
