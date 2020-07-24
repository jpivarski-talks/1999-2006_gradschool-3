default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/findbug17mc.rzn
hbook init

module sel CorbaModule
corba init
module sel RunStatisticsSourceModule
runstatistics in
module sel ConstantsModule
constants in $env(C3_CONST) meta FieldMap MetaListRetrieval

source_format sel MagFudgeFileSourceFormat
file in normal.magfudge

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel C3ccProd
prod sel MagFieldProd
prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd
prod sel EventPropertiesProd

source_format sel PDSSourceFormat
source create pdschain
file add pdschain 
file add pdschain 
file add pdschain 
source act pdschain event startrun beginrun endrun physics

proc sel ../build_SunOS/shlib/PointToError

prod lss

# report level DEBUG

# go 7000
go 500

hbook terminate
exit
