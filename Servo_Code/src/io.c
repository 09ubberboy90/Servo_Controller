/********************************************************************
 Software License Agreement:
 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.
 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

#include <io.h>
#include <stdbool.h>
#include <xc.h>
#include <pic18f45k50.h>


//MOTORA and MOTORB are useless since they are enable by pwm
#define SERVO_1_TRIS TRISDbits.TRISD0
#define SERVO_2_TRIS TRISDbits.TRISD1
#define SERVO_3_TRIS TRISDbits.TRISD2
#define SERVO_4_TRIS TRISDbits.TRISD3
#define SERVO_5_TRIS TRISDbits.TRISD4
#define SERVO_6_TRIS TRISDbits.TRISD5
#define SERVO_7_TRIS TRISDbits.TRISD6
#define SERVO_8_TRIS TRISDbits.TRISD7

#define SERVO_1_LAT LATDbits.LATD0
#define SERVO_2_LAT LATDbits.LATD1
#define SERVO_3_LAT LATDbits.LATD2
#define SERVO_4_LAT LATDbits.LATD3
#define SERVO_5_LAT LATDbits.LATD4
#define SERVO_6_LAT LATDbits.LATD5
#define SERVO_7_LAT LATDbits.LATD6
#define SERVO_8_LAT LATDbits.LATD7

#define SERVO_1_ANSEL ANSELDbits.ANSD0
#define SERVO_2_ANSEL ANSELDbits.ANSD1
#define SERVO_3_ANSEL ANSELDbits.ANSD2
#define SERVO_4_ANSEL ANSELDbits.ANSD3
#define SERVO_6_ANSEL ANSELDbits.ANSD5
#define SERVO_7_ANSEL ANSELDbits.ANSD6
#define SERVO_8_ANSEL ANSELDbits.ANSD7

#define ON  1
#define OFF 0

#define PIN_INPUT           1
#define PIN_OUTPUT          0

#define PIN_DIGITAL         0
#define PIN_ANALOG          1


void Pin_Init()
{
    for (int pin = PIN_FIRST; pin < PIN_LAST; ++pin)
    {
        Pin_Enable(pin);
    }

}


/*********************************************************************
* Function: void LED_On(LED led);
*
* Overview: Turns requested LED on
*
* PreCondition: LED configured via LED_Configure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_On(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void Pin_On(PIN led)
{
    switch(led)
    {
        case SERVO_1:
            SERVO_1_LAT = ON;
            break;
        case SERVO_2:
            SERVO_2_LAT = ON;
            break;
        case SERVO_3:
            SERVO_3_LAT = ON;
            break;
        case SERVO_4:
            SERVO_4_LAT = ON;
            break;
        case SERVO_5:
            SERVO_5_LAT = ON;
            break;
        case SERVO_6:
            SERVO_6_LAT = ON;
            break;
        case SERVO_7:
            SERVO_7_LAT = ON;
            break;
        case SERVO_8:
            SERVO_8_LAT = ON;
            break;

        default:
            break;
    }
}

/*********************************************************************
* Function: void LED_Off(LED led);
*
* Overview: Turns requested LED off
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Off(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void Pin_Off(PIN led)
{
    switch(led)
    {
        case SERVO_1:
            SERVO_1_LAT = OFF;
            break;
        case SERVO_2:
            SERVO_2_LAT = OFF;
            break;
        case SERVO_3:
            SERVO_3_LAT = OFF;
            break;
        case SERVO_4:
            SERVO_4_LAT = OFF;
            break;
        case SERVO_5:
            SERVO_5_LAT = OFF;
            break;
        case SERVO_6:
            SERVO_6_LAT = OFF;
            break;
        case SERVO_7:
            SERVO_7_LAT = OFF;
            break;
        case SERVO_8:
            SERVO_8_LAT = OFF;
            break;

        default:
            break;
    }
}

/*********************************************************************
* Function: void LED_Toggle(LED led);
*
* Overview: Toggles the state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Toggle(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void Pin_Toggle(PIN led)
{
    switch(led)
    {            
        case SERVO_1:
            SERVO_1_LAT ^= 1;
            break;
        case SERVO_2:
            SERVO_2_LAT ^= 1;
            break;
        case SERVO_3:
            SERVO_3_LAT ^= 1;
            break;
        case SERVO_4:
            SERVO_4_LAT ^= 1;
            break;
        case SERVO_5:
            SERVO_5_LAT ^= 1;
            break;
        case SERVO_6:
            SERVO_6_LAT ^= 1;
            break;
            
	case SERVO_7:
            SERVO_7_LAT ^= 1;
            break;
            
        case SERVO_8:
            SERVO_8_LAT ^= 1;
            break;	
        default:
            break;
    }
}
/*********************************************************************
* Function: void LED_Enable(LED led);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*
* Output: none
*
********************************************************************/
void Pin_Enable(PIN led)
{
    switch(led)
    {
        case SERVO_1:
            SERVO_1_TRIS = PIN_OUTPUT;
            SERVO_1_ANSEL = PIN_DIGITAL;
            break;
        case SERVO_2:
            SERVO_2_TRIS = PIN_OUTPUT;
            SERVO_2_ANSEL = PIN_DIGITAL;
            break;
        case SERVO_3:
            SERVO_3_TRIS = PIN_OUTPUT;
            SERVO_3_ANSEL = PIN_DIGITAL;
            break;
        case SERVO_4:
            SERVO_4_TRIS = PIN_OUTPUT;
            SERVO_4_ANSEL = PIN_DIGITAL;
            break;
        case SERVO_5:
            SERVO_5_TRIS = PIN_OUTPUT;
            break;
        case SERVO_6:
            SERVO_6_TRIS = PIN_OUTPUT;
            SERVO_6_ANSEL = PIN_DIGITAL;
            break;
        case SERVO_7:
            SERVO_7_TRIS = PIN_OUTPUT;
            SERVO_7_ANSEL = PIN_DIGITAL;
            break;
        case SERVO_8:
            SERVO_8_TRIS = PIN_OUTPUT;
            SERVO_8_ANSEL = PIN_DIGITAL;
            break;

        default:
            break;
    }
}
