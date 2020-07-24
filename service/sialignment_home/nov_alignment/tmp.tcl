default prompt off

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams svalignment svresolution
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

source_format sel SVResolutionGlobalFileSourceFormat
file in data8_loose_weights.svresolutionglobal
source_format sel ../build_SunOS/shlib/SVResolutionLayerFileSourceFormat
file in data8_loose_weights.svresolutionlayer

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
param DoitProd ContinueIfHitsMissing true

prod sel ChisqFitProd
param ChisqFitProd UseSVRPhi true
param ChisqFitProd UseSVZ true
param ChisqFitProd UseDRCathode false

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

prod sel ../build_SunOS/shlib/DualTrackHelicesProd_g.so

module sel DBModule
database in /nfs/cleo3/database/data7/db beginrun event startrun endrun
source activate db beginrun event startrun endrun
database read_collection rawdata mupair

emptysource def svalignment svalignment

module sel ../build_SunOS/shlib/EnderSiAlignmentMod_g.so
param EnderSiAlignmentMod_g target_hits 100000

param EnderSiAlignmentMod_g global_init data8_repass2.svalignmentglobal
param EnderSiAlignmentMod_g layer_init  data8_repass2.svalignmentlayer
param EnderSiAlignmentMod_g ladder_init data8_repass2.svalignmentladder
param EnderSiAlignmentMod_g sensor_init data8_repass2.svalignmentsensor

param EnderSiAlignmentMod_g file_output "global-data7.log"

param EnderSiAlignmentMod_g device "global"
param EnderSiAlignmentMod_g number 1

param EnderSiAlignmentMod_g rphi_cut 0.00012
param EnderSiAlignmentMod_g z_cut 0.0011

param EnderSiAlignmentMod_g rphi_norm 0.0013
param EnderSiAlignmentMod_g z_norm 0.0053

param EnderSiAlignmentMod_g deltaX_step 0.000050
param EnderSiAlignmentMod_g deltaY_step 0.000050
param EnderSiAlignmentMod_g deltaZ_step 0.002500
param EnderSiAlignmentMod_g phiX_step 0.002000
param EnderSiAlignmentMod_g phiY_step 0.005000
param EnderSiAlignmentMod_g phiZ_step 0.001000

report level DEBUG
iterator go
