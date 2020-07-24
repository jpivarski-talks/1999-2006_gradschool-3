default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/mc100.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval streams svalignment svcalibration svresolution svradcorrection
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry
module sel RunStatisticsSourceModule
runstatistics in

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel ASiStorePro
prod sel SiGeom
prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/DRWiggle

source_format sel PDSSourceFormat

# report level DEBUG

file in ../rzn/mc100.pds event
go

hbook terminate
exit
