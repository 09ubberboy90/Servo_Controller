#include <xc.h>
#include "servo.h"
#include "interupts.h"
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "system.h"
Servo_Pin_Control main_mvt;
uint8_t servo_nb = 0;
void Servo_Init()
{
    main_mvt.nb_servo = NB_OF_SERVO;
    PIN tmp_pin[NB_OF_SERVO] = {SERVO_1, SERVO_2, SERVO_3, SERVO_4, SERVO_5, SERVO_6};
    memcpy(main_mvt.pin, tmp_pin, sizeof (tmp_pin));
    //float tmp_time[NB_OF_SERVO] = {90, 90, 90, 90, 90, 90};
    uint16_t tmp_time[NB_OF_SERVO] = {2250, 3000, 2250, 2250, 2250, 2250};

    memcpy(main_mvt.time, tmp_time, sizeof (tmp_time));

}

void Servo_Value()
{
    uint8_t input[4];
    uint16_t result;
    char mess[37];
    bool error_flag = false;
    uint8_t numBytesRead = 0;
    uint8_t readBuffer[50];

    for (int i = 0; i < 4; i++)
    {
        do
        {
            numBytesRead = getsUSBUSART(readBuffer, sizeof (readBuffer));
        }
        while (numBytesRead == 0);
        input[i] = readBuffer[0];
        if (input[i]< '0' || input[i] > '9')
        {
            error_flag = true;
            break;
        }
    }
    if (input[0] >= '0' && input[0] < '6' && !error_flag)
    {
        result = (uint16_t)((input[1] - 0x30)*100 + (input[2] - 0x30)*10+(input[3] - 0x30))*16.6+750;
        if (result >= 750 && result <= 3750) // Prevent error based on wrong timing
        {
            input[0] = (uint8_t) input[0] - 0x30;
            servo_nb = input[0] + 2;
            T1CONbits.TMR1ON = 0; // stop the timer
            main_mvt.time[input[0]] = result;
            T1CONbits.TMR1ON = 1; // stop the timer
            sprintf(mess, "%s set with %*u clock cycles\r\n", pin_name[servo_nb],4, result);
        }
        else
        {
            error_flag = true;
        }
    }
    if (error_flag)
    {
        sprintf(mess, "Error Rotor\r\n");
        error_flag = false;
    }
    Send_Message(mess);
    
}

Servo_Pin_Control * Get_Gesture(void)
{
    return &main_mvt;
}

void Servo_Control()
{
    uint8_t input[10];
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    uint8_t numBytesRead = 0;
    //numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));
    do
    {
        numBytesRead = getsUSBUSART(input, sizeof (input));
    }
    while (numBytesRead == 0);

    /* For every byte that was read... */
    if (numBytesRead != 0)
    {
        switch (input[0])
        {
        case 0x45://E
        case 0x65://e
            T1CONbits.TMR1ON = 1;
            Send_Message("Servo Enabled\r\n");
            break;
        case 0x44://D
        case 0x64://d
            T1CONbits.TMR1ON = 0;
            Send_Message("Servo Disabled\r\n");
            break;
        case 0x53://S
        case 0x73://s
            Send_Message("Message Disable\r\n");
            message_display = false;
            break;
        case 0x56://V
        case 0x76://v
            Send_Message("Message Enable\r\n");
            message_display = true;

            break;

        default:
            Send_Message("Error Servo\r\n");
            break;
        }
    }

}
