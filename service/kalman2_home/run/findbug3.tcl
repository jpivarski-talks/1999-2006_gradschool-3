default prompt off
exception continueEventLoop on
module sel HbookHistogramModule
hbook file tmp3osf.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
	param KalmanProd HyposToFit pion
	param KalmanProd OptimizeForAllHypos true
        param KalmanProd HitListFromOneHypo true
	param KalmanProd ScaleCathodeResolution true 
	param KalmanProd CathodeResolutionScale 0.7     
#        param KalmanProd PrintDiagnostics true
        param KalmanProd UseDRCathode false

prod sel DBEventHeaderProd
proc sel ../build_OSF1/shlib/PointToError

module sel DBModule
database in /nfs/objy/data10b/db event startrun beginrun endrun
database read_collection rawdata hadron

report level ERROR
go 1000
exit
