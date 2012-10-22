#include "drv_bf4.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define BF4_DDR			DDRC
#define BF4_MASK			0b00011111

#define _BF4_MODE_NORMAL		0x00
#define _BF4_MODE_INVERTED		0xFF

#define BF4_POWER   GET_BIT(PORTB).bit0		// bf4 lcd power

#define BF4_SCLK    GET_BIT(PORTC).bit5		// bf4 lcd sclk
#define BF4_SDA     GET_BIT(PORTC).bit4		// bf4 lcd sda
#define BF4_A0      GET_BIT(PORTC).bit3		// bf4 lcd a0
#define BF4_CS      GET_BIT(PORTC).bit2		// bf4 lcd !cs
#define BF4_RES     GET_BIT(PORTC).bit1		// bf4 lcd !res

#define BF4_SCREEN_WIDTH	96
#define BF4_SCREEN_ROWS		8

#define register_set( REGISTER, VALUE, MASK ) do { REGISTER = ( ((REGISTER) & ~(MASK)) | ((VALUE) & (MASK)) ); } while (0)

#include "bit_field.h"

void _bf4_display_normal_char(const char ascii);
void _bf4_display_normal_symbol(const char symbol);

typedef void ( *_display_char_fct ) ( const char );
static volatile _display_char_fct _bf4_display_char = _bf4_display_normal_char;
static volatile _display_char_fct _bf4_display_symbol = _bf4_display_normal_symbol;

static const prog_uchar ascii_table[][5] = {
	{0x00, 0x00, 0x00, 0x00, 0x00},	/* sp */
	{0x00, 0x00, 0x2f, 0x00, 0x00},	/* ! */
	{0x00, 0x07, 0x00, 0x07, 0x00},	/* " */
	{0x14, 0x7f, 0x14, 0x7f, 0x14},	/* # */
	{0x24, 0x2a, 0x7f, 0x2a, 0x12},	/* $ */
	{0xc4, 0xc8, 0x10, 0x26, 0x46},	/* % */
	{0x36, 0x49, 0x55, 0x22, 0x50},	/* & */
	{0x00, 0x05, 0x03, 0x00, 0x00},	/* ' */
	{0x00, 0x1c, 0x22, 0x41, 0x00},	/* ( */
	{0x00, 0x41, 0x22, 0x1c, 0x00},	/* ) */
	{0x14, 0x08, 0x3E, 0x08, 0x14},	/* * */
	{0x08, 0x08, 0x3E, 0x08, 0x08},	/* + */
	{0x00, 0x00, 0x50, 0x30, 0x00},	/* , */
	{0x10, 0x10, 0x10, 0x10, 0x10},	/* - */
	{0x00, 0x60, 0x60, 0x00, 0x00},	/* . */
	{0x20, 0x10, 0x08, 0x04, 0x02},	/* / */
	{0x3E, 0x51, 0x49, 0x45, 0x3E},	/* 0 */
	{0x00, 0x42, 0x7F, 0x40, 0x00},	/* 1 */
	{0x42, 0x61, 0x51, 0x49, 0x46},	/* 2 */
	{0x21, 0x41, 0x45, 0x4B, 0x31},	/* 3 */
	{0x18, 0x14, 0x12, 0x7F, 0x10},	/* 4 */
	{0x27, 0x45, 0x45, 0x45, 0x39},	/* 5 */
	{0x3C, 0x4A, 0x49, 0x49, 0x30},	/* 6 */
	{0x01, 0x71, 0x09, 0x05, 0x03},	/* 7 */
	{0x36, 0x49, 0x49, 0x49, 0x36},	/* 8 */
	{0x06, 0x49, 0x49, 0x29, 0x1E},	/* 9 */
	{0x00, 0x36, 0x36, 0x00, 0x00},	/* : */
	{0x00, 0x56, 0x36, 0x00, 0x00},	/* ; */
	{0x08, 0x14, 0x22, 0x41, 0x00},	/* < */
	{0x14, 0x14, 0x14, 0x14, 0x14},	/* = */
	{0x00, 0x41, 0x22, 0x14, 0x08},	/* > */
	{0x02, 0x01, 0x51, 0x09, 0x06},	/* ? */
	{0x32, 0x49, 0x59, 0x51, 0x3E},	/* @ */
	{0x7E, 0x11, 0x11, 0x11, 0x7E},	/* A */
	{0x7F, 0x49, 0x49, 0x49, 0x36},	/* B */
	{0x3E, 0x41, 0x41, 0x41, 0x22},	/* C */
	{0x7F, 0x41, 0x41, 0x22, 0x1C},	/* D */
	{0x7F, 0x49, 0x49, 0x49, 0x41},	/* E */
	{0x7F, 0x09, 0x09, 0x09, 0x01},	/* F */
	{0x3E, 0x41, 0x49, 0x49, 0x7A},	/* G */
	{0x7F, 0x08, 0x08, 0x08, 0x7F},	/* H */
	{0x00, 0x41, 0x7F, 0x41, 0x00},	/* I */
	{0x20, 0x40, 0x41, 0x3F, 0x01},	/* J */
	{0x7F, 0x08, 0x14, 0x22, 0x41},	/* K */
	{0x7F, 0x40, 0x40, 0x40, 0x40},	/* L */
	{0x7F, 0x02, 0x0C, 0x02, 0x7F},	/* M */
	{0x7F, 0x04, 0x08, 0x10, 0x7F},	/* N */
	{0x3E, 0x41, 0x41, 0x41, 0x3E},	/* O */
	{0x7F, 0x09, 0x09, 0x09, 0x06},	/* P */
	{0x3E, 0x41, 0x51, 0x21, 0x5E},	/* Q */
	{0x7F, 0x09, 0x19, 0x29, 0x46},	/* R */
	{0x46, 0x49, 0x49, 0x49, 0x31},	/* S */
	{0x01, 0x01, 0x7F, 0x01, 0x01},	/* T */
	{0x3F, 0x40, 0x40, 0x40, 0x3F},	/* U */
	{0x1F, 0x20, 0x40, 0x20, 0x1F},	/* V */
	{0x3F, 0x40, 0x38, 0x40, 0x3F},	/* W */
	{0x63, 0x14, 0x08, 0x14, 0x63},	/* X */
	{0x07, 0x08, 0x70, 0x08, 0x07},	/* Y */
	{0x61, 0x51, 0x49, 0x45, 0x43},	/* Z */
	{0x00, 0x7F, 0x41, 0x41, 0x00},	/* [ */
	{0x55, 0x2A, 0x55, 0x2A, 0x55},	/* 55 */
	{0x00, 0x41, 0x41, 0x7F, 0x00},	/* ] */
	{0x04, 0x02, 0x01, 0x02, 0x04},	/* ^ */
	{0x40, 0x40, 0x40, 0x40, 0x40},	/* _ */
	{0x00, 0x02, 0x05, 0x02, 0x00},	/* ° instead of ` */ 
	{0x20, 0x54, 0x54, 0x54, 0x78},	/* a */
	{0x7F, 0x48, 0x44, 0x44, 0x38},	/* b */
	{0x38, 0x44, 0x44, 0x44, 0x20},	/* c */
	{0x38, 0x44, 0x44, 0x48, 0x7F},	/* d */
	{0x38, 0x54, 0x54, 0x54, 0x18},	/* e */
	{0x08, 0x7E, 0x09, 0x01, 0x02},	/* f */
	{0x0C, 0x52, 0x52, 0x52, 0x3E},	/* g */
	{0x7F, 0x08, 0x04, 0x04, 0x78},	/* h */
	{0x00, 0x44, 0x7D, 0x40, 0x00},	/* i */
	{0x20, 0x40, 0x44, 0x3D, 0x00},	/* j */
	{0x7F, 0x10, 0x28, 0x44, 0x00},	/* k */
	{0x00, 0x41, 0x7F, 0x40, 0x00},	/* l */
	{0x7C, 0x04, 0x18, 0x04, 0x78},	/* m */
	{0x7C, 0x08, 0x04, 0x04, 0x78},	/* n */
	{0x38, 0x44, 0x44, 0x44, 0x38},	/* o */
	{0x7C, 0x14, 0x14, 0x14, 0x08},	/* p */
	{0x08, 0x14, 0x14, 0x18, 0x7C},	/* q */
	{0x7C, 0x08, 0x04, 0x04, 0x08},	/* r */
	{0x48, 0x54, 0x54, 0x54, 0x20},	/* s */
	{0x04, 0x3F, 0x44, 0x40, 0x20},	/* t */
	{0x3C, 0x40, 0x40, 0x20, 0x7C},	/* u */
	{0x1C, 0x20, 0x40, 0x20, 0x1C},	/* v */
	{0x3C, 0x40, 0x30, 0x40, 0x3C},	/* w */
	{0x44, 0x28, 0x10, 0x28, 0x44},	/* x */
	{0x0C, 0x50, 0x50, 0x50, 0x3C},	/* y */
	{0x44, 0x64, 0x54, 0x4C, 0x44},	/* z */
};

//	{0x1C, 0x3E, 0x7C, 0x3E, 0x1C},	/* heart */

static const prog_uchar symbol_table[][5] = {
	{0xFF, 0x7E, 0x3C, 0x18, 0x00},	/* play */
	{0x00, 0x3C, 0x3C, 0x3C, 0x00},	/* stop */
	{0x3C, 0x3C, 0x00, 0x3C, 0x3C},	/* pause */
	{0xFE, 0x83, 0x83, 0x83, 0xFE},	/* battery  0/5 */
	{0xFE, 0xC3, 0xC3, 0xC3, 0xFE},	/* battery  1/5 */
	{0xFE, 0xE3, 0xE3, 0xE3, 0xFE},	/* battery  2/5 */
	{0xFE, 0xF3, 0xF3, 0xF3, 0xFE},	/* battery  3/5 */
	{0xFE, 0xFB, 0xFB, 0xFB, 0xFE},	/* battery  4/5 */
	{0xFE, 0xFF, 0xFF, 0xFF, 0xFE},	/* battery  5/5 */
	{0x80, 0x80, 0x80, 0x80, 0x00},	/* level  1/8 */
	{0xC0, 0xC0, 0xC0, 0xC0, 0x00},	/* level  2/8 */
	{0xE0, 0xE0, 0xE0, 0xE0, 0x00},	/* level  3/8 */
	{0xF0, 0xF0, 0xF0, 0xF0, 0x00},	/* level  4/8 */
	{0xF8, 0xF8, 0xF8, 0xF8, 0x00},	/* level  5/8 */
	{0xFC, 0xFC, 0xFC, 0xFC, 0x00},	/* level  6/8 */
	{0xFE, 0xFE, 0xFE, 0xFE, 0x00},	/* level  7/8 */
	{0xFF, 0xFF, 0xFF, 0xFF, 0x00},	/* level  8/8 */
};

static uint8_t	reverse_mode = _BF4_MODE_NORMAL;

typedef struct {
	uint8_t x;
	uint8_t y;
} position_t;

static position_t _bf4_current_position = {
	.x = 0,
	.y = 0
};

/**
 * @fn void bf4_init(void)
 * @brief bf4 LCD initialisation
 */
void
bf4_init(void)
{
	/** @note Alcatel bf4 LCD use S1D15B01  controller
	*/
	BF4_POWER = 1;

	BF4_DDR = 0xFE;

	BF4_A0=1;
	BF4_CS=1;					// chip disabled

	_delay_us(80);
	BF4_RES=0;				// reset chip during 250ms
	_delay_us(96);		// max (in us) = 768 / F_CPU (in Mhz) => at 8Mhz, 96 us
	BF4_RES=1;


//	command description ; display normal / reverse
	bf4_send_command(0xA6);		// display normal

//	command description ; segment driver direction select
	bf4_send_command(0xA0);		// normal

//	command description ; common driver direction select
	bf4_send_command(0xC8);		// reverse

//	command description ; lcd bias set
//	bf4_send_command(0xA2);		// 1/9 bias

//	command description ; power supply circuit & command description ; vo_resistor ratio set
	bf4_send_command(0x24);		// internal resistor

//	command description ; power supply circuit & command description ; electronic volume
	bf4_send_command(0x82);		// contrast volume x81

//	power control set
	bf4_send_command(0x2F);		// booster=on ; voltage regulator=on ; voltage follower=on

//	display ON/OFF	
	bf4_send_command(0xAF);		// ON

//	display all points ON/OFF
	bf4_send_command(0xA5);		// all points ON

	_delay_ms(100);	
/*	_delay_ms(200);*/	
// 	_delay_ms(200);	
// 	_delay_ms(200);	

//	display all points ON/OFF
	bf4_send_command(0xA4);		// normal display

	bf4_send_command(0x40);		// display start line address set
}

void
bf4_send_command(const uint8_t command)
{
	BF4_A0 = 0;		/* a command it is read with
				 * the eight SCLK pulse */
	BF4_CS = 0;		/* chip enabled */
	bf4_write(command);
	BF4_CS = 1;		/* chip disabled */
}

void
bf4_send_data(const uint8_t data)
{
	BF4_A0 = 1;
	BF4_CS = 0;		/* chip enabled */
	bf4_write(data ^ reverse_mode);
	BF4_CS = 1;		/* chip disabled */

	_bf4_current_position.x++;
	_bf4_current_position.x %= BF4_SCREEN_WIDTH;
}

void
bf4_write(uint8_t data)
{
	uint8_t 	bit;
	for (bit = 8; bit > 0; bit--) {
		BF4_SCLK = 0;

		if (data & 0x80) {
			BF4_SDA = 1;
		} else {
			BF4_SDA = 0;
		}
		BF4_SCLK = 1;
// 		_delay_us(1);

		data = data << 1;
	}
}

/**
 * @fn void bf4_gotoxy (const uint8_t x, const uint8_t y)
 * @brief Move cursor position to (x,y)
 * @param x x position in pixels
 * @param y y position in lines
 */
void
bf4_gotoxy(const uint8_t x, const uint8_t y)
{
				/* BF4 LCD Position cursor */
	bf4_send_command(0xB0|( y & 0x0F ));    // Y axe initialisation: 1011 yyyy	

	bf4_send_command(0x10|( ((x + 18) >> 4) & 0x0F ));    // X axe initialisation: 000x xxxx
	bf4_send_command(0x00|( (x + 18) & 0x0F ));    // X axe initialisation: 000x xxxx

	_bf4_current_position.x = x;
	_bf4_current_position.y = y;
}

void
_bf4_display_normal_char(const char ascii)
{
	uint8_t 	data[5];
	memcpy_P(data, ascii_table[ascii - 32], 5);

	bf4_send_data(0x00);	/* Display a blank vertical line */
	bf4_send_data(data[0]);
	bf4_send_data(data[1]);
	bf4_send_data(data[2]);
	bf4_send_data(data[3]);
	bf4_send_data(data[4]);
}

void
_bf4_display_big_char(const char ascii)
{
	const position_t current_position = _bf4_current_position;

	uint8_t	data[5];
	uint8_t big_upper_data[5];
	uint8_t big_lower_data[5];

	memcpy_P(data, ascii_table[ascii - 32], 5);

	for(uint8_t i = 0; i < 5; i++) {
		big_upper_data[i] = 0x00;
		for(uint8_t j = 0; j < 4; j++) {
			if( _BV(j) & data[i]) {
				big_upper_data[i] |= _BV(j*2);
				big_upper_data[i] |= _BV((j*2)+1);
			}
		}
		big_lower_data[i] = 0x00;
		for(uint8_t j = 4; j < 8; j++) {
			if( _BV(j) & data[i]) {
				big_lower_data[i] |= _BV((j-4)*2);
				big_lower_data[i] |= _BV(((j-4)*2)+1);
			}
		}
	}

	bf4_send_data(0x00);	/* Display a blank vertical line */
	bf4_send_data(0x00);	/* Display a blank vertical line */

	bf4_send_data(big_upper_data[0]);
	bf4_send_data(big_upper_data[0]);
	bf4_send_data(big_upper_data[1]);
	bf4_send_data(big_upper_data[1]);
	bf4_send_data(big_upper_data[2]);
	bf4_send_data(big_upper_data[2]);
	bf4_send_data(big_upper_data[3]);
	bf4_send_data(big_upper_data[3]);
	bf4_send_data(big_upper_data[4]);
	bf4_send_data(big_upper_data[4]);

	bf4_gotoxy(current_position.x, current_position.y+1);

	bf4_send_data(0x00);	/* Display a blank vertical line */
	bf4_send_data(0x00);	/* Display a blank vertical line */

	bf4_send_data(big_lower_data[0]);
	bf4_send_data(big_lower_data[0]);
	bf4_send_data(big_lower_data[1]);
	bf4_send_data(big_lower_data[1]);
	bf4_send_data(big_lower_data[2]);
	bf4_send_data(big_lower_data[2]);
	bf4_send_data(big_lower_data[3]);
	bf4_send_data(big_lower_data[3]);
	bf4_send_data(big_lower_data[4]);
	bf4_send_data(big_lower_data[4]);

	bf4_gotoxy((current_position.x+12)%BF4_SCREEN_WIDTH, current_position.y);
}

void
_bf4_display_normal_symbol(const char symbol)
{
	unsigned char 	data[5];
	memcpy_P(data, symbol_table[symbol + 0], 5);

	bf4_send_data(0x00);	/* Display a blank vertical line */
	bf4_send_data(data[0]);
	bf4_send_data(data[1]);
	bf4_send_data(data[2]);
	bf4_send_data(data[3]);
	bf4_send_data(data[4]);
}

void
_bf4_display_big_symbol(const char symbol)
{
	const position_t current_position = _bf4_current_position;

	unsigned char	data[5];
	uint8_t big_upper_data[5];
	uint8_t big_lower_data[5];

	memcpy_P(data, symbol_table[symbol + 0], 5);

	for(uint8_t i = 0; i < 5; i++) {
		big_upper_data[i] = 0x00;
		for(uint8_t j = 0; j < 4; j++) {
			if( _BV(j) & data[i]) {
				big_upper_data[i] |= _BV(j*2);
				big_upper_data[i] |= _BV((j*2)+1);
			}
		}
		big_lower_data[i] = 0x00;
		for(uint8_t j = 4; j < 8; j++) {
			if( _BV(j) & data[i]) {
				big_lower_data[i] |= _BV((j-4)*2);
				big_lower_data[i] |= _BV(((j-4)*2)+1);
			}
		}
	}

	bf4_send_data(0x00);	/* Display a blank vertical line */
	bf4_send_data(0x00);	/* Display a blank vertical line */

	bf4_send_data(big_upper_data[0]);
	bf4_send_data(big_upper_data[0]);
	bf4_send_data(big_upper_data[1]);
	bf4_send_data(big_upper_data[1]);
	bf4_send_data(big_upper_data[2]);
	bf4_send_data(big_upper_data[2]);
	bf4_send_data(big_upper_data[3]);
	bf4_send_data(big_upper_data[3]);
	bf4_send_data(big_upper_data[4]);
	bf4_send_data(big_upper_data[4]);

	bf4_gotoxy(current_position.x, current_position.y+1);

	bf4_send_data(0x00);	/* Display a blank vertical line */
	bf4_send_data(0x00);	/* Display a blank vertical line */

	bf4_send_data(big_lower_data[0]);
	bf4_send_data(big_lower_data[0]);
	bf4_send_data(big_lower_data[1]);
	bf4_send_data(big_lower_data[1]);
	bf4_send_data(big_lower_data[2]);
	bf4_send_data(big_lower_data[2]);
	bf4_send_data(big_lower_data[3]);
	bf4_send_data(big_lower_data[3]);
	bf4_send_data(big_lower_data[4]);
	bf4_send_data(big_lower_data[4]);

	bf4_gotoxy((current_position.x+12)%BF4_SCREEN_WIDTH, current_position.y);
}

void
bf4_display_char(const char ascii)
{
	_bf4_display_char(ascii);
}

void
bf4_display_symbol(const char symbol)
{
	_bf4_display_symbol(symbol);
}

void
bf4_display_string(const char *string)
{
	char c;
	while ((c = pgm_read_byte (string++)))
		bf4_display_char(c);
}

void
bf4_clear(void)
{
	for(uint8_t row = 0; row < BF4_SCREEN_ROWS; row++) {
		bf4_gotoxy(0, row);
		for(uint8_t pix = 0; pix < BF4_SCREEN_WIDTH; pix++) {
			bf4_send_data(0x00);
		}
	}

	bf4_gotoxy(0,0);
}

void
bf4_set_mode(uint8_t mode)
{
	reverse_mode = (mode) ? _BF4_MODE_INVERTED : _BF4_MODE_NORMAL;
}

void
bf4_set_size(uint8_t size)
{
	if( size == _BF4_SIZE_BIG ) {
		_bf4_display_char = _bf4_display_big_char;
		_bf4_display_symbol = _bf4_display_big_symbol;
	} else {
		_bf4_display_char = _bf4_display_normal_char;
		_bf4_display_symbol = _bf4_display_normal_symbol;
	}
}

void
bf4_finish_line(void)
{
	while (0 != _bf4_current_position.x) {
		bf4_display_char(' ');
	}
}
