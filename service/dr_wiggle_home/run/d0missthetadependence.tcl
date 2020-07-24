default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/d0missthetadependence.rzn
hbook init

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel DBEventHeaderProd

prod sel RunInfoProd
source_format sel RunInfoFileSourceFormat
file in /nfs/cleo3/Offline/InfoFiles/data/runinfo.runinfo beginrun startrun
source_format sel BeamSpotFileSourceFormat
file in $env(C3_INFO)/data/beamspot.beamspot beginrun

proc sel ../build_SunOS/shlib/D0MissThetaDependence

source_format sel PDSSourceFormat

file in ../rzn/stereoonly_midcell01.pds event
go 113910
source rm stereoonly_midcell01

file in ../rzn/stereoonly_midcell02.pds event
go 127426
source rm stereoonly_midcell02

file in ../rzn/stereoonly_midcell03.pds event
go 331696
source rm stereoonly_midcell03

file in ../rzn/stereoonly_midcell04.pds event
go 203969
source rm stereoonly_midcell04

file in ../rzn/stereoonly_midcell05.pds event
go 250275
source rm stereoonly_midcell05

file in ../rzn/stereoonly_midcell06.pds event
go 335454
source rm stereoonly_midcell06

file in ../rzn/stereoonly_midcell07.pds event
go 306223
source rm stereoonly_midcell07

file in ../rzn/stereoonly_midcell08.pds event
go 27580
source rm stereoonly_midcell08

file in ../rzn/stereoonly_midcell09.pds event
go 144430
source rm stereoonly_midcell09

file in ../rzn/stereoonly_midcell10.pds event
go 110456
source rm stereoonly_midcell10

file in ../rzn/stereoonly_midcell11.pds event
go 249741
source rm stereoonly_midcell11

file in ../rzn/stereoonly_midcell12.pds event
go 202977
source rm stereoonly_midcell12

file in ../rzn/stereoonly_midcell13.pds event
go 216896
source rm stereoonly_midcell13

file in ../rzn/stereoonly_midcell14.pds event
go 37940
source rm stereoonly_midcell14

file in ../rzn/stereoonly_midcell15.pds event
go 34876
source rm stereoonly_midcell15

file in ../rzn/stereoonly_midcell16.pds event
go 32941
source rm stereoonly_midcell16

file in ../rzn/stereoonly_midcell17.pds event
go 32864
source rm stereoonly_midcell17

file in ../rzn/stereoonly_midcell18.pds event
go 14224
source rm stereoonly_midcell18

hbook terminate
exit
