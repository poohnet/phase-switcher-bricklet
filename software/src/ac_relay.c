/* phase-switcher-bricklet
 * Copyright (C) 2024 Thomas Hein <github@poohnet.de>
 *
 * ac_relay.c: AC Relay driver
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

#include "ac_relay.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "communication.h"

ACRelay ac_relay[AC_RELAY_CHANNEL_NUM];

ACRelayLED ac_relay_led[AC_RELAY_CHANNEL_NUM] = {
  {AC_RELAY_CH0_LED_PIN},
  {AC_RELAY_CH1_LED_PIN}
};

bool ac_relay_get_value(uint8_t channel)
{
  switch (channel) {
    case 0:
      return XMC_GPIO_GetInput(AC_RELAY_CH0_PIN);

    case 1:
      return XMC_GPIO_GetInput(AC_RELAY_CH1_PIN);

    default:
      break;
  }

  return false;
}

void ac_relay_set_value(uint8_t channel, bool value)
{
  switch (channel) {
    case 0:
      XMC_GPIO_SetOutputLevel(AC_RELAY_CH0_PIN, value ? XMC_GPIO_OUTPUT_LEVEL_HIGH : XMC_GPIO_OUTPUT_LEVEL_LOW);
      break;

    case 1:
      XMC_GPIO_SetOutputLevel(AC_RELAY_CH1_PIN, value ? XMC_GPIO_OUTPUT_LEVEL_HIGH : XMC_GPIO_OUTPUT_LEVEL_LOW);
      break;

    default:
      break;
  }
}

void ac_relay_init()
{
  const XMC_GPIO_CONFIG_t channel_config = {
    .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
  };

  XMC_GPIO_Init(AC_RELAY_CH0_PIN, &channel_config);
  XMC_GPIO_Init(AC_RELAY_CH1_PIN, &channel_config);

  const XMC_GPIO_CONFIG_t channel_led_config = {
    .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  };

  XMC_GPIO_Init(AC_RELAY_CH0_LED_PIN, &channel_led_config);
  XMC_GPIO_Init(AC_RELAY_CH1_LED_PIN, &channel_led_config);

  ac_relay[0].led_flicker_state.config = PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS;
  ac_relay[1].led_flicker_state.config = PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS;
}

void ac_relay_tick()
{
  for (uint8_t ch = 0; ch < AC_RELAY_CHANNEL_NUM; ch++) {
    switch (ac_relay[ch].led_flicker_state.config) {
      case PHASE_SWITCHER_CHANNEL_LED_CONFIG_OFF:
        XMC_GPIO_SetOutputLevel(ac_relay_led[ch].port, ac_relay_led[ch].pin, XMC_GPIO_OUTPUT_LEVEL_HIGH);
        break;
  
      case PHASE_SWITCHER_CHANNEL_LED_CONFIG_ON:
        XMC_GPIO_SetOutputLevel(ac_relay_led[ch].port, ac_relay_led[ch].pin, XMC_GPIO_OUTPUT_LEVEL_LOW);
        break;
  
      case PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_HEARTBEAT:
        led_flicker_tick(&ac_relay[ch].led_flicker_state, system_timer_get_ms(), ac_relay_led[ch].port, ac_relay_led[ch].pin);
        break;
  
      case PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS:
        XMC_GPIO_SetOutputLevel(ac_relay_led[ch].port, ac_relay_led[ch].pin, ac_relay_get_value(ch) ? XMC_GPIO_OUTPUT_LEVEL_LOW : XMC_GPIO_OUTPUT_LEVEL_HIGH);
        break;
  
      default:
        break;
    }
  }
}
