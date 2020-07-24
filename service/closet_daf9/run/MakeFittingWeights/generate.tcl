# Load histogram stuff
module sel HbookHistogramModule
hbook init

# Get constants
# run_file $env(C3_SCRIPTS)/getConstants.tcl

 # database and metafiles -------
     module sel CorbaModule
     corba init
     module sel ConstantsModule
#     constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams drweightlayer drweighttheta drweightdrift drt0global drt0crate drt0card drt0hvcard drt0channelv2 drdriftdatav2
     constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams drweightlayer drweighttheta drweightdrift drdriftdatav2
     constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
     constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
 
  # We don't use the drcalibration stream
     emptysource def Test  drcalibration
 
 # Get material properties-------
     source_format sel AsciiSourceFormat
     file in $env(C3_DATA)/material.asc basegeometry
 
#  source_format sel DRt0globalFileSourceFormat
#  file in /cdat/sol406/disk1/drcal/main/version2_118193/asciifiles/DRt0global_t0_iter1_v3.drt0global
#  source_format sel DRt0crateFileSourceFormat
#  file in /cdat/sol406/disk1/drcal/main/version2_118193/asciifiles/DRt0crate_t0_iter1_v3.drt0crate
#  source_format sel DRt0cardFileSourceFormat
#  file in /cdat/sol406/disk1/drcal/main/version2_118193/asciifiles/DRt0card_t0_iter1_v3.drt0card
#  source_format sel DRt0hvcardFileSourceFormat
#  file in /cdat/sol406/disk1/drcal/main/version2_118193/asciifiles/DRt0hvcard_t0_iter1_v3.drt0hvcard
#  source_format sel DRt0channelv2FileSourceFormat
#  file in /cdat/sol406/disk1/drcal/main/version2_118193/asciifiles/DRt0channelv2_t0_iter1_v3.drt0channelv2
#  # source_format sel DRDriftDataFileSourceFormat
#  # file in /cdat/sol406/disk1/drcal/main/version2_109759/asciifiles/DRDriftData_dr_all_iter1_v7.drdriftdata
#  source_format sel DRDriftDatav2FileSourceFormat
#  file in /cdat/sol406/disk1/drcal/main/version2_118193/asciifiles/DRDriftDatav2_dr_all_iter1weights052201_v1.drdriftdatav2

source_format sel DRDriftDatav2FileSourceFormat
file in /cdat/sol406/disk1/drcal/main/version2_113239/asciifiles/DRDriftDatav2_dr_all_version15chisq_v1.drdriftdatav2

# This is done later down
#  emptysource def CcAlignmentSource  ccalignment
#  prod sel CcGeomProd
#  prod sel C3ccProd
#  prod sel C3ccConProd
#  prod sel C3ccReconProd
#  prod sel CcfcReconProd
#  proc sel CcEventSelectProc
#  param CcEventSelectProc MuPairKeep true
#  # param CcEventSelectProc BhabhaKeep true

# # New wedding cake alignment constants
# source_format sel DRGeomAlignmentFileSourceFormat
# file in /cdat/sol406/disk1/drcal/test_sag120_x_42.drgeomalignment
# source_format sel DRt0globalFileSourceFormat
# file in /cdat/sol406/disk1/drcal/main/version2_118193/asciifiles/DRt0global_t0_iter5_v1_min0ns.drt0global
# source_format sel DRDriftDatav2FileSourceFormat
# file in /cdat/sol406/disk1/drcal/main/version2_118193/asciifiles/DRDriftDatav2_dr_all_sag_iter3_v3.drdriftdatav2

# Explicitly set fitting weights
# constants in $env(C3_CONST) meta Offline exclude_streams drweightlayer drweighttheta drweightdrift
source_format sel DRWeightLayerFileSourceFormat
file in iter$env(USER_PREVIOUS_ITERATION).drweightlayer
source_format sel DRWeightThetaFileSourceFormat        
file in iter$env(USER_PREVIOUS_ITERATION).drweighttheta
source_format sel DRWeightDriftFileSourceFormat
file in iter$env(USER_PREVIOUS_ITERATION).drweightdrift
# source_format sel DRWeightPhiFileSourceFormat  
# file in iter$env(USER_PREVIOUS_ITERATION).drweightphi

# Load tracking producers

###########################################################################
### From $env(C3_SCRIPTS)/trackingDataFull.tcl 6/4/01, changed CDOffCal ###
###########################################################################

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

# Make CalibratedDRHits-----
prod sel CalibratedDRHitProd

# Make Si Hits-----
prod sel SiHitsProd

# Load the magnetic field----
prod sel MagFieldProd
param MagFieldProd alwaysUseDefault true

# CDOffCal
prod sel CDOffCal

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

# Load Kalman fit (don't use Si yet)
prod sel KalmanProd

# Set up NavTrack
prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

###########################################################################

param DoitProd UseSilicon false

param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false
param KalmanProd UseDRCathode false
param KalmanProd MaxHitsToDelete 0
# param KalmanProd PrintDiagnostics true

# prod sel ChisqFitProd
# param ChisqFitProd UseSVRPhi false
# param ChisqFitProd UseSVZ false
# param ChisqFitProd UseDRCathode false

# Load ccfc event selection stuff
run_file $env(C3_SCRIPTS)/Ccfc.tcl
#  proc sel CcEventSelectProc
#  param CcEventSelectProc MuPairKeep true
#  # param CcEventSelectProc BhabhaKeep true

# Load my processor
proc sel $env(USER_DAF)/build_fittingweights/shlib/MakeFittingWeights
param MakeFittingWeights avoidLattice true
param MakeFittingWeights gatherdata true
# param MakeFittingWeights edriftbound 0.1E-9
# param MakeFittingWeights edriftbound 0.2E-8
param MakeFittingWeights edriftbound 10000
param MakeFittingWeights process false
param MakeFittingWeights filename $env(USER_DAF)/MakeFittingWeights/eleventhTry/hitdata$env(USER_ITERATION).txt
param MakeFittingWeights flyresid $env(USER_DAF)/MakeFittingWeights/eleventhTry/resid-cur.txt
param MakeFittingWeights flyd0miss $env(USER_DAF)/MakeFittingWeights/eleventhTry/d0miss-cur.txt
param MakeFittingWeights flyz0miss $env(USER_DAF)/MakeFittingWeights/eleventhTry/z0miss-cur.txt

# Get some data to work on
# source_format sel BinarySourceFormat
# source create FittingWeightsChain
# These will always be here:
# file add FittingWeightsChain /cdat/sol300/disk2/cleo3/data/r109266.bin beginrun event endrun startrun
# file add FittingWeightsChain /cdat/sol300/disk1/cleo3/data/r109565.bin beginrun event endrun startrun
# file add FittingWeightsChain /cdat/sol303/disk3/cleo3/data/r111319.bin beginrun event endrun startrun
# file add FittingWeightsChain /cdat/sol303/disk6/cleo3/data/r111786.bin beginrun event endrun startrun
# file add FittingWeightsChain /cdat/sol303/disk8/cleo3/data/r111887.bin beginrun event endrun startrun
# file add FittingWeightsChain /cdat/sol300/disk2/cleo3/data/r112162.bin beginrun event endrun startrun
# file add FittingWeightsChain  beginrun event endrun startrun
# source activate FittingWeightsChain event beginrun startrun endrun

prod sel DBEventHeaderProd
proc sel $env(USER_BUILD)/shlib/TrueMuProc

# prod sel CesrBeamEnergyProd

module sel DBModule
database in /nfs/cleo3/database/data8/db event startrun endrun
database read_collection rawdata mupair
database run 113243 113361
source activate db event startrun endrun

emptysource def beginrun beginrun
source activate beginrun beginrun

prod lss

# Do the processing
exception continueEventLoop on
report level DEBUG
go 10
report level NOTICE
go

exit

