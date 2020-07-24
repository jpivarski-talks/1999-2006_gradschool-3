default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/getConstants.tcl

run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel ElectronFromPionFitProd
prod desel MuonFromPionFitProd
param KalmanProd OptimizeForAllHypos false

prod sel CesrBeamEnergyProd

# override producers in trackingDataFull.tcl
prod sel ChisqFitProd
#param ChisqFitProd ContinueIfAnyException true

prod sel DBEventHeaderProd

prod sel CcfcReconProd
prod sel C3ccReconProd
prod sel C3ccProd
prod sel CcGeomProd
prod sel C3ccConProd
proc sel CcEventSelectProc
param CcEventSelectProc BhabhaKeep true
#param CcEventSelectProc MuPairKeep true
#param CcEventSelectProc HadronKeep true

module sel DBModule
database in /nfs/cleo3/database/data16/db beginrun event startrun endrun
source activate db beginrun event startrun endrun
database read_collection rawdata bhabha

sink_format sel PDSSinkFormat
file sink ../dafedir/data16.pds event beginrun startrun endrun

report level INFO
go 10000
exit
