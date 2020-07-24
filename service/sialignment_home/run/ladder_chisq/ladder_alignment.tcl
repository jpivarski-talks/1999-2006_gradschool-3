module sel ../../build_SunOS/shlib/HbookHistogramModule
hbook file ladder$env(USER_ONUMBER).rzn
hbook init

default prompt off

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval streams svcalibration svefficiency svresolution svradcorrection
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

# prod sel CDOffCal
prod sel SiGeom
prod sel ASiStorePro
# prod sel IRGeomProd
# prod sel DRGeom
# prod sel ADRGeomProd
# prod sel DRHitCorrectorProd
# prod sel TrackDeliveryProd
# prod sel SeedToTRTrackProd
# prod sel NavigationProd

prod sel MagFieldProd
# param MagFieldProd alwaysUseDefault true

source_format sel PDSSourceFormat
file in ../../data16_dec10/tracks_121339.pds beginrun startrun event

emptysource def svalignment svalignment

module sel ../../build_SunOS/shlib/EnderSiAlignmentMod
param EnderSiAlignmentMod target_hits 100000

param EnderSiAlignmentMod global_init ../dec16.svalignmentglobal
param EnderSiAlignmentMod layer_init  ../dec18.svalignmentlayer
param EnderSiAlignmentMod ladder_init ../before_opening.svalignmentladder
param EnderSiAlignmentMod sensor_init ../before_opening.svalignmentsensor

param EnderSiAlignmentMod file_output ladder$env(USER_ONUMBER)-summary.log

param EnderSiAlignmentMod skim_in ../../data16_dec10/skim/ladder$env(USER_ONUMBER).eskim

param EnderSiAlignmentMod device "ladder"
param EnderSiAlignmentMod number $env(USER_NUMBER)

param EnderSiAlignmentMod rphi_cut 0.00012
param EnderSiAlignmentMod z_cut 0.0011

param EnderSiAlignmentMod rphi_norm 0.0013
param EnderSiAlignmentMod z_norm 0.0053

param EnderSiAlignmentMod deltaX_step 0.000010
param EnderSiAlignmentMod deltaY_step 0.000010
param EnderSiAlignmentMod deltaZ_step 0.000500
param EnderSiAlignmentMod phiX_step 0.000400
param EnderSiAlignmentMod phiY_step 0.001000
param EnderSiAlignmentMod phiZ_step 0.000200

param EnderSiAlignmentMod deltaX_limit 0.001000
param EnderSiAlignmentMod deltaY_limit 0.001000
param EnderSiAlignmentMod deltaZ_limit 0.010000
param EnderSiAlignmentMod phiX_limit 0.050000
param EnderSiAlignmentMod phiY_limit 0.050000
param EnderSiAlignmentMod phiZ_limit 0.050000

report level NOTICE
iterator go

exit

