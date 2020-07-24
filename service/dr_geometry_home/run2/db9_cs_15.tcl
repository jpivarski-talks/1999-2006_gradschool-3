default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db9_cs_15.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/BeamSpot.tcl

prod sel ChisqFitProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel KalmanProd
prod sel DBEventHeaderProd

param ChisqFitProd UseDRAxial false
param ChisqFitProd UseDRCathode false
param ChisqFitProd UseSVRPhi false
param ChisqFitProd UseSVZ false

module sel DBModule
database in /nfs/objy/data9c/db event startrun beginrun endrun
database read_collection rawdata bhabha
database run 114810 114825

proc sel ../build_SunOS/shlib/TwoCycleHist

report level INFO
go

