/* phase-switcher-bricklet
 * Copyright (C) 2024 Thomas Hein <github@poohnet.de>
 *
 * ac_in.h: Driver for AC input and LEDs 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/utility/led_flicker.h"

#define AC_IN_CHANNEL_NUM 2

#define AC_IN_CH0_PIN P2_2
#define AC_IN_CH1_PIN P2_9
#define AC_IN_LED_CH0_PIN P0_8
#define AC_IN_LED_CH1_PIN P0_9

typedef struct {
  bool value;
  bool last_value;
  uint32_t last_change;

  LEDFlickerState led_flicker_state;
} ACIn;

typedef struct {
  XMC_GPIO_PORT_t* port;
  const uint8_t pin;
} ACInLED;

extern ACIn ac_in[AC_IN_CHANNEL_NUM];
extern ACInLED ac_in_led[AC_IN_CHANNEL_NUM];

void ac_in_init();
void ac_in_tick();
