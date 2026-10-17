#include "rtc.h"



//Hook id to be used to set the interrupt policy
int rtc_hook_id = 8;

extern unsigned int round_time;
extern bool menu;
extern bool game_over;


void rtc_ih() 
{
        static unsigned int count = 0;

        uint8_t cause;
        
        sys_outb(RTC_ADDR_REG, RTC_REG_C);
        util_sys_inb(RTC_DATA_REG, &cause);
        if( cause & RTC_PF )
        {
                //printf("Received periodic interrupt\n");
                if(!menu && !game_over)
                {
                       count++;
                        if(count == 2)
                        {
                                round_time--;
                                count = 0;
                        }
                }
                
        }
}


int(rtc_subscribe_int)(int *bit_no)
{
        if (bit_no == NULL)
                return 1;

        if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id))
                return 1;


        uint8_t conf;

        if(rtc_read_data(RTC_REG_B,&conf)){
                return 1;
        }

        conf |= RTC_PIE;

        if(rtc_write_data(RTC_REG_B,conf)){
                return 1;
        }

        //Rate selector
        if(rtc_read_data(RTC_REG_A,&conf)){
                return 1;
        }

        conf |= RTC_RS3 | RTC_RS2 | RTC_RS1 | RTC_RS0 ;

        if(rtc_write_data(RTC_REG_A,conf)){
                return 1;
        }        

        return 0;
}

int (rtc_unsubscribe_int)()
{

        //Unsubscribing the interruptions
        if (sys_irqrmpolicy(&rtc_hook_id)){
                return 1;
        }


        uint8_t conf;

        if(rtc_read_data(RTC_REG_B,&conf)){
                return 1;
        }

        conf &= ~RTC_PIE;

        if(rtc_write_data(RTC_REG_B,conf)){
                return 1;
        }

        //rate selector
        if(rtc_read_data(RTC_REG_A,&conf)){
                return 1;
        }

        conf &= ~(RTC_RS3 | RTC_RS2 | RTC_RS1 | RTC_RS0);

        if(rtc_write_data(RTC_REG_A,conf)){
                return 1;
        }        


        return 0;

}

int rtc_write_data(int reg,uint8_t data)
{

        if (sys_outb(RTC_ADDR_REG,reg)) {
                return 1;
        }

        if (sys_outb(RTC_DATA_REG,data)) {
                return 1;
        }

        return 0;
}

int rtc_read_data(int reg,uint8_t * data)
{

        if (sys_outb(RTC_ADDR_REG,reg)) {
                return 1;
        }

        if (util_sys_inb(RTC_DATA_REG,data)) {
                return 1;
        }

        return 0;
}


int test_rtc()
{
        
        uint32_t rtc_irq_set = BIT(rtc_hook_id);

        if(rtc_subscribe_int(&rtc_hook_id)){
                printf("Failed to sunscribe rtc interrupts\n");
                return 1;
        }


        int ipc_status;
	message msg;
	int r;

        rtc_ih();
        while (1) {
                
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* Received notication */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* Hardware interrupt notification */
				if (msg.m_notify.interrupts & rtc_irq_set) {
                                        rtc_ih();
                                }
                                break;
                        default:
                                break;
                        }
                } else {}
        }




        if(rtc_unsubscribe_int()){
                printf("Failed to unsubscribe rtc interrupts\n");
                return 1;
        }

        return 0;

}

