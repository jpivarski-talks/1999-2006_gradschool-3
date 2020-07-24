
default prompt off
exception continueEventLoop on

module sel LoadHbook4MModule
module sel HbookHistogramModule
hbook file ../rzn/layer2loose_again.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Online
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams svresolution
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
# constants in $env(C3_CONST) meta RunByRun MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval
constants in $env(C3_CONST) meta Alignment MetaListRetrieval
constants in $env(C3_CONST) meta FieldMap MetaListRetrieval

source_format sel SVResolutionGlobalFileSourceFormat
file in data16.svresolutionglobal
source_format sel ../build_$env(OS_NAME)/shlib/SVResolutionLayerFileSourceFormat
file in layer2loose.svresolutionlayer

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

module sel RunStatisticsSourceModule
runstatistics in

# run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
# prod sel ChisqFitProd

# prod sel DualTrackHelicesProd

# prod sel DBRunHeaderProd
# prod sel DBEventHeaderProd

# module sel DBModule
# database in /nfs/cleo3/database/data16/db beginrun event startrun endrun
# source activate db beginrun event startrun endrun
# database read_collection rawdata bhabha

# sink_format sel PDSSinkFormat
# file sink ../pds/layer2loose.pds event beginrun startrun endrun

prod sel ASiStorePro
prod sel SiGeom
prod sel IRGeomProd
prod sel RawDataProd
prod sel CorrectedSVStripProd
prod sel SiHitsProd
prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd
source_format sel PDSSourceFormat
file source ../pds/layer2loose.pds event beginrun startrun endrun

proc sel ../build_$env(OS_NAME)/shlib/AlignmentPlots

param AlignmentPlots useKalmanResids false
param AlignmentPlots useMuPairs false
param AlignmentPlots useDualTracks true
param AlignmentPlots beamEnergy 5.1794
param AlignmentPlots weightHits false
param AlignmentPlots rphiCut 0.000200
param AlignmentPlots zCut 0.0015
param AlignmentPlots fillRphiHitNtuple false
param AlignmentPlots fillZHitNtuple false
param AlignmentPlots fillMatchedNtuple false
param AlignmentPlots fillRphiOverlapNtuple false
param AlignmentPlots fillZOverlapNtuple false

report level NOTICE
go 50000

hbook terminate
exit

