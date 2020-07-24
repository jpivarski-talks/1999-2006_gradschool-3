default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_nudge_2.rzn
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
file in nudge.drgeomalignment

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
file add pdschain /cdat/sol300/disk5/mccann/r130265.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130266.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130268.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130271.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130272.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130275.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130276.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130278.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130281.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130282.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130284.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130286.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130287.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130289.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130290.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130293.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130294.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130296.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130297.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130299.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130300.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130301.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130302.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
