#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include <lcom/ac97.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char **argv) {
  uintptr_t expected=argc>1?(uintptr_t)strtoull(argv[1],NULL,0):0xf0000000u;
  uintptr_t expected_pcm=argc>2?(uintptr_t)strtoull(argv[2],NULL,0):0xd0000000u;
  if(lcom_init()!=LCOM_OK)return 10;
  lcom_vbe_mode_info_t mode;
  if(lcom_vbe_get_mode_info(0x105,&mode)!=LCOM_OK || mode.framebuffer_phys!=expected) {
    fprintf(stderr,"VBE base mismatch: expected %lx, received %lx\n",(unsigned long)expected,(unsigned long)mode.framebuffer_phys);return 11;
  }
  vbe_mode_info_t bios;
  if(vbe_get_mode_info(0x105,&bios)!=OK || bios.PhysBasePtr!=expected)return 12;
  if(lcom_vbe_set_mode(0x105)!=LCOM_OK)return 13;
  void *mapped=NULL;
  if(lcom_phys_map(expected,4096,&mapped)!=LCOM_OK || !mapped)return 14;
  lcom_ac97_buffer_info_t audio;
  if(lcom_ac97_get_buffer(&audio)!=LCOM_OK || audio.pcm_phys!=expected_pcm || audio.pcm_bytes!=2u*1024u*1024u)return 18;
  void *pcm=NULL;
  if(lcom_phys_map(audio.pcm_phys,4096,&pcm)!=LCOM_OK || !pcm)return 19;
  memset(pcm,0x77,4096);
  uintptr_t other=expected==0xe0000000u?0xf0000000u:0xe0000000u;
  void *rejected=NULL;
  if(lcom_phys_map(other,4096,&rejected)==LCOM_OK)return 15;
  memset(mapped,0x22,4096);
  if(lcom_vbe_present()!=LCOM_OK || ((unsigned char*)mapped)[4095]!=0x22 || ((unsigned char*)pcm)[4095]!=0x77)return 16;
  if(lcom_phys_unmap(mapped,4096)!=LCOM_OK)return 17;
  if(lcom_phys_unmap(pcm,4096)!=LCOM_OK)return 20;
  printf("HARDWARE_CONFIG_OK %lx PCM %lx\n",(unsigned long)expected,(unsigned long)expected_pcm);
  lcom_exit();return 0;
}
