/* phase-switcher-bricklet
 * Copyright (C) 2024 Thomas Hein <github@poohnet.de>
 *
 * ac_relay.h: AC Relay driver
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

#define AC_RELAY_CHANNEL_NUM 2

#define AC_RELAY_CH0_PIN P1_0
#define AC_RELAY_CH1_PIN P1_1
#define AC_RELAY_CH0_LED_PIN P0_6
#define AC_RELAY_CH1_LED_PIN P0_7

typedef struct {
  LEDFlickerState led_flicker_state;
} ACRelay;

typedef struct {
  XMC_GPIO_PORT_t* port;
  const uint8_t pin;
} ACRelayLED;

extern ACRelay ac_relay[AC_RELAY_CHANNEL_NUM];
extern ACRelayLED ac_relay_led[AC_RELAY_CHANNEL_NUM];

bool ac_relay_get_value(uint8_t channel);
void ac_relay_set_value(uint8_t channel, bool value);

void ac_relay_init();
void ac_relay_tick();
