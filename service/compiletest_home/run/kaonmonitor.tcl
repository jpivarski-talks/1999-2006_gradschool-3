default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file kaonmonitor.rzn
hbook init

run_file $env(C3_SCRIPTS)/runOnPass2.tcl
prod sel CesrBeamEnergyProd

module sel DBModule
database in /nfs/cleo3/database/data16/db event startrun beginrun endrun
database read_collection pass2 hadron

# run_file $env(C3_SCRIPTS)/mcfile_command.tcl
# mcfile in dataset 16 physType ups3s -runmax 121361

proc sel ../build_SunOS/shlib/KaonMonitorProc
proc sel HistogramViewerProc
go

