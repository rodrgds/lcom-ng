#ifndef __UTILITIES_H
#define __UTILITIES_H

typedef struct{
	void (*handler) (unsigned int);
	int (*subscribe) (void);
	int (*unsubscribe) (void);
} Interrupt;


/**
 * @brief generic interrupt handler
 */
void interruptHandler();

/**
 * @brief read byte from data register > sys_in(DATA_REG,"ret")
 * @param ret variable used to stored the read data
 * @return 0 if done correctly, other value otherwise
 */
int read_DATA_REG_Byte(unsigned long *ret);

/**
 * @brief write to given port > sys_out(port,arg)
 * @param port destination to send arg
 * @param arg data to be sent
 * @return 0 if done correctly, other value otherwise
 */
int write_to(unsigned long port,unsigned long arg);

/*DEBUG*/
//#define DEBUG

#ifdef DEBUG
#define DEBUG_UDEC(x) {printf("%s: %u\n",#x,x);}
#define DEBUG_SDEC(x) {printf("%s: %d\n",#x,x);}
#define DEBUG_HEXADEC(x) {printf("%s: %X\n",#x,x);}
#define DEBUG_FLOAT(x) {printf("%s: %f\n",#x,x);}
#define DEBUG_STR(x) {printf("%s: %s\n",#x,x);}
#define DEBUG_MSG(x) {printf("%s\n",x);}
#else
#define DEBUG_UDEC(format, args...) ((void)0)
#define DEBUG_SDEC(format, args...) ((void)0)
#define DEBUG_HEXADEC(format, args...) ((void)0)
#define DEBUG_FLOAT(format, args...) ((void)0)
#define DEBUG_STR(format, args...) ((void)0)
#define DEBUG_MSG(format, args...) ((void)0)
#endif

/**
 * @brief sets subs_irq_set to 0
 */
void resetIrqSet();
/**
 * @brief adds bit in indicated position to the variable subs_irq_set (keep track on subscriptions)
 * @param irqBitPosition position of the bit to be set on the subs_irq_set
 */
void addIrq(unsigned short irqBitPosition);//updateIrqSetWithNewSubscrition

/**
 * @brief Initializes Interrupt List
 */
void initInterruptList();

/**
 * @brief add I/O too Interrupt List
 */
void addToInterrupt(void (*handler) (unsigned int),int (*subscribe) (void), int (*unsubscribe) (void));

/**
 * @brief Subscribes Interrupts on the  List
 */
void subscribeInterrupts();

/**
 * @brief Auxiliar function for generic handler. Calls the correct handler on the Interrupt's List
 */
void callInterruptsHandler(unsigned int bitmask);

/**
 * @brief Unsubscribes Interrupts on the  List
 */
void unsubscribeInterrupts();


/**
 * @brief Free's Interrupt List
 */
void freeInterruptList();




/*============ Useful macros  =============*/
//#include <minix/drivers.h> to use ALLOC AND REALLOC MACROS
#define ALLOC(type,number,destiny) do {	\
		type* tmp= (type *) malloc((number)*sizeof(type)); \
		if(tmp!=NULL) destiny=tmp; \
		printf("\nMALLOC FAILED");\
		} while(0)
#define REALLOC(type,number,scr) do {	\
		type* tmp= (type *) realloc(scr,(number)*sizeof(type)); \
		if(tmp!=NULL) scr=tmp; \
		printf("\nREALLOC FAILED");\
		} while(0)

#endif

/*=====DEBUG LOG AS IN THE POWERPOINT PRESENTATION*/
#include <stdio.h>
extern FILE* logfd;

//#define DEBUG_LOG

#define LOG_PATH "home/lcom/srcS/log.txt"
#ifdef DEBUG_LOG
#define LOG_SDEC(tag,var) {fprintf(logfd,"%s: %d\n",tag,var);}
#define LOG_UDEC(tag,var) {fprintf(logfd,"%s: %u\n",tag,var);}
#define LOG_CHAR(tag,var) {fprintf(logfd,"%s: %c\n",tag,var);}
#else
#define LOG_SDEC(format, args...) ((void)0)
#define LOG_UDEC(format, args...) ((void)0)
#define LOG_CHAR(format, args...) ((void)0)
#endif

