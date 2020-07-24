default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/db27_cs_zthousandth_1.rzn
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
file add pdschain /cdat/sol300/disk5/mccann/r130199.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130200.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130203.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130204.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130205.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130206.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130207.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130209.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130210.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130245.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130247.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130248.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130249.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130250.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130251.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130252.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130254.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130255.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130256.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130259.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130262.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130264.pds event

proc sel ../build_SunOS/shlib/TwoCycleHist

report level DEBUG
go 5
report level INFO
go
exit
