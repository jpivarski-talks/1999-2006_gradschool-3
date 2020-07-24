default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/mc_cs_4.rzn
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
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122274-E5000-DTS15102002_091613-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122275-E5000-DTS15102002_091624-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122276-E5000-DTS15102002_091637-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122277-E5000-DTS15102002_091648-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122278-E5000-DTS15102002_091700-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122279-E5000-DTS15102002_091712-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122280-E5000-DTS15102002_091725-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122281-E5000-DTS15102002_091739-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122283-E5000-DTS15102002_091752-cg3.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122285-E5000-DTS15102002_091804-cg3.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
