/* Compile the real adapter with getpid renamed only in its owning unit. */
#include <lcom/lcf.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
static pid_t selected_pid;
pid_t lcom_test_process_pid(void) { return selected_pid; }
const char *lcom_compat_project_name = "boundary-process";
int main(void) {
 const pid_t cases[]={0,1,SELF,ANY,NONE,0x3fffffff,0x40000000,INT_MAX};
 for(size_t i=0;i<sizeof(cases)/sizeof(cases[0]);i++) {
  selected_pid=cases[i];endpoint_t ep=-1;int flags=-1;char name[64];memset(name,0x5a,sizeof(name));
  int r=sys_whoami(&ep,name,sizeof(name),&flags);
  if(selected_pid<=0 || (uintmax_t)selected_pid>UINT32_C(0x3fffffff)) {
   if(r!=EOVERFLOW || ep!=-1 || flags!=-1 || name[0]!=0x5a)return 1;
  } else {
   if(r!=OK || ep!=(endpoint_t)(UINT32_C(0x40000000)|(uint32_t)selected_pid) ||
      ep==SELF || ep==ANY || ep==NONE || ep==HARDWARE || flags!=0)return 2;
   if(sys_enable_iop(ep)!=OK || sys_enable_iop(ANY)==OK || sys_enable_iop(NONE)==OK)return 3;
  }
 }
 puts("PROCESS_ENDPOINT_BOUNDARIES_OK");return 0;
}
