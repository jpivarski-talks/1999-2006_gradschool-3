default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_zhundred_8.rzn
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
file in zhundred.drgeomalignment

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
file add pdschain /cdat/sol300/disk5/mccann/r130512.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130513.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130514.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130515.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130517.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130518.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130519.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130520.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130521.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130523.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130525.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130527.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130528.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130529.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130530.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130531.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130532.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130533.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130535.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130536.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130537.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130541.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130542.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
