/**
  Generated Main Source File

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
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "mcc_generated_files/mcc.h"
#include "spicommhandler.h"
#include "slavestartup.h"
#include "mcc_generated_files/TCPIPLibrary/udpv4.h"
#include "mcc_generated_files/TCPIPLibrary/tcpip_config.h"
//#include "mcc_generated_files/TCPIPLibrary/dhcp_client.h"

#define HEADER 0xAA
#define FOOTER 0x55

void ToFromTerminal(void);
void Communications(void);
void SlaveAndIoData(void);
void BootLoaderData(void);

void WriteText(unsigned char *Text, unsigned int Ln, unsigned int Col);
void WriteData(unsigned int Data, unsigned int Ln, unsigned int Col);

/*
                         Main application
 */

/*----------------------------------------------------------------------------*/
static SLAVE_INFO         SlaveInfo[NUMBER_OF_SLAVES];   
static SLAVE_INFO         EthernetTarget;

static udpStart_t udpPacket;
static uint8_t DataFromSlaveSend = 0;
bool Init_UDP = true;
uint8_t division = 0;
uint8_t SlaveDataTx[2] = {HEADER, 0};
uint8_t *pSlaveDataSend;

unsigned int StateMachine = 0;

uint8_t UPDATE_TERMINAL     = false;
uint8_t UPDATE_SLAVE_TOxUDP = false; 
uint8_t UPDATE_MODBUS_DATA  = false;

uint8_t DUMMY[3] = {0xAA,0xEE,0x55};
typedef enum
{
    READ_VERSION  = 0x00, 
    READ_FLASH    = 0x01,
    WRITE_FLASH   = 0x02, 
    ERASE_FLASH   = 0x03,
    READ_EE_DATA  = 0x04,
    WRITE_EE_DATA = 0x05,
    READ_CONFIG   = 0x06,
    WRITE_CONFIG  = 0x07,
    CALC_CHECKSUM = 0x08,
    RESET_DEVICE  = 0x09,
    NEXT_TRANSMISSION = 0xFF
};

static uint8_t BytesToSent = 0;
static uint8_t BytesToReceive = 63;

typedef enum
{
    MODBUS_CMD          = 0xFF,
    BOOTLOAD_CMD        = 0xFE,
    ETHERNET_CMD        = 0xFD,
    DUMMY_CMD           = 0xFC,
            
    SEND_SLAVEIODATA    = 0x00,
    SEND_BOOTLOADER     = 0x01,
    INIT_PHASE_TRUE     = 0xFC,
    INIT_PHASE_FALSE    = 0xFD,
    RELEASE_ALL         = 0xFE,
    RESET_ALL           = 0xFF,
};

typedef enum 
{
    ETH_OK          = 0x82, 
    ETH_NOK         = 0x83, 
    ETH_BUSY        = 0x81,
    ETH_IDLE        = 0x80,    
};

typedef struct
{
    uint8_t header;
    uint8_t command;
    uint8_t data[80]; 
}udpDemoRecv_t;

udpDemoRecv_t udpRecv;

uint8_t ProcessEthData = false;

/*----------------------------------------------------------------------------*/

void main(void)
{
    // Initialize the SLAVE_INFO struct with slave numbers(fixed))
    for (unsigned int i = 0; i <NUMBER_OF_SLAVES; i++){
        SlaveInfo[i].SlaveNumber = i;
        SlaveInfo[i].Header = HEADER;
        SlaveInfo[i].Footer = FOOTER;
        if (i > 0){
            SlaveInfo[i].HoldingReg[0] = 0x818F;                                // Set all EMO and 50%
            SlaveInfo[i].HoldingReg[2] = i;                                     // Set address of slave
        }
    }
    
    EthernetTarget.SlaveNumber = NUMBER_OF_SLAVES;
    EthernetTarget.Header = HEADER;
    EthernetTarget.Footer = FOOTER;
    
/*----------------------------------------------------------------------------*/
    
    // Initialize the device
    SYSTEM_Initialize();
    
    INITxSPIxCOMMxHANDLER(SlaveInfo);                                           // Init SPI slave - master registers
        
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    /*----------------------------------------------------------------------------*/    

    /* UDP Packet Initializations*/
    udpPacket.destinationAddress = MAKE_IPV4_ADDRESS(192,168,1,19);
    udpPacket.sourcePortNumber = 65533;
    udpPacket.destinationPortNumber = 65531;

    error_msg ret = ERROR;
    /*            
    printf("PIC18f97j60 started up!!!\n\r");
    __delay_ms(10);
    printf("\f");                                                               // Clear terminal (printf("\033[2J");)
    __delay_ms(10);
    printf("\033[?25h");
    __delay_ms(10);   
    printf("PIC18f97j60 started up!!!\n\r");                                    // Welcome message
    */
    //TMR0_StartTimer();
    SEND_UDP_SetDigitalInput();
    LED3_SetDigitalOutput();
/*----------------------------------------------------------------------------*/
    
    while (1)
    {
        
        switch(StateMachine){
            case 0:
                Read_Check_LAT = 1;
                Network_Manage();
                Read_Check_LAT = 0;
                if(ETH_CheckLinkUp()){
                    StateMachine ++;
                }
                break;
                
            case 1:
                Read_Check_LAT = 1;
                Network_Manage();
                Read_Check_LAT = 0;
//                if(DHCP_BOUND()){
                    ModbusReset_LAT = 0;                                        // as last release the ModbusMaster.
                    ModbusReset_SetDigitalOutput();                             // set the Pin direction here to output to avoid power-on reset pulses to slaves!
                    StateMachine = 4;
//                }
                break;
                
            default :   
                break;
        }
        
        if(UPDATE_MODBUS_DATA == true){
            UPDATE_MODBUS_DATA = false;
            LED1_LAT = true;
            PROCESSxMODBUSxDATA();
            LED1_LAT = false;
        }
        
//        if(UPDATE_TERMINAL == true){
            Read_Check_LAT = 1;
            Network_Manage();
            Read_Check_LAT = 0;                        
//        }
        
        if((UPDATE_SLAVE_TOxUDP == true) && (TMR0L > 50)){
            UPDATE_SLAVE_TOxUDP = false;
            if(COMM_MODE_BOOTLOAD == false){
                LED1_LAT = 1;  
                ret = UDP_Start(udpPacket.destinationAddress, udpPacket.sourcePortNumber, udpPacket.destinationPortNumber);
                if(ret == SUCCESS)
                { 
                    LED2_LAT = 1;
                    Network_Manage();
                    if(DataFromSlaveSend == NUMBER_OF_SLAVES){
                        //pSlaveDataSend = &(EthernetTarget.Header);
                        SlaveDataTx[1] = ETHERNET_CMD;                              // send the Data Type
                        UDP_WriteBlock(SlaveDataTx, 2);
                        UDP_WriteBlock(&(EthernetTarget.Header), DATAxSTRUCTxLENGTH);
                    }
                    else{
                        //pSlaveDataSend = &(SlaveInfo[DataFromSlaveSend].Header);
                        SlaveDataTx[1] = MODBUS_CMD;                                // send the Data Type
                        UDP_WriteBlock(SlaveDataTx, 2);
                        UDP_WriteBlock(&(SlaveInfo[DataFromSlaveSend].Header), DATAxSTRUCTxLENGTH);
                    }
                    Network_Manage();
                    UDP_Send();
                    Network_Manage();

                    if (InitPhase == false){                                        // When init phase is done, communicate data to all slaves
                        DataFromSlaveSend++;   
                        if(DataFromSlaveSend > NUMBER_OF_SLAVES){                   // Defined are 51 slaves --> 51 == Ethernet target
                            DataFromSlaveSend = 0;
                        }                    
                    }
                    else{
                        if (DataFromSlaveSend == 0){
                            DataFromSlaveSend = NUMBER_OF_SLAVES;
                        }
                        else if(DataFromSlaveSend == NUMBER_OF_SLAVES){
                            DataFromSlaveSend = 0;
                        }            
                    }
                    LED2_LAT = 0;
                }
                else{
                    //printf("ret: %02X\n\r", ret);
                    if(ret == BUFFER_BUSY){
                        //printf("Flush and reset.");
                        //UDP_FlushTXPackets();
                        UDP_FlushRxdPacket(); 
                    }                
                }
                LED1_LAT = 0;
            }
        
//            else{
//                LED1_LAT = 1;  
//                ret = UDP_Start(udpPacket.destinationAddress, udpPacket.sourcePortNumber, udpPacket.destinationPortNumber);
//                if(ret == SUCCESS)
//                { 
//                    LED2_LAT = 1;
//                    Network_Manage();
////                    UDP_Write16(0xC0DE);
////                    UDP_Send();
//                    
//                    //pSlaveDataSend = &(RECEIVED_BOOT_LOAD_DATA[0]);
//                    SlaveDataTx[1] = BOOTLOAD_CMD;                              // send the Data Type
//                    UDP_WriteBlock(SlaveDataTx, 2);
////                    UDP_WriteBlock(&(RECEIVED_BOOT_LOAD_DATA[0]), DATAxSTRUCTxLENGTH2);
//                    UDP_WriteBlock(&(EthernetTarget.Header), DATAxSTRUCTxLENGTH);
//                    Network_Manage();
//                    UDP_Send();
//                    Network_Manage();                  
//                    LED2_LAT = 0;
//                }
//                else{
//                    printf("ret: %02X\n\r", ret);
//                    if(ret == BUFFER_BUSY){
//                        printf("Flush and reset.");
//                        //UDP_FlushTXPackets();
//                        UDP_FlushRxdPacket(); 
//                    }                
//                }
//                LED1_LAT = 0;
//            }
        }
        
        if(ProcessEthData == true){
            ProcessEthData = false;
            LED1_LAT = 1;
            if(udpRecv.header == HEADER)
            {
                switch(udpRecv.command)
                {
                    case MODBUS_CMD:
                        if(udpRecv.data[8] == FOOTER){
                            //printf("MODBUS_CMD received.\n\r");
                            SlaveInfo[0].HoldingReg[1] = ((uint16_t)udpRecv.data[3]<<8) + udpRecv.data[2];
                            SlaveInfo[0].HoldingReg[2] = ((uint16_t)udpRecv.data[5]<<8) + udpRecv.data[4];
                            SlaveInfo[0].HoldingReg[3] = ((uint16_t)udpRecv.data[7]<<8) + udpRecv.data[6];
                            SlaveInfo[0].HoldingReg[0] = ((uint16_t)udpRecv.data[1]<<8) + udpRecv.data[0];
                        }
                        break;

                    case ETHERNET_CMD:
                        if(udpRecv.data[1] == FOOTER){
                            switch(udpRecv.data[0]){
                                case RESET_ALL:
                                    //printf("Reset All slaves...\n\r");
        //                            RESET();
                                    COMM_MODE_BOOTLOAD = false;
                                    RCSTA1 = 0x00;
                                    PIE1bits.RC1IE = 0;
                                    PIE3bits.SSP2IE = 1;
                                    InitPhase = true;
                                    DataFromSlaveSend = 0;
                                    RESETxSPIxCOMMxHANDLER();
                                    ModbusReset_LAT = 1;
                                    __delay_ms(10);
                                    ModbusReset_LAT = 0;
                                    //__delay_ms(4000);
                                    EthernetTarget.InputReg[0] = ETH_OK;
                                    break;

                                case RELEASE_ALL:
                                    Read_Check_LAT ^= 1;
                                    EthernetTarget.InputReg[0] = ETH_IDLE;
                                    break;

                                case SEND_SLAVEIODATA:
                                   PIE3bits.SSP2IE = 1;
                                   COMM_MODE_BOOTLOAD = false;
                                   break;

                                case SEND_BOOTLOADER:
                                    PIE3bits.SSP2IE = 0;
                                    // enable receive interrupt
                                    RCSTA1 = 0x90;
                                    PIE1bits.RC1IE = 1;
                                    COMM_MODE_BOOTLOAD = true;
                                    break;

                                case ETH_IDLE:
                                    EthernetTarget.InputReg[0] = ETH_IDLE;
                                    //printf("EthernetTarget == ETH_IDLE\n\r");
                                    break;

                                case INIT_PHASE_TRUE:
                                    InitPhase = true;
                                    break;

                                case INIT_PHASE_FALSE:
                                    InitPhase = false;
                                    break;

                                default :
                                    break;
                            }
                        }
                        break;

                    case BOOTLOAD_CMD: 
                        NOP();                        
                        switch(udpRecv.data[1]){
                            case READ_VERSION       : BytesToSent = 10;   BytesToReceive = 26; break;
                            case READ_FLASH         : BytesToSent = 0;    BytesToReceive = 0;  break;
                            case WRITE_FLASH        : BytesToSent = 75;   BytesToReceive = 11; break;
                            case ERASE_FLASH        : BytesToSent = 10;   BytesToReceive = 11; break;
                            case READ_EE_DATA       : BytesToSent = 0;    BytesToReceive = 0;  break;
                            case WRITE_EE_DATA      : BytesToSent = 0;    BytesToReceive = 0;  break;
                            case READ_CONFIG        : BytesToSent = 0;    BytesToReceive = 0;  break;
                            case WRITE_CONFIG       : BytesToSent = 10;   BytesToReceive = 11; break;
                            case CALC_CHECKSUM      : BytesToSent = 10;   BytesToReceive = 12; break;
                            case RESET_DEVICE       : BytesToSent = 10;   BytesToReceive = 11; break;                            
                            default:                                                           break;            
                        }                        
                        for(uint8_t i = 0; i < BytesToSent; i++){
                            EUSART1_Write(udpRecv.data[i]);
                        }
                        break;      

                    default:
                        break;
                }
            }
            LED1_LAT = 0;
        }
            
        if(COMM_MODE_BOOTLOAD == true){
            if(EUSART1_is_rx_ready() == BytesToReceive){
                
                LED1_LAT = 1;  
                ret = UDP_Start(udpPacket.destinationAddress, udpPacket.sourcePortNumber, udpPacket.destinationPortNumber);
                if(ret == SUCCESS)
                { 
                    LED2_LAT = 1;
                    Network_Manage();
                    SlaveDataTx[1] = BOOTLOAD_CMD;                              // send the Data Type
                    UDP_WriteBlock(SlaveDataTx, 2);
                    //UDP_WriteBlock(&(eusart1RxBuffer), EUSART1_RX_BUFFER_SIZE);
                    for(uint8_t i = 0; i < EUSART1_is_rx_ready(); i++){
                        UDP_Write8(EUSART1_Read());
                    }
                    Network_Manage();
                    UDP_Send();
                    Network_Manage();                  
                    LED2_LAT = 0;
                }
                else{
                    //printf("ret: %02X\n\r", ret);
                    if(ret == BUFFER_BUSY){
                        //printf("Flush and reset.");
                        //UDP_FlushTXPackets();
                        UDP_FlushRxdPacket(); 
                    }                
                }
                LED1_LAT = 0;
            }
//            else{
//                LED1_LAT = 1;  
//                ret = UDP_Start(udpPacket.destinationAddress, udpPacket.sourcePortNumber, udpPacket.destinationPortNumber);
//                if(ret == SUCCESS)
//                { 
//                    LED2_LAT = 1;
//                    Network_Manage();
//                    SlaveDataTx[1] = DUMMY_CMD;                              // send the Data Type
//                    UDP_WriteBlock(SlaveDataTx, 2);
//                    UDP_WriteBlock(DUMMY, 3);
//                    Network_Manage();
//                    UDP_Send();
//                    Network_Manage();                  
//                    LED2_LAT = 0;
//                }
//                else{
//                    //printf("ret: %02X\n\r", ret);
//                    if(ret == BUFFER_BUSY){
//                        //printf("Flush and reset.");
//                        //UDP_FlushTXPackets();
//                        UDP_FlushRxdPacket(); 
//                    }                
//                }
//                LED1_LAT = 0;
//            }
        }
    }
}

/*
void BOOTxLOADxHANDLER(){
    if(BOOT_LOAD_DATA_READY == true){
        switch(BOOT_DATA_TO_SLAVE[2]){
            case READ_VERSION       : BytesToSent = 10;   BytesToReceive = 26; break;
            case READ_FLASH         : BytesToSent = 0;    BytesToReceive = 0;  break;
            case WRITE_FLASH        : BytesToSent = 75;   BytesToReceive = 11; break;
            case ERASE_FLASH        : BytesToSent = 10;   BytesToReceive = 11; break;
            case READ_EE_DATA       : BytesToSent = 0;    BytesToReceive = 0;  break;
            case WRITE_EE_DATA      : BytesToSent = 0;    BytesToReceive = 0;  break;
            case READ_CONFIG        : BytesToSent = 0;    BytesToReceive = 0;  break;
            case WRITE_CONFIG       : BytesToSent = 10;   BytesToReceive = 11; break;
            case CALC_CHECKSUM      : BytesToSent = 10;   BytesToReceive = 12; break;
            case RESET_DEVICE       : BytesToSent = 10;   BytesToReceive = 11; break;
            case NEXT_TRANSMISSION  : BytesToSent = 0;    BytesToReceive = 0; 
                                      BOOT_LOAD_DATA_READY = false;            break;
            default:                                                           break;            
        }
        
        if(BOOT_LOAD_DATA_SENT == false){
            for(uint8_t i = 0; i < BytesToSent; i++){
                EUSART1_Write(BOOT_DATA_TO_SLAVE[i]);
            }
            BOOT_LOAD_DATA_SENT = true;
        }
    }
    else if(EUSART1_is_rx_ready() > (BytesToReceive - 1)){
        for(uint8_t i = 0; i < BytesToReceive; i++){
            BOOT_DATA_TO_ETHERNET[i] = EUSART1_Read();
        }        
    }    
}
*/
/*#--------------------------------------------------------------------------#*/
/*  Description: UDP_DATA_RECV()
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



void UDP_DATA_RECV(int length)
{    
    UDP_ReadBlock(&udpRecv,sizeof(udpDemoRecv_t));
    ProcessEthData = true;
}
    
//    printf("header received: %02X\n\r"    , udpRecv.header);
//    printf("command received: %02X\n\r"   , udpRecv.command);
//    printf("data[0 ]: %02X\n\r"            , udpRecv.data[0 ]);
//    printf("data[1 ]: %02X\n\r"            , udpRecv.data[1 ]);
//    printf("data[2 ]: %02X\n\r"            , udpRecv.data[2 ]);
//    printf("data[3 ]: %02X\n\r"            , udpRecv.data[3 ]);
//    printf("data[4 ]: %02X\n\r"            , udpRecv.data[4 ]);
//    printf("data[5 ]: %02X\n\r"            , udpRecv.data[5 ]);
//    printf("data[6 ]: %02X\n\r"            , udpRecv.data[6 ]);
//    printf("data[7 ]: %02X\n\r"            , udpRecv.data[7 ]);
//    printf("data[8 ]: %02X\n\r"            , udpRecv.data[8 ]);
//    printf("data[9 ]: %02X\n\r"            , udpRecv.data[9 ]);
//    printf("data[10]: %02X\n\r"            , udpRecv.data[10]);
//	  printf("data[11]: %02X\n\r"            , udpRecv.data[11]);
//    printf("data[12]: %02X\n\r"            , udpRecv.data[12]);
    
    