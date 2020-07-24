module sel ../build_SunOS/shlib/HbookHistogramModule
hbook file align3.rzn
hbook init

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) standard CLEO3default streams basegeometry drdaqcellidmap drwirequality drvarious drgarfield_distancecorrection drgarfield_timecorrection drweight2layerdriftentang drweight2layertheta drweight2layerphi dradc2charge dradcpedestal drt0card drt0hvcard drt0channelv2 drtdc2time drfbuschannel drelecmap dradcsaturation drcapedestal drcagain superconductingquad magfudge drdriftdatav2 drt0global drt0crate
source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc material
module sel RunStatisticsSourceModule
runstatistics in

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

source_format sel PDSSourceFormat
source create pdschain
file add pdschain ../pds/better.pds event

module sel ../build_SunOS/shlib/DRAlignmentMod
param DRAlignmentMod geom cleo3.drgeomalignment

report level WARNING
iterator go
