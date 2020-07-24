default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_push_7.rzn
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
file add pdschain /cdat/sol300/disk5/mccann/r130484.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130485.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130486.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130488.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130489.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130490.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130491.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130492.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130494.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130495.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130496.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130497.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130500.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130501.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130502.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130503.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130505.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130506.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130507.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130508.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130509.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130511.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
