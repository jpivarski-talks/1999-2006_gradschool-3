module sel ../build_SunOS/shlib/HbookHistogramModule
hbook file tmp.rzn
hbook init

default prompt off
exception continueEventLoop on

# database and metafiles -------
module sel CorbaModule
corba init
module sel ConstantsModule

constants in $env(C3_CONST) meta Offline MetaListRetrieval streams svcalibration svefficiency svresolution svradcorrection
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval


# Get material properties-------
source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

# prod sel CDOffCal

prod sel SiGeom
prod sel ASiStorePro
# prod sel IRGeomProd

#  prod sel DRGeom
#  prod sel ADRGeomProd
#  prod sel DRHitCorrectorProd

prod sel MagFieldProd
#  param MagFieldProd alwaysUseDefault true

#  prod sel TrackDeliveryProd
#  prod sel SeedToTRTrackProd
#  prod sel NavigationProd

source_format sel PDSSourceFormat
file in ../data16_dec10/tracks_121339.pds beginrun startrun event

emptysource def svalignment svalignment

module sel ../build_SunOS/shlib/HandSiAlignmentMod

param HandSiAlignmentMod global_init dec16.svalignmentglobal
param HandSiAlignmentMod layer_init  dec18.svalignmentlayer
param HandSiAlignmentMod ladder_init before_opening.svalignmentladder
param HandSiAlignmentMod sensor_init before_opening.svalignmentsensor

param HandSiAlignmentMod skim_dir ../data16_dec10/skim/

param HandSiAlignmentMod device "ladder"
param HandSiAlignmentMod number 61

report level NOTICE
iterator go

