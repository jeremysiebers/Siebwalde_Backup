/*
 * File:   newmain.c
 * Author: Jeremy Siebers
 *
 * Created on April 16, 2018, 10:06 PM
 */
#include <xc.h>
#include "main.h"
#include "mcc_generated_files/mcc.h"
#include "modbus/Interrupts.h"
#include "modbus/PetitModbus.h"
#include "commhandler.h"

#define ALLxSLAVESxDATA ((unsigned int)((NUMBER_OF_SLAVES-1)*3) * 4)



/*----------------------------------------------------------------------------*/

static SLAVE_INFO         SlaveInfo[NUMBER_OF_SLAVES];                          // Array of structs holding the data of all the slaves connected  
static SLAVE_INFO         SlaveDump[1];

/*----------------------------------------------------------------------------*/

static uint16_t LED_TX_prev, LED_RX_prev, LED_ERR_prev, LED_WAR_prev = 0;
static uint16_t LED_TX_STATE, LED_RX_STATE, LED_ERR_STATE, LED_WAR_STATE = 0;
uint16_t LED_ERR = 0;
static uint16_t LED_WAR = 0;
uint16_t UpdateNextSlave = false;
uint16_t AllSlavesReadAllDataCounter = 1;
uint16_t InitDone = false;
/*
                         Main application
 */
void main(void)
{    
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
        
    //__delay_ms(2000);                                                           // Wait longer then the slaves (1000ms)
    
    // Initialize the SLAVE_INFO struct with slave numbers
    for (char i = 0; i <NUMBER_OF_SLAVES; i++){
        SlaveInfo[i].SlaveNumber = i;
        SlaveInfo[i].Header = 0xAA;
        SlaveInfo[i].Footer = 0x55;
    }
    
    InitPetitModbus(SlaveInfo, SlaveDump, (NUMBER_OF_SLAVES + 5));                    // Pass address of array of struct for data storage
    InitSlaveCommunication(SlaveInfo, SlaveDump);                                          // Pass address of array of struct for data storage
    TX_ENA_LAT = 1;                                                             // Enable TX, master TX is always enabled.
        
    LED_RUN_LAT = 1;
    LED_WAR_LAT = 0;
    LED_ERR_LAT = 0;
    
    modbus_sync_LAT = 0;
    
    /* Start the message communication here as last 
       to prevent early communication while initializing
     */
    TMR2_Start();
    
    /* Wait till all slave data is send to SPI master from SPI slave */
    __delay_ms(100);
    
    COMM_MODE_BOOTLOAD = false;
    
    while(1)
    {
        if ((SlaveInfo[0].HoldingReg[0] & 0x01) == 0){                          // Initialization starts here, after init of all slaves, the regular updates can take place
            SLAVExCOMMANDxHANDLER(false);
            InitDone = false;
        }
        else if ((SlaveInfo[0].HoldingReg[0] & 0x01) == 1){                     // Regular slave communication
            InitDone = true;
            if (UpdateNextSlave == true){
                UpdateNextSlave = false;
                ProcessNextSlave();                    
                AllSlavesReadAllDataCounter++;
                if (AllSlavesReadAllDataCounter > ALLxSLAVESxDATA){
                    AllSlavesReadAllDataCounter = 1;
                }
            }
            ProcessSlaveCommunication();
        }
        ProcessPetitModbus();   
        Led_Blink();
        
        if(COMM_MODE_BOOTLOAD == true){
            LED_WAR++;
        }
    }
}

void Led_Blink (){
    if(PIR0bits.TMR0IF){
            
        switch(LED_TX_STATE){
            case 0 : 
                if (LED_TX > 0){
                    LED_TX_LAT = 1;
                    LED_TX_prev = LED_TX;
                    LED_TX_STATE = 1;
                }
                break;

            case 1 :
                if (LED_TX == LED_TX_prev || LED_TX != LED_TX_prev){
                    LED_TX_LAT = 0;
                    LED_TX_prev = 0;
                    LED_TX = 0;
                    LED_TX_STATE = 0;
                }
                break;

            default :
                LED_TX_STATE = 0;
                break;                       
        }

        switch(LED_RX_STATE){
            case 0 : 
                if (LED_RX > 0){
                    LED_RX_LAT = 1;
                    LED_RX_prev = LED_RX;
                    LED_RX_STATE = 1;
                }
                break;

            case 1 :
                if (LED_RX == LED_RX_prev || LED_RX != LED_RX_prev){
                    LED_RX_LAT = 0;
                    LED_RX_prev = 0;
                    LED_RX = 0;
                    LED_RX_STATE = 0;
                }
                break;

            default :
                LED_RX_STATE = 0;
                break;                       
        }
        
        switch(LED_ERR_STATE){
            case 0 : 
                if (LED_ERR > 0){
                    LED_ERR_LAT = 1;
                    LED_ERR_prev = LED_ERR;
                    LED_ERR_STATE = 1;                        
                }
                break;

            case 1 :
                if (LED_ERR == LED_ERR_prev || LED_ERR != LED_ERR_prev){
                    LED_ERR_LAT = 0;
                    LED_ERR_prev = 0;
                    LED_ERR = 0;
                    LED_ERR_STATE = 0;                        
                }
                break;

            default :
                LED_ERR_STATE = 0;
                break;                       
        }
        
        switch(LED_WAR_STATE){
            case 0 : 
                if (LED_WAR > 0){
                    LED_WAR_LAT = 1;
                    LED_WAR_prev = LED_WAR;
                    LED_WAR_STATE = 1;                        
                }
                break;

            case 1 :
                if (LED_WAR == LED_WAR_prev || LED_WAR != LED_WAR_prev){
                    LED_WAR_LAT = 0;
                    LED_WAR_prev = 0;
                    LED_WAR = 0;
                    LED_WAR_STATE = 0;                        
                }
                break;

            default :
                LED_WAR_STATE = 0;
                break;                       
        }
        
        PIR0bits.TMR0IF = 0;
        TMR0_Reload();
    }
}

/**
 End of File
*/