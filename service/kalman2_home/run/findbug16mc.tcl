default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/findbug16mc.rzn
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
file add pdschain /cdat/sol221/disk5/c3mc/data16/cont/output/cont-Jun27_02_MC-MN-1-R121339-E261-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk5/c3mc/data16/cont/output/cont-Jun27_02_MC-MN-1-R121340-E396-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk5/c3mc/data16/cont/output/cont-Jun27_02_MC-MN-1-R121343-E315-pass2.pds event startrun beginrun endrun physics
source act pdschain event startrun beginrun endrun physics

proc sel ../build_SunOS/shlib/PointToError

prod lss

# report level DEBUG

# go 7000
go 500

hbook terminate
exit
