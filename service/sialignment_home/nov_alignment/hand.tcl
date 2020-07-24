module sel ../build_SunOS/shlib/HbookHistogramModule
hbook file hand.rzn
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

prod sel SiGeom
prod sel ASiStorePro
prod sel MagFieldProd
#  param MagFieldProd alwaysUseDefault true

source_format sel PDSSourceFormat
file in ../loose_tracks/dual_chi_newalignment_loose1.pds beginrun startrun event

emptysource def svalignment svalignment

module sel ../build_SunOS/shlib/HandSiAlignmentMod

param HandSiAlignmentMod global_init state.svalignmentglobal
param HandSiAlignmentMod layer_init  state.svalignmentlayer
param HandSiAlignmentMod ladder_init state.svalignmentladder
param HandSiAlignmentMod sensor_init state.svalignmentsensor

param HandSiAlignmentMod rphi_cut 0.00012
param HandSiAlignmentMod z_cut 0.0011

param HandSiAlignmentMod skim_dir ../loose_tracks/skim/

param HandSiAlignmentMod device "ladder"
param HandSiAlignmentMod number 61

report level NOTICE
iterator go

