default prompt off
exception continueEventLoop on
module sel HbookHistogramModule
hbook file tmp4.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
# run_file $env(C3_SCRIPTS)/trackingDataFull.tcl


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
        prod sel ElectronFromPionFitProd
        prod sel MuonFromPionFitProd
        prod sel NavigationProd
        prod sel TrackDeliveryProd
        prod sel SeedToTRTrackProd

	prod sel /cdat/dafe/mccann/kalman2/makecvschanges/shlib/KalmanProd.so
	param KalmanProd HyposToFit pion
	param KalmanProd OptimizeForAllHypos true
        param KalmanProd HitListFromOneHypo true
	param KalmanProd ScaleCathodeResolution true 
	param KalmanProd CathodeResolutionScale 0.7     
#        param KalmanProd PrintDiagnostics true
        param KalmanProd UseDRCathode false


prod sel DBEventHeaderProd
prod sel DetectorConfigurationProd 
proc sel ../build_SunOS/shlib/PointToError

# run_file /nfs/cleo3/Offline/rel/current/src/SuezScripts/viewTrackFitter.tcl

module sel DBModule
database in /nfs/objy/data10b/db event startrun beginrun endrun
database read_collection rawdata hadron

report level NOTICE

go 1000
exit

