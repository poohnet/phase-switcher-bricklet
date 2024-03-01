/* phase-switcher-bricklet
 * Copyright (C) 2024 Thomas Hein <github@poohnet.de>
 *
 * dc_relay.c: DC Relay driver
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

#include "dc_relay.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "communication.h"

DCRelay dc_relay;

bool dc_relay_get_value()
{
  return !XMC_GPIO_GetInput(DC_RELAY_PIN);
}

void dc_relay_set_value(bool value)
{
  XMC_GPIO_SetOutputLevel(DC_RELAY_PIN, value ? XMC_GPIO_OUTPUT_LEVEL_LOW : XMC_GPIO_OUTPUT_LEVEL_HIGH);
}

void dc_relay_init()
{
  const XMC_GPIO_CONFIG_t channel_config = {
    .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  };

  XMC_GPIO_Init(DC_RELAY_PIN, &channel_config);

  const XMC_GPIO_CONFIG_t channel_led_config = {
    .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  };

  XMC_GPIO_Init(DC_RELAY_LED_PIN, &channel_led_config);

  dc_relay.led_flicker_state.config = PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS;
}

void dc_relay_tick()
{
  switch (dc_relay.led_flicker_state.config) {
    case PHASE_SWITCHER_CHANNEL_LED_CONFIG_OFF:
      XMC_GPIO_SetOutputLevel(DC_RELAY_LED_PIN, XMC_GPIO_OUTPUT_LEVEL_HIGH);
      break;

    case PHASE_SWITCHER_CHANNEL_LED_CONFIG_ON:
      XMC_GPIO_SetOutputLevel(DC_RELAY_LED_PIN, XMC_GPIO_OUTPUT_LEVEL_LOW);
      break;

    case PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_HEARTBEAT:
      led_flicker_tick(&dc_relay.led_flicker_state, system_timer_get_ms(), DC_RELAY_LED_PIN);
      break;

    case PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS:
      XMC_GPIO_SetOutputLevel(DC_RELAY_LED_PIN, dc_relay_get_value() ? XMC_GPIO_OUTPUT_LEVEL_LOW : XMC_GPIO_OUTPUT_LEVEL_HIGH);
      break;

    default:
      break;
  }
}
