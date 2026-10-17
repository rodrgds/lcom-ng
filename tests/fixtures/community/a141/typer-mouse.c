#include <lcom/lcf.h>
#include <assert.h>
static uint8_t status_value=0x21,bytes[8];static int at,reads;
int util_sys_inb(int port,uint8_t *value){if(port==0x64)*value=status_value;else{assert(port==0x60);*value=bytes[at++];reads++;}return 0;}
#include MOUSE_SOURCE
int main(void){
 bytes[0]=0x29;bytes[1]=10;bytes[2]=0xF4;
 for(int i=0;i<3;i++)mouse_ih();
 assert(reads==3);assert(get_packets_count()==3);Event e=mouse_get_event();assert(e.type==MOUSE);assert(e.info.mouse.x_delta==10&&e.info.mouse.y_delta==-12&&e.info.mouse.clicked);
 packets_count=0;at=reads=0;bytes[0]=0;bytes[1]=8;bytes[2]=5;bytes[3]=7;
 for(int i=0;i<4;i++)mouse_ih();e=mouse_get_event();assert(reads==4);assert(get_packets_count()==3);assert(e.info.mouse.x_delta==5&&e.info.mouse.y_delta==7&&!e.info.mouse.clicked);
 packets_count=0;at=reads=0;status_value=1;mouse_ih();assert(reads==0&&get_packets_count()==0);
 return 0;
}
