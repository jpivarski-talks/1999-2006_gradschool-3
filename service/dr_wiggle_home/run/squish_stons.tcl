default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/squish_stons.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
run_file $env(C3_SCRIPTS)/Ccfc.tcl

source_format sel DRWeightLayerFileSourceFormat
file in stereo_only.drweightlayer

source_format sel ADRGeomLayerFileSourceFormat
file in no_sag.adrgeomlayer

prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd
prod sel EventPropertiesProd
prod sel TrackShowerMatchingProd

prod desel KalmanProd
prod sel ChisqFitProd
param ChisqFitProd ForceSVRFittingWeight true 
param ChisqFitProd ForceSVZFittingWeight true
param ChisqFitProd SVRFittingWeight 1000.
param ChisqFitProd SVZFittingWeight 1000.
param ChisqFitProd UseDRCathode false

proc sel ../build_SunOS/shlib/SquishChecker.so

module sel DBModule
database in /nfs/cleo3/database/data19/db event startrun beginrun endrun
database read_collection rawdata bhabha
database run 124624 125320

go 10000

hbook terminate
exit
