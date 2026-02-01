// file:  processNxCoreStatusMessage.cpp

/*
struct NxCoreSystem{...} is used to generate a NxMSG_STATUS report and return to the
OnNxCoreCallback(const NxCoreSystem *pNxCoreSys, const NxCoreMessage *pNxCoreMsg) method.
This function processes a NxMSG_STATUS message and prints relevant information to standard output.
*/

#include <iostream>
#include <iomanip>
#include "nxcaExceptions.hpp"
#include "processNxCoreStatusMessage.hpp"

extern NxCoreClass NxCore; // defined in main.cpp

int processNxCoreStatusMessage(
    const NxCoreSystem *pNxCoreSys,
    [[maybe_unused]] const NxCoreMessage *pNxCoreMsg)
{
    using namespace std;

    // Print time every minute (including sentinel 24:00:00)
    if (pNxCoreSys->ClockUpdateInterval >= NxCLOCK_MINUTE &&
        pNxCoreSys->nxTime.Hour <= 24)
    {
        cout.fill('0');
        cout << "NxCore Time: "
             << setw(2) << static_cast<int>(pNxCoreSys->nxDate.Month) << "/"
             << setw(2) << static_cast<int>(pNxCoreSys->nxDate.Day) << "/"
             << setw(4) << static_cast<int>(pNxCoreSys->nxDate.Year) << " "
             << setw(2) << static_cast<int>(pNxCoreSys->nxTime.Hour) << ":"
             << setw(2) << static_cast<int>(pNxCoreSys->nxTime.Minute) << ":"
             << setw(2) << static_cast<int>(pNxCoreSys->nxTime.Second) << endl;
    }

    /*
    NxCLOCK_NOCHANGE  0	Indicates that the NxCore Feed Timestamp has not changed.
    NxCLOCK_CLOCK     1	Indicates that the member nxTime.Millisecond has changed (but not Second, Minute, or Hour)
    NxCLOCK_SECOND    2	Indicates that the member nxTime.Second has changed (as has Millisecond, but not Minute or Hour).
    NxCLOCK_MINUTE    3	Indicates that the member nxTime.Minute has changed (as has Millisecond and Second, but not Hour).
    NxCLOCK_HOUR      4	Indicates that the member nxTime.Hour has changed (as has Millisecond, Second, and Minute).
    */

    if (pNxCoreSys->ClockUpdateInterval >= NxCLOCK_MINUTE &&
        pNxCoreSys->nxTime.Hour == 24)
    {
        cout << "NxCore tape file sentinel read -> hour of the day == 24.\n";
    }

    /* Detailed NxCore Status Message Descriptions:
    NxCORESTATUS_RUNNING            0	Signals that processing is proceeding normally.
    NxCORESTATUS_INITIALIZING       1	Signals that NxCoreAPI has started processing a real-time or historic tape. The first callback message is always NxMSG_STATUS with the Status member set to NxCORESTATUS_INITIALIZING. You will not get another message with this value unless recoverable errors are encountered by NxCoreAPI.
    NxCORESTATUS_COMPLETE           2	Signals that NxCoreAPI has completed processing a tape. This is the last message callback you will receive regarding a tape. You will always get this message unless you abort processing by returning a value of NxCOREBACKRETURN_STOP in your callback.
    NxCORESTATUS_SYNCHRONIZING      3	Synchronization reset detected in the tape, continuing processing
    NxCORESTATUS_ERROR              4	Signals that an error was encountered by NxCoreAPI when processing a Tape. More information on the error encountered may be available in the members StatusData and StatusDisplay.
    NxCORESTATUS_WAITFORCOREACCESS  5	If NxCoreAccess is not present (perhaps it was temporarily stopped because you upgraded it, for example), system will wait for it to return at the point it left off and it will set the value of Status to this value at regular intervals. This allows your application to take special action, or terminate the processing of the NxCore Tape.
    NxCORESTATUS_RESTARTING_TAPE    6	Signals that your callback returned NxCALLBACKRETURN_RESTART from a previous callback, and system is restarting the same tape from the beginning
    NxCORESTATUS_LOADED_STATE       7	System is initialized from state
    NxCORESTATUS_SAVING_STATE       8	System will capture the state after this message
    NxCORESTATUS_SYMBOLSPIN         9	signals system symbol spin state: StatusData == 0 when starting, 1 when complete
    */
    switch (pNxCoreSys->Status)
    {
    case NxCORESTATUS_RUNNING:
        break;
    case NxCORESTATUS_INITIALIZING:
        cout << "NxCore Initialize Message.\n";
        cout << "libnx.so version is v"
             << NxCore.GetMajorVersion(pNxCoreSys->DLLVersion) << "."
             << NxCore.GetMinorVersion(pNxCoreSys->DLLVersion) << "."
             << NxCore.GetBuildVersion(pNxCoreSys->DLLVersion) << "\n";
        break;
    case NxCORESTATUS_COMPLETE:
        cout << "NxCore Complete Message.\n";
        break;
    case NxCORESTATUS_SYNCHRONIZING:
        cout << "NxCore Synchronizing Message.\n";
        break;
    case NxCORESTATUS_ERROR:
        cout << "NxCore Error:  " << pNxCoreSys->StatusDisplay
             << " (" << pNxCoreSys->StatusData << ")  "
             << to_string(static_cast<NxCAException>(pNxCoreSys->StatusData)) << "\n";
        return NxCALLBACKRETURN_STOP;
    case NxCORESTATUS_WAITFORCOREACCESS:
        cout << "NxCore Wait For Access.\n";
        break;
    case NxCORESTATUS_RESTARTING_TAPE:
        cout << "NxCore Restart Tape Message.\n";
        break;
    case NxCORESTATUS_LOADED_STATE:
        cout << "NxCore System Loaded from State.\n";
        break;
    case NxCORESTATUS_SAVING_STATE:
        cout << "NxCore System Will Save State After this Message.\n";
        break;
    case NxCORESTATUS_SYMBOLSPIN:
        cout << "NxCore System symbol spin state: StatusData == " << pNxCoreSys->StatusData;
        if (pNxCoreSys->StatusData == 0)
            cout << "  Starting symbol spin...\n";
        else if (pNxCoreSys->StatusData == 1)
            cout << "  Symbol spin complete.\n";
        else
        {
            cout << "  Symbol spin StatusData error.  "
                 << pNxCoreSys->StatusDisplay
                 << " (" << pNxCoreSys->StatusData << ") \n";
            return NxCALLBACKRETURN_STOP;
        }
        break;
    default:
        cout << "NxCore Unknown (default) Status Message:  "
             << pNxCoreSys->StatusDisplay << " (" << pNxCoreSys->StatusData << ")\n";
    }
    return NxCALLBACKRETURN_CONTINUE;
}

/*
Status                          StatusData
NxCORESTATUS_RUNNING
                                NxCSRUNMODE_SRC_DISK_FILE           0	processing from a tape on disk
                                NxCSRUNMODE_SRC_ACCESS_FILE         1	processing from NxCoreAccess's file -- dll has not yet reached memory buffers
                                NxCSRUNMODE_SRC_ACCESS_MEMORY       2	processing from NxCoreAccess's memory buffers
                                NxCSRUNMODE_SRC_ACCESS_MB_OLDEST    2	processing oldest of NxCoreAccess's memory buffers
                                NxCSRUNMODE_SRC_ACCESS_MB_LOWER     3	processing older half of NxCoreAccess's memory buffers
                                NxCSRUNMODE_SRC_ACCESS_MB_UPPER	    4	processing most recent half of NxCoreAccess's memory buffers
                                NxCSRUNMODE_SRC_ACCESS_MB_SECOND    5	processing 2nd most recent.
                                NxCSRUNMODE_SRC_ACCESS_MB_CURRENT   6	processing most recent of NxCoreAccess's memory buffers
NxCORESTATUS_INITIALIZING	    0
NxCORESTATUS_COMPLETE	        0
NxCORESTATUS_SYNCHRONIZING
NxCORESTATUS_ERROR              Corresponds to Error Codes defined in nxcaExceptions.cpp
NxCORESTATUS_WAITFORCOREACCESS  0
NxCORESTATUS_RESTARTING_TAPE    0
NxCORESTATUS_LOADED_STATE       The size of the state in the specified file. Note that the file may be larger that the state if the user appends additional data to it as shown here
NxCORESTATUS_SAVING_STATE
                                NxCSSAVESTATE_CAPTURE            0	the core state will be captured after you return from this callback
                                NxCSSAVESTATE_COMPLETE           1	the save core state operation is complete
                                NxCSSAVESTATE_ERR_CREATEFILE	-1	failed to open the specified tape filename
                                NxCSSAVESTATE_ERR_DISKSPACE     -2	not enough disk space to complete the operation
                                NxCSSAVESTATE_ERR_MEMORY	    -3	insufficient memory to complete the operation
NxCORESTATUS_SYMBOLSPIN
                                NxCSSYMBOLSPIN_STARTING          0	the system symbol spin is starting -- note -- status will return NxCORESTATUS_RUNNING after this message
                                NxCSSYMBOLSPIN_COMPLETE          1	the system symbol spin has completed
*/