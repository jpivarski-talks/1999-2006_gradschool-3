default prompt off
exception continueEventLoop on

module sel LoadHbook4MModule
module sel HbookHistogramModule
hbook file ../dafedir/$env(USER_HISTOGRAM_FILE)
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams svalignment
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

source_format sel SVAlignmentGlobalFileSourceFormat
file in $env(HOME)/sialignment/run/nov_24.svalignmentglobal
source_format sel SVAlignmentLayerFileSourceFormat
file in $env(HOME)/sialignment/run/nov_24.svalignmentlayer
source_format sel SVAlignmentLadderFileSourceFormat
file in $env(HOME)/sialignment/run/nov_24.svalignmentladder
source_format sel SVAlignmentSensorFileSourceFormat
file in $env(HOME)/sialignment/run/nov_24.svalignmentsensor

source_format sel SVResolutionGlobalFileSourceFormat
file in 

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
param DoitProd UseSilicon true
param DoitProd ContinueIfHitsMissing true

prod sel $env(USER_FITTER)
param $env(USER_FITTER) UseSVRPhi true
param $env(USER_FITTER) UseSVZ true
param $env(USER_FITTER) UseDRCathode false

# prod sel NavigationProd
# prod sel TrackDeliveryProd
# prod sel SeedToTRTrackProd

prod sel DBEventHeaderProd

module sel DBModule
database in /nfs/cleo3/database/data7/db event
source activate db event
database read_collection rawdata mupair

#  source_format sel BinarySourceFormat
#  source create rawChain
#  file add rawChain /home/sialign/miniPass2Data/rawdata_113239-113347.bin event
#  file add rawChain /home/sialign/miniPass2Data/rawdata_113347-113455.bin event
#  file add rawChain /home/sialign/miniPass2Data/rawdata_113455-113560.bin event
#  file add rawChain /home/sialign/miniPass2Data/rawdata_113560-113668.bin event
#  file add rawChain /home/sialign/miniPass2Data/rawdata_113668-113721.bin event
#  source activate rawChain event

emptysource def idontcare beginrun startrun endrun

# proc sel $env(HOME)/truemu/build_SunOS/shlib/TrueMuProc
proc sel ../build_SunOS/shlib/FindBug2
#  proc sel HistogramViewerProc

report level INFO
go 10
report level WARNING
go 50000

exit
