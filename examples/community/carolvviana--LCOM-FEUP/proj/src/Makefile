PROG = proj

# .PATH: ${.CURDIR}/timer
# .PATH: ${.CURDIR}/gpu
# .PATH: ${.CURDIR}/kbd_mouse
# .PATH: ${.CURDIR}/rtc
# .PATH: ${.CURDIR}/mvc

SRCS = main.c utils.c timer/timer.c gpu/gpu.c kbd_mouse/kbd.c kbd_mouse/mouse.c rtc/rtc.c mvc/controller.c mvc/model.c mvc/viewer.c mvc/xpms.c serialport/serialport.c

# additional compilation flags
# use -D DEBUG for debug mode
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2 -D DEBUG

DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm

timer/timer.o: timer/timer.c timer/timer.h
	cc $(CFLAGS) -o timer/timer.o -c timer/timer.c 
gpu/gpu.o: gpu/gpu.c gpu/gpu.h
	cc $(CFLAGS) -c gpu/gpu.c -o gpu/gpu.o
# kbd_mouse/KBC.o: kbd_mouse/KBC.c kbd_mouse/KBC.h
# 	cc $(CFLAGS) -c kbd_mouse/KBC.c -o kbd_mouse/KBC.o
kbd_mouse/kbd.o: kbd_mouse/kbd.c kbd_mouse/kbd.h
	cc $(CFLAGS) -c kbd_mouse/kbd.c -o kbd_mouse/kbd.o
kbd_mouse/mouse.o: kbd_mouse/mouse.c kbd_mouse/mouse.h
	cc $(CFLAGS) -c kbd_mouse/mouse.c -o kbd_mouse/mouse.o
rtc/rtc.o: rtc/rtc.c rtc/rtc.h
	cc $(CFLAGS) -c rtc/rtc.c -o rtc/rtc.o
serialport/serialport.o: serialport/serialport.c serialport/serialport.h
	cc $(CFLAGS) -c serialport/serialport.c -o serialport/serialport.o
mvc/controller.o: mvc/controller.c mvc/controller.h
	cc $(CFLAGS) -c mvc/controller.c -o mvc/controller.o
mvc/model.o: mvc/model.c mvc/model.h
	cc $(CFLAGS) -c mvc/model.c -o mvc/model.o
mvc/viewer.o: mvc/viewer.c mvc/viewer.h
	cc $(CFLAGS) -c mvc/viewer.c -o mvc/viewer.o
mvc/xpms.o: mvc/xpms.c mvc/xpms.h
	cc $(CFLAGS) -c mvc/xpms.c -o mvc/xpms.o


# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

