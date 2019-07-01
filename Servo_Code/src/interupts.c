/*
 * File:   interupts.c
 * Author: audof
 *
 * Created on 11 juin 2019, 16:04
 */


#include <xc.h>
#include "io.h"
#include "interupts.h"
#include "system.h"
#include "servo.h"

static char out;
STATE_MACHINE state = STATE_OFF;
uint8_t servo_id = 0;
uint16_t tmp_time = 0;
uint16_t total_time = 0;
void Interupt_Init()
{
    INTCONbits.PEIE = 1; // enable peripheral interrupts
    INTCONbits.GIE = 1; // enable interrupts globally
    //Timer0_Init();
    Timer1_Init();
    //Timer2_Init();
    //Timer3_Init();
}

//void Timer0_Init()
//{
//    T0CONbits.TMR0ON = 0; // stop the timer
//    T0CONbits.T08BIT = 0; // timer configured as 16-bit
//    T0CONbits.T0CS = 0; // use system clock
//    T0CONbits.PSA = 0; // use prescaler
//    T0CONbits.T0PS = 0b111;
//    // prescaler 1:8
//    TMR0 = -9523; // setup initial timer value
//    INTCONbits.TMR0IF = 0; // reset timer interrupt flag
//    INTCONbits.TMR0IE = 1; // enable timer interrupts
//    RCONbits.IPEN = 1; // use priorities
//    //T0CONbits.TMR0ON = 1;
//}

void Timer1_Init()
{
    T1CONbits.TMR1ON = 0; // stop the timer
    T1CONbits.RD16 = 1; // timer configured as 16-bit
    T1CONbits.TMR1CS = 0; // use system clock
    T1CONbits.T1CKPS = 0b11;
    // prescaler 1:8
    TMR1 = 65535-30000; // setup initial timer value
    PIR1bits.TMR1IF = 0; // reset timer interrupt flag
    PIE1bits.TMR1IE = 1; // enable timer interrupts
    //T1CONbits.TMR1ON = 1;
}

//void Timer2_Init()
//{
//    T2CONbits.TMR2ON = 0; // stop the timer
//    T2CONbits.T2CKPS = 0b01; 
//    // prescaler 1:4
//    TMR2 = -255; // setup initial timer value
//    PIR1bits.TMR2IF = 0; // reset timer interrupt flag
//    PIE1bits.TMR2IE = 1; // enable timer interrupts
//    //T0CONbits.TMR0ON = 1;
//}
//

//void Timer3_Init()
//{
//    T3CONbits.TMR3ON = 0; // stop the timer
//    T3CONbits.RD16 = 1; // timer configured as 16-bit
//    T3CONbits.TMR3CS = 0; // use system clock
//    T3CONbits.T3CKPS = 0b11;
//    // prescaler 1:32
//    TMR3 = -3000; // setup initial timer value
//    PIR2bits.TMR3IF = 0; // reset timer interrupt flag
//    PIE2bits.TMR3IE = 1; // enable timer interrupts
//    //T0CONbits.TMR0ON = 1;
//}
void __interrupt(low_priority) ISR_Control() //Low priority interrupt
{
    if (PIR1bits.TMR1IF == 1) // Set the period of pwm
    {
        Servo_Pin_Control *gesture = Get_Gesture();
        switch(state)
        {
        case STATE_ON:
            Pin_Off(gesture->pin[servo_id]);
            tmp_time = PWM_MIN_TIME - gesture->time[servo_id];
            servo_id++;
            if (tmp_time>0) // if pin was needed to be on for less than what was supposed to wait
            {
                tmp_time = -tmp_time*150;
                TMR1H = (tmp_time&0xFF00)>>8;
                TMR1L = (tmp_time&0x00FF);
                state = STATE_OFF;
                break; 
            }
            //else no need for break directly go into off state 
            //implicitly set state to off and hence start the next servo
            //although no need to do it since it would mean to wait for another interupt;
        case STATE_OFF: // If you're off then it means you can start a new led if there is one;
            if (servo_id < gesture->nb_servo)// if there's still some Led turn them on and restart the process
            {
                tmp_time = gesture->time[servo_id];
                tmp_time = -tmp_time*150;
                TMR1H = (tmp_time&0xFF00)>>8;
                TMR1L = (tmp_time&0x00FF);
                Pin_On(gesture->pin[servo_id]);
                state = STATE_ON;
                break;
            }
            //else no need to wait for another interupt fall into waiting state 
            //and wait for whatever amount of time is left
        case STATE_WAITING:
            tmp_time = PWM_PERIOD-gesture->nb_servo*PWM_MIN_TIME;
            tmp_time = -tmp_time*150;
            TMR1H = (tmp_time&0xFF00)>>8;
            TMR1L = (tmp_time&0x00FF);

            state = STATE_OFF;
            total_time = 0;
            servo_id = 0;
            break;
        }
        PIR1bits.TMR1IF = 0; // reset interupt flag
    }

}
