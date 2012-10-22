# Copyright (c) 2004,2005  Theodore A. Roth
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
# * Neither the name of the copyright holders nor the names of
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#
OBJCOPY=$(AVROBJCOPY)

noinst_PROGRAMS = comeos.elf comeos.hex

comeos.hex: comeos.elf
	$(OBJCOPY) -R .eeprom -O ihex comeos.elf comeos.hex

comeos_elf_SOURCES = \
	app_exposition.c \
	app_interval.c \
	app_light.c \
	app_mainmenu.c \
	app_shots.c \
	app_sound.c \
	app_state.c \
	comeos.c \
	drv_battery.c \
	drv_bf4.c \
	drv_buzzer.c \
	drv_keyboard.c \
	drv_light.c \
	drv_pwm1.c \
	lcd.c \
	menus.c \
	scheduler.c \
	windowmanager.c

comeos_headers = 	\
	app_exposition.h \
	app_interval.h \
	application.h \
	app_light.h \
	app_mainmenu.h \
	app_shots.h \
	app_sound.h \
	app_state.h \
	bit_field.h \
	comeos.h \
	drv_battery.h \
	drv_bf4.h \
	drv_buzzer.h \
	drv_keyboard.h \
	drv_light.h \
	drv_pwm1.h \
	lcd.h \
	menus.h \
	scheduler.h \
	windowmanager.h 

EXTRA_DIST = \
	$(comeos_headers)

