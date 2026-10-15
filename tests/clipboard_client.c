#include <lcom/lcom.h>
#include <stdio.h>
#include <string.h>

#define CHECK(condition) do { if (!(condition)) { fprintf(stderr, "clipboard check failed at %d\n", __LINE__); return 1; } } while (0)

int main(void) {
  char text[64] = "untouched";
  char maximum[4092];
  char oversized[4093];
  memset(oversized, 'b', sizeof(oversized) - 1);
  oversized[sizeof(oversized) - 1] = '\0';
  memset(maximum, 'a', sizeof(maximum) - 1);
  maximum[sizeof(maximum) - 1] = '\0';
  CHECK(lcom_init() == LCOM_OK);
  CHECK(lcom_clipboard_set_text("Hello, café\nsecond line") == LCOM_OK);
  CHECK(lcom_clipboard_get_text(text, sizeof(text)) == LCOM_OK);
  CHECK(strcmp(text, "Hello, café\nsecond line") == 0);
  strcpy(text, "untouched");
  CHECK(lcom_clipboard_get_text(text, 4) == LCOM_ERR);
  CHECK(strcmp(text, "untouched") == 0);
  CHECK(lcom_clipboard_get_text(NULL, 0) == LCOM_ERR);
  CHECK(lcom_clipboard_set_text(NULL) == LCOM_ERR);
  CHECK(lcom_clipboard_set_text(maximum) == LCOM_OK);
  CHECK(lcom_clipboard_set_text(oversized) == LCOM_ERR);
  CHECK(lcom_clipboard_get_text(maximum, sizeof(maximum)) == LCOM_OK);
  for (size_t i = 0; i < sizeof(maximum) - 1; i++) CHECK(maximum[i] == 'a');
  CHECK(maximum[sizeof(maximum) - 1] == '\0');
  CHECK(lcom_clipboard_set_text("") == LCOM_OK);
  CHECK(lcom_clipboard_get_text(text, 1) == LCOM_OK && text[0] == '\0');
  lcom_exit();
  return 0;
}
