#!/usr/bin/env sh

echo "-------------------------------------------"
echo "Testing Suez: $0                           "
echo " Testing CXX = $CXX DEBUG = $DEBUG         "
echo "-------------------------------------------"

${C3_BIN}/${CXX}/suez -q << inp
default prompt off
source_format sel AsciiSourceFormat
module sel HbookHistogramModule
hbook help
hbook file test.rzn
hbook lun 15
hbook init
proc sel SampleHistogramProc
file in /nfs/cleo3/Offline/data/runBeginRunEvent.asc
go 2
exit
inp



