default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/sneak.rzn
hbook init

run_file $env(C3_SCRIPTS)/runOnPass2.tcl
run_file $env(C3_SCRIPTS)/BeamSpot.tcl
run_file $env(C3_SCRIPTS)/CcHotList.tcl

# run_file $env(C3_SCRIPTS)/C3cc.tcl
# prod sel CcP2ReconProd
# prod sel TrackShowerMatchingProd

# prod sel DBEventHeaderProd
# prod sel CesrBeamEnergyProd
# prod sel EventPropertiesProd

prod sel DedxInfoProd

proc sel ../build_SunOS/shlib/Sneak.so
param Sneak explicitBeamSpot false

module sel DBModule
database in /nfs/cleo3/database/data11/db event startrun beginrun endrun
database read_collection pass2 hadron

report level INFO
go 100


