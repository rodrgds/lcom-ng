#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
	if (NULL == lsb) return -1;
	
	*lsb = val;
	return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
	if (NULL == msb) return -1;

	val = val >> 8;
	*msb = val;
	return 0;
}
#ifdef LAB_3
    uint32_t num_interrupts = 0;
#endif

int (util_sys_inb)(int port, uint8_t *value){
    #ifdef LAB_3
		num_interrupts++;
	#endif
	uint32_t aux;
	int call = sys_inb(port, &aux);
	if (call != 0){
		printf("Error on sys_inb() call.\n");
		return -1;
	}
	
	*value = aux;
	return 0;
}
