module sel HbookHistogramModule
hbook file tmp.rzn
hbook init

default prompt off
# exception continueEventLoop on

run_file $env(C3_SCRIPTS)/getConstants.tcl

source_format sel SVHybridEfficiencyFileSourceFormat
file in $env(HOME)/weddingcake_alignment/run/normal.svhybridefficiency

run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod sel DRElecMapProd
prod sel TriggerL1DataProd
prod sel CcGeomProd
prod sel C3ccProd
prod sel C3ccConProd
prod sel C3ccReconProd
prod sel CcfcReconProd

prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd

module sel DBModule
database in /nfs/cleo3/database/data17/db beginrun event startrun endrun
source activate db beginrun event startrun endrun
database read_collection rawdata bhabha
# database run 121339

proc sel ../build_SunOS/shlib/P2TrackMonProc
param P2TrackMonProc MonitorBFieldCorrection true

# proc sel HistogramViewerProc

report level DEBUG
go 1000
exit
