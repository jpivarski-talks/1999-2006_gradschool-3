default prompt off

proc sel ../build_SunOS/shlib/HistogramViewerProc

# module sel ../build_SunOS/shlib/LoadHbook4MModule
module sel ../build_SunOS/shlib/HbookHistogramModule
hbook file test1.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams drweightlayer drweightdrift drweighttheta
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

prod sel ADRGeomProd
prod sel DRGeom
prod sel ASiStorePro
prod sel SiGeom
prod sel IRGeomProd
prod sel RawDataProd
prod sel DRCACorrectHitProd
prod sel DRCAClusterProd
prod sel CalibratedDRHitProd
prod sel SiHitsProd
prod sel CDOffCal
prod sel TriggerL1DataProd
prod sel TrackletBunchFinder
prod sel DRHitCorrectorProd

prod sel MagFieldProd
param MagFieldProd alwaysUseDefault true

prod sel DoitProd
param DoitProd UseSilicon false
param DoitProd ContinueIfHitsMissing true

prod sel KalmanProd
param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false
param KalmanProd UseDRCathode false
param KalmanProd MaxHitsToDelete 0

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

#  prod sel DBEventHeaderProd
#  proc sel $env(HOME)/truemu/build_SunOS/shlib/TrueMuProc

#  module sel DBModule
#  database in /nfs/cleo3/database/data8/db beginrun event startrun endrun
#  source activate db beginrun event startrun endrun
#  database read_collection rawdata bhabha
#  database run 113243 113361

source_format sel BinarySourceFormat
source create rawChain
file add rawChain /home/sialign/miniPass2Data/rawdata_113239-113347.bin event
file add rawChain /home/sialign/miniPass2Data/rawdata_113347-113455.bin event
file add rawChain /home/sialign/miniPass2Data/rawdata_113455-113560.bin event
file add rawChain /home/sialign/miniPass2Data/rawdata_113560-113668.bin event
file add rawChain /home/sialign/miniPass2Data/rawdata_113668-113721.bin event
source activate rawChain event

emptysource def idontcare beginrun startrun endrun
emptysource def fakeweights drweightlayer drweightdrift drweighttheta

module sel ../build_SunOS/shlib/DRFittingWeights.so
param DRFittingWeights layer_init unmoved.drweightlayer
param DRFittingWeights drift_init unmoved.drweightdrift
param DRFittingWeights theta_init unmoved.drweighttheta
param DRFittingWeights target_hits 1000
param DRFittingWeights iterations 3
param DRFittingWeights particle muon
param DRFittingWeights kumac check_fits.kumac

iterator go
