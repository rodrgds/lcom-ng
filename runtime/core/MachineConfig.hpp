#ifndef LCOM_MACHINE_CONFIG_HPP
#define LCOM_MACHINE_CONFIG_HPP

#include <lcom/memory_map.h>
#include <lcom/vbe_modes.h>
#include <charconv>
#include <cstdint>
#include <string>
#include <string_view>
#include <system_error>

namespace lcom {

struct MachineConfig {
  uint32_t vbe_physical_base = LCOM_VBE_DEFAULT_PHYS;
  uint32_t ac97_pcm_base = LCOM_AC97_PCM_PHYS;
};

// Both device windows are page-aligned, 32-bit apertures outside the Minix
// BIOS/low-memory backing and the fixed VGA text aperture. Their placement
// belongs to the machine, not getenv.
inline bool validPhysicalAperture(uint64_t base, uint64_t bytes) {
  return base <= UINT32_MAX && !(base & 4095u) &&
         base >= LCOM_MINIX_LOW_BASE + LCOM_MINIX_LOW_SIZE &&
         base + bytes <= (uint64_t{1} << 32) &&
         !(base < LCOM_VGA_TEXT_PHYS + LCOM_VGA_TEXT_BYTES &&
           LCOM_VGA_TEXT_PHYS < base + bytes);
}

inline bool validMachineConfig(const MachineConfig &config) {
  if (!validPhysicalAperture(config.vbe_physical_base, LCOM_VBE_VRAM_BYTES) ||
      !validPhysicalAperture(config.ac97_pcm_base, LCOM_AC97_PCM_BYTES)) return false;
  return !(uint64_t(config.vbe_physical_base) < uint64_t(config.ac97_pcm_base) + LCOM_AC97_PCM_BYTES &&
           uint64_t(config.ac97_pcm_base) < uint64_t(config.vbe_physical_base) + LCOM_VBE_VRAM_BYTES);
}

// Preserve validation of a VBE-only override against the default audio window.
inline bool validVbePhysicalBase(uint64_t base) {
  return base <= UINT32_MAX && validMachineConfig({static_cast<uint32_t>(base), LCOM_AC97_PCM_PHYS});
}

inline bool parsePhysicalBase(const char *value, uint32_t &base) {
  if (!value) return true;
  std::string_view text(value);
  int radix = 10;
  if (text.size() >= 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
    text.remove_prefix(2); radix = 16;
  }
  uint64_t number = 0;
  const auto result = std::from_chars(text.data(), text.data() + text.size(), number, radix);
  if (text.empty() || result.ec != std::errc{} || result.ptr != text.data() + text.size() || number > UINT32_MAX)
    return false;
  base = static_cast<uint32_t>(number);
  return true;
}

// Parse together so a valid explicit relocation is not rejected against the
// other device's default. Failed parsing never partially updates the caller.
inline bool parseMachineConfig(const char *vbe, const char *pcm, MachineConfig &out, std::string &error) {
  MachineConfig candidate;
  if (!parsePhysicalBase(vbe, candidate.vbe_physical_base) ||
      !parsePhysicalBase(pcm, candidate.ac97_pcm_base) || !validMachineConfig(candidate)) {
    error = "LCOM_VBE_PHYSICAL_BASE and LCOM_AC97_PCM_BASE must select disjoint page-aligned 32-bit apertures outside Minix low memory and VGA text memory";
    return false;
  }
  out = candidate;
  error.clear(); return true;
}

inline bool parseMachineConfig(const char *vbe, MachineConfig &out, std::string &error) {
  return parseMachineConfig(vbe, nullptr, out, error);
}

} // namespace lcom
#endif
