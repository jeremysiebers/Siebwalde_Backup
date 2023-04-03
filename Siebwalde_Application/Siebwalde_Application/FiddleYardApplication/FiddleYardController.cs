﻿using System;

namespace Siebwalde_Application
{
    public interface iFiddleYardController
    {        
        void FYLinkActivityUpdate();                // Update Link activity in main form
        void ClearEventLoggers();                   // Clear event loggers interface to form eventloggers for clearing
        void ReConnect();                           // Re-connect to target
        Sender GetFYSender();                       // interface to FYSender
        Receiver GetFYReceiver();                   // interface to Receiver        
    }
    
    public class FiddleYardController : iFiddleYardController
    {
        public IMain m_iMain;                                       // connect variable to connect to FYController class to Main for application logging        
        public const int SEND_DELAY = 10;
        public FiddleYardIOHandle FYIOHandleTOP;
        public FiddleYardIOHandle FYIOHandleBOT;
        public const string FYTarget = "FIDDLEYARD";
        public Sender FYSender = new Sender(FYTarget);
        private Receiver FYReceiver;
        public PingTarget m_PingTarget = new PingTarget { };
        private int m_FYReceivingPort = 0;
        private int m_FYSendingPort   = 0;
        private bool FYSimulatorActive = false;        
        private byte[,] m_macAddr;
        private byte[,] m_ipAddr;

        public Sender GetFYSender()
        {
            return FYSender;
        }

        public Receiver GetFYReceiver()
        {
            return FYReceiver;
        }   

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController
         *               Constructor
         *              
         *               
         *
         *  Input(s)   : MacAddress, IPAddress, Receiving port for fiddleyard
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
        public FiddleYardController(IMain iMainCtrl, byte[,] macAddr, byte[,] ipAddr, int FYReceivingPort, int FYSendingPort)
        {
            m_iMain = iMainCtrl;                        // connect to Main interface for application text logging and link activity update, save interface in variable
            m_FYReceivingPort = FYReceivingPort;
            m_FYSendingPort   = FYSendingPort;
            m_macAddr = macAddr;
            m_ipAddr = ipAddr;
            FYReceiver = new Receiver(m_FYReceivingPort);
            FYIOHandleTOP = new FiddleYardIOHandle("TOP", this);
            FYIOHandleBOT = new FiddleYardIOHandle("BOT", this);                 
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController start
         *               Try to connect to FIDDLEYARD target py using ping
         *               if successful start the receiver
         *               else set logging to that the simulator will be active
         *               Start both the FYIOHanlde for TOP and BOTTOM
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
        public void Start()
        {
            if (ConnectFiddleYard(m_macAddr, m_ipAddr) == true) // when connection was succesfull and target was found and is connected
            {
                m_iMain.SiebwaldeAppLogging("FYCTRL: Fiddle Yard uController target in real mode");
                FYIOHandleTOP.FYApp.FYFORM.FiddleYardFormLogging.Log(GetType().Name, "###Fiddle Yard uController target in real mode###");
                FYIOHandleTOP.FYApp.FYFORM.SimMode(FYSimulatorActive);
                FYIOHandleBOT.FYApp.FYFORM.FiddleYardFormLogging.Log(GetType().Name, "###Fiddle Yard uController target in real mode###");
                FYIOHandleBOT.FYApp.FYFORM.SimMode(FYSimulatorActive);
                FYReceiver.Start();
            }
            else
            {
                m_iMain.SiebwaldeAppLogging("FYCTRL: Fiddle Yard uController target in simulator mode");
                FYIOHandleTOP.FYApp.FYFORM.FiddleYardFormLogging.Log(GetType().Name, "###Fiddle Yard uController target in simulation mode###");
                FYIOHandleTOP.FYApp.FYFORM.SimMode(FYSimulatorActive);
                FYIOHandleBOT.FYApp.FYFORM.FiddleYardFormLogging.Log(GetType().Name, "###Fiddle Yard uController target in simulation mode###");
                FYIOHandleBOT.FYApp.FYFORM.SimMode(FYSimulatorActive);
            }
            FYIOHandleTOP.Start(FYSimulatorActive);   // use these to disable this layer of the fiddle yard in order to do easy debugging by commenting this line
            FYIOHandleBOT.Start(FYSimulatorActive);   // use these to disable this layer of the fiddle yard in order to do easy debugging by commenting this line
            m_iMain.SiebwaldeAppLogging("FYCTRL: Fiddle Yard uController Reset.");      
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController ConnectFiddleYard
         *               
         *
         *  Input(s)   : macAddr, ipAddr
         *
         *  Output(s)  : FYSimulatorActive
         *
         *  Returns    : ConnectFiddleYard
         *
         *  Pre.Cond.  :
         *
         *  Post.Cond. :
         *
         *  Notes      :
         */
        /*#--------------------------------------------------------------------------#*/
        private bool ConnectFiddleYard(byte[,] macAddr, byte[,] ipAddr)
        {
            string PingReturn = "";
            try
            {
                m_iMain.SiebwaldeAppLogging("FYCTRL: Pinging FIDDLEYARD target...");
                PingReturn = m_PingTarget.TargetFound(FYTarget);
                if (PingReturn == "targetfound")
                {
                    m_iMain.SiebwaldeAppLogging("FYCTRL: Ping successfull.");

                    m_iMain.SiebwaldeAppLogging("FYCTRL: FiddleYard Connecting...");
                    FYSender.ConnectUdp(m_FYSendingPort);

                    m_iMain.SiebwaldeAppLogging("FYCTRL: FiddleYard Connected.");
                    m_iMain.SiebwaldeAppLogging("FYCTRL: FiddleYard Send MAC and IP...");

                    ProgramMACIPPORT(macAddr, ipAddr);
                    FYSimulatorActive = false;
                    return true; // connection succesfull to FIDDLEYARD
                }
                else
                {
                    m_iMain.SiebwaldeAppLogging("FYCTRL: " + PingReturn);
                    FYSimulatorActive = true;
                    return false; // ping was unsuccessfull    
                }

            }
            catch (Exception)
            {
                m_iMain.SiebwaldeAppLogging("FYCTRL: FiddleYard failed to connect.");
                FYSimulatorActive = true;
                return false; // ping was successfull but connecting failed
            }
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController ReConnect
         *               
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
        public void ReConnect()
        {
            if (FYSimulatorActive == false)
            {

            }
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController stop
         *               
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
        public void Stop()
        {
            if (FYSender != null) // when a real connection is made, close the UDP port
            {                
                FYSender.CloseUdp();
            }
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController ClearEventLoggers
         *               
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
        public void ClearEventLoggers()
        {
            FYIOHandleTOP.FYApp.FYFORM.ClearReceivedCmd();
            FYIOHandleBOT.FYApp.FYFORM.ClearReceivedCmd();
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController FYLinkActivityUpdate
         *               
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
        public void FYLinkActivityUpdate()
        {
            m_iMain.FYLinkActivityUpdate();
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController FYTOPShow
         *               
         *
         *  Input(s)   : autoscroll, height, width, LocX, LocY, View
         *
         *  Output(s)  : autoscroll, height, width, LocX, LocY, View
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
        public void FYTOPShow(bool autoscroll, int height, int width, int LocX, int LocY, bool View)
        {
            FYIOHandleTOP.FYApp.FYFORMShow(autoscroll, height, width, LocX, LocY, View);
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController FYBOTShow
         *               
         *
         *  Input(s)   : autoscroll, height, width, LocX, LocY, View
         *
         *  Output(s)  : autoscroll, height, width, LocX, LocY, View
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
        public void FYBOTShow(bool autoscroll, int height, int width, int LocX, int LocY, bool View)
        {
            FYIOHandleBOT.FYApp.FYFORMShow(autoscroll, height, width, LocX, LocY, View);
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardController ProgramMACIPPORT
         *               
         *
         *  Input(s)   : macAddr, ipAddr
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
        private void ProgramMACIPPORT(byte[,] macAddr, byte[,] ipAddr)
        {
            byte[] Send = new byte[3];
            for (int i = 0; i < 12; i++)
            {
                Send[0] = macAddr[i, 0];
                Send[1] = macAddr[i, 1];
                Send[2] = macAddr[i, 2];
                FYSender.SendUdp(Send);
                System.Threading.Thread.Sleep(50);
            }
            m_iMain.SiebwaldeAppLogging("FYCTRL: FiddleYard MAC is sent.");

            for (int i = 0; i < 4; i++)
            {
                Send[0] = ipAddr[i, 0];
                Send[1] = ipAddr[i, 1];
                Send[2] = ipAddr[i, 2];
                FYSender.SendUdp(Send);
                System.Threading.Thread.Sleep(50);
            }
            m_iMain.SiebwaldeAppLogging("FYCTRL: FiddleYard IP is sent.");

            System.Threading.Thread.Sleep(50);

            // Also sent to FY uController to set the port on which it has to sent its data to the PC: m_FYReceivingPort
            Send[0] = Convert.ToByte('r');
            Send[1] = Convert.ToByte(m_FYReceivingPort >> 8);
            Send[2] = 0xD;
            FYSender.SendUdp(Send);
            System.Threading.Thread.Sleep(50);
            
            Send[0] = Convert.ToByte('s');
            Send[1] = Convert.ToByte(m_FYReceivingPort & 0x00FF);// >> 8);
            Send[2] = 0xD;
            FYSender.SendUdp(Send);
            System.Threading.Thread.Sleep(50);
            m_iMain.SiebwaldeAppLogging("FYCTRL: FiddleYard sending Port is sent.");



            Send[0] = Convert.ToByte('t');
            Send[1] = 0x1;
            Send[2] = 0xD;
            FYSender.SendUdp(Send);
            System.Threading.Thread.Sleep(50);
            m_iMain.SiebwaldeAppLogging("FYCTRL: FiddleYard MAC_IP_READY is sent.");
        }               
        
    }
}
