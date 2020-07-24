default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/mc_cs_3.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval
constants in $env(C3_CONST) meta Alignment MetaListRetrieval
constants in $env(C3_CONST) meta FieldMap MetaListRetrieval
constants in $env(C3_CONST) meta MagFudge MetaListRetrieval
constants in $env(C3_CONST) meta DAQID MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

module sel RunStatisticsSourceModule
runstatistics in

source_format sel BeamSpotFileSourceFormat
file in $env(C3_INFO)/mc/zeromc.beamspot beginrun

prod sel ChisqFitProd
run_file $env(C3_SCRIPTS)/trackingMCFull.tcl
prod desel KalmanProd
param ChisqFitProd UseSVRPhi false
param ChisqFitProd UseSVZ false
param ChisqFitProd UseDRAxial false
param ChisqFitProd UseDRCathode false

run_file $env(C3_SCRIPTS)/C3cc.tcl

source create pdschain
source_format sel PDSSourceFormat
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121474-E5000-DTS15102002_101141-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121476-E5000-DTS15102002_101153-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121477-E5000-DTS15102002_101205-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121479-E5000-DTS15102002_101217-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121480-E5000-DTS15102002_101229-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121519-E5000-DTS15102002_101244-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121523-E5000-DTS15102002_101258-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121525-E5000-DTS15102002_101311-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121526-E5000-DTS15102002_101324-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R121527-E5000-DTS15102002_101337-cg3.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
