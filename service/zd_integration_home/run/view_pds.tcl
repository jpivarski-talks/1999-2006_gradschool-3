#==============================================================
# file: import from viewTrackFinder.tcl
#==============================================================
# geometry
prod sel ../build_OSF1/shlib/DetectorConfigurationProd
	param DetectorConfigurationProd        Override      true;
	param DetectorConfigurationProd        UseZDnotSI    true;
proc sel SpExtractStandAloneGeomProc
#proc sel SpExtractDetectorStoreProc

prod sel DRGeom
prod sel ADRGeomProd

# MC Particles
# proc sel SpExtractMCDecayTreeProc
# MC Response
proc sel ../build_OSF1/shlib/SpExtractMCZDResponseProc
proc sel SpExtractMCDRResponseProc
# hits
proc sel ../build_OSF1/shlib/SpExtractZDHitsProc
proc sel SpExtractDRHitsProc
#proc sel SpExtractSeedTrackProc
#proc sel SpExtractSeedTrackDRHitsLinkProc
#proc sel SpExtractSeedTrackMCParticleLinkProc
#proc sel SpExtractKinePionFitProc
proc sel SpViewerProc

# constants
tcl_source $env(C3_SCRIPTS)/getNewestConstants.tcl

source_format sel ../build_OSF1/shlib/AZDGeomLayerFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayerFileSourceFormat/zd.azdgeomlayer
source_format sel ../build_OSF1/shlib/AZDGeomLayersEndsFileSourceFormat
file in $env(C3_CVSSRC)/AZDGeomLayersEndsFileSourceFormat/zd.azdgeomlayersends

# ZD producers
prod sel ../build_OSF1/shlib/AZDGeomProd

source_format sel ../build_OSF1/shlib/ZDGeomAlignmentFileSourceFormat
file in $env(C3_CVSSRC)/ZDGeomAlignmentFileSourceFormat/zd.zdgeomalignment
prod sel ../build_OSF1/shlib/ZDGeomProd

# input file
source_format sel PDSSourceFormat
file in ../pds/single.0.pds event

report level DEBUG
go
