default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_throttle_3.rzn
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

source_format sel DRGeomAlignmentFileSourceFormat
file in throttle.drgeomalignment

run_file $env(C3_SCRIPTS)/BeamSpot.tcl

# for DetectorConfiguration
prod sel DetectorConfigurationProd
# for ADRSenseWireStore and ADRCathodeStore
prod sel ../build_SunOS/shlib/ADRGeomProd
# for DGDetectorStore
prod sel DRGeom
# for HitCorrector
prod sel DRHitCorrectorProd
# for DriftFunction
prod sel CDOffCal
# for ASiStore
prod sel ASiStorePro
# for DGDetectorStore
prod sel SiGeom
# for DGDetectorStore
prod sel IRGeomProd
# for MagneticField
prod sel MagFieldProd

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

prod sel ChisqFitProd
param ChisqFitProd UseSVRPhi false
param ChisqFitProd UseSVZ false
param ChisqFitProd UseDRAxial false
param ChisqFitProd UseDRCathode false

run_file $env(C3_SCRIPTS)/C3cc.tcl

source create pdschain
source_format sel PDSSourceFormat
file add pdschain /cdat/sol300/disk5/mccann/r130303.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130306.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130307.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130308.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130310.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130312.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130315.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130316.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130317.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130318.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130319.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130320.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130324.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130328.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130329.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130332.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130333.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130335.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130337.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130338.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130339.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130343.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
