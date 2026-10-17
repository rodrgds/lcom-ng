/* Exercise the real LCF RNG through its normal entropy-file boundary. */
#include <lcom/lcf.h>
#include <stdint.h>
static unsigned opens, reads, closes;
static int fail_read;
FILE *lcom_compat_fopen(const char *path, const char *mode) {
  if (strcmp(path, "/dev/urandom") || strcmp(mode, "rb")) abort();
  ++opens;
  return (FILE *)(uintptr_t)1;
}
size_t lcom_test_entropy_read(void *data, size_t size, size_t count, FILE *stream) {
  if (stream != (FILE *)(uintptr_t)1 || size != sizeof(uint32_t) || count != 1) abort();
  ++reads;
  uint32_t seed = 5489;
  memcpy(data, &seed, sizeof(seed));
  return fail_read ? 0 : 1;
}
int lcom_test_entropy_close(FILE *stream) {
  if (stream != (FILE *)(uintptr_t)1) abort();
  ++closes;
  return 0;
}
int main(int argc, char **argv) {
  (void)argv;
  fail_read = argc > 1;
  /* Reference MT19937 seed 5489, then the original LCF low-31-bit conversion.
   * The later checkpoints cross two 624-word refill boundaries. */
  const struct { unsigned index; int value; } reference[] = {
    {0,1351727964}, {1,581869302}, {2,1742863086}, {3,1438850937},
    {623,1872842239}, {624,2031410264}, {625,610818241},
    {999,1341017984}, {1248,358555951}
  };
  unsigned checkpoint = 0;
  for (unsigned i = 0; i < 1280; ++i) {
    srand(i); /* libc state must not change LCF's independent stream. */
    int value = rand_();
    if (value < 0 || (uint32_t)value > UINT32_C(0x7fffffff)) return 1;
    if (checkpoint < sizeof(reference)/sizeof(reference[0]) &&
        i == reference[checkpoint].index) {
      if (value != reference[checkpoint].value) return 2;
      ++checkpoint;
    }
  }
  if (opens != 1 || reads != 1 || closes != 1) return 3;
  puts("LCF independently seeded 31-bit random stream passed");
  return 0;
}
