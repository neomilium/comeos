#ifndef _APP_MAINMENU_H_
#define _APP_MAINMENU_H_

#include "application.h"
#include "drv_keyboard.h"

#define app_mainmenu_init()	menu_t mainmenu;								\
					menu_item_t mainmenu_items[] = {					\
					{ PSTR("Exposition"), &app_exposition, {0, 0} },			\
					{ PSTR("State"), &app_state, {66, 0} },					\
					{ PSTR("Shots"), &app_shots, {0, 1} },					\
					{ PSTR("Interval"), &app_interval, {48, 1} },					\
					{ PSTR("Sound"), &app_sound, {0, 2} },						\
					{ PSTR("Light"), &app_light, {66, 2} },						\
				};										\
				mainmenu.menu_items = mainmenu_items;						\
				mainmenu.item_count = sizeof(mainmenu_items)/sizeof(menu_item_t);		\
				application_t app_mainmenu = { &_app_mainmenu_init, &_app_mainmenu_event_handler, 0, &mainmenu }

void		_app_mainmenu_init(void *);
void		_app_mainmenu_event_handler(const keyboard_event_t);


#endif
