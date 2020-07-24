default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/mc.rzn
hbook init

run_file $env(C3_SCRIPTS)/getMCNewestConstants.tcl
run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel ASiStorePro
prod sel SiGeom
prod sel DBEventHeaderProd
prod sel MagFieldProd

proc sel ../build_SunOS/shlib/DRWiggle
param DRWiggle trackUse "pion"
param DRWiggle crossingAngle 0.
param DRWiggle getDuals false
param DRWiggle getResiduals false

report level DEBUG

source_format sel PDSSourceFormat
source create pdschain

# file add pdschain /cdat/sol221/disk3/c3mc/data7/mumu/output/mupair-cleo3_MCPS2_Jan30_2002-MN-1-R112802-E3387-pass2.pds event
# file add pdschain /cdat/sol221/disk3/c3mc/data7/mumu/output/mupair-cleo3_MCPS2_Jan30_2002-MN-1-R112803-E5539-pass2.pds event
# file add pdschain /cdat/sol221/disk3/c3mc/data7/mumu/output/mupair-cleo3_MCPS2_Jan30_2002-MN-1-R112804-E4832-pass2.pds event
# file add pdschain /cdat/sol221/disk3/c3mc/data7/mumu/output/mupair-cleo3_MCPS2_Jan30_2002-MN-1-R112805-E5269-pass2.pds event
# file add pdschain /cdat/sol221/disk3/c3mc/data7/mumu/output/mupair-cleo3_MCPS2_Jan30_2002-MN-1-R112806-E4977-pass2.pds event

file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121474-E5000-DTS15102002_101141-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121476-E5000-DTS15102002_101153-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121477-E5000-DTS15102002_101205-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121479-E5000-DTS15102002_101217-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121480-E5000-DTS15102002_101229-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121519-E5000-DTS15102002_101244-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121523-E5000-DTS15102002_101258-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121525-E5000-DTS15102002_101311-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121526-E5000-DTS15102002_101324-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121527-E5000-DTS15102002_101337-pass2.pds event

file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122274-E5000-DTS15102002_091613-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122275-E5000-DTS15102002_091624-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122276-E5000-DTS15102002_091637-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122277-E5000-DTS15102002_091648-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122278-E5000-DTS15102002_091700-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122279-E5000-DTS15102002_091712-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122280-E5000-DTS15102002_091725-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122281-E5000-DTS15102002_091739-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122283-E5000-DTS15102002_091752-pass2.pds event
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122285-E5000-DTS15102002_091804-pass2.pds event

source act pdschain event
go

hbook terminate
exit
