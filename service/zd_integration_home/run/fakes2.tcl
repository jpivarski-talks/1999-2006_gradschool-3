run_file $env(C3_SCRIPTS)/getNewestConstants.tcl

# emptysource def myzd zdalignment
# source_format sel ZDGeomAlignmentFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDGeomAlignmentFileSourceFormat/zd.zdgeomalignment zdalignment
# source_format sel AZDGeomLayerFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/AZDGeomLayerFileSourceFormat/zd.azdgeomlayer basegeometry

# source_format sel ZDadcpedestalFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDadcpedestalFileSourceFormat/test.zdadcpedestal
# source_format sel ZDWeightDriftFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdweightdrift
# source_format sel ZDWeightLayerFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdweightlayer
# source_format sel ZDWeightPhiFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdweightphi
# source_format sel ZDWeightThetaFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdweighttheta
# source_format sel ZDDaqCellIDMapFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDDaqCellIDMapFileSourceFormat/test.zddaqcellidmap
# source_format sel ZDWireQualityFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDWireQualityFileSourceFormat/test.zdwirequality
# source_format sel ZDadc2chargeFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDadc2chargeFileSourceFormat/test.zdadc2charge
# source_format sel ZDDriftDataFileSourceFormat
# file in /home/huanggs/cleo3src/test.zddriftdata
# source_format sel ZDt0channelFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDt0channelFileSourceFormat/test.zdt0channel
# source_format sel ZDt0cardFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDt0cardFileSourceFormat/test2.zdt0card
# source_format sel ZDt0hvcardFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDt0hvcardFileSourceFormat/test.zdt0hvcard
# source_format sel ZDt0crateFileSourceFormat
# file in /nfs/cleo3/Offline/rel/development/src/ZDt0crateFileSourceFormat/test2.zdt0crate
# source_format sel ZDt0globalFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdt0global

source_format sel ZDGeomAlignmentFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDGeomAlignmentFileSourceFormat/zd.zdgeomalignment zdalignment
source_format sel AZDGeomLayerFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayerFileSourceFormat/zd.azdgeomlayer basegeometry
source_format sel AZDGeomLayersEndsFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayersEndsFileSourceFormat/zd.azdgeomlayersends

# prod sel ../build_SunOS/shlib/CalibratedDRHitProd
# prod sel ../build_SunOS/shlib/ZDOffCal
# prod sel ../build_SunOS/shlib/CalibratedZDHitProd
prod sel ../build_SunOS/shlib/DoitProd
prod sel ../build_SunOS/shlib/KalmanProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod sel DBEventHeaderProd

prod sel AZDGeomProd
prod sel ZDGeomProd
# prod sel ZDWireLayerProd
# prod sel ../build_SunOS/shlib/FakeATrackProd

param DetectorConfigurationProd Override true;
param DetectorConfigurationProd UseZDnotSI true;
param DoitProd UseSilicon false
param DoitProd UseZD true
# param KalmanProd SeedTrackUsageTag "fake-o"
# param KalmanProd DRHitUsageTag "fake-o"
# param KalmanProd ZDHitUsageTag "fake-o"
param KalmanProd HyposToFit pion
param KalmanProd CorrectDRHits false
param KalmanProd CorrectZDHits false
param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false
param KalmanProd UseDRAxial true
param KalmanProd UseDRStereoU true
param KalmanProd UseDRStereoV true
param KalmanProd UseDRCathode true
param KalmanProd UseZDStereoU true
param KalmanProd UseZDStereoV true
param KalmanProd PrintDiagnostics true

proc sel ../build_SunOS/shlib/ExtractFakes
param ExtractFakes explicitExtraction true

# source_format sel BinarySourceFormat
# file in /cdat/sol191/disk2/cleo3/data/r132153.bin event startrun beginrun endrun

source_format sel PDSSourceFormat
file src /home/axp/huanggs/cleo3src/zdoutput.pds

# proc sel SpExtractStandAloneGeomProc
# proc sel SpExtractDRHitsProc
# # proc sel SpExtractSVHitsProc
# proc sel SpExtractSeedTrackProc
# proc sel SpExtractSeedTrackDRHitsLinkProc
# # proc sel SpExtractSeedTrackSVHitsLinkProc
# proc sel SpExtractKinePionFitProc
# proc sel SpExtractZDHitsProc
# proc sel SpViewerProc

report level DEBUG
go 1
