#ifndef __MENUS_H__
#define __MENUS_H__

#include <stdint.h>
#include "application.h"

#include <avr/pgmspace.h>

#define MENU_NO_CURRENT_ITEM 0xFF

typedef struct {
	uint8_t x;
	uint8_t y;
} position_t;

typedef struct {
	prog_char      *title;
	application_t  *application;
	position_t	position;
} menu_item_t;

typedef struct {
	menu_item_t    *menu_items;
	uint8_t 		item_count;
} menu_t;

void		menus_init(void);
void		menus_display(const menu_t * menu, const uint8_t current_menu_item);

#endif
