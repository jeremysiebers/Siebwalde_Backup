﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Siebwalde_Application
{
    public class FiddleYardTrainDetection
    {
        private FiddleYardIOHandleVariables m_FYIOHandleVar;             // connect variable to connect to FYIOH class for defined variables
        private FiddleYardApplicationVariables m_FYAppVar;
        private FiddleYardMip50 m_FYMIP50;
        private ILogger m_FYAppLog;

        private enum State
        {
            Idle, MoveToTrack1, MoveToTrack11, TDT
        };
        private State State_Machine;
        private bool CL10Heart = false;
        private bool F10 = false;

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardTrainDetection
         *               Constructor
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
        public FiddleYardTrainDetection(FiddleYardIOHandleVariables FYIOHandleVar, FiddleYardApplicationVariables FYAppVar, FiddleYardMip50 FYMIP50, ILogger FiddleYardApplicationLogging)
        {
            m_FYIOHandleVar = FYIOHandleVar;
            m_FYAppVar = FYAppVar;
            m_FYMIP50 = FYMIP50;
            m_FYAppLog = FiddleYardApplicationLogging;
            State_Machine = State.Idle;

            Sensor Sns_CL_10_Heart = new Sensor("CL10Heart", " CL 10 Heart ", 0, (name, val, log) => SetLedIndicator(name, val, log)); // initialize and subscribe sensors
            FYAppVar.CL10Heart.Attach(Sns_CL_10_Heart);
            Sensor Sns_F10 = new Sensor("F10", " F10 ", 0, (name, val, log) => SetLedIndicator(name, val, log)); // initialize and subscribe sensors
            FYAppVar.F10.Attach(Sns_F10);
        }

        void SetLedIndicator(string name, int val, string log)
        {
            if (name == "CL10Heart")
            {
                if(val == 0)
                {
                    CL10Heart = false;
                }
                else if (val > 0)
                {
                    CL10Heart = true;
                }
            }
            else if (name == "F10")
            {
                if (val == 0)
                {
                    F10 = false;
                }
                else if (val > 0)
                {
                    F10 = true;
                }
            }
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardInitReset()
         *               Reset
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
        public void FiddleYardTdtReset()
        {
            State_Machine = State.Idle;            
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: Init()
         *               This wil try to initialise the Fiddle yard, checking various
         *               start conditions and start a train detection
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
        public string Traindetection()
        {
            string _Return = "Busy";
            string SubProgramReturnVal = null;

            switch (State_Machine)
            {
                case State.Idle:
                    if(m_FYAppVar.GetTrackNr() != 1 && m_FYAppVar.GetTrackNr() != 11)
                    {
                        if (m_FYAppVar.GetTrackNr() < 7)        // Go to track 1
                        {
                            m_FYMIP50.MIP50xMOVExCALC(1);
                            m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() m_FYMIP50.MIP50xMOVExCALC(1)");
                            State_Machine = State.MoveToTrack1;
                            m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() State_Machine = State.MoveToTrack1");
                        }
                        else if (m_FYAppVar.GetTrackNr() > 6)   // Go to track 11
                        {
                            m_FYMIP50.MIP50xMOVExCALC(11);
                            m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() m_FYMIP50.MIP50xMOVExCALC(11)");
                            State_Machine = State.MoveToTrack11;
                            m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() State_Machine = State.MoveToTrack11");
                        }
                    }
                    else if(m_FYAppVar.GetTrackNr() == 1)       // On track 1, Go to Track 11 for scan
                    {
                        m_FYMIP50.MIP50xMOVExCALC(11);
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() m_FYMIP50.MIP50xMOVExCALC(11)");
                        State_Machine = State.TDT;
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() State_Machine = State.TDT");
                        m_FYAppVar.TrainDetectionStarted.UpdateMessage();                           // Set message on Form
                    }
                    else if (m_FYAppVar.GetTrackNr() == 11)       // On track 11, Go to Track 1 for scan
                    {
                        m_FYMIP50.MIP50xMOVExCALC(1);
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() m_FYMIP50.MIP50xMOVExCALC(1)");
                        State_Machine = State.TDT;
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() State_Machine = State.TDT");
                        m_FYAppVar.TrainDetectionStarted.UpdateMessage();                           // Set message on Form
                    }                    
                    break;

                /*--------------------------------------------------------------------------------------------------------------------------------------------------------
                */

                case State.MoveToTrack1:                            // After startup track was not on 1, move to track 1
                    SubProgramReturnVal = m_FYMIP50.MIP50xMOVE();
                    if (SubProgramReturnVal == "Finished")
                    {
                        m_FYMIP50.MIP50xMOVExCALC(11);
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() m_FYMIP50.MIP50xMOVExCALC(11)");
                        State_Machine = State.TDT;                 // When finished moving, execute TrainDetection TDT
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() State_Machine = State.TDT");
                        m_FYAppVar.TrainDetectionStarted.UpdateMessage();                           // Set message on Form
                    }
                    break;
                case State.MoveToTrack11:                           // After startup track was not on 11, move to track 11
                    SubProgramReturnVal = m_FYMIP50.MIP50xMOVE();
                    if (SubProgramReturnVal == "Finished")
                    {
                        m_FYMIP50.MIP50xMOVExCALC(1);
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() m_FYMIP50.MIP50xMOVExCALC(1)");
                        State_Machine = State.TDT;
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() State_Machine = State.TDT");
                        m_FYAppVar.TrainDetectionStarted.UpdateMessage();                           // Set message on Form
                    }
                    break;

                /*--------------------------------------------------------------------------------------------------------------------------------------------------------
                */

                case State.TDT:
                    SubProgramReturnVal = m_FYMIP50.MIP50xMOVE();                                               // Keep kicking MIP50xMOVE until "Finished"
                    if (SubProgramReturnVal == "Finished")
                    {
                        m_FYAppVar.TrainDetection.UpdateMessage();                                              // Set Train detection finished in Form, set initialized to true in form
                        m_FYAppVar.TrackTrainsOnFYUpdater();                                                    // Force update Form

                        State_Machine = State.Idle;
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() State_Machine = State.Idle");
                        _Return = "Finished";
                        m_FYAppLog.Log(GetType().Name, "FYTDT.Traindetection() _Return = Finished");
                    }

                    if (CL10Heart == true && F10 == true && m_FYAppVar.GetTrackNr() > 0)                        // While checking if heartbit is true and F10 is true and GetTracknr() is 1 <> 11
                    {
                        m_FYAppVar.UpdateTrainsOnFY(FiddleYardTdtTrackName(m_FYAppVar.GetTrackNr()), 1, "");    // If true then update UpdateTrainsOnFY[] with 1
                        //System.Diagnostics.Debug.Write("Train on Track: " + m_FYAppVar.GetTrackNr() + Environment.NewLine);
                    }
                    else if (CL10Heart == true && F10 == false && m_FYAppVar.GetTrackNr() > 0)                  // While checking if heartbit is true and F10 is true and GetTracknr() is 1 <> 11
                    {
                        m_FYAppVar.UpdateTrainsOnFY(FiddleYardTdtTrackName(m_FYAppVar.GetTrackNr()), 0, "");    // If false then update UpdateTrainsOnFY[] with 0
                        //System.Diagnostics.Debug.Write("No Train on Track: " + m_FYAppVar.GetTrackNr() + Environment.NewLine);
                    }
                    break;
                    

                default: break;
            }

            return (_Return);
        }

        /*#--------------------------------------------------------------------------#*/
        /*  Description: FiddleYardTdtTrackName(int Track)
         *               
         * 
         *  Input(s)   : track number in integer
         *
         *  Output(s)  : 
         *track number in string written as Track<x>
         *  Returns    :
         *
         *  Pre.Cond.  :
         *
         *  Post.Cond. :
         *
         *  Notes      : 
         */
        /*#--------------------------------------------------------------------------#*/
        public string FiddleYardTdtTrackName(int Track)
        {
            string _Return = null;

            switch(Track)
            {
                case 0:
                    _Return = "Track0";
                        break;

                case 1:
                    _Return = "Track1";
                    break;


                case 2:
                    _Return = "Track2";
                    break;

                case 3:
                    _Return = "Track3";
                    break;

                case 4:
                    _Return = "Track4";
                    break;

                case 5:
                    _Return = "Track5";
                    break;

                case 6:
                    _Return = "Track6";
                    break;

                case 7:
                    _Return = "Track7";
                    break;

                case 8:
                    _Return = "Track8";
                    break;

                case 9:
                    _Return = "Track9";
                    break;


                case 10:
                    _Return = "Track10";
                    break;


                case 11:
                    _Return = "Track11";
                    break;

                default: _Return = "Track0";
                    break;

            }

            return (_Return);
        }
    }
}
