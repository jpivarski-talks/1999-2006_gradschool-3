default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn3/stretch_$env(DIRTY_STRETCH)_6.rzn
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
file add pdschain /cdat/sol300/disk5/mccann/r130424.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130425.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130426.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130427.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130429.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130456.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130460.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130461.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130462.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130463.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130464.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130467.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130468.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130469.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130470.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130472.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130473.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130474.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130475.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130476.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130477.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130478.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130482.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
