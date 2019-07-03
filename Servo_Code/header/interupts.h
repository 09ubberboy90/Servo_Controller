/* 
 * File:   interupts.h
 * Author: audof
 *
 * Created on 11 juin 2019, 16:04
 */

#ifndef INTERUPTS_H
#define	INTERUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif
/*
 * Initialize all the timer and enable interupts
 */
    typedef enum{
    STATE_ON,
    STATE_WAITING,
    STATE_OFF
}STATE_MACHINE;

#define PWM_MIN_TIME 3750 // 2.5ms
#define PWM_PERIOD 30000 // 20ms
// both value are based on the clock speed and the conversion from angle to time
#define HARD_MULTIPLE 16.65
#define HARD_SUM 750 //
void Interupt_Init(void);
//void Timer0_Init(void);
void Timer1_Init(void);
//void Timer2_Init(void);
//void Timer3_Init(void);

/*
 * Main interupt routine
 * Curently handle 2 timer;
 */
void __interrupt() ISR_Control(void);    //Low priority interrupt


#ifdef	__cplusplus
}
#endif

#endif	/* INTERUPTS_H */

