# source_format sel ZDWeightLayerFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdweightlayer
# source_format sel ZDWeightDriftFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdweightdrift
# source_format sel ZDWeightThetaFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdweighttheta
# source_format sel ZDWeightPhiFileSourceFormat
# file in /home/huanggs/cleo3src/test.zdweightphi

source_format sel ../build_SunOS/shlib/ZDWeight2LayerDriftEntAngFileSourceFormat
file in test2.zdweight2layerdriftentang
source_format sel ../build_SunOS/shlib/ZDWeight2LayerThetaFileSourceFormat
file in test2.zdweight2layertheta
source_format sel ../build_SunOS/shlib/ZDWeight2LayerPhiFileSourceFormat
file in test2.zdweight2layerphi

source_format sel ../build_SunOS/shlib/DRWeight2LayerDriftEntAngFileSourceFormat
file in ../translate/sample.drweight2layerdriftentang
source_format sel ../build_SunOS/shlib/DRWeight2LayerThetaFileSourceFormat
file in ../translate/sample.drweight2layertheta
source_format sel ../build_SunOS/shlib/DRWeight2LayerPhiFileSourceFormat
file in test2.drweight2layerphi

module sel CorbaModule
corba init
module sel ../build_SunOS/shlib/ConstantsModule
# module sel ConstantsModule
constants in $env(C3_CONST) standard CLEO3default
source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry
module sel RunStatisticsSourceModule
runstatistics in
prod select MuConsProd

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
source_format sel AZDGeomLayersEndsFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayersEndsFileSourceFormat/zd.azdgeomlayersends

prod sel ../build_SunOS/shlib/CDOffCal
prod sel ../build_SunOS/shlib/DRHitCorrectorProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod sel DBEventHeaderProd

prod sel AZDGeomProd
prod sel ZDGeomProd
prod sel ../build_SunOS/shlib/ZDDriftFunctionProd
prod sel ../build_SunOS/shlib/ZDHitCorrectorProd
# prod sel ZDDriftFunctionProd
# prod sel ZDHitCorrectorProd

param DetectorConfigurationProd Override true;
param DetectorConfigurationProd UseZDnotSI true;
param KalmanProd HyposToFit pion

source_format sel PDSSourceFormat
# file src /home/huanggs/cleo3src/zdoutput.pds
file src /cdat/lnspu4/disk4/huanggs/zdoutput.pds

proc sel ../build_SunOS/shlib/AskForTracks

report level VERIFY
go 1
report level ERROR
go
