default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn3/stretchad_$env(DIRTY_STRETCH)_$env(DIRTY_WHICH)_2.rzn
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
file add pdschain /cdat/sol300/disk5/mccann/r130265_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130266_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130268_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130271_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130272_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130275_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130276_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130278_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130281_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130282_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130284_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130286_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130287_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130289_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130290_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130293_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130294_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130296_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130297_hadron.pds event
file add pdschain /cdat/sol300/disk5/mccann/r130299_hadron.pds event

proc sel ../build_SunOS/shlib/ChiSquaredsProc

report level DEBUG
go 1
report level INFO
go
exit
