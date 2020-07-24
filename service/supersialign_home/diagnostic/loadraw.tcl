run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod sel ChisqFitProd

prod sel DualTrackHelicesProd

prod sel DBRunHeaderProd
prod sel DBEventHeaderProd

module sel DBModule
database in /nfs/cleo3/database/data16/db beginrun event startrun endrun
source activate db beginrun event startrun endrun
database read_collection rawdata bhabha

sink_format sel PDSSinkFormat
file sink $pdsfile event beginrun startrun endrun
