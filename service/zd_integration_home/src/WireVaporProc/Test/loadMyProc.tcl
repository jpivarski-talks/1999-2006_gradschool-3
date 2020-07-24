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

source_format sel ZDGeomAlignmentFileSourceFormat
file in /nfs/cleo3/Offline/rel/development/src/ZDGeomAlignmentFileSourceFormat/zd.zdgeomalignment zdalignment
source_format sel AZDGeomLayerFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayerFileSourceFormat/zd.azdgeomlayer basegeometry
source_format sel AZDGeomLayersEndsFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayersEndsFileSourceFormat/zd.azdgeomlayersends

prod sel AZDGeomProd
prod sel ZDGeomProd

prod sel ADRGeomProd
prod sel DRGeom

proc sel ../build_SunOS/shlib/WireVaporProc

emptysource def frankenstein event
go 1
