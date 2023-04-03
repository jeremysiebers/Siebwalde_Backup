/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC18F97J60
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/
#include <string.h>
#include "main.h"
#include "mcc_generated_files/mcc.h"
#include "spicommhandler.h"
#include "slavestartup.h"


void ToFromTerminal(void);
void WriteText(unsigned char *Text, unsigned int Ln, unsigned int Col);
void WriteData(unsigned int Data, unsigned int Ln, unsigned int Col);

/*
                         Main application
 */

/*----------------------------------------------------------------------------*/
static SLAVE_INFO         SlaveInfo[NUMBER_OF_SLAVES];   

unsigned int StateMachine = 1;
unsigned int _Delay = 0;
unsigned int disable_trace = 1;
unsigned int re_write_table = 0;
/*----------------------------------------------------------------------------*/

void main(void)
{
    // Initialize the SLAVE_INFO struct with slave numbers(fixed))
    for (unsigned int i = 0; i <NUMBER_OF_SLAVES; i++){
        SlaveInfo[i].SlaveNumber = i;
        SlaveInfo[i].Header = 0xAA;
        SlaveInfo[i].Footer = 0x55;
        if (i > 0){
            SlaveInfo[i].HoldingReg[0] = 0x818F;                                // Set all EMO an 50%
            SlaveInfo[i].HoldingReg[2] = i;                                     // Set address of slave
        }
    }
/*----------------------------------------------------------------------------*/
    
    // Initialize the device
    SYSTEM_Initialize();
    
    INITxSPIxCOMMxHANDLER(SlaveInfo);                                           // Init SPI slave - master registers
    INITxSLAVExSTARTUP(SlaveInfo);                                              // Init Slave startup 
    
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
        
    printf("PIC18f97j60 started up!!!\n\r");
    __delay_ms(10);
    printf("\f");                                                               // Clear terminal (printf("\033[2J");)
    __delay_ms(10);
    printf("\033[?25h");
    __delay_ms(10);   
    printf("PIC18f97j60 started up!!!\n\r");                                    // Welcome message
    
    ModbusReset_LAT = 0;                                                        // as last release the ModbusMaster.
    TRISDbits.RD3 = 0;                                                          // set the Pin direction here to output to avoid power-on reset pulses to slaves!
    
    __delay_ms(500);                                                            // wait 0.5 second to gather all slave data via SPI (update of SlaveInfo)
    
/*----------------------------------------------------------------------------*/
    disable_trace = 1;
    
    while (1)
    {
        ToFromTerminal();
        
        
        switch(StateMachine){
            case 1:
                if(SLAVExINITxANDxCONFIG() == true){
                    StateMachine = 2;
                }
                break;
                
            case 2:
                if(ENABLExAMPLIFIER() == true){
                    StateMachine = 3;
                }
                break;
                
            default :
                break;
        }
    }
}
/*----------------------------------------------------------------------------*/




/*#--------------------------------------------------------------------------#*/
/*  Description: ToFromTerminal()
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

#define COL0	0
#define COL1	30
#define COL2	60
#define COL3	90
#define COL4	120

#define COL0D	COL0 + 20
#define COL1D	COL1 + 20
#define COL2D	COL2 + 20
#define COL3D	COL3 + 20
#define COL4D	COL4 + 20

unsigned char UPDATExTERMINAL;
unsigned char terminal = 0;

void ToFromTerminal(){
    
    if(EUSART1_is_rx_ready()){
        switch(EUSART1_Read()){
            case 0x2E://period sign
                SlaveInfo[1].HoldingReg[3] ^= 0x0001;
                break;
                
            case 0x2D://minus sign
                disable_trace ^= 0x1;
                if (disable_trace == 0){
                    re_write_table = 1;
                    PIE1bits.TMR1IE = 0;
                }  
                else{
                    PIE1bits.TMR1IE = 1;
                }
                break;
                
            case 0x30:
                ModbusReset_LAT = 1;
                RESET();                    
                break;

            case 0x31:
                (unsigned int)SlaveInfo[1].HoldingReg[0] &= 0x0FFF; 
                if((SlaveInfo[1].HoldingReg[0] & 0x3FF) > 60){               // PWM On and 50% duty
                    (unsigned int)SlaveInfo[1].HoldingReg[0]-= 50; 
                    SlaveInfo[4].HoldingReg[0] = SlaveInfo[1].HoldingReg[0];
                }          
                break;

            case 0x32:
                (unsigned int)SlaveInfo[2].HoldingReg[0] &= 0x0FFF; 
                if((SlaveInfo[2].HoldingReg[0] & 0x3FF) > 60){               // PWM On and 50% duty
                    (unsigned int)SlaveInfo[2].HoldingReg[0]-= 50;          
                }
                break;

            case 0x33:
                (unsigned int)SlaveInfo[3].HoldingReg[0] &= 0x0FFF; 
                if((SlaveInfo[3].HoldingReg[0] & 0x3FF) > 60){               // PWM On and 50% duty
                    (unsigned int)SlaveInfo[3].HoldingReg[0]-= 50;          
                }
                break;

            case 0x34:
                (unsigned int)SlaveInfo[1].HoldingReg[0] = 0x018F;          // 50%     
                SlaveInfo[4].HoldingReg[0] = SlaveInfo[1].HoldingReg[0];
                break;

            case 0x35:
                (unsigned int)SlaveInfo[2].HoldingReg[0] = 0x018F;
                break;

            case 0x36:
                (unsigned int)SlaveInfo[3].HoldingReg[0] = 0x018F;
                break;

            case 0x37:
                (unsigned int)SlaveInfo[1].HoldingReg[0] &= 0x0FFF; 
                if((SlaveInfo[1].HoldingReg[0] & 0x3FF) < 720){
                    (unsigned int)SlaveInfo[1].HoldingReg[0]+= 50;    
                    SlaveInfo[4].HoldingReg[0] = SlaveInfo[1].HoldingReg[0];
                }
                break;

            case 0x38:
                (unsigned int)SlaveInfo[2].HoldingReg[0] &= 0x0FFF; 
                if((SlaveInfo[2].HoldingReg[0] & 0x3FF) < 720){
                    (unsigned int)SlaveInfo[2].HoldingReg[0]+= 50; 
                }
                break;

            case 0x39:
                (unsigned int)SlaveInfo[3].HoldingReg[0] &= 0x0FFF; 
                if((SlaveInfo[3].HoldingReg[0] & 0x3FF) < 720){
                    (unsigned int)SlaveInfo[3].HoldingReg[0]+= 50; 
                }
                break;

            default:
                break;
        }
    }

    if(UPDATExTERMINAL == 1 && disable_trace == 0x1){// && EUSART1_is_tx_ready() == 1024){
        if(re_write_table ){
            terminal = 0;
            re_write_table = 0;
            printf("PIC18f97j60 started up!!!\n\r");
            __delay_ms(10);
            printf("\f");                                                               // Clear terminal (printf("\033[2J");)
            __delay_ms(10);
            printf("\033[?25h");
            __delay_ms(10);   
            printf("PIC18f97j60 started up!!!\n\r");                                    // Welcome message
        }
        switch(terminal){
            case 0:             //"SpiCommErrorCount "//
                WriteText((char *)"ModBus_Mastr:",2, COL0);
                WriteText((char *)"ModBus_Slave:",2, COL1);
                WriteText((char *)"ModBus_Slave:",2, COL2);
                WriteText((char *)"ModBus_Slave:",2, COL3);   
                WriteText((char *)"ModBus_Slave:",2, COL4); 
                terminal++;
                //break;                   

            case 1:
                WriteText((char *)"HoldingReg0:",3, COL0);
                WriteText((char *)"HoldingReg0:",3, COL1);
                WriteText((char *)"HoldingReg0:",3, COL2);
                WriteText((char *)"HoldingReg0:",3, COL3);         
                WriteText((char *)"HoldingReg0:",3, COL4);     
                terminal++;                       
                //break;                            

            case 2:                               
                WriteText((char *)"HoldingReg1:",4, COL0);
                WriteText((char *)"HoldingReg1:",4, COL1);
                WriteText((char *)"HoldingReg1:",4, COL2);
                WriteText((char *)"HoldingReg1:",4, COL3);   
                WriteText((char *)"HoldingReg1:",4, COL4); 
                terminal++;                       
                //break;                            

            case 3:                               
                WriteText((char *)"HoldingReg2:",5, COL0);
                WriteText((char *)"HoldingReg2:",5, COL1);
                WriteText((char *)"HoldingReg2:",5, COL2);
                WriteText((char *)"HoldingReg2:",5, COL3);   
                WriteText((char *)"HoldingReg2:",5, COL4); 
                terminal++;                       
                break;                            

            case 4:                               
                WriteText((char *)"HoldingReg3:",6, COL0);
                WriteText((char *)"HoldingReg3:",6, COL1);
                WriteText((char *)"HoldingReg3:",6, COL2);
                WriteText((char *)"HoldingReg3:",6, COL3);  
                WriteText((char *)"HoldingReg3:",6, COL4);
                terminal++;                       
                //break;                            

            case 5:
                WriteText((char *)"InputReg0:",7, COL0);
                WriteText((char *)"BEMF     :",7, COL1);
                WriteText((char *)"InputReg0:",7, COL2);
                WriteText((char *)"InputReg0:",7, COL3);  
                WriteText((char *)"InputReg0:",7, COL4);  
                terminal++;
                //break;

            case 6:
                WriteText((char *)"InputReg1:",8, COL0);
                WriteText((char *)"kp       :",8, COL1);
                WriteText((char *)"InputReg1:",8, COL2);
                WriteText((char *)"InputReg1:",8, COL3); 
                WriteText((char *)"InputReg1:",8, COL4);
                terminal++;
                //break;

            case 7:
                WriteText((char *)"InputReg2:",9, COL0);
                WriteText((char *)"plant    :",9, COL1);
                WriteText((char *)"InputReg2:",9, COL2);
                WriteText((char *)"InputReg2:",9, COL3);  
                WriteText((char *)"InputReg2:",9, COL4);
                terminal++;
                break;

            case 8:
                WriteText((char *)"InputReg3:",10, COL0);
                WriteText((char *)"error    :",10, COL1);
                WriteText((char *)"InputReg3:",10, COL2);
                WriteText((char *)"InputReg3:",10, COL3); 
                WriteText((char *)"InputReg3:",10, COL4);  
                terminal++;
                //break;

            case 9:
                WriteText((char *)"InputReg4:",11, COL0);
                WriteText((char *)"output   :",11, COL1);
                WriteText((char *)"InputReg4:",11, COL2);
                WriteText((char *)"InputReg4:",11, COL3);    
                WriteText((char *)"InputReg4:",11, COL4);
                terminal++;
                //break;

            case 10:
                WriteText((char *)"InputReg5:",12, COL0);
                WriteText((char *)"pwm setp.:",12, COL1);
                WriteText((char *)"InputReg5:",12, COL2);
                WriteText((char *)"InputReg5:",12, COL3);  
                WriteText((char *)"InputReg5:",12, COL4);
                terminal++;
                //break;

            case 11:
                WriteText((char *)"DiagReg0:",13, COL0);
                WriteText((char *)"DiagReg0:",13, COL1);
                WriteText((char *)"DiagReg0:",13, COL2);
                WriteText((char *)"DiagReg0:",13, COL3);
                WriteText((char *)"DiagReg0:",13, COL4);
                terminal++;                
                break;                     

            case 12:                       
                WriteText((char *)"DiagReg1:",14, COL0);
                WriteText((char *)"DiagReg1:",14, COL1);
                WriteText((char *)"DiagReg1:",14, COL2);
                WriteText((char *)"DiagReg1:",14, COL3); 
                WriteText((char *)"DiagReg1:",14, COL4);
                terminal++;                
                //break;                     

            case 13:
                WriteText((char *)"MbReceiveCounter:",15, COL0);
                WriteText((char *)"MbReceiveCounter:",15, COL1);
                WriteText((char *)"MbReceiveCounter:",15, COL2);
                WriteText((char *)"MbReceiveCounter:",15, COL3);  
                WriteText((char *)"MbReceiveCounter:",15, COL4);
                terminal++;
                //break;

            case 14:
                WriteText((char *)"MbSentCounter:",16, COL0);
                WriteText((char *)"MbSentCounter:",16, COL1);
                WriteText((char *)"MbSentCounter:",16, COL2);
                WriteText((char *)"MbSentCounter:",16, COL3); 
                WriteText((char *)"MbSentCounter:",16, COL4);
                terminal++;
                //break;

            case 15:
                WriteText((char *)"MbCommError:",17, COL0);
                WriteText((char *)"MbCommError:",17, COL1);
                WriteText((char *)"MbCommError:",17, COL2);
                WriteText((char *)"MbCommError:",17, COL3); 
                WriteText((char *)"MbCommError:",17, COL4);
                terminal++;
                break;

            case 16:
                WriteText((char *)"MbExceptionCode:",18, COL0);
                WriteText((char *)"MbExceptionCode:",18, COL1);
                WriteText((char *)"MbExceptionCode:",18, COL2);
                WriteText((char *)"MbExceptionCode:",18, COL3); 
                WriteText((char *)"MbExceptionCode:",18, COL4); 
                terminal++;
                //break;

            case 17:
                WriteText((char *)"SpiCommErrorCount:",19, COL0);
                WriteText((char *)"SpiCommErrorCount:",19, COL1);
                WriteText((char *)"SpiCommErrorCount:",19, COL2);
                WriteText((char *)"SpiCommErrorCount:",19, COL3);
                WriteText((char *)"SpiCommErrorCount:",19, COL4);
                terminal++;
                //break;

            case 18:
                WriteText((char *)"SpiSLCommErrCnt:",20, COL0); 
                WriteText((char *)" "               ,20, COL1);
                WriteText((char *)" "               ,20, COL2);
                WriteText((char *)" "               ,20, COL3);
                WriteText((char *)" "               ,20, COL4);
                terminal++;
                break;                    
/*------------------------------------------------------------------------------------------*/                    
            case 19:
                //Read_Check_LAT ^= 1;
                WriteData(SlaveInfo[0].SlaveNumber,2, COL0D);
                WriteData(SlaveInfo[1].SlaveNumber,2, COL1D);
                WriteData(SlaveInfo[2].SlaveNumber,2, COL2D);
                WriteData(SlaveInfo[3].SlaveNumber,2, COL3D);
                WriteData(SlaveInfo[4].SlaveNumber,2, COL4D);
              terminal++;
              //break;

          case 20:                    
                WriteData(SlaveInfo[0].HoldingReg[0],3, COL0D);
                WriteData(SlaveInfo[1].HoldingReg[0],3, COL1D);
                WriteData(SlaveInfo[2].HoldingReg[0],3, COL2D);
                WriteData(SlaveInfo[3].HoldingReg[0],3, COL3D);
                WriteData(SlaveInfo[4].HoldingReg[0],3, COL4D);
              terminal++;
              //break;

          case 21:
                WriteData(SlaveInfo[0].HoldingReg[1],4, COL0D);
                WriteData(SlaveInfo[1].HoldingReg[1],4, COL1D);
                WriteData(SlaveInfo[2].HoldingReg[1],4, COL2D);
                WriteData(SlaveInfo[3].HoldingReg[1],4, COL3D); 
                WriteData(SlaveInfo[4].HoldingReg[1],4, COL4D);
              terminal++;
              //break;

          case 22:
                WriteData(SlaveInfo[0].HoldingReg[2],5, COL0D);
                WriteData(SlaveInfo[1].HoldingReg[2],5, COL1D);
                WriteData(SlaveInfo[2].HoldingReg[2],5, COL2D);
                WriteData(SlaveInfo[3].HoldingReg[2],5, COL3D); 
                WriteData(SlaveInfo[4].HoldingReg[2],5, COL4D);
              terminal++;
              break;

          case 23:
                WriteData(SlaveInfo[0].HoldingReg[3],6, COL0D);
                WriteData(SlaveInfo[1].HoldingReg[3],6, COL1D);
                WriteData(SlaveInfo[2].HoldingReg[3],6, COL2D);
                WriteData(SlaveInfo[3].HoldingReg[3],6, COL3D); 
                WriteData(SlaveInfo[4].HoldingReg[3],6, COL4D);
              terminal++;
              //break;

          case 24:
                WriteData(SlaveInfo[0].InputReg[0],7, COL0D);
                WriteData(SlaveInfo[1].InputReg[0],7, COL1D);
                WriteData(SlaveInfo[2].InputReg[0],7, COL2D);
                WriteData(SlaveInfo[3].InputReg[0],7, COL3D);   
                WriteData(SlaveInfo[4].InputReg[0],7, COL4D);
              terminal++;
              //break;

          case 25:
                WriteData(SlaveInfo[0].InputReg[1],8, COL0D);
                WriteData(SlaveInfo[1].InputReg[1],8, COL1D);
                WriteData(SlaveInfo[2].InputReg[1],8, COL2D);
                WriteData(SlaveInfo[3].InputReg[1],8, COL3D); 
                WriteData(SlaveInfo[4].InputReg[1],8, COL4D); 
              terminal++;
              //break;

          case 26:
                WriteData(SlaveInfo[0].InputReg[2],9, COL0D);
                WriteData(SlaveInfo[1].InputReg[2],9, COL1D);
                WriteData(SlaveInfo[2].InputReg[2],9, COL2D);
                WriteData(SlaveInfo[3].InputReg[2],9, COL3D); 
                WriteData(SlaveInfo[4].InputReg[2],9, COL4D);
              terminal++;
              break;

          case 27:
                WriteData(SlaveInfo[0].InputReg[3],10, COL0D);
                WriteData(SlaveInfo[1].InputReg[3],10, COL1D);
                WriteData(SlaveInfo[2].InputReg[3],10, COL2D);
                WriteData(SlaveInfo[3].InputReg[3],10, COL3D); 
                WriteData(SlaveInfo[4].InputReg[3],10, COL4D);
              terminal++;
              //break;

          case 28:
                WriteData(SlaveInfo[0].InputReg[4],11, COL0D);
                WriteData(SlaveInfo[1].InputReg[4],11, COL1D);
                WriteData(SlaveInfo[2].InputReg[4],11, COL2D);
                WriteData(SlaveInfo[3].InputReg[4],11, COL3D); 
                WriteData(SlaveInfo[4].InputReg[4],11, COL4D);
              terminal++;
              //break;

          case 29:
                WriteData(SlaveInfo[0].InputReg[5],12, COL0D);
                WriteData(SlaveInfo[1].InputReg[5],12, COL1D);
                WriteData(SlaveInfo[2].InputReg[5],12, COL2D);
                WriteData(SlaveInfo[3].InputReg[5],12, COL3D); 
                WriteData(SlaveInfo[4].InputReg[5],12, COL4D);
              terminal++;
              //break;

          case 30:
                WriteData(SlaveInfo[0].DiagReg[0],13, COL0D);
                WriteData(SlaveInfo[1].DiagReg[0],13, COL1D);
                WriteData(SlaveInfo[2].DiagReg[0],13, COL2D);
                WriteData(SlaveInfo[3].DiagReg[0],13, COL3D); 
                WriteData(SlaveInfo[4].DiagReg[0],13, COL4D); 
              terminal++;
              break;

          case 31:
                WriteData(SlaveInfo[0].DiagReg[1],14, COL0D);
                WriteData(SlaveInfo[1].DiagReg[1],14, COL1D);
                WriteData(SlaveInfo[2].DiagReg[1],14, COL2D);
                WriteData(SlaveInfo[3].DiagReg[1],14, COL3D);  
                WriteData(SlaveInfo[4].DiagReg[1],14, COL4D);
              terminal++;
              //break;

          case 32:
                WriteData(SlaveInfo[0].MbReceiveCounter,15, COL0D);
                WriteData(SlaveInfo[1].MbReceiveCounter,15, COL1D);
                WriteData(SlaveInfo[2].MbReceiveCounter,15, COL2D);
                WriteData(SlaveInfo[3].MbReceiveCounter,15, COL3D);    
                WriteData(SlaveInfo[4].MbReceiveCounter,15, COL4D);
              terminal++;
              //break;

          case 33:
                WriteData(SlaveInfo[0].MbSentCounter,16, COL0D);
                WriteData(SlaveInfo[1].MbSentCounter,16, COL1D);
                WriteData(SlaveInfo[2].MbSentCounter,16, COL2D);
                WriteData(SlaveInfo[3].MbSentCounter,16, COL3D); 
                WriteData(SlaveInfo[4].MbSentCounter,16, COL4D);
              terminal++;
              //break;

          case 34:
                WriteData((char)(SlaveInfo[0].MbCommError),17, COL0D);
                WriteData((char)(SlaveInfo[1].MbCommError),17, COL1D);
                WriteData((char)(SlaveInfo[2].MbCommError),17, COL2D);
                WriteData((char)(SlaveInfo[3].MbCommError),17, COL3D);  
                WriteData((char)(SlaveInfo[4].MbCommError),17, COL4D);
              terminal++;
              break;

          case 35:
                WriteData(SlaveInfo[0].MbExceptionCode,18, COL0D);
                WriteData(SlaveInfo[1].MbExceptionCode,18, COL1D);
                WriteData(SlaveInfo[2].MbExceptionCode,18, COL2D);
                WriteData(SlaveInfo[3].MbExceptionCode,18, COL3D); 
                WriteData(SlaveInfo[4].MbExceptionCode,18, COL4D);
              terminal++;
              //break;

          case 36:
                WriteData(SlaveInfo[0].SpiCommErrorCounter,19, COL0D);
                WriteData(SlaveInfo[1].SpiCommErrorCounter,19, COL1D);
                WriteData(SlaveInfo[2].SpiCommErrorCounter,19, COL2D);
                WriteData(SlaveInfo[3].SpiCommErrorCounter,19, COL3D);
                WriteData(SlaveInfo[4].SpiCommErrorCounter,19, COL4D);
              terminal++;
              //break;

          case 37:
                WriteData(SpiSlaveCommErrorCounter,20, COL0D);
                WriteData(0                       ,20, COL1D);
                WriteData(0                       ,20, COL2D);
                WriteData(0                       ,20, COL3D);
                WriteData(0                       ,20, COL4D);
                terminal = 19;
                break;

            default :
                terminal = 19;
                break;
        }             
        UPDATExTERMINAL = 0;
    }        
    
}


/*#--------------------------------------------------------------------------#*/
/*  Description: WriteText(unsigned char *Text, unsigned int Ln, unsigned int Col)
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
void WriteText(unsigned char *Text, unsigned int Ln, unsigned int Col){
    
    //   [{ROW};{COLUMN}f    
    printf("\033[%u;%uf", Ln, Col);    
    printf(Text);    
}


/*#--------------------------------------------------------------------------#*/
/*  Description: WriteData(unsigned long int Data, unsigned int Ln, unsigned int Col)
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
void WriteData(unsigned int Data, unsigned int Ln, unsigned int Col){
    
    char padding[10];
    
         if(Data < 10){
        strcpy(padding, "    ");
    }
    else if(Data <100){
        strcpy(padding, "   ");
    }
    else if(Data <1000){
        strcpy(padding, "  ");
    }
    else if(Data <10000){
        strcpy(padding, " ");
    }
    else{
        strcpy(padding, "");
    }
    
    printf("\033[%u;%uf", Ln, Col);
    printf("%d%s", Data, padding);   
}