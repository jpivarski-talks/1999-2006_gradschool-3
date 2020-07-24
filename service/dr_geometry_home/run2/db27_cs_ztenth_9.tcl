default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_ztenth_9.rzn
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
file in ztenth.drgeomalignment

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
file add pdschain /cdat/sol300/disk5/mccann/r130543.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130546.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130547.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130548.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130551.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130554.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130556.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130557.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130559.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130561.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130562.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130563.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130564.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130567.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130569.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130572.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130573.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130577.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130579.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130586.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130588.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130589.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
