default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/zdal_764.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl

prod sel /home/mccann/cakealign/build_SunOS/shlib/CDOffCal
prod sel /home/mccann/cakealign/build_SunOS/shlib/DRHitCorrectorProd
prod sel /home/mccann/cakealign/build_SunOS/shlib/ZDDriftFunctionProd
prod sel /home/mccann/cakealign/build_SunOS/shlib/ZDHitCorrectorProd
prod sel AZDGeomProd
prod sel ZDGeomProd
prod sel CalibratedZDHitProd
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
param McCannProd UseZDStereoU false
param McCannProd UseZDStereoV false

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
file in /cdat/sol191/disk1/cleo-c/c200764.bin event beginrun startrun endrun

proc sel ../build_SunOS/shlib/CompareDRandZD
go 600000

