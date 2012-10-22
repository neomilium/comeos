#ifndef __BF4_H_DRIVER__
#define __BF4_H_DRIVER__

#include <stdint.h>

#define _BF4_SIZE_NORMAL		0
#define _BF4_SIZE_BIG			1

/* Fonctions qui concernent les manipulations hardware (ceci est le driver) */
void		bf4_init(void);
void		bf4_reset_DDRAM(void);
void		bf4_write(uint8_t data);
void		bf4_send_command(const uint8_t command);
void		bf4_send_data(const uint8_t data);

/* Fonctions qui gerent l'affichage (API : Application Programming Interface) */
void		bf4_clear(void);
void		bf4_gotoxy(const uint8_t x, const uint8_t y);
void		bf4_display_char(const char ascii);
void		bf4_display_symbol(const char symbol);
void		bf4_display_string(const char *string);
void		bf4_set_mode(uint8_t mode);
void		bf4_set_size(uint8_t size);
void		bf4_finish_line(void);
#endif
