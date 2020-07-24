default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/nt_midcell.rzn
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

file in ../rzn/midcell01.pds event
go 127465
source rm midcell01

file in ../rzn/midcell02.pds event
go 142909
source rm midcell02

file in ../rzn/midcell03.pds event
go 370753
# died prematurely
source rm midcell03

file in ../rzn/midcell04.pds event
go 228067
# died prematurely
source rm midcell04

file in ../rzn/midcell05.pds event
go 279751
# died prematurely
source rm midcell05

file in ../rzn/midcell06.pds event
go 375429
# died prematurely
source rm midcell06

file in ../rzn/midcell07.pds event
go 342675
# died prematurely
source rm midcell07

file in ../rzn/midcell08.pds event
go 30849
source rm midcell08

file in ../rzn/midcell09.pds event
go 161487
# died prematurely
source rm midcell09

file in ../rzn/midcell10.pds event
go 123596
source rm midcell10

file in ../rzn/midcell11.pds event
go 279238
# died prematurely
source rm midcell11

file in ../rzn/midcell12.pds event
go 178646
# died prematurely
source rm midcell12

file in ../rzn/midcell13.pds event
go 241968
source rm midcell13

file in ../rzn/midcell14.pds event
go 55763
# died prematurely
source rm midcell14

file in ../rzn/midcell15.pds event
go 46409
# died prematurely
source rm midcell15

file in ../rzn/midcell16.pds event
go 33415
# died prematurely
source rm midcell16

file in ../rzn/midcell17.pds event
go 32336
# died prematurely
source rm midcell17

file in ../rzn/midcell18.pds event
go 19202
# died prematurely
source rm midcell18

hbook terminate
exit
