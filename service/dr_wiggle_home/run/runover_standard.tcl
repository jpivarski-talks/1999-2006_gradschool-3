default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/standard2.rzn
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
param DRWiggle getResiduals false

source_format sel PDSSourceFormat

report level NOTICE

file in ../rzn/standard01.pds event
go 129054
source rm standard01

file in ../rzn/standard02.pds event
go 144633
source rm standard02

file in ../rzn/standard03.pds event
go 375470
# died prematurely
source rm standard03

file in ../rzn/standard04.pds event
go 230962
# died prematurely
source rm standard04

file in ../rzn/standard05.pds event
go 283154
# died prematurely
source rm standard05

file in ../rzn/standard06.pds event
go 380068
# died prematurely
source rm standard06

file in ../rzn/standard07.pds event
go 347083
# died prematurely
source rm standard07

file in ../rzn/standard08.pds event
go 31218
source rm standard08

file in ../rzn/standard09.pds event
go 163573
# died prematurely
source rm standard09

file in ../rzn/standard10.pds event
go 125177
source rm standard10

file in ../rzn/standard11.pds event
go 282859
# died prematurely
source rm standard11

file in ../rzn/standard12.pds event
go 229759
# died prematurely
source rm standard12

file in ../rzn/standard13.pds event
go 245053
# died prematurely
source rm standard13

file in ../rzn/standard14.pds event
go 64791
# died prematurely
source rm standard14

file in ../rzn/standard15.pds event
go 45493
# died prematurely
source rm standard15

file in ../rzn/standard16.pds event
go 34167
# died prematurely
source rm standard16

file in ../rzn/standard17.pds event
go 33595
# died prematurely
source rm standard17

file in ../rzn/standard18.pds event
go 20056
# died prematurely
source rm standard18

hbook terminate
exit
