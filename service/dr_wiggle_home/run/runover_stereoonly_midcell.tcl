default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/stereoonly_midcell.rzn
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

proc sel ../build_SunOS/shlib/DRWiggle

source_format sel PDSSourceFormat

# report level DEBUG

file in ../rzn/stereoonly_midcell01.pds event
go 113910
source rm stereoonly_midcell01

file in ../rzn/stereoonly_midcell02.pds event
go 127426
source rm stereoonly_midcell02

file in ../rzn/stereoonly_midcell03.pds event
go 331696
# died prematurely
source rm stereoonly_midcell03

file in ../rzn/stereoonly_midcell04.pds event
go 203969
# died prematurely
source rm stereoonly_midcell04

file in ../rzn/stereoonly_midcell05.pds event
go 250275
# died prematurely
source rm stereoonly_midcell05

file in ../rzn/stereoonly_midcell06.pds event
go 335454
# died prematurely
source rm stereoonly_midcell06

file in ../rzn/stereoonly_midcell07.pds event
go 306223
# died prematurely
source rm stereoonly_midcell07

file in ../rzn/stereoonly_midcell08.pds event
go 27580
source rm stereoonly_midcell08

file in ../rzn/stereoonly_midcell09.pds event
go 144430
# died prematurely
source rm stereoonly_midcell09

file in ../rzn/stereoonly_midcell10.pds event
go 110456
source rm stereoonly_midcell10

file in ../rzn/stereoonly_midcell11.pds event
go 249741
# died prematurely
source rm stereoonly_midcell11

file in ../rzn/stereoonly_midcell12.pds event
go 202977
source rm stereoonly_midcell12

file in ../rzn/stereoonly_midcell13.pds event
go 216896
# died prematurely
source rm stereoonly_midcell13

file in ../rzn/stereoonly_midcell14.pds event
go 37940
# died prematurely
source rm stereoonly_midcell14

file in ../rzn/stereoonly_midcell15.pds event
go 34876
# died prematurely
source rm stereoonly_midcell15

file in ../rzn/stereoonly_midcell16.pds event
go 32941
# died prematurely
source rm stereoonly_midcell16

file in ../rzn/stereoonly_midcell17.pds event
go 32864
# died prematurely
source rm stereoonly_midcell17

file in ../rzn/stereoonly_midcell18.pds event
go 14224
# died prematurely
source rm stereoonly_midcell18

hbook terminate
exit
