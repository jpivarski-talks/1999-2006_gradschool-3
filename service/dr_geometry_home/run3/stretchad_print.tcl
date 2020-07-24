default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn3/stretchad_$env(DIRTY_STRETCH)_$env(DIRTY_WHICH)_print.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams drgarfield_distancecorrection
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

source_format sel DRGarfield_DistanceCorrectionFSF
file in /cdat/lnssb2/disk2/dubrovin/cleo3/test_tuning/build/EA_COR/d130016.drgarfield_distancecorrection

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

prod sel KalmanProd
param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false
param KalmanProd UseDRAxial true
param KalmanProd UseDRCathode false

prod sel TrkmanProd
prod sel TrkmanSelectionProd

run_file $env(C3_SCRIPTS)/C3cc.tcl

source create pdschain
source_format sel PDSSourceFormat
file add pdschain /cdat/sol300/disk5/mccann/r130300_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130301_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130302_hadron.pds event

proc sel ../build_SunOS/shlib/ChiSquaredsProc

report level WARNING
go
exit
