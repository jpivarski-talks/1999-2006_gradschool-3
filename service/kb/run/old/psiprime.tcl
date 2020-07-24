default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/psiprime.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/runOnPass2.tcl

proc sel ../build_SunOS/shlib/KarlsPlots.so

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 126945 126960

go 10000

hbook terminate
exit
