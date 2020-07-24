run_file $env(C3_SCRIPTS)/getNewestConstants.tcl

source_format sel ZDGeomAlignmentFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDGeomAlignmentFileSourceFormat/zd.zdgeomalignment zdalignment
source_format sel AZDGeomLayerFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayerFileSourceFormat/zd.azdgeomlayer basegeometry
source_format sel AZDGeomLayersEndsFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayersEndsFileSourceFormat/zd.azdgeomlayersends

prod sel ../build_SunOS/shlib/KalmanProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod sel DBEventHeaderProd

prod sel AZDGeomProd
prod sel ZDGeomProd
# prod sel ZDWireLayerProd
prod sel ../build_SunOS/shlib/FakeATrackProd

param DetectorConfigurationProd Override true;
param DetectorConfigurationProd UseZDnotSI true;
param DoitProd UseSilicon false
param DoitProd UseZD false
param KalmanProd SeedTrackUsageTag "fake-o"
param KalmanProd DRHitUsageTag "fake-o"
param KalmanProd ZDHitUsageTag "fake-o"
param KalmanProd HyposToFit pion
param KalmanProd CorrectDRHits false
param KalmanProd CorrectZDHits false
param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false
param KalmanProd UseDRAxial true
param KalmanProd UseDRStereoU false
param KalmanProd UseDRStereoV false
param KalmanProd UseDRCathode false
param KalmanProd UseZDStereoU false
param KalmanProd UseZDStereoV false
param KalmanProd PrintDiagnostics true

proc sel ../build_SunOS/shlib/ExtractFakes
param ExtractFakes explicitExtraction true

source_format sel BinarySourceFormat
file in /cdat/sol191/disk2/cleo3/data/r132153.bin event startrun beginrun endrun

report level DEBUG
go 1
