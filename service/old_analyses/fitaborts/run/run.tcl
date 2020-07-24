default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/getConstants.tcl

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

proc sel ../build_SunOS/shlib/FindFitAborts

module sel DBModule
database in /nfs/cleo3/database/data8/db beginrun event startrun endrun
source activate db beginrun event startrun endrun
database read_collection rawdata bhabha
database run 113239 0

report level NOTICE
go 100
