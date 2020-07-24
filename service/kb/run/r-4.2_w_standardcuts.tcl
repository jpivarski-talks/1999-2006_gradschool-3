default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/r-4.2_w_standardcuts.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel CesrBeamEnergyProd

prod sel EventPropertiesProd
proc sel ../build_SunOS/shlib/BeamGasFilterProc.so
param BeamGasFilterProc explicitBeamSpot true

proc sel ../build_SunOS/shlib/KarlsPlots.so

module sel DBModule
database in /nfs/cleo3/database/data20/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 125422 125435

go 100000

hbook terminate
exit
