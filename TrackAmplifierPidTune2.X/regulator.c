#include "regulator.h"
#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "processio.h"
#include "main.h"

static unsigned int PwmDutyCyclePrev = 0;
static unsigned int test = 0;

unsigned int MeausureBemfCnt = 0;
unsigned int UpdatePID = 0;
unsigned int UPDATExPID = 0;

static int setpoint = 300;
static int error = 0;
static int input = 400;
static int output = 200;
static int kp = 2;
static int pwm = 200;
static int plant = 15;

/*#--------------------------------------------------------------------------#*/
/*  Description: Regulator_Init()
 *
 *  Input(s)   : 
 *
 *  Output(s)  :
 *
 *  Returns    :
 *
 *  Pre.Cond.  :
 *
 *  Post.Cond. :
 *
 *  Notes      : 
 */
/*#--------------------------------------------------------------------------#*/
void Regulator_Init(){
    LM_DIR_LAT      = 0;
    LM_PWM_LAT      = 1;
    LM_BRAKE_LAT    = 0;
    PWM3CON         = 0x80;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    
    PWM3_LoadDutyValue(399);
    
    PwmDutyCyclePrev = ((unsigned int)(PWM3DCH << 2)) + 
                       ((unsigned int)(PWM3DCL >> 6));    
}

/*#--------------------------------------------------------------------------#*/
/*  Description: Regulator()
 *
 *  Input(s)   : 
 *
 *  Output(s)  :
 *
 *  Returns    :
 *
 *  Pre.Cond.  :
 *
 *  Post.Cond. :
 *
 *  Notes      : 
 */
/*#--------------------------------------------------------------------------#*/
void Regulator(){
    
    if (PetitHoldingRegisters[0].ActValue & 0x8000){                            // If EMO command active kill PWM
        PWM3CON         = 0x00;
        LM_DIR_LAT      = 0;
        LM_PWM_LAT      = 0;
        LM_BRAKE_LAT    = 1;        
    }
    else if (0 == PWM3CON){                                                     // When no EMO, check if PWM is initialized
        Regulator_Init();                                                       // Init PWM if not initialized
    }
    else{
    
        //LM_DIR_LAT =  PetitHoldingRegisters[0].ActValue & 0x0400;               // load direction from register only when single sided PWM
        LM_BRAKE_LAT = PetitHoldingRegisters[0].ActValue & 0x0800;              // load brake from register
        
        if ((PwmDutyCyclePrev != PetitHoldingRegisters[0].ActValue & 0x03FF) && PetitHoldingRegisters[3].ActValue == 0x0000){
            PWM3_LoadDutyValue(PetitHoldingRegisters[0].ActValue & 0x03FF);     // load duty cycle from register
            PwmDutyCyclePrev = PetitHoldingRegisters[0].ActValue & 0x03FF;
        }
        else if(PwmDutyCyclePrev != (unsigned int)pwm && PetitHoldingRegisters[3].ActValue == 0x0001){
            PWM3_LoadDutyValue((unsigned int)pwm);
        }
    }
    
    if(UPDATExPID){
        ProcessBMF();
    }
    
    if (PIR4bits.TMR1IF){
        MeausureBemfCnt++;
        if (MeausureBemfCnt > 99){                                              // disable H bridge (67))
            MeausureBemfCnt = 0;
            TRISCbits.TRISC4 = 1;
            TRISCbits.TRISC5 = 1;
            TRISCbits.TRISC6 = 1;
            UpdatePID = true;                                                   // set update PID
        }
        if (MeausureBemfCnt > 8){                                               // after 600us enable H bridge again
            TRISCbits.TRISC4 = 0;
            TRISCbits.TRISC5 = 0;
            TRISCbits.TRISC6 = 0;                
        }

        if (MeausureBemfCnt > 6 && MeausureBemfCnt < 8){                        // Read back EMF as late as possible
            //PORTAbits.RA6 = 1;
            //ProcessBMF();
            UPDATExPID = true;                                                  // wait on modbus
        }
        else{
            PORTAbits.RA6 = 0;
            if(UPDATExPID){
                PORTAbits.RA6 = 1;
                if(PetitHoldingRegisters[1].ActValue > 0){
                    EUSART_Write(PetitInputRegisters[0].ActValue  & 0x000F);
                    EUSART_Write((PetitInputRegisters[0].ActValue & 0x00F0) >> 4);
                    EUSART_Write((PetitInputRegisters[0].ActValue & 0x0F00) >> 8);
                    EUSART_Write((PetitInputRegisters[0].ActValue & 0xF000) >> 12);
                    EUSART_Write(0xAA);
                }                
            }
            UPDATExPID = false;
            //ProcessIO();
        }
        
        PIR4bits.TMR1IF = 0;
        TMR1_Reload();
    }
}






/*
 * HoldingReg0, 0 - 9           PWM set point       	R/W     No/Yes			// new PWM setpoint (speed in 0 - 255 km/h)
 * HoldingReg0, 10				PWM direction       	R/W     No/Yes          // Forward / Backward
 * HoldingReg0, 11              PWM enable          	R/W     No/Yes          // enabling of the H-bridge
 * HoldingReg0, 12                                  	
 * HoldingReg0, 13                                  	
 * HoldingReg0, 14                                  	
 * HoldingReg0, 15              Emo Stop            	R/W     No/Yes			// Slave takes action to stop train as fast as possible
 */ 