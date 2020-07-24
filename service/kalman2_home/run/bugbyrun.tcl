default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file tmp.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/runOnPass2.tcl
# run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
# run_file $env(C3_SCRIPTS)/richAna.tcl

prod sel DBEventHeaderProd
proc sel ../build_SunOS/shlib/BugByRun

prod lss

module sel DBModule
database in /nfs/cleo3/database/data10/db event startrun beginrun endrun
database read_collection pass2 hadron

go

exit
