module sel LoadHbook4MModule
module sel ../build_SunOS/shlib/HbookHistogramModule
hbook file ../rzn/hand.rzn
hbook init

# database and metafiles -------
module sel CorbaModule
corba init
module sel ConstantsModule

constants in $env(C3_CONST) meta Offline MetaListRetrieval streams svcalibration svefficiency svresolution svradcorrection
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

prod sel ../build_SunOS/shlib/SiGeom
prod sel ../build_SunOS/shlib/ASiStorePro
prod sel MagFieldProd

# this is a total placebo--- everything I want comes from stream svalignment
source_format sel PDSSourceFormat
file in $env(USER_DAFE)/sialignment/loose_tracks/dual_chi_newalignment_loose1.pds event

emptysource def svalignment svalignment

module sel ../build_SunOS/shlib/HandSiAlignmentMod

param HandSiAlignmentMod globalFile  normal.svalignmentglobal
param HandSiAlignmentMod layerFile   normal.svalignmentlayer
param HandSiAlignmentMod ladderFile  rounded.svalignmentladder
param HandSiAlignmentMod sensorFile  unmoved.svalignmentsensor
param HandSiAlignmentMod sagittaFile zeros.sagitta

param HandSiAlignmentMod maxHits -1
param HandSiAlignmentMod rphiCut 0.001

ladder 61
iterator go
