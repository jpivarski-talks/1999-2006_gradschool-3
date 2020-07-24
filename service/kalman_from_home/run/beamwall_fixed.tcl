default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/beamwall_fixed.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl

# Load DR Geometry-------
prod sel ADRGeomProd
prod sel DRGeom

# Load Silicon and IR geometry-------
prod sel /nfs/cleo3/Offline/rel/Jul13_02_P2/shlib/SunOS/CC/ASiStorePro
prod sel /nfs/cleo3/Offline/rel/Jul13_02_P2/shlib/SunOS/CC/SiGeom
prod sel ../build_SunOS/shlib/IRGeomProd

# Load Raw data---
prod sel RawDataProd

# Convert Raw cathode hits to DRCACorrectHits----
prod sel DRCACorrectHitProd

# Cluster cathodes---
prod sel DRCAClusterProd

# Make PreliminaryDRHits-----
prod sel PreliminaryDRHitProd

# Make CalibratedDRHits-----
prod sel CalibratedDRHitProd

# Make Si Hits-----
prod sel CorrectedSVStripProd
prod sel SiHitsProd

# Load the magnetic field----
prod sel MagFieldProd

# CDOffCal
prod sel CDOffCal

# Get trigger information
prod sel TriggerL1DataProd
prod sel TriggerPhaseForDataProd

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
prod sel ../build_SunOS/fixed_beampipe/shlib/KalmanProd
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

# My own producers
prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd
prod sel EventPropertiesProd

#  # Data
#  module sel DBModule
#  database in /nfs/cleo3/database/data20/db event startrun beginrun endrun
#  database read_collection rawdata
#  database run 125422

source_format sel BinarySourceFormat
file in /cdat/sol191/disk2/cleo3/data/r122246.bin event startrun beginrun endrun

# My diagnostic processor
proc sel ../build_SunOS/shlib/BeamWallStudyProc

source ls
prod lss

#  report level DEBUG
#  go 1
#  param KalmanProd PrintDiagnostics true
#  go 1
#  report level ERROR
param KalmanProd PrintDiagnostics false
go 10000

hbook terminate
exit
