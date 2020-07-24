default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/squish_sto_weddingcake.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
run_file $env(C3_SCRIPTS)/Ccfc.tcl

source_format sel DRWeightLayerFileSourceFormat
file in wedding_cake.drweightlayer

prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd
prod sel EventPropertiesProd
prod sel TrackShowerMatchingProd

prod desel KalmanProd
prod sel ChisqFitProd
param ChisqFitProd ForceSVRFittingWeight true 
param ChisqFitProd ForceSVZFittingWeight false
param ChisqFitProd SVRFittingWeight 1000.
# param ChisqFitProd SVZFittingWeight 1000.
param ChisqFitProd UseDRCathode true

proc sel ../build_SunOS/shlib/SquishChecker.so
param SquishChecker requireZInfo true

module sel DBModule
database in /nfs/cleo3/database/data19/db event startrun beginrun endrun
database read_collection rawdata bhabha
# database run 124624 125320
database run 124624 124640

# go 10000
go

hbook terminate
exit
