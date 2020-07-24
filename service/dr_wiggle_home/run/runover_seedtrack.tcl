default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/seedtrack.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval streams svalignment svcalibration svresolution svradcorrection dralignment drdaqcellidmap drwirequality drvarious drdriftdata drdriftdatav2 drgarfield_distancecorrection drgarfield_timecorrection drweightdrift drweightlayer drweighttheta drweightphi dradc2charge dradcpedestal drt0global drt0crate drt0card drt0hvcard drt0channel drt0channelv2 drtdc2time drelecmap dradcsaturation drcapedestal drcagain
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta MagFudge MetaListRetrieval
constants in $env(C3_CONST) meta FieldMap MetaListRetrieval
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
prod sel ADRGeomProd
prod sel DRGeom
prod sel RawDataProd
prod sel PreliminaryDRHitProd
prod sel CalibratedDRHitProd
prod sel MagFieldProd
prod sel CDOffCal
prod sel TrackletBunchFinder
prod sel TriggerL1DataProd
prod sel TriggerPhaseForDataProd
prod sel DRHitCorrectorProd

proc sel ../build_SunOS/shlib/DRWiggle
param DRWiggle trackUse "seed"

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection rawdata bhabha

source_format sel PDSSourceFormat

file in ../rzn/seedtrack01.pds
source act seedtrack01 event
go 104353
source rm seedtrack01

file in ../rzn/seedtrack02.pds
source act seedtrack02 event
go 102153
source rm seedtrack02

file in ../rzn/seedtrack03.pds
source act seedtrack03 event
go 85955
source rm seedtrack03

file in ../rzn/seedtrack04.pds
source act seedtrack04 event
go 86205
source rm seedtrack04

file in ../rzn/seedtrack05.pds
source act seedtrack05 event
go 83146
source rm seedtrack05

file in ../rzn/seedtrack06.pds
source act seedtrack06 event
go 84740
source rm seedtrack06

file in ../rzn/seedtrack07.pds
source act seedtrack07 event
go 83745
source rm seedtrack07

file in ../rzn/seedtrack08.pds
source act seedtrack08 event
go 31213
source rm seedtrack08

file in ../rzn/seedtrack09.pds
source act seedtrack09 event
go 83963
source rm seedtrack09

file in ../rzn/seedtrack10.pds
source act seedtrack10 event
go 78011
source rm seedtrack10

file in ../rzn/seedtrack11.pds
source act seedtrack11 event
go 78004
source rm seedtrack11

file in ../rzn/seedtrack12.pds
source act seedtrack12 event
go 77539
source rm seedtrack12

file in ../rzn/seedtrack13.pds
source act seedtrack13 event
go 76503
source rm seedtrack13

file in ../rzn/seedtrack14.pds
source act seedtrack14 event
go 79666
source rm seedtrack14

file in ../rzn/seedtrack15.pds
source act seedtrack15 event
go 72226
source rm seedtrack15

file in ../rzn/seedtrack16.pds
source act seedtrack16 event
go 72629
source rm seedtrack16

file in ../rzn/seedtrack17.pds
source act seedtrack17 event
go 70651
source rm seedtrack17

file in ../rzn/seedtrack18.pds
source act seedtrack18 event
go 26111
source rm seedtrack18

file in ../rzn/seedtrack19.pds
source act seedtrack19 event
go 65594
source rm seedtrack19

file in ../rzn/seedtrack20.pds
source act seedtrack20 event
go 54730
source rm seedtrack20

file in ../rzn/seedtrack21.pds
source act seedtrack21 event
go 64062
source rm seedtrack21

file in ../rzn/seedtrack22.pds
source act seedtrack22 event
go 62613
source rm seedtrack22

file in ../rzn/seedtrack23.pds
source act seedtrack23 event
go 51140
source rm seedtrack23

file in ../rzn/seedtrack24.pds
source act seedtrack24 event
go 53520
source rm seedtrack24

file in ../rzn/seedtrack25.pds
source act seedtrack25 event
go 43183
source rm seedtrack25

file in ../rzn/seedtrack26.pds
source act seedtrack26 event
go 46925
source rm seedtrack26

file in ../rzn/seedtrack27.pds
source act seedtrack27 event
go 42035
source rm seedtrack27

file in ../rzn/seedtrack28.pds
source act seedtrack28 event
go 49685
source rm seedtrack28

file in ../rzn/seedtrack29.pds
source act seedtrack29 event
go 40693
source rm seedtrack29

file in ../rzn/seedtrack30.pds
source act seedtrack30 event
go 40538
source rm seedtrack30

hbook terminate
exit
