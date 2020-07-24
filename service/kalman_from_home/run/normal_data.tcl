default prompt off
exception continueEventLoop on

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval
constants in $env(C3_CONST) meta Online MetaListRetrieval
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta RunByRun MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval
constants in $env(C3_CONST) meta Alignment MetaListRetrieval
constants in $env(C3_CONST) meta FieldMap MetaListRetrieval
constants in $env(C3_CONST) meta Dedx MetaListRetrieval
constants in $env(C3_CONST) meta MagFudge MetaListRetrieval
constants in $env(C3_CONST) meta DAQID MetaListRetrieval
source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry
module sel RunStatisticsSourceModule
runstatistics in
prod select MuConsProd

prod sel DetectorConfigurationProd
prod sel ADRGeomProd
prod sel DRGeom
prod sel ASiStorePro
prod sel SiGeom
prod sel IRGeomProd
prod sel RawDataProd
prod sel DRCACorrectHitProd
prod sel DRCAClusterProd
prod sel PreliminaryDRHitProd
prod sel CalibratedDRHitProd
prod sel CorrectedSVStripProd
prod sel SiHitsProd
prod sel MagFieldProd
prod sel CDOffCal
prod sel TriggerL1DataProd
prod sel TriggerPhaseForDataProd
prod sel TrackletBunchFinder
prod sel DoitProd
param DoitProd ContinueIfHitsMissing true
prod sel DRHitCorrectorProd
param DRHitCorrectorProd ApplyEntranceAngleCorr false           
param DRHitCorrectorProd ApplyStandardCorrections false   
prod sel ElectronFromPionFitProd
prod sel MuonFromPionFitProd
prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

prod sel DBEventHeaderProd

# prod sel ../build_SunOS/shlib/KalmanProd
prod sel KalmanProd
param KalmanProd HyposToFit pion kaon proton
param KalmanProd OptimizeForAllHypos true
param KalmanProd HitListFromOneHypo true
param KalmanProd ScaleCathodeResolution true
param KalmanProd CathodeResolutionScale 0.7

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection rawdata

# report level EMERGENCY

proc sel ../build_SunOS/shlib/DoLittle
go 10
