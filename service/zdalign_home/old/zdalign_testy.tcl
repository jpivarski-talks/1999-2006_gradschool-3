module sel HbookHistogramModule
hbook file ../rzn/zdalign_testy.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/exclude_constants_streams.tcl
exclude_constants_streams {zdgeomalignment drdriftdatav2 drt0card drt0channelv2 drt0crate drt0global drt0hvcard zddriftdata}

source_format sel DRDriftDatav2FileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRDriftDatav2.drdriftdatav2
source_format sel DRt0cardFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0card.drt0card
source_format sel DRt0channelv2FileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0channelv2.drt0channelv2
source_format sel DRt0crateFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0crate.drt0crate
source_format sel DRt0globalFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0global.drt0global
source_format sel DRt0hvcardFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0hvcard.drt0hvcard
source_format sel ZDDriftDataFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/ZDDriftDatav8.zddriftdata

source_format sel ZDGeomAlignmentFileSourceFormat
file in testy.zdgeomalignment

prod sel AZDGeomProd
prod sel ZDGeomProd
prod sel CalibratedZDHitProd
prod sel ZDDriftFunctionProd
prod sel ZDHitCorrectorProd
prod sel CesrBeamEnergyProd

prod sel ../build_SunOS/shlib/McCannProd
prod sel ../build_SunOS/shlib/PivarskiProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel KalmanProd

param McCannProd HyposToFit pion
param McCannProd UseDRCathode true
param McCannProd UseDRStereoV true
param McCannProd UseDRStereoU true
param McCannProd UseDRAxial   true
param McCannProd UseZDStereoU true
param McCannProd UseZDStereoV true
param McCannProd ForceZDFittingWeight true
param McCannProd ZDFittingWeight 1e-6

param PivarskiProd HyposToFit pion
param PivarskiProd UseDRCathode false
param PivarskiProd UseDRStereoV false
param PivarskiProd UseDRStereoU false
param PivarskiProd UseDRAxial   false
param PivarskiProd UseZDStereoU true
param PivarskiProd UseZDStereoV true
param PivarskiProd FixCurvature true
param PivarskiProd SeedTrackUsageTag "McCann"

run_file $env(C3_SCRIPTS)/CcP2.tcl

param DetectorConfigurationProd Override true;
param DetectorConfigurationProd UseZDnotSI true;

source_format sel BinarySourceFormat
file in /cdat/sol191/disk1/cleo-c/r200854.bin event beginrun startrun endrun

proc sel ../build_SunOS/shlib/CompareDRandZD
go 10000

