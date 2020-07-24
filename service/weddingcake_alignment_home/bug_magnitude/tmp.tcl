module sel LoadHbook4MModule
module sel HbookHistogramModule
hbook file ../dafedir/bugless1.rzn
hbook init

default prompt off
exception continueEventLoop on

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Online MetaListRetrieval
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams drweightlayer dralignment
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
# constants in $env(C3_CONST) meta RunByRun MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval
constants in $env(C3_CONST) meta Alignment MetaListRetrieval
# constants in $env(C3_CONST) meta FieldMap MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

module sel RunStatisticsSourceModule
runstatistics in

source_format sel DRWeightLayerFileSourceFormat
file in stereo_only.drweightlayer

source_format sel DRGeomAlignmentFileSourceFormat
file in sept_alignment.drgeomalignment

#########################################################
# Load DR Geometry-------
	prod sel ADRGeomProd
	prod sel DRGeom

# Load Silicon and IR geometry-------
	prod sel ASiStorePro
	prod sel SiGeom
	prod sel IRGeomProd

# Load Raw data---
	prod sel RawDataProd

# Convert Raw cathode hits to DRCACorrectHits----
	prod sel DRCACorrectHitProd

# Cluster cathodes---
	prod sel DRCAClusterProd

# Make PreliminaryDRHits-----
#	prod sel PreliminaryDRHitProd

# Make CalibratedDRHits-----
	prod sel CalibratedDRHitProd

# Make Si Hits-----
#	prod sel CorrectedSVStripProd
	prod sel SiHitsProd

# Load the magnetic field----
	prod sel MagFieldProd

# CDOffCal
	prod sel ../build_$env(OS_NAME)/shlib/CDOffCal

# Get trigger information
	prod sel TriggerL1DataProd

# Get bunch decision
	prod sel TrackletBunchFinder

# Load Doit
	prod sel DoitProd
	param DoitProd ContinueIfHitsMissing true

# Load DRHitCorrections
	prod sel DRHitCorrectorProd
	 param DRHitCorrectorProd ApplyEntranceAngleCorr false           
         param DRHitCorrectorProd ApplyStandardCorrections false   

# Load Kalman fit
	prod sel ../build_$env(OS_NAME)/shlib/KalmanProd
# Set Kalman to only fit pion kaon and proton since that is all
#  we want store from Pass2
	param KalmanProd HyposToFit pion kaon proton
	param KalmanProd OptimizeForAllHypos true
        param KalmanProd HitListFromOneHypo true
	param KalmanProd ScaleCathodeResolution true 
	param KalmanProd CathodeResolutionScale 0.7     

# If someone requests electrons or muons, give them the pion fit
	prod sel ElectronFromPionFitProd
	prod sel MuonFromPionFitProd

# Set up NavTrack
	prod sel NavigationProd
	prod sel TrackDeliveryProd
	prod sel SeedToTRTrackProd
#########################################################

param DoitProd UseSilicon false

param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false
param KalmanProd UseDRCathode false
param KalmanProd UseDRAxial true
param KalmanProd UseDRStereoU true
param KalmanProd UseDRStereoV true

prod sel DBRunHeaderProd
prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd

prod sel CcfcReconProd
prod sel C3ccReconProd
proc sel CcEventSelectProc
prod sel C3ccProd
prod sel CcGeomProd
prod sel C3ccConProd
param CcEventSelectProc BhabhaKeep true

source_format sel BinarySourceFormat
file in cont_123592.bin event beginrun startrun endrun
source act cont_123592 event beginrun startrun endrun

proc sel ../build_$env(OS_NAME)/shlib/HitLevel
proc sel HistogramViewerProc

report level DEBUG
go 10
report level NOTICE
