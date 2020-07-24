module sel ../build_SunOS/shlib/HbookHistogramModule
hbook file align.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) standard CLEO3default streams basegeometry drdaqcellidmap drwirequality drvarious drgarfield_distancecorrection drgarfield_timecorrection drweight2layerdriftentang drweight2layertheta drweight2layerphi dradc2charge dradcpedestal drt0card drt0hvcard drt0channelv2 drtdc2time drfbuschannel drelecmap dradcsaturation drcapedestal drcagain superconductingquad magfudge
source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc material
module sel RunStatisticsSourceModule
runstatistics in

source_format sel DRDriftDataFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRDriftDatav2v15.drdriftdatav2
source_format sel DRt0globalFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0global_v8.drt0global
source_format sel DRt0crateFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0crate_v8.drt0crate

emptysource def dralignment dralignment

prod sel ADRGeomProd
prod sel DRGeom
prod sel CDOffCal
prod sel MagFieldProd
prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel DetectorConfigurationProd
param DetectorConfigurationProd Override true;
param DetectorConfigurationProd UseZDnotSI true;
prod sel DRHitCorrectorProd
param DRHitCorrectorProd DisableAllCorrections false
param DRHitCorrectorProd ApplyStandardCorrections true
param DRHitCorrectorProd ApplyEntranceAngleCorr true
param DRHitCorrectorProd ApplyPulseHeightCorr true
param DRHitCorrectorProd ApplySignalPropagationCorr true
param DRHitCorrectorProd ApplyTimeOfFlightCorr true

# Include this to generate your own tracks (be sure to include FATable<CollatedRawDRHit> in your pds files!)
# prod sel CalibratedDRHitProd

source_format sel PDSSourceFormat
source create pdschain
file add pdschain ../pds/run200700.pds event
file add pdschain ../pds/run200709.pds event
file add pdschain ../pds/run200712.pds event
# file add pdschain ../pds/run200716.pds event
# file add pdschain ../pds/run200717.pds event
# file add pdschain ../pds/run200720.pds event
# file add pdschain ../pds/run200723.pds event
# file add pdschain ../pds/run200727.pds event
# file add pdschain ../pds/run200729.pds event
# file add pdschain ../pds/run200743.pds event
# file add pdschain ../pds/run200744.pds event
# file add pdschain ../pds/run200745.pds event
# file add pdschain ../pds/run200853.pds event
# file add pdschain ../pds/run200854.pds event
# file add pdschain ../pds/run200981.pds event
# file add pdschain ../pds/run200983.pds event
# file add pdschain ../pds/run200986.pds event
# file add pdschain ../pds/run200993.pds event
# file add pdschain ../pds/run201001.pds event
# file add pdschain ../pds/run201003.pds event
# file add pdschain ../pds/run201018.pds event
# file add pdschain ../pds/run201027.pds event
# file add pdschain ../pds/run201028.pds event
# file add pdschain ../pds/run201031.pds event
# file add pdschain ../pds/run201042.pds event
# file add pdschain ../pds/run201043.pds event
# file add pdschain ../pds/run201074.pds event
# file add pdschain ../pds/run201075.pds event
# file add pdschain ../pds/run201076.pds event
# file add pdschain ../pds/run201077.pds event
# file add pdschain ../pds/run201079.pds event
# file add pdschain ../pds/run201087.pds event
# file add pdschain ../pds/run201089.pds event
# file add pdschain ../pds/run201091.pds event
# file add pdschain ../pds/run201095.pds event
# file add pdschain ../pds/run201096.pds event
# file add pdschain ../pds/run201109.pds event
# file add pdschain ../pds/run201111.pds event
# file add pdschain ../pds/run201113.pds event
# file add pdschain ../pds/run201116.pds event
# file add pdschain ../pds/run201117.pds event
# file add pdschain ../pds/run201119.pds event
# file add pdschain ../pds/run201120.pds event
# file add pdschain ../pds/run201133.pds event
# file add pdschain ../pds/run201136.pds event
# file add pdschain ../pds/run201168.pds event
# file add pdschain ../pds/run201214.pds event
# file add pdschain ../pds/run201215.pds event
# file add pdschain ../pds/run201219.pds event
# file add pdschain ../pds/run201228.pds event
# file add pdschain ../pds/run201236.pds event
# file add pdschain ../pds/run201321.pds event
# file add pdschain ../pds/run201324.pds event

module sel ../build_SunOS/shlib/DRAlignmentMod
param DRAlignmentMod geom cleo3.drgeomalignment

report level WARNING
iterator go
