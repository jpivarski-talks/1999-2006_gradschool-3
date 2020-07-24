default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_heave_5.rzn
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
file in heave.drgeomalignment

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
file add pdschain /cdat/sol300/disk5/mccann/r130386.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130387.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130389.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130391.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130392.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130394.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130395.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130398.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130399.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130400.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130401.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130402.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130403.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130404.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130405.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130406.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130408.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130409.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130419.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130420.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130421.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130423.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
