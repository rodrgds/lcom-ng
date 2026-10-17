#include "RTC.h"

clock_fields_struct info_rtc;  // struct que contem info do RTC em segundos, minutos, etc
int hookid_rtc = 4; 
uint8_t rtc_mode; 


//subscribe RTC interrupts
int rtc_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL){
        return 1;
    }
    *bit_no = BIT(hookid_rtc);
    /*The RTC has only one IRQ line, which is connected to line IRQ0 of PIC2, i.e. IRQ8*/
    return sys_irqsetpolicy(rtc_irq, IRQ_REENABLE, &hookid_rtc);
}

//unsubscribe RTC interrupts
int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&hookid_rtc);
}


void get_rtc_info() {

    if(get_rtc_mode()!=0){
        rtc_mode = 1; //mode is binary
    }
    else{
        rtc_mode = 0; //mode is BCD
    }

    clock_fields_update();
    printf("got rtc info");
}

//determine whether the RTC is operating in binary mode (return 1) or not (return 0)
int get_rtc_mode() {
   
    uint8_t res; //stores the output of the RTC
    //REG_B: 11 is the adress of Control/Status RegisterB we want to read->BIT(2)'DM' set to 1 to set time and date registers in binary, set to 0 for BCD.
    if (extract_rtc_data(REG_B, &res)){
        printf("error reading from RTC");
        return 1; 
    }
    else{
        return res & BIT(2); //BIT(2) active -> means rtc configuration is in binary
    }
}


/* PDF NOTES:
The PC uses two ports to access the RTC’s internal registers:
    RTC_ADDR_REG on port 0x70, which must be loaded with the address of the RTC register to be accessed
    RTC_DATA_REG on port 0x71, which is used to transfer the data to/from the RTC’s register accessed
▶ To read/write a register of the RTC requires always:
    1. writing the address of the register to the RTC_ADDR_REG
    2. reading/writing one byte from/to the RTC_DATA_REG
*/

//reads time and date information from RTC registers given a write command
int extract_rtc_data(uint8_t reg_to_access, uint8_t *output_data) {
/*RTC has several registers that store information about the time and date. Each register is identified by an adress.
The function sends the adress to the RTC via the input port (0x70) and reads the result from the output port (0x71).*/
    
    if (sys_outb(0x70, reg_to_access) != 0){ //caso haja erro ao escrever (load) no reg de controlo (0x70) o endereço do reg que queremos aceder
        printf("Error writing to port 0x70");
        return 1;
    }
	if (util_sys_inb(0x71, output_data) != 0){ //caso haja erro ao ler do registo de output 0x71
        printf("Error reading from port 0x71");
        return 1;
    }
    return 0;
}


//check if RTC is updating registers and if so do not read from it -> if RTC is updating returns 1, otherwise returns 0.
int rtc_doing_update() { // if rtc is updating
/*When the RTC is updating, the information in its registers is not reliable and should not be read.
The function sends the REGISTER_A adress (adress: 10) to the RTC and reads the response. If the response has the
BIT(7) bit set, it means that the RTC is updating, and the function returns 1. Otherwise, it returns 0.*/

    uint8_t response; //result
    if (extract_rtc_data(REG_A, &response)) return 1; //REGISTER_UPDATING   10
	return response & BIT(7); //BIT(7) active means RTC is updating
}


// Converts BCD value into binary
uint8_t convert_in_binary(uint8_t bcd_value) {
    return ((bcd_value >> 4) * 10) + (bcd_value & 0xF);
}


// Actualizar os campos da struct time_info;
int clock_fields_update() {
    
    uint8_t time_info;
    // caso o RTC esteja a fazer update e a actualizar os registos não podemos ler a informação
    if (rtc_doing_update() != 0){
        printf("action failed: RTC is updating!");
        return 1;
    }

    // segundos address location: 0
    if (extract_rtc_data(0, &time_info) != 0){
        printf("action failed: RTC is updating!");    
        return 1;
    }
    if (rtc_mode) {
        info_rtc.sec = time_info;
    } else {
        info_rtc.sec = convert_in_binary(time_info);
    }

    // minutos address location: 2
    if (extract_rtc_data(2, &time_info) != 0) return 1;
    if (rtc_mode) {
        info_rtc.min = time_info;
    } else {
        info_rtc.min = convert_in_binary(time_info);
    }

    // horas address location: 4 
    if (extract_rtc_data(4 , &time_info) != 0) return 1;
    if (rtc_mode) {
        info_rtc.h = time_info;
    } else {
        info_rtc.h = convert_in_binary(time_info);
    }

    // dia address location: 7
    if (extract_rtc_data(7, &time_info) != 0) return 1;
    if (rtc_mode) {
        info_rtc.d = time_info;
    } else {
        info_rtc.d = convert_in_binary(time_info);
    }

    // mês address location: 8
    if (extract_rtc_data(8, &time_info) != 0) return 1;
   if (rtc_mode) {
        info_rtc.m = time_info;
    } else {
        info_rtc.m = convert_in_binary(time_info);
    }

    // ano address location: 9
    if (extract_rtc_data(9, &time_info) != 0) return 1;
    if (rtc_mode) {
        info_rtc.y = time_info;
    } else {
        info_rtc.y = convert_in_binary(time_info);
    }

    return 0;
}
