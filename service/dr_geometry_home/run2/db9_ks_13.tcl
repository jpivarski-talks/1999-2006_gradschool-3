default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db9_ks_13.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod sel DBEventHeaderProd

param KalmanProd UseDRAxial false
param KalmanProd UseDRCathode false
param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false

module sel DBModule
database in /nfs/objy/data9c/db event startrun beginrun endrun
database read_collection rawdata bhabha
database run 114780 114795

proc sel ../build_SunOS/shlib/TwoCycleHist

report level INFO
go

