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
file in ../bug_magnitude_pds/bugless_yet_again.pds beginrun startrun event

emptysource def svalignment svalignment

module sel ../build_SunOS/shlib/HandSiAlignmentMod

param HandSiAlignmentMod global_init normal.svalignmentglobal
param HandSiAlignmentMod layer_init  normal.svalignmentlayer
param HandSiAlignmentMod ladder_init normal.svalignmentladder
param HandSiAlignmentMod sensor_init normal.svalignmentsensor

# param HandSiAlignmentMod skim_dir ../bug_magnitude_pds/bugless_skim/
param HandSiAlignmentMod skim_dir ../loose_tracks/skim/
param HandSiAlignmentMod device "ladder"

report level NOTICE
param HandSiAlignmentMod number 52
iterator go
