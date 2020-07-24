default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_zthousandth_4.rzn
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
file in zthousandth.drgeomalignment

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
file add pdschain /cdat/sol300/disk5/mccann/r130344.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130345.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130346.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130347.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130348.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130350.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130351.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130352.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130354.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130355.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130357.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130358.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130359.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130360.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130364.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130366.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130367.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130369.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130372.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130374.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130375.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130379.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130384.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
