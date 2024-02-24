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

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
  const uint8_t length = ((TFPMessageHeader*)message)->length;
  switch(tfp_get_fid_from_message(message)) {
    case FID_SET_CONTROL_PILOT_DISCONNECT: return length != sizeof(SetControlPilotDisconnect) ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_control_pilot_disconnect(message, response);
    case FID_GET_CONTROL_PILOT_DISCONNECT: return length != sizeof(GetControlPilotDisconnect) ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_control_pilot_disconnect(message, response);
    case FID_SET_PHASES:                   return length != sizeof(SetPhases)                 ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_phases(message);
    case FID_GET_PHASES:                   return length != sizeof(GetPhases)                 ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_phases(message, response);
    default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
  }
}


BootloaderHandleMessageResponse set_control_pilot_disconnect(const SetControlPilotDisconnect *data, SetControlPilotDisconnect_Response *response) {
  response->header.length = sizeof(SetControlPilotDisconnect_Response);

  return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_control_pilot_disconnect(const GetControlPilotDisconnect *data, GetControlPilotDisconnect_Response *response) {
  response->header.length = sizeof(GetControlPilotDisconnect_Response);

  return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_phases(const SetPhases *data) {

  return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_phases(const GetPhases *data, GetPhases_Response *response) {
  response->header.length = sizeof(GetPhases_Response);

  return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}





void communication_tick(void) {
  communication_callback_tick();
}

void communication_init(void) {
  communication_callback_init();
}
