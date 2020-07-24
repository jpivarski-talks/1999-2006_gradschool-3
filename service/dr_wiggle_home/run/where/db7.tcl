default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../../rzn/wheredb7.rzn
hbook init

run_file $env(C3_SCRIPTS)/runOnPass2.tcl
run_file $env(C3_SCRIPTS)/BeamSpot.tcl

prod sel DBEventHeaderProd

proc sel ../../build_SunOS/shlib/WheresTheBeamspot.so

module sel DBModule
database in /nfs/cleo3/database/data7/db event startrun beginrun endrun
database read_collection pass2 hadron
go

