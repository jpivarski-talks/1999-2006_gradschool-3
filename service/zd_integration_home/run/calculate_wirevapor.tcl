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

# proc sel ../build_SunOS/shlib/WireVaporProc
proc sel ../build_SunOS/shlib/WireVaporProc2

# emptysource one alignment 12345 0 dralignment      
# go 1 dralignment
emptysource def frank event
go 1

#######################################################################
# 
# ASSUMPTIONS
# 
# sense wires: 20 microns W
# field wires: 110 microns Al with a 1.5 micron Au coat
# 
# All layers have twice as many field wires as sense wires,
#    except for layer one, which has the same number of each
# 
# DR has two field layers between layers 16 and 17.
# 
#######################################################################
# 
# EXPLAINATIONS
# 
# probability of striking a wire's worth of material
# 
# average pathlength through a wire of each type
# product of average pathlength * prob of hitting wire
# 
# inner and outer radii (zd pathlength is outer - inner)
# scale := average pathlength * prob of hitting wire / zd pathlength
# 
# Vaporized Wire Gas: 
#    W:                     scale
#   Al:      -------------------------------------
#   Au:        1 + W_scale + Al_scale + Au_scale
# 
#######################################################################
# 
# DR version
# 
# prob W 0.0674055 Al 1.1332 Au 1.14865
# 
# Avg path/wire W 1.5708e-05 Al 8.63938e-05 Au 2.34035e-06
# Path/track W 1.0588e-06 Al 9.79012e-05 Au 2.68824e-06
# 
# DR inner rad 0.118377 outer rad 0.81675
# Scale factor W 1.5161e-06 Al 0.000140184 Au 3.84928e-06
# 
# Vaporized Wire Gas: 
#    W: 1.51588e-06
#   Al: 0.000140164
#   Au: 3.84872e-06
# 
#######################################################################
# 
# "Gas Wire Vapor Material"  75  1 f 1.288 f 206.942  9 f 4 f 2 f 0.0744 f 12.01 f 6 f 0.458 f 1.01 f 1 f 0.1026 f 183.85 f 74 f 0.023 f 196.967 f 79 f 0.057 f 24.305 f 12 f 0.
# 
#######################################################################
# 
# ZD version
# 
# prob W 0.0124862 Al 0.203497 Au 0.206272
# 
# Avg path/wire W 1.5708e-05 Al 8.63938e-05 Au 2.34035e-06
# Path/track W 1.96133e-07 Al 1.75809e-05 Au 4.82748e-07
# 
# ZD inner rad 0.0417957 outer rad 0.1143
# Scale factor W 2.70512e-06 Al 0.000242481 Au 6.6582e-06
# 
# Vaporized Wire Gas: 
#    W: 2.70444e-06
#   Al: 0.00024242
#   Au: 6.65652e-06
# 
