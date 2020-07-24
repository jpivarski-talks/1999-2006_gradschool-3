default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/checksimplething.rzn
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
prod sel ADRGeomProd
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

prod sel KalmanProd
param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false
param KalmanProd UseDRAxial false
param KalmanProd UseDRCathode true
param KalmanProd ScaleCathodeResolution true
param KalmanProd CathodeResolutionScale 1e8

prod sel TrkmanProd
prod sel TrkmanSelectionProd

run_file $env(C3_SCRIPTS)/C3cc.tcl

source create pdschain
source_format sel PDSSourceFormat
file add pdschain /cdat/sol300/disk5/mccann/r130199_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130200_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130203_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130204_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130205_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130206_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130207_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130209_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130210_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130245_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130247_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130248_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130249_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130250_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130251_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130252_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130254_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130255_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130256_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130259_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130262_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130264_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130265_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130266_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130268_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130271_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130272_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130275_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130276_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130278_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130281_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130282_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130283_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130284_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130286_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130287_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130289_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130290_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130293_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130294_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130296_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130297_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130299_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130300_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130301_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130302_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130303_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130306_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130307_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130308_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130310_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130312_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130315_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130316_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130317_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130318_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130319_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130320_hadron.pds event
# file add pdschain /cdat/sol300/disk5/mccann/r130324_hadron.pds event

proc sel ../build_SunOS/shlib/CheckSimpleThing
go
