default prompt off
exception continueEventLoop on

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Online
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams svresolution
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

source_format sel SVResolutionGlobalFileSourceFormat
file in loose.svresolutionglobal

source_format sel ../build_$env(OS_NAME)/shlib/SVResolutionLayerFileSourceFormat
file in loose.svresolutionlayer

# source_format sel SVHybridEfficiencyFileSourceFormat
# file in normal.svhybridefficiency

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
	param MagFieldProd alwaysUseDefault true
	param MagFieldProd defaultHallProbeReading 1.49123

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

	param KalmanProd HyposToFit electron muon
	param KalmanProd OptimizeForAllHypos true
        param KalmanProd HitListFromOneHypo true
	param KalmanProd ScaleCathodeResolution true 
	param KalmanProd CathodeResolutionScale 0.7     

# If someone requests electrons or muons, give them the pion fit
# 	prod sel ElectronFromPionFitProd
# 	prod sel MuonFromPionFitProd

# Set up NavTrack
	prod sel NavigationProd
	prod sel TrackDeliveryProd
	prod sel SeedToTRTrackProd
#########################################################

# param KalmanProd MaxHitsToDelete 0

prod sel ../build_SunOS/shlib/DualTrackHelicesProd

prod sel DBRunHeaderProd
prod sel DBEventHeaderProd
#  prod sel CesrBeamEnergyProd

#  prod sel CcfcReconProd
#  prod sel C3ccReconProd
#  proc sel CcEventSelectProc
#  prod sel C3ccProd
#  prod sel CcGeomProd
#  prod sel C3ccConProd
#  param CcEventSelectProc BhabhaKeep true
#  param CcEventSelectProc MuPairKeep true

#  source_format sel BinarySourceFormat
#  file in cont_123592.bin event beginrun startrun endrun
#  source act cont_123592 event beginrun startrun endrun

module sel DBModule
database in /nfs/cleo3/database/data16/db beginrun event startrun endrun
source activate db beginrun event startrun endrun
database read_collection rawdata bhabha

proc sel ../build_SunOS/shlib/FakeSiAlignmentProc

sink_format sel PDSSinkFormat
file sink $env(USER_DAFE)/sialignment/bug_magnitude_pds/bugless_yet_again.pds event beginrun startrun endrun

report level NOTICE
go 10000

exit
