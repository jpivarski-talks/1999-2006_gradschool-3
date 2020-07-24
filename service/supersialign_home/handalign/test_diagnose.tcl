module sel LoadHbook4MModule
module sel ../build_SunOS/shlib/HbookHistogramModule
hbook file ../rzn/handL1.rzn
hbook init

# database and metafiles -------
module sel CorbaModule
corba init
module sel ConstantsModule

constants in $env(C3_CONST) meta Offline MetaListRetrieval streams svcalibration svefficiency svresolution svradcorrection svalignment
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

proc sel ../build_SunOS/shlib/DiagnoseHits

param DiagnoseHits maxHits 1000

param DiagnoseHits diagLadderStart 37
param DiagnoseHits diagLadderStop 37

param DiagnoseHits nthresh -0.027
param DiagnoseHits pthresh  0.027

go 1

hbook terminate
exit
