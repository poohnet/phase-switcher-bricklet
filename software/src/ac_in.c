/* phase-switcher-bricklet
 * Copyright (C) 2024 Thomas Hein <github@poohnet.de>
 *
 * ac_in.c: Driver for AC input and LEDs 
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

#include "ac_in.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "communication.h"

ACIn ac_in[AC_IN_CHANNEL_NUM];

ACInLED ac_in_led[AC_IN_CHANNEL_NUM] = {
  {AC_IN_LED_CH0_PIN},
  {AC_IN_LED_CH1_PIN}
};

void ac_in_init()
{
  const XMC_GPIO_CONFIG_t channel_config = {
    .mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
    .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD,
  };

  XMC_GPIO_Init(AC_IN_CH0_PIN, &channel_config);
  XMC_GPIO_Init(AC_IN_CH1_PIN, &channel_config);

  const XMC_GPIO_CONFIG_t channel_led_config = {
    .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
    .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
  };

  XMC_GPIO_Init(AC_IN_LED_CH0_PIN, &channel_led_config);
  XMC_GPIO_Init(AC_IN_LED_CH1_PIN, &channel_led_config);

  // Initialize current value, last value and LED states
  ac_in[0].value = XMC_GPIO_GetInput(AC_IN_CH0_PIN);
  ac_in[1].value = XMC_GPIO_GetInput(AC_IN_CH1_PIN);

  ac_in[0].last_value = ac_in[0].value;
  ac_in[1].last_value = ac_in[1].value;

  ac_in[0].last_change = 0;
  ac_in[1].last_change = 0;

  ac_in[0].led_flicker_state.config = PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS;
  ac_in[1].led_flicker_state.config = PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS;
}

void ac_in_tick()
{
  // Handle AC input
  bool new_value [2] = {
    XMC_GPIO_GetInput(AC_IN_CH0_PIN),
    XMC_GPIO_GetInput(AC_IN_CH1_PIN)
  };

  for (uint8_t ch = 0; ch < AC_IN_CHANNEL_NUM; ch++) {
    if (new_value[ch] != ac_in[ch].last_value) {
      ac_in[ch].last_value  = new_value[ch];
      ac_in[ch].last_change = system_timer_get_ms();
      ac_in[ch].value       = true;
    }

    // At 50Hz we should see a change every 20ms
    // Using 100ms without change as indicator for "no AC voltage connected"
    if (system_timer_is_time_elapsed_ms(ac_in[ch].last_change, 100)) {
      // re-set last change to 50ms in the past to make sure we never
      // get a false positive because of the uint32 overflow
      ac_in[ch].last_change = system_timer_get_ms() - 150;
      ac_in[ch].value       = false;
    }
  }

  // Handle LEDs
  for (uint8_t ch = 0; ch < AC_IN_CHANNEL_NUM; ch++) {
    switch (ac_in[ch].led_flicker_state.config) {
      case PHASE_SWITCHER_CHANNEL_LED_CONFIG_OFF:
        XMC_GPIO_SetOutputLevel(ac_in_led[ch].port, ac_in_led[ch].pin, XMC_GPIO_OUTPUT_LEVEL_HIGH);
        break;

      case PHASE_SWITCHER_CHANNEL_LED_CONFIG_ON:
        XMC_GPIO_SetOutputLevel(ac_in_led[ch].port, ac_in_led[ch].pin, XMC_GPIO_OUTPUT_LEVEL_LOW);
        break;

      case PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_HEARTBEAT:
        led_flicker_tick(&ac_in[ch].led_flicker_state, system_timer_get_ms(), ac_in_led[ch].port, ac_in_led[ch].pin);
        break;

      case PHASE_SWITCHER_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS:
        XMC_GPIO_SetOutputLevel(ac_in_led[ch].port, ac_in_led[ch].pin, ac_in[ch].value ? XMC_GPIO_OUTPUT_LEVEL_LOW : XMC_GPIO_OUTPUT_LEVEL_HIGH);
        break;

      default:
        break;
    }
  }
}
