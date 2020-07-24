default prompt off
exception continueEventLoop on

# module sel LoadHbook4MModule
module sel HbookHistogramModule
hbook file test.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams drweightlayer dralignment
constants in $env(C3_CONST) meta Online MetaListRetrieval
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta RunByRun MetaListRetrieval
constants in $env(C3_CONST) meta Alignment MetaListRetrieval
constants in $env(C3_CONST) meta FieldMap MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

source_format sel SVHybridEfficiencyFileSourceFormat
file in normal.svhybridefficiency

source_format sel DRWeightLayerFileSourceFormat
file in stereoonly.drweightlayer

source_format sel DRGeomAlignmentFileSourceFormat
file in from_sixth.drgeomalignment

### run_file $env(C3_SCRIPTS)/trackingDataFull.tcl ####################

prod sel ADRGeomProd
prod sel DRGeom

prod sel ASiStorePro
prod sel SiGeom
prod sel IRGeomProd

prod sel RawDataProd

prod sel DRCACorrectHitProd

prod sel DRCAClusterProd

prod sel PreliminaryDRHitProd

prod sel CalibratedDRHitProd

prod sel CorrectedSVStripProd
prod sel SiHitsProd

prod sel MagFieldProd
param MagFieldProd alwaysUseDefault true
param MagFieldProd defaultHallProbeReading 1.4944

prod sel CDOffCal

prod sel TriggerL1DataProd
# prod sel TriggerPhaseForDataProd

prod sel TrackletBunchFinder

prod sel DoitProd
param DoitProd ContinueIfHitsMissing true
param DoitProd UseSilicon false

prod sel DRHitCorrectorProd
param DRHitCorrectorProd ApplyEntranceAngleCorr false           
param DRHitCorrectorProd ApplyStandardCorrections false   

# prod sel ElectronFromPionFitProd
# prod sel MuonFromPionFitProd

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

########################################################################

prod sel ChisqFitProd
param ChisqFitProd UseSVRPhi false
param ChisqFitProd UseSVZ false

param ChisqFitProd UseDRAxial true
param ChisqFitProd UseDRStereoU true
param ChisqFitProd UseDRStereoV true

param ChisqFitProd UseDRCathode false

prod sel DBEventHeaderProd
prod sel DualTrackHelicesProd

module sel DBModule
database in /nfs/cleo3/database/data11/db beginrun event startrun endrun
source activate db beginrun event startrun endrun
database read_collection rawdata bhabha
database run 116368

proc sel ../build_SunOS/shlib/CakeAlignFlat
proc sel HistogramViewerProc

# sink_format sel PDSSinkFormat
# file out ../dafedir/april_tracks1.pds beginrun event startrun endrun

go 1
