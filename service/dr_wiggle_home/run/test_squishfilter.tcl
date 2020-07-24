default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/test_squishfilter.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
# run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd
prod sel EventPropertiesProd
# prod sel TrackletBunchFinder
# prod desel TrackletBunchFinder

proc sel ../build_SunOS/shlib/SquishFilter.so

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection rawdata bhabha

go 1000

hbook terminate
exit
