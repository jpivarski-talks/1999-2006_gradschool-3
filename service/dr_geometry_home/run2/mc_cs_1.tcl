default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/mc_cs_1.rzn
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
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/diagnostics/cleogoutput/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112802-E3387-cleog.pds event
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/diagnostics/cleogoutput/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112803-E5539-cleog.pds event
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/diagnostics/cleogoutput/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112804-E4832-cleog.pds event
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/diagnostics/cleogoutput/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112806-E4977-cleog.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
