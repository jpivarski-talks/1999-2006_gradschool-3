default prompt off
# exception continueEventLoop off

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

module sel RunStatisticsSourceModule
runstatistics in

prod sel SiGeom
prod sel ASiStorePro
prod sel IRGeomProd

prod sel DRGeom
prod sel ADRGeomProd
prod sel DRHitCorrectorProd

prod sel MagFieldProd
# param MagFieldProd alwaysUseDefault true

prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd
prod sel NavigationProd

source_format sel PDSSourceFormat
file in ../bug_magnitude_pds/bugless_yet_again.pds beginrun startrun event

proc sel ../build_SunOS/shlib/EnderSkim
param EnderSkim target_hits 250000
param EnderSkim device "ladder"
param EnderSkim skim_dir ../bug_magnitude_pds/bugless_skim/

report level INFO
go
exit
