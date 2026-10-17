#include "devices.h"
#include "variables.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "video_gr.h"
#include "rtc.h"
#include "macros.h"
#include "vbe.h"

int devices_subscriptions() {
	// Entra no modo grafico
	char *video_mem;
	video_mem = (char *)vg_init(VG_MODE);

	if (video_mem == NULL)
		return 1;

	// Subscricao do keyboard
	kbd_subs = kbd_subscribe();

	if (kbd_subs == -1) {
		return 1;
	}

	kbd_set = BIT(kbd_subs);

	// Subscricao do timer
	time_subs = timer_subscribe_int();

	if (time_subs == -1) {
		return 1;
	}

	timer_set = BIT(time_subs);

	// Subscricao do mouse
	mouse_subs = mouse_subscribe();

	if (mouse_subs == -1) {
		return 1;
	}

	mouse_command(SET_STREAM);
	mouse_command(DR_ENA);

	mouse_set = BIT(mouse_subs);

	// Subscricao do RTC
	rtc_subs = rtc_subscribe();

	if (rtc_subs == -1) {
		return 1;
	}

	rtc_set = BIT(rtc_subs);

	return 0;
}

int devices_unsubscriptions() {
	// Sai do modo grafico
	if (vg_exit())
		return 1;

	// Retira a subscricao do keyboard

	kbd_subs = kbd_unsubscribe();

	if (kbd_subs == -1) {
		return 1;
	}

	// Retira a subscricao do timer
	time_subs = timer_unsubscribe_int();

	if (time_subs == -1) {
		return 1;
	}

	// Retira a subscricao do mouse
	mouse_subs = mouse_unsubscribe();

	if (mouse_subs == -1) {
		return 1;
	}
	mouse_command(DR_DIS);
	mouse_command(SET_STREAM);

	// Retira a subscricao do rtc
	rtc_subs = rtc_unsubscribe();

	if (rtc_subs == -1) {
		return 1;
	}

	return 0;
}
