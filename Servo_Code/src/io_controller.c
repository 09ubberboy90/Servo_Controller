/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
 *******************************************************************************/

/** INCLUDES *******************************************************/
#include "system.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "io_controller.h"
#include "io.h"
//#include "magnetometer.h"
#include "servo.h"
#include "interupts.h"
#include "system.h"

/** VARIABLES ******************************************************/

bool autoMode = false;

/*********************************************************************
 * Function: void APP_DeviceCDCBasicDemoInitialize(void);
 *
 * Overview: Initializes the demo code
 *
 * PreCondition: None
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void Io_Init()
{
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 9600;


}

/*********************************************************************
 * Function: void APP_DeviceCDCBasicDemoTasks(void);
 *
 * Overview: Keeps the demo running.
 *
 * PreCondition: The demo should have been initialized and started via
 *   the APP_DeviceCDCBasicDemoInitialize() and APP_DeviceCDCBasicDemoStart() demos
 *   respectively.
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void Io_Listener()
{
    uint8_t input[10];
    char tmp[50];
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if (USBGetDeviceState() < CONFIGURED_STATE)
    {
        return;
    }
    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if (USBIsDeviceSuspended() == true)
    {
        return;
    }

    if (USBUSARTIsTxTrfReady() == true)
    {
        uint8_t numBytesRead = 0;
        //numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));
        numBytesRead = getsUSBUSART(input, sizeof (input));

        /* For every byte that was read... */
        if (numBytesRead != 0)
        {
            switch (input[0])
            {

            case 0x52: //R 
            case 0x72: //r
                Servo_Value();
                break;

            case 0x53: //S 
            case 0x73: //s
                Servo_Control();
                break;

            case 0x0D:
            case 0x0A:
                break;
            default:
                Send_Message("ERROR main\n\r");
                break;
            }
        }
    }
    CDCTxService();
}

