default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/pointtoerror.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/PointToError

module sel DBModule
database in /nfs/cleo3/database/data17/db event startrun beginrun endrun
database read_collection pass2 hadron

go 100000

hbook terminate
exit
