/*
 * File:   commhandler.c
 * Author: Jeremy Siebers
 *
 * Created on aug 28, 2018, 14:15 PM
 */
#include <xc.h>
#include "main.h"
#include "mcc_generated_files/mcc.h"
#include "modbus/Interrupts.h"
#include "modbus/PetitModbus.h"

#define MAILBOX_SIZE 4                                                          // How many messages are non-critical

void SendDataToEthernet(void);

/*#--------------------------------------------------------------------------#*/
/*  Description: InitSlaveCommunication(SLAVE_INFO *location)
 *
 *  Input(s)   : location of stored data array of struct
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
static SLAVE_INFO   *MASTER_SLAVE_DATA = 0;                                     // Holds the address were the received slave data is stored
static SLAVE_INFO   SlaveInfoReadMask;                                          // Read mask for slave data from EhternetTarget
unsigned char       *pSlaveDataReceived, *pSlaveInfoReadMask;

void InitSlaveCommunication(SLAVE_INFO *location)                                  
{   
    MASTER_SLAVE_DATA  =  location;
    SlaveInfoReadMask.Header           = 0x00;
    SlaveInfoReadMask.SlaveNumber      = 0x00;                                  // Mask for data write to local MASTER_SLAVE_DATA from EthernetTarget
    SlaveInfoReadMask.HoldingReg[0]    = 0xFFFF;                                // only new setpoints/settings are allowed to be read which need to be written to modbus slaves
    SlaveInfoReadMask.HoldingReg[1]    = 0xFFFF;
    SlaveInfoReadMask.HoldingReg[2]    = 0xFFFF;
    SlaveInfoReadMask.HoldingReg[3]    = 0xFFFF;
    SlaveInfoReadMask.InputReg[0]      = 0x0000;
    SlaveInfoReadMask.InputReg[1]      = 0x0000;
    SlaveInfoReadMask.InputReg[2]      = 0x0000;
    SlaveInfoReadMask.InputReg[3]      = 0x0000;
    SlaveInfoReadMask.InputReg[4]      = 0x0000;
    SlaveInfoReadMask.InputReg[5]      = 0x0000;
    SlaveInfoReadMask.DiagReg[0]       = 0x0000;
    SlaveInfoReadMask.DiagReg[1]       = 0x0000;
    SlaveInfoReadMask.MbReceiveCounter = 0x0000;
    SlaveInfoReadMask.MbSentCounter    = 0x0000;
    SlaveInfoReadMask.MbCommError      = 0x0000;
    SlaveInfoReadMask.MbExceptionCode  = 0x00;
    SlaveInfoReadMask.SpiCommErrorCounter = 0x0000;
    SlaveInfoReadMask.Footer           = 0x00;
}

/*#--------------------------------------------------------------------------#*/
/*  Description: ProcessNextSlave()
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
 *  Notes      : Handles communication message to all slaves
 */
/*#--------------------------------------------------------------------------#*/

typedef enum
{
    NrRegLo = 0,
    NrRegHi = 1,
    StAdLo  = 2,
    StAdHi  = 3     
};

typedef enum
{
    MESSAGE1 = 1,
    MESSAGE2 = 2,
    MESSAGE3 = 3,
    MESSAGE4 = 4 
};
unsigned char HoldingRegistersRead [4] = {0, 0, 0, 0};                          // {start address High, start address Low, number of registers High, number of registers Low}
unsigned char HoldingRegistersWrite[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};           // {start address High, start address Low, number of registers High, number of registers Low, byte count, Register Value Hi, Register Value Lo, Register Value Hi, Register Value Lo} 
unsigned char   InputRegistersRead [4] = {0, 0, 0, 0};
unsigned char    DiagRegistersRead [4] = {0, 0, 0, 0};

static unsigned char ProcessSlave = 1;
static unsigned int Mailbox = 1;
static unsigned int Message = MESSAGE1;

void ProcessNextSlave(){    
    
    if (ProcessSlave > (NUMBER_OF_SLAVES-1)){
        ProcessSlave = 1;
        
        Message++;
        if (Message > MESSAGE3){
            Message = MESSAGE1;
            
            Mailbox++;
            if (Mailbox > 4){
                Mailbox = 1;
            }
        }                
    }
    
    switch (Message){
        case MESSAGE1:
            HoldingRegistersWrite[8]  = MASTER_SLAVE_DATA[ProcessSlave].HoldingReg[1];
            HoldingRegistersWrite[7]  = MASTER_SLAVE_DATA[ProcessSlave].HoldingReg[1] >> 8;
            HoldingRegistersWrite[6]  = MASTER_SLAVE_DATA[ProcessSlave].HoldingReg[0];
            HoldingRegistersWrite[5]  = MASTER_SLAVE_DATA[ProcessSlave].HoldingReg[0] >> 8;
            HoldingRegistersWrite[4]  = 4;
            HoldingRegistersWrite[3]  = 2;
            HoldingRegistersWrite[2]  = 0;
            HoldingRegistersWrite[1]  = 0;
            HoldingRegistersWrite[0]  = 0;
            SendPetitModbus(ProcessSlave, PETITMODBUS_WRITE_MULTIPLE_REGISTERS, HoldingRegistersWrite, 9);
            break;

        case MESSAGE2:
            InputRegistersRead[3]  = 2;
            InputRegistersRead[2]  = 0;
            InputRegistersRead[1]  = 0;
            InputRegistersRead[0]  = 0;
            SendPetitModbus(ProcessSlave, PETITMODBUS_READ_INPUT_REGISTERS, InputRegistersRead, 4);    
            break;

        case MESSAGE3:
            switch (Mailbox){
                case 1:
                    HoldingRegistersWrite[8]  = MASTER_SLAVE_DATA[ProcessSlave].HoldingReg[3];
                    HoldingRegistersWrite[7]  = MASTER_SLAVE_DATA[ProcessSlave].HoldingReg[3] >> 8;
                    HoldingRegistersWrite[6]  = MASTER_SLAVE_DATA[ProcessSlave].HoldingReg[2];
                    HoldingRegistersWrite[5]  = MASTER_SLAVE_DATA[ProcessSlave].HoldingReg[2] >> 8;
                    HoldingRegistersWrite[4]  = 4;
                    HoldingRegistersWrite[3]  = 2;
                    HoldingRegistersWrite[2]  = 0;
                    HoldingRegistersWrite[1]  = 2;
                    HoldingRegistersWrite[0]  = 0;
                    SendPetitModbus(ProcessSlave, PETITMODBUS_WRITE_MULTIPLE_REGISTERS, HoldingRegistersWrite, 9);
                    break;
                    
                case 2:
                    DiagRegistersRead[3]  = 2;
                    DiagRegistersRead[2]  = 0;
                    DiagRegistersRead[1]  = 0;
                    DiagRegistersRead[0]  = 0;
                    SendPetitModbus(ProcessSlave, PETITMODBUS_DIAGNOSTIC_REGISTERS, DiagRegistersRead, 4);
                    break;
                
                case 3:
                    InputRegistersRead[3]  = 2;
                    InputRegistersRead[2]  = 0;
                    InputRegistersRead[1]  = 2;
                    InputRegistersRead[0]  = 0;
                    SendPetitModbus(ProcessSlave, PETITMODBUS_READ_INPUT_REGISTERS, InputRegistersRead, 4);  
                    break;
                
                case 4:
                    InputRegistersRead[3]  = 2;
                    InputRegistersRead[2]  = 0;
                    InputRegistersRead[1]  = 4;
                    InputRegistersRead[0]  = 0;
                    SendPetitModbus(ProcessSlave, PETITMODBUS_READ_INPUT_REGISTERS, InputRegistersRead, 4);  
                    break;
                
                default:
                    break;                
            }
            break;
            
        default :
            break;
    }  
}

/*#--------------------------------------------------------------------------#*/
/*  Description: ProcessSlaveCommunication()
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
 *  Notes      : Keeps track of communication with a slave
 */
/*#--------------------------------------------------------------------------#*/
unsigned int ProcessSlaveCommunication(){
    
    unsigned int Return_Val = false;
    
    switch (MASTER_SLAVE_DATA[ProcessSlave].MbCommError){
        case SLAVE_DATA_BUSY:
            Return_Val = false;
            // count here how long the Mod-bus stack is busy, otherwise reset/action             
            break;
            
        case SLAVE_DATA_NOK:
            // count here how often the slave data is NOK, otherwise stop all slaves with broadcast
            MASTER_SLAVE_DATA[ProcessSlave].MbCommError = SLAVE_DATA_IDLE;            
            ProcessSlave++;         
            Return_Val = true;
            LED_ERR_LAT ^= 1;
            break;
            
        case SLAVE_DATA_OK:
            MASTER_SLAVE_DATA[ProcessSlave].MbCommError = SLAVE_DATA_IDLE;            
            ProcessSlave++;         
            Return_Val = true;
            LED_ERR_LAT = 0;
            break;            
            
        case SLAVE_DATA_TIMEOUT:
            // count here how often the slave data is timeout, otherwise stop all slaves with broadcast
            MASTER_SLAVE_DATA[ProcessSlave].MbCommError = SLAVE_DATA_IDLE;            
            ProcessSlave++;         
            Return_Val = true;
            LED_ERR_LAT ^= 1;
            break;
            
        case SLAVE_DATA_EXCEPTION:
            // count here how often the slave data is timeout, otherwise stop all slaves with broadcast
            MASTER_SLAVE_DATA[ProcessSlave].MbCommError = SLAVE_DATA_IDLE;            
            ProcessSlave++;         
            Return_Val = true;
            LED_ERR_LAT ^= 1;
            break;
            
        default : Return_Val = true;                                            // Idle is here, still send data over SPI to Ethernet Target        
            break;
    }
    
    if (Return_Val == true){
        //__delay_us(60);                                                       // when debugging is required on oscilloscope, waittime till tmr3 is done
        while (T2TMR < 0xD0){                                                   // Send SPI data on a defined moment in time within the 2ms TMR2 interrupt
            
        }
        SendDataToEthernet();
    }
    
    return (Return_Val);
}

/*#--------------------------------------------------------------------------#*/
/*  Description: SendDataToEthernet()
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
 *  Notes      : Keeps track of communication with a slave
 */
/*#--------------------------------------------------------------------------#*/
static unsigned int DataFromSlave = 0;
const unsigned char DATAxSTRUCTxLENGTH = sizeof(SLAVE_INFO) + 1;      // add one byte to send dummy
static unsigned char RECEIVEDxDATAxRAW[DATAxSTRUCTxLENGTH];                            // One dummy byte extra (SPI master will send extra byte to receive last byte from slave)
static uint8_t bytesWritten = 0;
static uint8_t *dataIn, *dataOut;

void SendDataToEthernet(){
    //modbus_sync_LAT = 1;
    //SPI1_Exchange8bitBuffer(&(MASTER_SLAVE_DATA[DataFromSlave].Header), 
    //        DATAxSTRUCTxLENGTH, &(RECEIVEDxDATAxRAW[0]));                       // SPI send/receive data    
    bytesWritten = 0;
    dataIn  = &(MASTER_SLAVE_DATA[DataFromSlave].Header);
    dataOut = &(RECEIVEDxDATAxRAW[0]);
    SS1_LAT = 0;                                                                // Activate slave
    while(bytesWritten < DATAxSTRUCTxLENGTH){
        SSP1CON1bits.WCOL = 0;
        SSP1BUF = dataIn[bytesWritten];
        while(SSP1STATbits.BF == 0){
        }
        dataOut[bytesWritten] = SSP1BUF;
        bytesWritten++;
    }
    SS1_LAT = 1;   
    
    if(RECEIVEDxDATAxRAW[2] < NUMBER_OF_SLAVES && RECEIVEDxDATAxRAW[1]==0xAA && 
            RECEIVEDxDATAxRAW[DATAxSTRUCTxLENGTH-1]==0x55){                     // Check if received slave number is valid(during debugging sometimes wrong number received)
        pSlaveDataReceived = &(MASTER_SLAVE_DATA[RECEIVEDxDATAxRAW[2]].Header); // set the pointer to the first element of the received slave number in RECEIVEDxDATAxRAW[1](first element is dummy byte)    
        pSlaveInfoReadMask = &(SlaveInfoReadMask.Header);                       // set the pointer to the first element of the SlaveInfoReadMask
        for(char i = 1; i < DATAxSTRUCTxLENGTH-1; i++){
            if(*pSlaveInfoReadMask){
                *pSlaveDataReceived = RECEIVEDxDATAxRAW[i];                     // for DATAxSTRUCTxLENGTH set every byte into RECEIVEDxDATAxRAW array according to read mask
            }        
            pSlaveDataReceived += 1;                                            // Increment pointer
            pSlaveInfoReadMask += 1;                                            // Increment pointer        
        }   
    } 
    else{
        MASTER_SLAVE_DATA[0].SpiCommErrorCounter += 1;                          // Count error SPI messages 
    }
    
    DataFromSlave++;                                                            // send data from next slave
    if (DataFromSlave > NUMBER_OF_SLAVES-1){
        DataFromSlave = 0;
    }
    //modbus_sync_LAT = 0;
}