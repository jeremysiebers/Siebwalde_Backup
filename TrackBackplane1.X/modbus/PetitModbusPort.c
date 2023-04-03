#include "General.h"
#include "PetitModbus.h"
#include "PetitModbusPort.h"
#include "../mcc_generated_files/mcc.h"

// Modbus RTU Variables
volatile unsigned char   PetitReceiveBuffer[PETITMODBUS_RECEIVE_BUFFER_SIZE];   // Buffer to collect data from hardware
volatile unsigned char   PetitReceiveCounter=0;                                 // Collected data number

// UART Initialize for Microconrollers, yes you can use another phsycal layer!
void PetitModBus_UART_Initialise(void)
{
    InitUART();
}

// Timer Initialize for Petit Modbus, 1ms Timer will be good for us!
void PetitModBus_TIMER_Initialise(void)
{
    InitTMR();
}

// This is used for send one character
void PetitModBus_UART_Putch(unsigned char c)
{
    while(!TXSTAbits.TRMT);
    TXREG = c;
}

// This is used for send string, better to use DMA for it ;)
unsigned char PetitModBus_UART_String(unsigned char *s, unsigned int Length)
{
    unsigned short  DummyCounter;
    LED_TX++;
    TX_ENA_LAT = 1;                                                             // enable the driver of the rs485 as close as possible to the first bit to avoid glitch, levels are corrected with bias resistors!
    	    
    for(DummyCounter=0;DummyCounter<Length;DummyCounter++){
        PetitModBus_UART_Putch(s[DummyCounter]);
    }
    
    while(!TXSTAbits.TRMT);                                                     // Due to RS485 enable latch, wait until last bit is sent
    
    TX_ENA_LAT = 0;
    LED_TX++;
    
    return TRUE;
}

/*************************Interrupt Fonction Slave*****************************/
// Call this function into your UART Interrupt. Collect data from it!
// Better to use DMA
void ReceiveInterrupt(unsigned char Data)
{
    PetitReceiveBuffer[PetitReceiveCounter]   =Data;
    PetitReceiveCounter++;

    if(PetitReceiveCounter>PETITMODBUS_RECEIVE_BUFFER_SIZE){
        PetitReceiveCounter=0;
    }
    PetitModbusTimerValue=0;
}

// Call this function into 1ms Interrupt or Event!
void PetitModBus_TimerValues(void)
{
    //PetitModbusTimerValue++;
    PetitModbusTimerValue = 3;
}
/******************************************************************************/
