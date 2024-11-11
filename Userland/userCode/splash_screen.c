// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <splash_screen.h>


void show_splash_screen() {
	sys_clear_screen(0);

	uint32_t startXPos = (sys_get_screen_width() - G19_WIDTH * G19_SCALE) / 2;
	uint32_t startYPos = (sys_get_screen_height() - G19_HEIGHT * G19_SCALE) / 2 - LOADING_BAR_HEIGHT;

	uint32_t loadingBarStartYPos = startYPos + G19_HEIGHT * G19_SCALE + LOADING_BAR_HEIGHT;
	for (uint32_t i = 0; i < G19_WIDTH; i++) {
		for (uint32_t y = 0; y < G19_HEIGHT; y++) {
			for (uint32_t x = 0; x < G19_WIDTH; x++) { sys_draw_square(g19[y * G19_WIDTH + x], startXPos + x * G19_SCALE, startYPos + y * G19_SCALE, G19_SCALE); }
		}

		sys_draw_square(0xffffff, startXPos + i * G19_SCALE, loadingBarStartYPos, G19_SCALE);
		sleep(TOTAL_TIME / G19_WIDTH);
	}
}
