default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/comparePionKaon_Jun27_02_MC.rzn
hbook init

#  run_file $env(C3_SCRIPTS)/runOnPass2.tcl
run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl

prod sel DBEventHeaderProd

proc sel ../build_Jun27_02_MC/shlib/ComparePionKaon
proc sel HistogramViewerProc

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection rawdata hadron

go 100

hbook terminate
exit
