#include "menus.h"

#include "lcd.h"
#include "windowmanager.h"

void
menus_init(void)
{
}

void
menus_display(const menu_t * menu, const uint8_t current_menu_item)
{
	uint8_t i;
	for (i = 0; i < menu->item_count; i++) {
		lcd_gotoxy(menu->menu_items[i].position.x, menu->menu_items[i].position.y);

		if (i == current_menu_item) {
			lcd_set_mode(LCD_MODE_INVERTED);
		}
		lcd_display_string(menu->menu_items[i].title);
		lcd_set_mode(LCD_MODE_NORMAL);
	}
	if( current_menu_item != MENU_NO_CURRENT_ITEM) {
		menu->menu_items[current_menu_item].application->fn_display_preview();
	}
}
