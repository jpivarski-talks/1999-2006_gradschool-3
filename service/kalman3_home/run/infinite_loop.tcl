default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/infinite_loop.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
# run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
        prod sel DetectorConfigurationProd
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
	prod sel CDOffCal
	prod sel TriggerL1DataProd
	prod sel TriggerPhaseForDataProd
	prod sel TrackletBunchFinder
	prod sel DoitProd
	param DoitProd ContinueIfHitsMissing true
	prod sel DRHitCorrectorProd
        param DRHitCorrectorProd ApplyEntranceAngleCorr false           
        param DRHitCorrectorProd ApplyStandardCorrections false   

	prod sel ../build_SunOS/shlib/KalmanProd.so
	param KalmanProd HyposToFit pion kaon proton
	param KalmanProd OptimizeForAllHypos true
        param KalmanProd HitListFromOneHypo true
	param KalmanProd ScaleCathodeResolution true 
	param KalmanProd CathodeResolutionScale 0.7     

	prod sel ElectronFromPionFitProd
	prod sel MuonFromPionFitProd
	prod sel NavigationProd
	prod sel TrackDeliveryProd
	prod sel SeedToTRTrackProd

param KalmanProd PrintDiagnostics true

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/AskForTracks.so

module sel DBModule
database in /nfs/cleo3/database/data19/db event startrun beginrun endrun
database read_collection rawdata
database run 124688

goto 124688 103540
report level DEBUG
go 1
