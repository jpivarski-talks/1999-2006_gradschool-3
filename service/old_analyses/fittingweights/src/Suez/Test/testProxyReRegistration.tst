#!/usr/bin/env sh

echo "-------------------------------------------"
echo "Testing Suez: $0                           "
echo " Testing CXX = $CXX DEBUG = $DEBUG         "
echo "-------------------------------------------"

${C3_BIN}/${CXX}/suez -q << inp
default prompt off
source_format sel AsciiSourceFormat
prod sel DBEventHeaderProd
file in /nfs/cleo3/Offline/data/noDBEventHeader.asc
proc sel RunEventNumberProc
go 2
source clear
file in /nfs/cleo3/Offline/data/noDBEventHeader.asc
go 2
exit
inp

