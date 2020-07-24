default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/compare_both.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval
constants in $env(C3_CONST) meta Alignment MetaListRetrieval
constants in $env(C3_CONST) meta FieldMap MetaListRetrieval
constants in $env(C3_CONST) meta MagFudge MetaListRetrieval
constants in $env(C3_CONST) meta DAQID MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

module sel RunStatisticsSourceModule
runstatistics in

# for DetectorConfiguration
prod sel DetectorConfigurationProd
# for ADRSenseWireStore and ADRCathodeStore
prod sel ADRGeomProd
# for DGDetectorStore
prod sel DRGeom
# for HitCorrector
prod sel DRHitCorrectorProd
# for DriftFunction
prod sel CDOffCal
# for ASiStore
prod sel ASiStorePro
# for DGDetectorStore
prod sel SiGeom
# for DGDetectorStore
prod sel IRGeomProd
# for MagneticField
prod sel MagFieldProd

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

# What about these two?
#   FAConstants<DBSVRPhiHitRadialCorrections>     (from the constants database, I guess)
#   FATable<DGConstMaterialPtr>                   (from material.asc, I guess)

prod sel ../build_SunOS_both/shlib/KalmanProd
param KalmanProd HyposToFit pion kaon proton
param KalmanProd OptimizeForAllHypos true
param KalmanProd HitListFromOneHypo true
param KalmanProd ScaleCathodeResolution true 
param KalmanProd CathodeResolutionScale 0.7     

prod sel DoitProd
prod desel DoitProd
proc sel ../build_SunOS_original/shlib/CompareInOutChi2.so

source_format sel PDSSourceFormat
file source /home/mccann/kalman5/pds/r114141.pds

go
