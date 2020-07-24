default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/test.rzn
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
proc sel HistogramViewerProc

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection rawdata bhabha

source_format sel PDSSourceFormat

#  report level DEBUG

file in ../rzn/$env(USER_FILE).pds
source act $env(USER_FILE) event
go

exit
