default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_push_10.rzn
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
file in push.drgeomalignment

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
file add pdschain /cdat/sol300/disk5/mccann/r130590.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130591.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130592.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130593.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130595.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130597.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130600.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130602.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130604.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130605.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130606.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130607.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130609.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130610.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130611.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130614.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130615.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130616.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130626.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130628.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130629.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130630.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
