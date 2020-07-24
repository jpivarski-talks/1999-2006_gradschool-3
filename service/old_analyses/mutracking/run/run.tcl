default prompt off
exception continueEventLoop on

module sel ../build_SunOS/shlib/HbookHistogramModule
hbook file muscan1.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Online
constants in $env(C3_CONST) meta Offline MetaListRetrieval
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta RunByRun MetaListRetrieval
# constants in $env(C3_CONST) meta SVHybridEfficiency
emptysource def svhybridefficiency svhybridefficiency

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

module sel RunStatisticsSourceModule
runstatistics in

prod select MuConsProd

prod sel CesrBeamEnergyProd
# prod sel RunInfoProd
# run_file $env(C3_SCRIPTS)/RunInfo.tcl

prod sel ADRGeomProd
prod sel DRGeom
prod sel ASiStorePro
prod sel SiGeom
prod sel IRGeomProd
prod sel RawDataProd
prod sel DRCACorrectHitProd
prod sel DRCAClusterProd
prod sel CalibratedDRHitProd
prod sel SiHitsProd
prod sel CDOffCal
prod sel TriggerL1DataProd
prod sel TrackletBunchFinder
prod sel DRHitCorrectorProd

prod sel MagFieldProd
param MagFieldProd alwaysUseDefault true

prod sel DoitProd
param DoitProd UseSilicon true
param DoitProd ContinueIfHitsMissing true

prod sel KalmanProd
param KalmanProd UseSVRPhi true
param KalmanProd UseSVZ true
param KalmanProd UseDRCathode true
param KalmanProd MaxHitsToDelete 0

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

run_file $env(C3_SCRIPTS)/C3cc.tcl
prod sel CcfcReconProd
prod sel TrackShowerMatchingProd
prod sel DBEventHeaderProd
prod sel EventPropertiesProd
prod sel EventTypeProd

source_format sel BinarySourceFormat
source create rawChain
file add rawChain /cdat/sol300/disk1/cleo3/data/r120924.bin event beginrun endrun
file add rawChain /cdat/sol300/disk4/cleo3/data/r120962.bin event beginrun endrun
file add rawChain /cdat/sol300/disk6/cleo3/data/r120976.bin event beginrun endrun
file add rawChain /cdat/sol300/disk1/cleo3/data/r120926.bin event beginrun endrun
file add rawChain /cdat/sol300/disk4/cleo3/data/r120964.bin event beginrun endrun
file add rawChain /cdat/sol300/disk7/cleo3/data/r120967.bin event beginrun endrun
file add rawChain /cdat/sol300/disk2/cleo3/data/r120965.bin event beginrun endrun
file add rawChain /cdat/sol300/disk4/cleo3/data/r120970.bin event beginrun endrun
file add rawChain /cdat/sol300/disk7/cleo3/data/r120975.bin event beginrun endrun
file add rawChain /cdat/sol300/disk2/cleo3/data/r120972.bin event beginrun endrun
file add rawChain /cdat/sol300/disk4/cleo3/data/r120973.bin event beginrun endrun
file add rawChain /cdat/sol300/disk4/cleo3/data/r120961.bin event beginrun endrun
file add rawChain /cdat/sol300/disk6/cleo3/data/r120966.bin event beginrun endrun
source activate rawChain event beginrun endrun

proc sel ../build_SunOS/shlib/MuTracking
proc sel ../build_SunOS/shlib/HistogramViewerProc
go 100
