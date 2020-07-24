default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/findbug20.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/runOnPass2.tcl
# run_file $env(C3_SCRIPTS)/trackingDataFull.tcl

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/PointToError

module sel DBModule
database in /nfs/cleo3/database/data20/db event startrun beginrun endrun
database read_collection pass2 hadron

prod lss

# report level DEBUG

# go 7000
go 500

hbook terminate
exit
