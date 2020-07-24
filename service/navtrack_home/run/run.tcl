run_file $env(C3_SCRIPTS)/getNewestConstants.tcl

source_format sel ZDadcpedestalFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDadcpedestalFileSourceFormat/test.zdadcpedestal
source_format sel ZDDaqCellIDMapFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDDaqCellIDMapFileSourceFormat/test.zddaqcellidmap
source_format sel ZDWireQualityFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDWireQualityFileSourceFormat/test.zdwirequality
source_format sel ZDadc2chargeFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDadc2chargeFileSourceFormat/test.zdadc2charge
source_format sel ZDDriftDataFileSourceFormat
file in /home/huanggs/cleo3src/test.zddriftdata
source_format sel ZDt0channelFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDt0channelFileSourceFormat/test.zdt0channel
source_format sel ZDt0cardFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDt0cardFileSourceFormat/test2.zdt0card
source_format sel ZDt0hvcardFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDt0hvcardFileSourceFormat/test.zdt0hvcard
source_format sel ZDt0crateFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDt0crateFileSourceFormat/test2.zdt0crate
source_format sel ZDt0globalFileSourceFormat
file in /home/huanggs/cleo3src/test.zdt0global
source_format sel ZDGeomAlignmentFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDGeomAlignmentFileSourceFormat/zd.zdgeomalignment zdalignment
source_format sel AZDGeomLayerFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayerFileSourceFormat/zd.azdgeomlayer basegeometry
source_format sel ZDWeightLayerFileSourceFormat
file in /home/huanggs/cleo3src/test.zdweightlayer
source_format sel ZDWeightDriftFileSourceFormat
file in /home/huanggs/cleo3src/test.zdweightdrift
source_format sel ZDWeightThetaFileSourceFormat
file in /home/huanggs/cleo3src/test.zdweighttheta
source_format sel ZDWeightPhiFileSourceFormat
file in /home/huanggs/cleo3src/test.zdweightphi

prod sel /cdat/daf9/mccann/navtrack/build_SunOS/shlib/NavigationProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod sel DBEventHeaderProd
prod sel ZDDriftFunctionProd
prod sel ZDHitCorrectorProd
prod sel AZDGeomProd
prod sel ZDGeomProd
prod sel CalibratedZDHitProd

param DetectorConfigurationProd Override true;
param DetectorConfigurationProd UseZDnotSI true;
param KalmanProd HyposToFit pion

proc sel /cdat/daf9/mccann/navtrack/build_SunOS/shlib/TestNavTrack

source_format sel BinarySourceFormat
file in /cdat/sol191/disk1/cleo-c/r200558.bin event beginrun startrun endrun

prod lss

go 10
