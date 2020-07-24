default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/runover_test.rzn
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
#  prod sel PionFitForMissingProd

proc sel ../build_SunOS/shlib/DRWiggle

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection rawdata bhabha

source_format sel PDSSourceFormat

# report level DEBUG

file in ../rzn/standard01.pds
source act standard01 event
go 129054
source rm standard01

file in ../rzn/standard02.pds
source act standard02 event
go 144633
source rm standard02

file in ../rzn/standard03.pds
source act standard03 event
go 375470
# died prematurely
source rm standard03

file in ../rzn/standard04.pds
source act standard04 event
go 230962
# died prematurely
source rm standard04

file in ../rzn/standard05.pds
source act standard05 event
go 283154
# died prematurely
source rm standard05

file in ../rzn/standard06.pds
source act standard06 event
go 380068
# died prematurely
source rm standard06

file in ../rzn/standard07.pds
source act standard07 event
go 347083
# died prematurely
source rm standard07

#  file in ../rzn/standard08.pds
#  source act standard08 event
#  go 31218
#  source rm standard08

#  file in ../rzn/standard09.pds
#  source act standard09 event
#  go 163573
#  # died prematurely
#  source rm standard09

#  file in ../rzn/standard10.pds
#  source act standard10 event
#  go 125177
#  source rm standard10

#  file in ../rzn/standard11.pds
#  source act standard11 event
#  go 282859
#  # died prematurely
#  source rm standard11

#  file in ../rzn/standard12.pds
#  source act standard12 event
#  go 229759
#  # died prematurely
#  source rm standard12

#  file in ../rzn/standard13.pds
#  source act standard13 event
#  go 245053
#  # died prematurely
#  source rm standard13

#  file in ../rzn/standard14.pds
#  source act standard14 event
#  go 64791
#  # died prematurely
#  source rm standard14

#  file in ../rzn/standard15.pds
#  source act standard15 event
#  go 45493
#  # died prematurely
#  source rm standard15

#  file in ../rzn/standard16.pds
#  source act standard16 event
#  go 34167
#  # died prematurely
#  source rm standard16

#  file in ../rzn/standard17.pds
#  source act standard17 event
#  go 33595
#  # died prematurely
#  source rm standard17

#  file in ../rzn/standard18.pds
#  source act standard18 event
#  go 20056
#  # died prematurely
#  source rm standard18

hbook terminate
exit
