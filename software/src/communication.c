/* phase-switcher-bricklet
 * Copyright (C) 2024 Thomas Hein <github@poohnet.de>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"
#include "ac_in.h"
#include "ac_relay.h"
#include "dc_relay.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"

BootloaderHandleMessageResponse handle_message(const void* message, void* response)
{
  const uint8_t length = ((TFPMessageHeader*)message)->length;

  switch(tfp_get_fid_from_message(message)) {
    case FID_SET_CONTROL_PILOT_DISCONNECT: return length != sizeof(SetControlPilotDisconnect) ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_control_pilot_disconnect(message);
    case FID_SET_PHASES_WANTED:            return length != sizeof(SetPhasesWanted)           ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_phases_wanted(message);
    case FID_GET_ALL_DATA:                 return length != sizeof(GetAllData)                ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_all_data(message, response);
    default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
  }
}


BootloaderHandleMessageResponse set_control_pilot_disconnect(const SetControlPilotDisconnect* data)
{
  dc_relay_set_value(!data->control_pilot_disconnect);

  return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_phases_wanted(const SetPhasesWanted* data)
{
  switch (data->phases_wanted) {
    case 0:
      ac_relay_set_value(0, false);
      ac_relay_set_value(1, false);
      break;

    case 1:
      ac_relay_set_value(0, true);
      ac_relay_set_value(1, false);
      break;

    case 3:
      ac_relay_set_value(0, true);
      ac_relay_set_value(1, true);
      break;

    default:
      return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
  }

  return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_all_data(const GetAllData* data, GetAllData_Response* response)
{
  response->header.length = sizeof(GetAllData_Response);
  response->is_control_pilot_disconnected = !dc_relay_get_value();
  response->phases_wanted = ((ac_relay_get_value(0) ? 1 : 0) | (ac_relay_get_value(1) ? 2 : 0));
  response->phases_active = ((ac_in[0].value ? 1 : 0) | (ac_in[1].value ? 2 : 0));

  return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}





void communication_tick()
{
  communication_callback_tick();
}

void communication_init()
{
  communication_callback_init();
}
