default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db9chi2.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
prod sel ChisqFitProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel KalmanProd
prod sel DBEventHeaderProd

param ChisqFitProd UseDRCathode false
param ChisqFitProd UseDRAxial false
param ChisqFitProd UseSVRPhi false
param ChisqFitProd UseSVZ false

module sel DBModule
database in /nfs/objy/data9c/db event startrun beginrun endrun
database read_collection rawdata bhabha
database run 114600 115063

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 1
report level INFO
go 10000

