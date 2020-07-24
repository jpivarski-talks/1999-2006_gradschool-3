default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/nt_stereoonly.rzn
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
param DRWiggle useDualTracks false

source_format sel PDSSourceFormat

# report level DEBUG

file in ../rzn/stereoonly01.pds event
go 114886
source rm stereoonly01

file in ../rzn/stereoonly02.pds event
go 77949
# died prematurely
source rm stereoonly02

file in ../rzn/stereoonly03.pds event
go 231548
# died prematurely
source rm stereoonly03

file in ../rzn/stereoonly04.pds event
go 16793
# died prematurely
source rm stereoonly04

file in ../rzn/stereoonly05.pds event
go 252320
source rm stereoonly05

file in ../rzn/stereoonly06.pds event
go 338074
# died prematurely
source rm stereoonly06

file in ../rzn/stereoonly07.pds event
go 308854
# died prematurely
source rm stereoonly07

file in ../rzn/stereoonly08.pds event
go 27810
source rm stereoonly08

file in ../rzn/stereoonly09.pds event
go 145589
# died prematurely
source rm stereoonly09

file in ../rzn/stereoonly10.pds event
go 111327
source rm stereoonly10

file in ../rzn/stereoonly11.pds event
go 251751
# died prematurely
source rm stereoonly11

file in ../rzn/stereoonly12.pds event
go 204489
# died prematurely
source rm stereoonly12

file in ../rzn/stereoonly13.pds event
go 218527
# died prematurely
source rm stereoonly13

file in ../rzn/stereoonly14.pds event
go 60808
# died prematurely
source rm stereoonly14

file in ../rzn/stereoonly15.pds event
go 40787
# died prematurely
source rm stereoonly15

file in ../rzn/stereoonly16.pds event
go 34502
# died prematurely
source rm stereoonly16

file in ../rzn/stereoonly17.pds event
go 33551
# died prematurely
source rm stereoonly17

file in ../rzn/stereoonly18.pds event
go 19119
# died prematurely
source rm stereoonly18

hbook terminate
exit
