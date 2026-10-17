#include <lcom/lcf.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do {if(!(c)){fprintf(stderr,"DAC check failed line%d AX=%04x BH=%u\n",__LINE__,regs.ax,regs.bh);return 1;}}while(0)
static reg86_t regs;
static int bios(uint16_t function,uint16_t bx,uint16_t count,uint16_t first,mmap_t *table) {
 memset(&regs,0,sizeof regs);regs.intno=0x10;regs.ax=function;regs.bx=bx;regs.cx=count;regs.dx=first;
 if(table){regs.es=table->phys>>4;regs.di=table->phys&15;}
 return sys_int86(&regs);
}
int main(int argc,char **argv) {
 CHECK(lcom_init()==LCOM_OK);
 CHECK(bios(0x4f02,0x4105,0,0,NULL)==OK);
 CHECK(bios(0x4f08,0x0800,0,0,NULL)==OK && regs.ax==0x004f && regs.bh==8);
 CHECK(bios(0x4f08,1,0,0,NULL)==OK && regs.bh==8);
 mmap_t map;uint8_t *table=lm_alloc(1024,&map);CHECK(table);
 const uint8_t colors[8]={0x81,0x42,0xe3,0xcc,0xf0,0x80,0x10,0xee};memcpy(table,colors,8);
 CHECK(bios(0x4f09,0,2,254,&map)==OK && regs.ax==0x004f);
 memset(table,0xff,8);CHECK(bios(0x4f09,1,2,254,&map)==OK);
 CHECK(table[0]==0x81 && table[1]==0x42 && table[2]==0xe3 && table[3]==0);
 CHECK(table[4]==0xf0 && table[5]==0x80 && table[6]==0x10 && table[7]==0);
 CHECK(sys_outb(0x3c7,254)==OK);uint32_t value;
 CHECK(sys_inb(0x3c9,&value)==OK && value==0xe3);
 CHECK(sys_inb(0x3c9,&value)==OK && value==0x42);
 CHECK(sys_inb(0x3c9,&value)==OK && value==0x81);
 CHECK(bios(0x4f09,0,2,255,&map)!=OK && regs.ax==0x014f);
 CHECK(bios(0x4f09,0,257,0,&map)!=OK && regs.ax==0x014f);
 CHECK(bios(0x4f09,2,1,0,&map)!=OK && regs.ax==0x024f);
 CHECK(bios(0x4f08,0x0700,0,0,NULL)==OK && regs.bh==6);
 CHECK(bios(0x4f08,0x0900,0,0,NULL)==OK && regs.bh==8);
 CHECK(bios(0x4f08,0x0500,0,0,NULL)!=OK && regs.ax==0x014f);
 CHECK(bios(0x4f08,1,0,0,NULL)==OK && regs.bh==8);
 CHECK(bios(0x4f02,0x4105,0,0,NULL)==OK);
 CHECK(bios(0x4f08,1,0,0,NULL)==OK && regs.bh==6);
 memcpy(table,colors,8);CHECK(bios(0x4f09,0x80,1,254,&map)==OK);
 memset(table,0,4);CHECK(bios(0x4f09,1,1,254,&map)==OK);
 CHECK(table[0]==1 && table[1]==2 && table[2]==35 && table[3]==0);
 CHECK(bios(0x4f02,0x4115,0,0,NULL)==OK);
 CHECK(bios(0x4f08,1,0,0,NULL)!=OK && regs.ax==0x034f);
 CHECK(bios(0x4f09,0,1,254,&map)!=OK && regs.ax==0x034f);
 CHECK(lcom_vbe_set_mode(0x115)==LCOM_OK);
 CHECK(bios(0x4f08,1,0,0,NULL)!=OK && regs.ax==0x034f);
 CHECK(lcom_vbe_set_mode(0x105)==LCOM_OK);
 CHECK(bios(0x4f08,0x0800,0,0,NULL)==OK);
 memcpy(table,colors,8);CHECK(bios(0x4f09,0,1,254,&map)==OK);
 mmap_t controller;uint8_t *info=lm_alloc(512,&controller);CHECK(info);
 CHECK(bios(0x4f00,0,0,0,&controller)==OK && (info[10]&1));
 lcom_vbe_mode_info_t mode;CHECK(lcom_vbe_get_mode_info(0x105,&mode)==LCOM_OK);
 uint8_t *pixels=NULL;CHECK(lcom_phys_map(mode.framebuffer_phys,mode.framebuffer_size,(void**)&pixels)==LCOM_OK);
 if(argc>1 && !strcmp(argv[1],"left")){
  CHECK(bios(0x4f08,0x0600,0,0,NULL)==OK && regs.bh==6);
  table[0]=3;table[1]=21;table[2]=63;table[3]=0;CHECK(bios(0x4f09,0,1,254,&map)==OK);
 }
 memset(pixels,254,mode.framebuffer_size);CHECK(lcom_vbe_present()==LCOM_OK);
 CHECK(lcom_phys_unmap(pixels,mode.framebuffer_size)==LCOM_OK);
 CHECK(lm_free(&map) && lm_free(&controller));
 puts("VBE_DAC_OK rgb227,66,129");lcom_exit();return 0;
}
