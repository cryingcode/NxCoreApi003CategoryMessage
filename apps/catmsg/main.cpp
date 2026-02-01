// file: main.cpp

////////////////////////////////////////////////////
//// SampleApp3.cpp                             ////
//// Sample App for NxCore in C++               ////
//// Author: Jeffrey Donovan, Jeremy Truelove   ////
//// Date: 2017-02-09                           ////
////////////////////////////////////////////////////
//// Demonstrates:                              ////
//// Handling the NxCore Category Message       ////
////////////////////////////////////////////////////
//// To Read as Written:                        ////
//// Tab Size: 4  Indent Size: 2, Keep Tabs     ////
////////////////////////////////////////////////////

#include <cstdlib>
#include <filesystem>              // executable filename
#include <iostream>                // cout, endl
#include <iomanip>                 // setw, fixed, setprecision
#include <stdio.h>                 // sprintf
#include <string>                  // executable filename
#include <stdexcept>               // executable filename
#include "NxCoreAPI_Wrapper_C++.h" // NxCore C++ wrapper with NxCoreClass

#include "executableUtils.hpp"
#include "nxcoreCallback.hpp"

NxCoreClass NxCore; // global instance (consider dependency injection in larger apps)

constexpr unsigned int DEFAULT_PROCESS_FLAGS =
    NxCF_EXCLUDE_QUOTES |
    NxCF_EXCLUDE_QUOTES2;
// | NxCF_EXCLUDE_OPRA;

// Main Entry Point for app.
int main(int argc, char *argv[])
{
    std::string executableFileName;

    try
    {
        executableFileName = get_executable_filename();
    }
    catch (const exception &e)
    {
        std::cerr << "Failed to get executable name: " << e.what() << "\n";
        return 1;
    }

    if (argc < 2)
    {
        cout << "Program derived from NxCore API SampleApp3\n";
        cout << executableFileName << " libnx.so [path to tapefile]\n";
        return EXIT_SUCCESS;
    }

    cout << "NxCore C++ " << executableFileName << " Start.\n";

    int returnValue = 0;

    // If we can load the NxCore libnx.so
    if (NxCore.LoadNxCore(argv[1]))
    {
        // If a tape filename was passed in command line argument,
        // call ProcessTape with that argument.
        if (argv[2])
        {
            cout << "Processing the tape: " << argv[2] << endl;
            returnValue = NxCore.ProcessTape(argv[2], NULL, 0, 0,
                                             static_cast<NxCoreCallback>(OnNxCoreCallback));
        }
        else
            returnValue = NxCore.ProcessTape("", NULL, 0, 0,
                                             static_cast<NxCoreCallback>(OnNxCoreCallback));

        // output return value from processTape(), helps in troubleshooting potential issues
        NxCore.ProcessReturnValue(returnValue);

        // Unload NxCore DLL
        NxCore.UnloadNxCore();
    }
    else
    {
        cout << "Failed to load NxCore Library: " << dlerror() << endl;
        exit(EXIT_FAILURE);
    }

    cout << "NxCore C++ " << executableFileName << " Stop.\n";
    return EXIT_SUCCESS;
}