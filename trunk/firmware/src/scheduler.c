#include "scheduler.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "scheduler.h"

#define SCHEDULER_MAX_HOOK_FCT		10

void scheduler_process_hooks ( void );

typedef void ( *_scheduler_hook_fct ) ( void );

static volatile uint8_t	_scheduler_hook_fct_count = 0;
static volatile _scheduler_hook_fct _scheduler_hook_fcts[SCHEDULER_MAX_HOOK_FCT];
static volatile _scheduler_hook_fct _scheduler_hook_high_priority_fct = 0;

#define TIMER0_CNT (255 - 30)

static volatile uint8_t scheduler_software_counter;

ISR ( TIMER2_COMP_vect, ISR_NOBLOCK )
{
	TIFR |= _BV( OCF2 );	// OCF2 = 1
	TIMSK |= _BV ( OCIE2 );	/* Enable interrupt */

	scheduler_software_counter++;
	if ( scheduler_software_counter == 25 ) {
		// Enter here at 250hz / 25
		scheduler_software_counter=0;
		if ( _scheduler_hook_high_priority_fct != 0 ) {
			_scheduler_hook_high_priority_fct();
		}
	}
// 	scheduler_process_hooks();
}

ISR ( TIMER0_OVF_vect, ISR_NOBLOCK )
{
	TCNT0 = TIMER0_CNT;
	TIFR |=  _BV( TOV0 );
	TIMSK |= _BV ( TOIE0 );
	scheduler_process_hooks();	
}


void
scheduler_init ( void )
{
	/* Timer0 init */
	TCCR0 = _BV( CS02 ) | _BV( CS00 ); /* prescale = 1024 */
	TCNT0 = TIMER0_CNT;

	/* Timer2 init */
	TCCR2 = _BV ( WGM21 ) | _BV ( CS21 ) | _BV ( CS20 );		//mode CTC ; clk = 1Mhz, prescal = 32 soit 31,250Khz
	TCNT2 = 0x00;
	OCR2 = 124;							// Match at 31250hz / 125 = 250hz

	/* Timers interrupts register */
	TIFR |= _BV( TOV0 ) | _BV( OCF2 );	/* Enable Timer0 overflow flag and Timer2 output compare flag */
	TIMSK |= _BV ( TOIE0 ) | _BV( OCIE2 ); /* Enable Timer0 interrupt and Timer2 interrupt */
}

void
scheduler_hook_high_priority_fct ( void ( *fct ) ( void ) )
{
	_scheduler_hook_high_priority_fct = fct;
}

void
scheduler_add_hook_fct ( void ( *fct ) ( void ) )
{
	_scheduler_hook_fcts[_scheduler_hook_fct_count++] = fct;
}

void
scheduler_process_hooks ( void )
{
	for ( uint8_t i = 0; i < _scheduler_hook_fct_count; i++ )
	{
		_scheduler_hook_fcts[i] ();
	}
}

void
scheduler_suspend(void)
{
	/* Timers interrupts register */
	TIFR &= ~(_BV( TOV0 ) | _BV( OCF2 ));	/* Disable Timer0 overflow flag and Timer2 output compare flag */
	TIMSK &= ~(_BV ( TOIE0 ) | _BV( OCIE2 )); /* Disable Timer0 interrupt and Timer2 interrupt */
}

void
scheduler_resume(void)
{
	/* Timers interrupts register */
	TIFR |= _BV( TOV0 ) | _BV( OCF2 );	/* Enable Timer0 overflow flag and Timer2 output compare flag */
	TIMSK |= _BV ( TOIE0 ) | _BV( OCIE2 ); /* Enable Timer0 interrupt and Timer2 interrupt */
}
