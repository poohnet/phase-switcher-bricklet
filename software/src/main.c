/* phase-switcher-bricklet
 * Copyright (C) 2024 Thomas Hein <github@poohnet.de>
 *
 * main.c: Initialization for Phase Switcher Bricklet
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

#include <stdio.h>
#include <stdbool.h>

#include "configs/config.h"

#include "bricklib2/bootloader/bootloader.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "communication.h"

#include "ac_in.h"
#include "ac_relay.h"
#include "dc_relay.h"

int main(void)
{
  ac_in_init();
  ac_relay_init();
  dc_relay_init();
  communication_init();

  while (true) {
    bootloader_tick();
    communication_tick();
    ac_in_tick();
    ac_relay_tick();
    dc_relay_tick();
  }
}
