#!/usr/bin/env bash

# file: NxCoreApi003CategoryMessage/run.sh

# Script to run the project app file for testing.
# Only runs if the current directory ends with one of the allowed names

# How to run:
# cd ~/Crymoney/CrymoneyCodeLinux/NxCoreApi003CategoryMessage
# ./run.sh

APPFILENAME="catmsg"
TAPEFILEPATH="$HOME/Crymoney/CrymoneyTest/NanexNxCoreDataSets/20211013.WE.nx2"

# Optional: verify the file exists (very useful for debugging)
if [[ ! -f "$TAPEFILEPATH" ]]; then
    echo "Error: Tape file not found: $TAPEFILEPATH"
    exit 2
fi

echo "Reading from tapefile: $TAPEFILEPATH"

$APPFILENAME libnx.so $TAPEFILEPATH

# Optional: show exit status
if [ $? -eq 0 ]; then
    echo "Execution finished successfully."
else
    echo "Execution failed (exit code $?)"
fi

# End of file run.sh
