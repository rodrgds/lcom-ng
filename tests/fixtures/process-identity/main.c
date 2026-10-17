#include <lcom/lcf.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#define EXPECT(x) do {if(!(x)){fprintf(stderr,"identity failed line %d: %s\n",__LINE__,#x);return 1;}}while(0)
int main(void) {
 endpoint_t current = -1; int flags=-1; char name[64];
 memset(name,0x5a,sizeof(name));
 EXPECT(sys_whoami(&current,name,1,&flags)==EINVAL);
 EXPECT(current==-1 && flags==-1 && name[0]==0x5a);
 EXPECT(sys_whoami(&current,name,sizeof(name),&flags)==OK);
 EXPECT(current==(endpoint_t)(UINT32_C(0x40000000)|(uint32_t)getpid()) && current>0);
 EXPECT(strcmp(name,"identity-proces")==0 && flags==0);
 EXPECT(name[48]==0x5a); /* Primary library only fills min(len,48) bytes including NUL. */
 char short_name[4]={0x5a,0x5a,0x5a,0x5a};
 EXPECT(sys_whoami(&current,short_name,3,&flags)==OK);
 EXPECT(strcmp(short_name,"id")==0 && short_name[3]==0x5a);
 EXPECT(sys_enable_iop(current)==OK && sys_enable_iop(SELF)==OK);
 endpoint_t other=current+1;if(other==SELF||other==ANY)other=current+2;
 EXPECT(sys_enable_iop(other)!=OK);
 EXPECT(sys_enable_iop(ANY)!=OK && sys_enable_iop(NONE)!=OK && sys_enable_iop(HARDWARE)!=OK);
 struct minix_mem_range range={0xe0000000u,0xe0001000u};
 EXPECT(sys_privctl(current,SYS_PRIV_ADD_MEM,&range)==OK);
 EXPECT(sys_privctl(other,SYS_PRIV_ADD_MEM,&range)!=OK);
 reg86_t regs={0};regs.intno=0x10;regs.ax=0x4f02;regs.bx=0x4105;
 EXPECT(sys_int86(&regs)==OK && regs.ax==0x004f);
 void *p=vm_map_phys(current,(void*)(uintptr_t)range.mr_base,4096);
 EXPECT(p!=MAP_FAILED);((volatile unsigned char*)p)[0]=0x27;
 EXPECT(vm_unmap_phys(other,p,4096)!=OK);
 EXPECT(vm_unmap_phys(current,p,4096)==OK);
 EXPECT(vm_map_phys(other,(void*)(uintptr_t)range.mr_base,4096)==MAP_FAILED);
 printf("PROCESS_IDENTITY_OK endpoint=%d name=%s flags=%d\n",current,name,flags);
 return 0;
}
