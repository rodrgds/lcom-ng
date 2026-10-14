#include <lcom/libxpm.h>

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

struct color {
  const char *symbol;
  uint32_t rgb;
  uint8_t index;
  bool indexed;
  bool transparent;
};

struct named_color {
  const char *name;
  uint32_t rgb;
};

/* X11 spellings, case and spaces are ignored when matching names. */
static const struct named_color named_colors[] = {
  {"x11gray", 0xbebebe}, {"x11grey", 0xbebebe},
  {"webgray", 0x808080}, {"webgrey", 0x808080},
  {"x11green", 0x00ff00}, {"webgreen", 0x008000},
  {"x11maroon", 0xb03060}, {"webmaroon", 0x800000},
  {"x11purple", 0xa020f0}, {"webpurple", 0x800080},
  {"rebeccapurple", 0x663399},
  {"aliceblue", 0xf0f8ff}, {"antiquewhite", 0xfaebd7},
  {"aqua", 0x00ffff}, {"aquamarine", 0x7fffd4}, {"azure", 0xf0ffff},
  {"beige", 0xf5f5dc}, {"bisque", 0xffe4c4}, {"black", 0x000000},
  {"blanchedalmond", 0xffebcd}, {"blue", 0x0000ff},
  {"blueviolet", 0x8a2be2}, {"brown", 0xa52a2a}, {"burlywood", 0xdeb887},
  {"cadetblue", 0x5f9ea0}, {"chartreuse", 0x7fff00},
  {"chocolate", 0xd2691e}, {"coral", 0xff7f50},
  {"cornflowerblue", 0x6495ed}, {"cornsilk", 0xfff8dc},
  {"crimson", 0xdc143c}, {"cyan", 0x00ffff},
  {"darkblue", 0x00008b}, {"darkcyan", 0x008b8b},
  {"darkgoldenrod", 0xb8860b}, {"darkgray", 0xa9a9a9},
  {"darkgreen", 0x006400}, {"darkgrey", 0xa9a9a9},
  {"darkkhaki", 0xbdb76b}, {"darkmagenta", 0x8b008b},
  {"darkolivegreen", 0x556b2f}, {"darkorange", 0xff8c00},
  {"darkorchid", 0x9932cc}, {"darkred", 0x8b0000},
  {"darksalmon", 0xe9967a}, {"darkseagreen", 0x8fbc8f},
  {"darkslateblue", 0x483d8b}, {"darkslategray", 0x2f4f4f},
  {"darkslategrey", 0x2f4f4f}, {"darkturquoise", 0x00ced1},
  {"darkviolet", 0x9400d3}, {"deeppink", 0xff1493},
  {"deepskyblue", 0x00bfff}, {"dimgray", 0x696969}, {"dimgrey", 0x696969},
  {"dodgerblue", 0x1e90ff}, {"firebrick", 0xb22222},
  {"floralwhite", 0xfffaf0}, {"forestgreen", 0x228b22},
  {"fuchsia", 0xff00ff}, {"gainsboro", 0xdcdcdc},
  {"ghostwhite", 0xf8f8ff}, {"gold", 0xffd700}, {"goldenrod", 0xdaa520},
  {"gray", 0xbebebe}, {"green", 0x00ff00}, {"greenyellow", 0xadff2f},
  {"grey", 0xbebebe}, {"honeydew", 0xf0fff0}, {"hotpink", 0xff69b4},
  {"indianred", 0xcd5c5c}, {"indigo", 0x4b0082}, {"ivory", 0xfffff0},
  {"khaki", 0xf0e68c}, {"lavender", 0xe6e6fa},
  {"lavenderblush", 0xfff0f5}, {"lawngreen", 0x7cfc00},
  {"lemonchiffon", 0xfffacd}, {"lightblue", 0xadd8e6},
  {"lightcoral", 0xf08080}, {"lightcyan", 0xe0ffff},
  {"lightgoldenrod", 0xeedd82}, {"lightgoldenrodyellow", 0xfafad2},
  {"lightgray", 0xd3d3d3}, {"lightgreen", 0x90ee90},
  {"lightgrey", 0xd3d3d3}, {"lightpink", 0xffb6c1},
  {"lightsalmon", 0xffa07a}, {"lightseagreen", 0x20b2aa},
  {"lightskyblue", 0x87cefa}, {"lightslateblue", 0x8470ff},
  {"lightslategray", 0x778899}, {"lightslategrey", 0x778899},
  {"lightsteelblue", 0xb0c4de}, {"lightyellow", 0xffffe0},
  {"lime", 0x00ff00}, {"limegreen", 0x32cd32}, {"linen", 0xfaf0e6},
  {"magenta", 0xff00ff}, {"maroon", 0xb03060},
  {"mediumaquamarine", 0x66cdaa}, {"mediumblue", 0x0000cd},
  {"mediumorchid", 0xba55d3}, {"mediumpurple", 0x9370db},
  {"mediumseagreen", 0x3cb371}, {"mediumslateblue", 0x7b68ee},
  {"mediumspringgreen", 0x00fa9a}, {"mediumturquoise", 0x48d1cc},
  {"mediumvioletred", 0xc71585}, {"midnightblue", 0x191970},
  {"mintcream", 0xf5fffa}, {"mistyrose", 0xffe4e1}, {"moccasin", 0xffe4b5},
  {"navajowhite", 0xffdead}, {"navy", 0x000080}, {"navyblue", 0x000080},
  {"oldlace", 0xfdf5e6}, {"olive", 0x808000}, {"olivedrab", 0x6b8e23},
  {"orange", 0xffa500}, {"orangered", 0xff4500}, {"orchid", 0xda70d6},
  {"palegoldenrod", 0xeee8aa}, {"palegreen", 0x98fb98},
  {"paleturquoise", 0xafeeee}, {"palevioletred", 0xdb7093},
  {"papayawhip", 0xffefd5}, {"peachpuff", 0xffdab9}, {"peru", 0xcd853f},
  {"pink", 0xffc0cb}, {"plum", 0xdda0dd}, {"powderblue", 0xb0e0e6},
  {"purple", 0xa020f0}, {"red", 0xff0000}, {"rosybrown", 0xbc8f8f},
  {"royalblue", 0x4169e1}, {"saddlebrown", 0x8b4513}, {"salmon", 0xfa8072},
  {"sandybrown", 0xf4a460}, {"seagreen", 0x2e8b57}, {"seashell", 0xfff5ee},
  {"sienna", 0xa0522d}, {"silver", 0xc0c0c0}, {"skyblue", 0x87ceeb},
  {"slateblue", 0x6a5acd}, {"slategray", 0x708090}, {"slategrey", 0x708090},
  {"snow", 0xfffafa}, {"springgreen", 0x00ff7f}, {"steelblue", 0x4682b4},
  {"tan", 0xd2b48c}, {"teal", 0x008080}, {"thistle", 0xd8bfd8},
  {"tomato", 0xff6347}, {"turquoise", 0x40e0d0}, {"violet", 0xee82ee},
  {"violetred", 0xd02090}, {"wheat", 0xf5deb3}, {"white", 0xffffff},
  {"whitesmoke", 0xf5f5f5}, {"yellow", 0xffff00}, {"yellowgreen", 0x9acd32}
};

/* Numbered X11 variants from xorg rgb.txt, mirrored by NetBSD xsrc:
 * https://github.com/NetBSD/xsrc/blob/trunk/external/mit/rgb/dist/rgb.txt */
static const struct {
  const char *name;
  uint32_t shades[4];
} numbered_colors[] = {
  {"antiquewhite", {0xffefdb, 0xeedfcc, 0xcdc0b0, 0x8b8378}},
  {"aquamarine", {0x7fffd4, 0x76eec6, 0x66cdaa, 0x458b74}},
  {"azure", {0xf0ffff, 0xe0eeee, 0xc1cdcd, 0x838b8b}},
  {"bisque", {0xffe4c4, 0xeed5b7, 0xcdb79e, 0x8b7d6b}},
  {"blue", {0x0000ff, 0x0000ee, 0x0000cd, 0x00008b}},
  {"brown", {0xff4040, 0xee3b3b, 0xcd3333, 0x8b2323}},
  {"burlywood", {0xffd39b, 0xeec591, 0xcdaa7d, 0x8b7355}},
  {"cadetblue", {0x98f5ff, 0x8ee5ee, 0x7ac5cd, 0x53868b}},
  {"chartreuse", {0x7fff00, 0x76ee00, 0x66cd00, 0x458b00}},
  {"chocolate", {0xff7f24, 0xee7621, 0xcd661d, 0x8b4513}},
  {"coral", {0xff7256, 0xee6a50, 0xcd5b45, 0x8b3e2f}},
  {"cornsilk", {0xfff8dc, 0xeee8cd, 0xcdc8b1, 0x8b8878}},
  {"cyan", {0x00ffff, 0x00eeee, 0x00cdcd, 0x008b8b}},
  {"darkgoldenrod", {0xffb90f, 0xeead0e, 0xcd950c, 0x8b6508}},
  {"darkolivegreen", {0xcaff70, 0xbcee68, 0xa2cd5a, 0x6e8b3d}},
  {"darkorange", {0xff7f00, 0xee7600, 0xcd6600, 0x8b4500}},
  {"darkorchid", {0xbf3eff, 0xb23aee, 0x9a32cd, 0x68228b}},
  {"darkseagreen", {0xc1ffc1, 0xb4eeb4, 0x9bcd9b, 0x698b69}},
  {"darkslategray", {0x97ffff, 0x8deeee, 0x79cdcd, 0x528b8b}},
  {"deeppink", {0xff1493, 0xee1289, 0xcd1076, 0x8b0a50}},
  {"deepskyblue", {0x00bfff, 0x00b2ee, 0x009acd, 0x00688b}},
  {"dodgerblue", {0x1e90ff, 0x1c86ee, 0x1874cd, 0x104e8b}},
  {"firebrick", {0xff3030, 0xee2c2c, 0xcd2626, 0x8b1a1a}},
  {"gold", {0xffd700, 0xeec900, 0xcdad00, 0x8b7500}},
  {"goldenrod", {0xffc125, 0xeeb422, 0xcd9b1d, 0x8b6914}},
  {"green", {0x00ff00, 0x00ee00, 0x00cd00, 0x008b00}},
  {"honeydew", {0xf0fff0, 0xe0eee0, 0xc1cdc1, 0x838b83}},
  {"hotpink", {0xff6eb4, 0xee6aa7, 0xcd6090, 0x8b3a62}},
  {"indianred", {0xff6a6a, 0xee6363, 0xcd5555, 0x8b3a3a}},
  {"ivory", {0xfffff0, 0xeeeee0, 0xcdcdc1, 0x8b8b83}},
  {"khaki", {0xfff68f, 0xeee685, 0xcdc673, 0x8b864e}},
  {"lavenderblush", {0xfff0f5, 0xeee0e5, 0xcdc1c5, 0x8b8386}},
  {"lemonchiffon", {0xfffacd, 0xeee9bf, 0xcdc9a5, 0x8b8970}},
  {"lightblue", {0xbfefff, 0xb2dfee, 0x9ac0cd, 0x68838b}},
  {"lightcyan", {0xe0ffff, 0xd1eeee, 0xb4cdcd, 0x7a8b8b}},
  {"lightgoldenrod", {0xffec8b, 0xeedc82, 0xcdbe70, 0x8b814c}},
  {"lightpink", {0xffaeb9, 0xeea2ad, 0xcd8c95, 0x8b5f65}},
  {"lightsalmon", {0xffa07a, 0xee9572, 0xcd8162, 0x8b5742}},
  {"lightskyblue", {0xb0e2ff, 0xa4d3ee, 0x8db6cd, 0x607b8b}},
  {"lightsteelblue", {0xcae1ff, 0xbcd2ee, 0xa2b5cd, 0x6e7b8b}},
  {"lightyellow", {0xffffe0, 0xeeeed1, 0xcdcdb4, 0x8b8b7a}},
  {"magenta", {0xff00ff, 0xee00ee, 0xcd00cd, 0x8b008b}},
  {"maroon", {0xff34b3, 0xee30a7, 0xcd2990, 0x8b1c62}},
  {"mediumorchid", {0xe066ff, 0xd15fee, 0xb452cd, 0x7a378b}},
  {"mediumpurple", {0xab82ff, 0x9f79ee, 0x8968cd, 0x5d478b}},
  {"mistyrose", {0xffe4e1, 0xeed5d2, 0xcdb7b5, 0x8b7d7b}},
  {"navajowhite", {0xffdead, 0xeecfa1, 0xcdb38b, 0x8b795e}},
  {"olivedrab", {0xc0ff3e, 0xb3ee3a, 0x9acd32, 0x698b22}},
  {"orange", {0xffa500, 0xee9a00, 0xcd8500, 0x8b5a00}},
  {"orangered", {0xff4500, 0xee4000, 0xcd3700, 0x8b2500}},
  {"orchid", {0xff83fa, 0xee7ae9, 0xcd69c9, 0x8b4789}},
  {"palegreen", {0x9aff9a, 0x90ee90, 0x7ccd7c, 0x548b54}},
  {"paleturquoise", {0xbbffff, 0xaeeeee, 0x96cdcd, 0x668b8b}},
  {"palevioletred", {0xff82ab, 0xee799f, 0xcd6889, 0x8b475d}},
  {"peachpuff", {0xffdab9, 0xeecbad, 0xcdaf95, 0x8b7765}},
  {"pink", {0xffb5c5, 0xeea9b8, 0xcd919e, 0x8b636c}},
  {"plum", {0xffbbff, 0xeeaeee, 0xcd96cd, 0x8b668b}},
  {"purple", {0x9b30ff, 0x912cee, 0x7d26cd, 0x551a8b}},
  {"red", {0xff0000, 0xee0000, 0xcd0000, 0x8b0000}},
  {"rosybrown", {0xffc1c1, 0xeeb4b4, 0xcd9b9b, 0x8b6969}},
  {"royalblue", {0x4876ff, 0x436eee, 0x3a5fcd, 0x27408b}},
  {"salmon", {0xff8c69, 0xee8262, 0xcd7054, 0x8b4c39}},
  {"seagreen", {0x54ff9f, 0x4eee94, 0x43cd80, 0x2e8b57}},
  {"seashell", {0xfff5ee, 0xeee5de, 0xcdc5bf, 0x8b8682}},
  {"sienna", {0xff8247, 0xee7942, 0xcd6839, 0x8b4726}},
  {"skyblue", {0x87ceff, 0x7ec0ee, 0x6ca6cd, 0x4a708b}},
  {"slateblue", {0x836fff, 0x7a67ee, 0x6959cd, 0x473c8b}},
  {"slategray", {0xc6e2ff, 0xb9d3ee, 0x9fb6cd, 0x6c7b8b}},
  {"snow", {0xfffafa, 0xeee9e9, 0xcdc9c9, 0x8b8989}},
  {"springgreen", {0x00ff7f, 0x00ee76, 0x00cd66, 0x008b45}},
  {"steelblue", {0x63b8ff, 0x5cacee, 0x4f94cd, 0x36648b}},
  {"tan", {0xffa54f, 0xee9a49, 0xcd853f, 0x8b5a2b}},
  {"thistle", {0xffe1ff, 0xeed2ee, 0xcdb5cd, 0x8b7b8b}},
  {"tomato", {0xff6347, 0xee5c42, 0xcd4f39, 0x8b3626}},
  {"turquoise", {0x00f5ff, 0x00e5ee, 0x00c5cd, 0x00868b}},
  {"violetred", {0xff3e96, 0xee3a8c, 0xcd3278, 0x8b2252}},
  {"wheat", {0xffe7ba, 0xeed8ae, 0xcdba96, 0x8b7e66}},
  {"yellow", {0xffff00, 0xeeee00, 0xcdcd00, 0x8b8b00}},
};

static const char *skip_space(const char *p) {
  while (isspace((unsigned char)*p)) ++p;
  return p;
}

static bool read_number(const char **p, size_t *value) {
  const char *start = skip_space(*p);
  if (!isdigit((unsigned char)*start)) return false;
  errno = 0;
  char *end;
  uintmax_t number = strtoumax(start, &end, 10);
  if (errno == ERANGE || number > SIZE_MAX || (*end && !isspace((unsigned char)*end))) return false;
  *p = end;
  *value = (size_t)number;
  return true;
}

static bool read_header(const char *row, size_t *width, size_t *height,
                        size_t *count, size_t *cpp) {
  if (!row || !read_number(&row, width) || !read_number(&row, height) ||
      !read_number(&row, count)) return false;
  *cpp = 1;
  row = skip_space(row);
  if (*row && !read_number(&row, cpp)) return false;
  row = skip_space(row);
  /* Standard XPM may include a pair of hotspot coordinates and XPMEXT. */
  if (isdigit((unsigned char)*row)) {
    size_t x, y;
    if (!read_number(&row, &x) || !read_number(&row, &y)) return false;
    row = skip_space(row);
  }
  if (*row && strcmp(row, "XPMEXT") != 0) return false;
  return *width > 0 && *height > 0 && *width <= UINT16_MAX &&
         *height <= UINT16_MAX && *count > 0 && *cpp > 0;
}

static bool parse_rgb(const char *start, const char *end, struct color *color) {
  while (end > start && isspace((unsigned char)end[-1])) --end;
  if (start == end) return false;
  if (*start == '#') {
    size_t length = (size_t)(end - start - 1);
    if (length != 3 && length != 6 && length != 9 && length != 12) return false;
    unsigned digits = (unsigned)(length / 3);
    uint32_t rgb = 0;
    ++start;
    for (unsigned component = 0; component < 3; ++component) {
      unsigned value = 0;
      for (unsigned i = 0; i < digits; ++i) {
        unsigned char c = (unsigned char)*start++;
        if (!isxdigit(c)) return false;
        value = value * 16 + (isdigit(c) ? c - '0' : (unsigned)(tolower(c) - 'a' + 10));
      }
      value = digits == 1 ? value * 17 : value >> (4 * digits - 8);
      rgb = (rgb << 8) | value;
    }
    color->rgb = rgb;
    return true;
  }
  char name[64];
  size_t n = 0;
  for (const char *p = start; p < end; ++p) {
    if (isspace((unsigned char)*p)) continue;
    if (n == sizeof(name) - 1) return false;
    name[n++] = (char)tolower((unsigned char)*p);
  }
  name[n] = '\0';
  if (strcmp(name, "none") == 0) {
    color->transparent = true;
    return true;
  }
  if (strncmp(name, "gray", 4) == 0 || strncmp(name, "grey", 4) == 0) {
    const char *p = name + 4;
    size_t percentage;
    if (*p && read_number(&p, &percentage) && !*p && percentage <= 100) {
      unsigned gray = (unsigned)((percentage * 255 + 50) / 100);
      /* Historical X11 rgb.txt rounds these two entries down. */
      if (percentage == 50 || percentage == 90) --gray;
      color->rgb = gray * 0x010101;
      return true;
    }
  }
  for (size_t i = 0; i < sizeof(named_colors) / sizeof(named_colors[0]); ++i) {
    if (strcmp(name, named_colors[i].name) == 0) {
      color->rgb = named_colors[i].rgb;
      return true;
    }
  }
  if (n > 1 && name[n - 1] >= '1' && name[n - 1] <= '4') {
    unsigned shade = (unsigned)(name[n - 1] - '1');
    name[n - 1] = '\0';
    for (size_t i = 0; i < sizeof(numbered_colors) / sizeof(numbered_colors[0]); ++i) {
      if (strcmp(name, numbered_colors[i].name) == 0) {
        color->rgb = numbered_colors[i].shades[shade];
        return true;
      }
    }
    name[n - 1] = (char)('1' + shade);
  }
  /* Numeric palette indices in both legacy maps and XPM color fields. */
  const char *p = name;
  size_t index;
  if (read_number(&p, &index) && !*p && index <= UINT8_MAX) {
    color->indexed = true;
    color->index = (uint8_t)index;
    return true;
  }
  return false;
}

static int color_key(const char *start, const char *end) {
  size_t length = (size_t)(end - start);
  if (length == 1) {
    switch (*start) {
      case 's': return 1;
      case 'm': return 2;
      case 'g': return 4;
      case 'c': return 5;
      default: return 0;
    }
  }
  return length == 2 && start[0] == 'g' && start[1] == '4' ? 3 : 0;
}

static bool read_color(const char *row, size_t cpp, struct color *color) {
  if (!row || strlen(row) < cpp) return false;
  color->symbol = row;
  const char *p = skip_space(row + cpp);
  if (isdigit((unsigned char)*p)) return parse_rgb(p, p + strlen(p), color);
  int best = 0;
  const char *chosen = NULL, *chosen_end = NULL;
  while (*p) {
    const char *key = p;
    while (*p && !isspace((unsigned char)*p)) ++p;
    int rank = color_key(key, p);
    if (!rank) return false;
    const char *value = skip_space(p);
    p = value;
    while (*p) {
      const char *token = p;
      while (*p && !isspace((unsigned char)*p)) ++p;
      if (color_key(token, p)) { p = token; break; }
      p = skip_space(p);
    }
    if (rank > best && rank != 1) {
      best = rank;
      chosen = value;
      chosen_end = p;
    }
  }
  return chosen && parse_rgb(chosen, chosen_end, color);
}

static size_t symbol_hash(const char *symbol, size_t cpp) {
  /* XPM generators enumerate consecutive symbols. A base-33 polynomial packs
   * three printable bytes into too few buckets for large color tables. FNV-1a
   * plus an avalanche distributes those symbols across the full table. */
  uint64_t hash = UINT64_C(14695981039346656037);
  for (size_t i = 0; i < cpp; ++i) {
    hash ^= (unsigned char)symbol[i];
    hash *= UINT64_C(1099511628211);
  }
  hash ^= hash >> 33;
  hash *= UINT64_C(0xff51afd7ed558ccd);
  hash ^= hash >> 33;
  hash *= UINT64_C(0xc4ceb9fe1a85ec53);
  hash ^= hash >> 33;
  return (size_t)hash;
}

static struct color *color_slot(struct color *table, size_t slots,
                                const char *symbol, size_t cpp) {
  size_t slot = symbol_hash(symbol, cpp) % slots;
  while (table[slot].symbol && memcmp(table[slot].symbol, symbol, cpp))
    slot = (slot + 1) % slots;
  return &table[slot];
}

static unsigned bytes_per_pixel(enum xpm_image_type type) {
  switch (type) {
    case XPM_INDEXED: return 1;
    case XPM_1_5_5_5: case XPM_5_6_5:
    case XPM_GRAY_1_5_5_5: case XPM_GRAY_5_6_5: return 2;
    case XPM_8_8_8: case XPM_GRAY_8_8_8: return 3;
    case XPM_8_8_8_8: case XPM_GRAY_8_8_8_8: return 4;
    default: return 0;
  }
}

uint32_t xpm_transparency_color(enum xpm_image_type type) {
  switch (type) {
    case XPM_1_5_5_5: case XPM_GRAY_1_5_5_5: return TRANSPARENCY_COLOR_1_5_5_5;
    case XPM_5_6_5: case XPM_GRAY_5_6_5: return CHROMA_KEY_GREEN_565;
    case XPM_8_8_8: case XPM_GRAY_8_8_8: return CHROMA_KEY_GREEN_888;
    case XPM_8_8_8_8: case XPM_GRAY_8_8_8_8: return TRANSPARENCY_COLOR_8_8_8_8;
    default: return 0;
  }
}

static bool pixel_value(const struct color *color, enum xpm_image_type type,
                         uint32_t *value) {
  static const uint32_t vga[] = {
    0x000000, 0x0000aa, 0x00aa00, 0x00aaaa,
    0xaa0000, 0xaa00aa, 0xaa5500, 0xaaaaaa,
    0x555555, 0x5555ff, 0x55ff55, 0x55ffff,
    0xff5555, 0xff55ff, 0xffff55, 0xffffff
  };
  if (type == XPM_INDEXED) {
    if (!color->indexed) return false;
    *value = color->index;
    return true;
  }
  if (color->transparent) {
    *value = xpm_transparency_color(type);
    return true;
  }
  uint32_t rgb = color->rgb;
  if (color->indexed) {
    if (color->index >= sizeof(vga) / sizeof(vga[0])) return false;
    rgb = vga[color->index];
  }
  unsigned r = (rgb >> 16) & 255, g = (rgb >> 8) & 255, b = rgb & 255;
  if (type >= XPM_GRAY_1_5_5_5 && type <= XPM_GRAY_8_8_8_8) {
    r = g = b = (299 * r + 587 * g + 114 * b) / 1000;
  }
  switch (type) {
    case XPM_1_5_5_5: case XPM_GRAY_1_5_5_5:
      *value = ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3); break;
    case XPM_5_6_5: case XPM_GRAY_5_6_5:
      *value = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3); break;
    default: *value = (r << 16) | (g << 8) | b; break;
  }
  return true;
}

uint8_t *xpm_load(xpm_map_t map, enum xpm_image_type type, xpm_image_t *img) {
  if (!img) return NULL;
  *img = (xpm_image_t){.type = INVALID_XPM};
  unsigned bpp = bytes_per_pixel(type);
  size_t width, height, count, cpp;
  if (!map || !bpp || !read_header(map[0], &width, &height, &count, &cpp)) return NULL;
  if (width > SIZE_MAX / cpp || width > SIZE_MAX / height ||
      width * height > SIZE_MAX / bpp || count > (SIZE_MAX - height - 1) ||
      count > (SIZE_MAX - 1) / 2) return NULL;
  size_t slots = count * 2 + 1;
  if (slots > SIZE_MAX / sizeof(struct color)) return NULL;
  struct color *colors = calloc(slots, sizeof(*colors));
  if (!colors) return NULL;
  uint8_t *data = NULL;
  for (size_t i = 0; i < count; ++i) {
    struct color color = {0};
    uint32_t value;
    if (!read_color(map[i + 1], cpp, &color) || !pixel_value(&color, type, &value)) goto fail;
    struct color *slot = color_slot(colors, slots, color.symbol, cpp);
    if (slot->symbol) goto fail;
    color.rgb = value;
    *slot = color;
  }
  size_t size = width * height * bpp;
  data = malloc(size);
  if (!data) goto fail;
  uint8_t *output = data;
  for (size_t y = 0; y < height; ++y) {
    const char *row = map[count + y + 1];
    if (!row || strlen(row) != width * cpp) goto fail;
    for (size_t x = 0; x < width; ++x) {
      const struct color *color = color_slot(colors, slots, row + x * cpp, cpp);
      if (!color->symbol) goto fail;
      for (unsigned byte = 0; byte < bpp; ++byte)
        *output++ = (uint8_t)(color->rgb >> (8 * byte));
    }
  }
  free(colors);
  *img = (xpm_image_t){.type = type, .width = (uint16_t)width,
                       .height = (uint16_t)height, .size = size, .data = data};
  return data;
fail:
  free(data);
  free(colors);
  return NULL;
}

bool xpm_free_data(xpm_image_t *img) {
  if (!img || !img->data) return false;
  free(img->data);
  *img = (xpm_image_t){.type = INVALID_XPM};
  return true;
}
