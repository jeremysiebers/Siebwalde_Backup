/**
  Section: Included Files
 */
#include "uart1.h"
#include <xc.h>

/**
  Section: Macro Declarations
 */
#define FP 60000000
#define BAUDRATE 57600
#define BRGVAL ((FP/BAUDRATE)/16)-1
#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop(); // 105uS delay

#define EUSART1_TX_BUFFER_SIZE 20
#define EUSART1_RX_BUFFER_SIZE 126

/**
  Section: Global Variables
 */

static uint8_t eusart1TxHead = 0;
static uint8_t eusart1TxTail = 0;
static uint8_t eusart1TxBuffer[EUSART1_TX_BUFFER_SIZE];
volatile uint8_t eusart1TxBufferRemaining;

static uint8_t eusart1RxHead = 0;
static uint8_t eusart1RxTail = 0;
static uint8_t eusart1RxBuffer[EUSART1_RX_BUFFER_SIZE];
volatile uint8_t eusart1RxCount;

/**
  Section: EUSART1 APIs
 */

void EUSART1xInitialize(void) {
    
    U1MODEbits.STSEL = 0; // 1-Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    U1BRG = BRGVAL; // Baud Rate setting for 57600
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.URXISEL0 = 0; // Interrupt after one RX character is received;
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.URXISEL1 = 0;
    IEC0bits.U1TXIE = 1; // Enable UART TX interrupt
    IEC0bits.U1RXIE = 1; // Enable UART RX interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
        
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    DELAY_105uS


    // initializing the driver state
    eusart1TxHead = 0;
    eusart1TxTail = 0;
    eusart1TxBufferRemaining = sizeof (eusart1TxBuffer);

    eusart1RxHead = 0;
    eusart1RxTail = 0;
    eusart1RxCount = 0;
}

uint8_t EUSART1xRead(void) {
    uint8_t readValue = 0;

    while (0 == eusart1RxCount) {
    }

    IEC0bits.U1RXIE = 0;

    readValue = eusart1RxBuffer[eusart1RxTail++];
    if (sizeof (eusart1RxBuffer) <= eusart1RxTail) {
        eusart1RxTail = 0;
    }
    eusart1RxCount--;
    IEC0bits.U1RXIE = 1;

    return readValue;
}

void EUSART1xWrite(uint8_t txData) {
    while (0 == eusart1TxBufferRemaining) {
    }

    if (0 == IEC0bits.U1TXIE) {
        U1TXREG = txData;
    } else {
        IEC0bits.U1TXIE = 0;
        eusart1TxBuffer[eusart1TxHead++] = txData;
        if (sizeof (eusart1TxBuffer) <= eusart1TxHead) {
            eusart1TxHead = 0;
        }
        eusart1TxBufferRemaining--;
    }
    IEC0bits.U1TXIE = 1;
}

void EUSART1xTransmitxISR(void) {

    // add your EUSART1 interrupt custom code
    if (sizeof (eusart1TxBuffer) > eusart1TxBufferRemaining) {
        U1TXREG = eusart1TxBuffer[eusart1TxTail++];
        if (sizeof (eusart1TxBuffer) <= eusart1TxTail) {
            eusart1TxTail = 0;
        }
        eusart1TxBufferRemaining++;
    } else {
        IEC0bits.U1TXIE = 0;
    }
}

void EUSART1xReceivexISR(void) {
    if (1 == U1STAbits.OERR) {
        // EUSART1 error - restart

        U1STAbits.OERR = 0;        
    }

    /*
     from datasheet:
     * if(U1STAbits.URXDA == 1)
        {
        ReceivedChar = U1RXREG;
        }
     */
    // buffer overruns are ignored
    eusart1RxBuffer[eusart1RxHead++] = U1RXREG;
    if (sizeof (eusart1RxBuffer) <= eusart1RxHead) {
        eusart1RxHead = 0;
    }
    eusart1RxCount++;
}
/**
  End of File
 */
