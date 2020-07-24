#!/usr/bin/env sh

echo "--------------------------------------------"
echo "Testing Suez: $0                            "
echo " Testing CXX = $CXX DEBUG = $DEBUG          "
echo "--------------------------------------------"

${C3_BIN}/${CXX}/suez -q << inp
run_file \$env(C3_CVSSRC)/Suez/Test/testStopCounter.tcl
exit
inp
