#include <lcom/libxpm.h>
#include <lcom/xpm.h>
#include <lcom/read_xpm.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check_pixels(xpm_map_t map, enum xpm_image_type type,
                         const uint8_t *expected, size_t size) {
  xpm_image_t img;
  uint8_t *pixels = xpm_load(map, type, &img);
  assert(pixels != NULL);
  assert(pixels == img.data && pixels == img.bytes && img.type == type && img.size == size);
  assert(memcmp(pixels, expected, size) == 0);
  assert(xpm_free_data(&img));
  assert(img.data == NULL && img.size == 0);
  assert(!xpm_free_data(&img));
}

static void color_formats(void) {
  static xpm_row_t map[] = {"3 1 3 3", "red c #f00", "blu c #123456", "nil c None", "redblunil"};
  const uint8_t rgb[] = {0, 0, 255, 0x56, 0x34, 0x12, 0x40, 0xb1, 0};
  const uint8_t rgba[] = {0, 0, 255, 0, 0x56, 0x34, 0x12, 0, 0, 0, 0, 255};
  const uint8_t rgb555[] = {0, 0x7c, 0xca, 0x08, 0, 0x80};
  const uint8_t rgb565[] = {0, 0xf8, 0xaa, 0x11, 0x88, 0x05};
  check_pixels(map, XPM_8_8_8, rgb, sizeof(rgb));
  check_pixels(map, XPM_8_8_8_8, rgba, sizeof(rgba));
  check_pixels(map, XPM_1_5_5_5, rgb555, sizeof(rgb555));
  check_pixels(map, XPM_5_6_5, rgb565, sizeof(rgb565));
  assert(xpm_transparency_color(XPM_1_5_5_5) == 0x8000);
  assert(xpm_transparency_color(XPM_5_6_5) == 0x0588);
  assert(xpm_transparency_color(XPM_8_8_8) == 0x00b140);
  assert(xpm_transparency_color(XPM_8_8_8_8) == 0xff000000);
}

static void lcf_hex_suffixes(void) {
  /* Original LCF xpm_load scans three two-digit bytes with sscanf and accepts
   * the suffixes found in preserved student font palettes. */
  static xpm_row_t map[] = {"4 1 4 1", "a c #7E7E7E1", "b c #12345678",
                            "c c #FFFFFF0", "d c #00000012", "abcd"};
  const uint8_t rgba[] = {0x7e,0x7e,0x7e,0,0x56,0x34,0x12,0,
                          255,255,255,0,0,0,0,0};
  const uint8_t rgb565[] = {0xef,0x7b,0xaa,0x11,0xff,0xff,0,0};
  check_pixels(map, XPM_8_8_8_8, rgba, sizeof(rgba));
  check_pixels(map, XPM_5_6_5, rgb565, sizeof(rgb565));
}

static void lcf_five_digit_colors(void) {
  /* Three original %02hhx conversions consume two, two and one digits.
   * The last nibble is a byte value, not an expanded CSS-style channel. */
  static xpm_row_t map[] = {"4 1 4 1", "a c #D9D9D", "b c #12345",
                            "c c #abcde", "d c #00000", "abcd"};
  const uint8_t rgba[] = {0x0d,0xd9,0xd9,0,0x05,0x34,0x12,0,
                          0x0e,0xcd,0xab,0,0,0,0,0};
  const uint8_t rgb565[] = {0xc1,0xde,0xa0,0x11,0x61,0xae,0,0};
  check_pixels(map, XPM_8_8_8_8, rgba, sizeof(rgba));
  check_pixels(map, XPM_5_6_5, rgb565, sizeof(rgb565));
}

static void grayscale_and_names(void) {
  static xpm_row_t map[] = {"3 1 3 1", "r c red", "w c white", "t c None", "rwt"};
  const uint8_t gray[] = {76,76,76,255,255,255,0x40,0xb1,0};
  const uint8_t gray32[] = {76,76,76,0,255,255,255,0,0,0,0,255};
  const uint8_t gray15[] = {0x29,0x25,0xff,0x7f,0,0x80};
  const uint8_t gray16[] = {0x69,0x4a,0xff,0xff,0x88,0x05};
  check_pixels(map, XPM_GRAY_8_8_8, gray, sizeof(gray));
  check_pixels(map, XPM_GRAY_8_8_8_8, gray32, sizeof(gray32));
  check_pixels(map, XPM_GRAY_1_5_5_5, gray15, sizeof(gray15));
  check_pixels(map, XPM_GRAY_5_6_5, gray16, sizeof(gray16));
  static xpm_row_t names[] = {"3 1 3 1", "a s symbolic m white c Light Sky Blue", "b c grey50", "c c #123456789", "abc"};
  const uint8_t colors[] = {250,206,135,127,127,127,0x78,0x45,0x12};
  check_pixels(names, XPM_8_8_8, colors, sizeof(colors));
  static xpm_row_t variants[] = {"2 1 2 4", "pink c LightPink4", "gray c gray90", "pinkgray"};
  const uint8_t shades[] = {101,95,139,229,229,229};
  check_pixels(variants, XPM_8_8_8, shades, sizeof(shades));
}

static void indexed_maps(void) {
  static xpm_row_t map[] = {"4 1 3", "  0", "r 4", "y 14", " ryr"};
  const uint8_t indices[] = {0,4,14,4};
  check_pixels(map, XPM_INDEXED, indices, sizeof(indices));
  static xpm_row_t direct[] = {"1 1 1 1", "a c #000000", "a"};
  xpm_image_t img;
  assert(xpm_load(direct, XPM_INDEXED, &img) == NULL);
  const uint8_t vga[] = {0,0,0,0,0,170,85,255,255,0,0,170};
  check_pixels(map, XPM_8_8_8, vga, sizeof(vga));
}

static void malformed_maps(void) {
  static xpm_row_t maps[][5] = {
    {"0 1 1 1", "a c red", "a"},
    {"65536 1 1 1", "a c red", "a"},
    {"1 1 1 18446744073709551616", "a c red", "a"},
    {"1 1 18446744073709551615 1", "a c red", "a"},
    {"2 1 1 1", "a c red", "a"},
    {"1 1 1 1", "a c red", "aa"},
    {"1 1 1 1", "a c red", "b"},
    {"1 1 1 1", "a c #ggg", "a"},
    {"1 1 1 1", "a c #1234", "a"},
    {"1 1 1 1", "a c #12345g7", "a"},
    {"1 1 1 1", "a c #123456g", "a"},
    {"1 1 1 1", "a c #1234567g", "a"},
    {"1 1 1 1", "a c mystery", "a"},
    {"1 1 2 1", "a c red", "a c blue", "a"},
    {"1 1 1 2", "a", "aa"},
    {"1 1 1 1", NULL, "a"},
    {"1 1 1 1", "a c red", NULL},
    {"1 1 1 -1", "a c red", "a"},
    {"1 1 1 1 garbage", "a c red", "a"},
    {"1 1 1", "a 256", "a"}
  };
  for (size_t i=0; i<sizeof(maps)/sizeof(maps[0]); ++i) {
    xpm_image_t img = {.type=XPM_8_8_8, .width=42, .height=12, .size=1};
    assert(xpm_load(maps[i], XPM_8_8_8, &img) == NULL);
    assert(img.type == INVALID_XPM && img.data == NULL && img.size == 0);
  }
  static xpm_row_t good[] = {"1 1 1 1", "a c red", "a"};
  xpm_image_t img;
  assert(xpm_load(NULL, XPM_8_8_8, &img) == NULL);
  assert(xpm_load(good, INVALID_XPM, &img) == NULL);
  assert(xpm_load(good, XPM_8_8_8, NULL) == NULL);
  assert(!xpm_free_data(NULL));
  assert(xpm_transparency_color(INVALID_XPM) == 0);
}

static void legacy_indexed(void) {
  char *map[] = {"3 1 2", "a 2", "b 7", "aba"};
  int width = -1, height = -1;
  char *pixels = read_xpm(map, &width, &height);
  assert(pixels && width == 3 && height == 1);
  assert(pixels[0] == 2 && pixels[1] == 7 && pixels[2] == 2);
  free(pixels);
  assert(!read_xpm(NULL, &width, &height));
  assert(width == 0 && height == 0);
}

int main(void) {
  legacy_indexed();
  color_formats(); lcf_hex_suffixes(); lcf_five_digit_colors(); grayscale_and_names(); indexed_maps(); malformed_maps();
  puts("compat XPM tests passed");
  return 0;
}
