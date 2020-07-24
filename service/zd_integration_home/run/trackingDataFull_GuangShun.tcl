#######################################################
# file: $C3_SCRIPTS/trackingDataFull.tcl
# 
# purpose: Standard tracking script
#
#######################################################
# This is the "standard" script for tracking. It will be updated 
# with the latest and greatest tracking improvements as they become 
# available. Please see the "Cleo III Tracking Status" web page,
# available from the Cleo III Software web page for the lastest
# tracking news. Updates to this script will be detailed there.
#
# This script now only loads tracking packages. No constants, no event 
#  selection, no data files. See sampleTrackingOnData.tcl for 
#  a full script that calls this one.
#
# Load this script with...
#
#    Suez> run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
#
# This script will *not* work on Monte Carlo. See trackingMCFull.tcl
#    for a script that will work on MC.
#
#
# DISCLAIMER: You will get best results for runs that are calibrated
# (Global T0, Layer/HV-Section/Channel T0, Drift functions). 
#        
#   This script does full tracking (Doit pattern recognition and Kalman
#   fitting) and makes NavTracks. Cathodes are included in the fit, 
#   but Silicon is not (due to alignment problems).
#
#  Questions should go to Adam Lyon (lyon@mail.lns.cornell.edu)
#
# $Id: trackingDataFull.tcl,v 1.19 2002/09/27 20:41:18 bkh Exp $
# $Log: trackingDataFull.tcl,v $
# Revision 1.19  2002/09/27 20:41:18  bkh
# Add DetectorConfigurationProd to cleog and tracking to prepare for ZD era
#
# Revision 1.18  2002/04/04 15:40:46  bkh
# Alter for accomodation of trigger phase producers
#
# Revision 1.17  2001/11/29 22:48:22  bkh
# Eliminate the bad word I probably cannot spell out here but
# backwards is "rorre" because it caused the test script to crash
#
# Revision 1.16  2001/11/27 14:26:07  bkh
# Add PreliminaryDRHitProd and CorrectedSVStripProd to trackingDataFull.tcl
# Make MC tracking tcl file driven off of data tracking tcl file
#
# Revision 1.15  2001/10/12 11:31:51  cdj
# no longer set parameters for DRHitCorrectorProd BEFORE the producer is loaded
#
# Revision 1.14  2001/10/10 17:41:28  jed
#  turn off entrance angle corrections in DRHitCorrectorProd
#
# Revision 1.13  2001/10/03 15:22:37  jed
#  Scale Cathode Resolution by 0.7 in KalmanProd as per hennessy
#
# Revision 1.12  2001/10/02 22:14:15  jed
#  minod comments mods, add in KalmanProd HitListFromOneHypo true parameter
#
# Revision 1.11  2001/09/17 15:09:55  cdj
# Added Prod to give Electron and Muon from the Pion fits
#
# Revision 1.10  2001/05/08 18:49:35  lyon
# trackingDataFull: remove explicit CDOffCal parameters (they are VERY BAD) and turn on Si by default
#
# Revision 1.9  2001/03/29 17:46:25  ajm36
# clean out old scripts, get rid of drcalibration emptysources
#
# Revision 1.8  2000/11/16 03:13:01  lkg
# Changed MagField to MagFieldProd
#
# Revision 1.7  2000/11/01 15:27:58  lyon
# Removed prod sel CESR -- no longer needed
#
# Revision 1.6  2000/07/25 13:53:38  lyon
# TriggerDataProd -> TriggerL1DaqtaProd
#
# Revision 1.5  2000/06/22 15:06:27  lyon
# Restructured tracking scripts
#
# Revision 1.4  2000/06/15 15:03:50  lyon
# Added DRHitCorrectorProd
#
# Revision 1.3  2000/06/13 18:41:39  lyon
# Removed parameters for TrackletBunchFinder (default are now valid for running on data. And added source format for DRCAPedestal constants
#
# Revision 1.2  2000/05/30 22:46:19  lyon
# Changed CalibratedCathodeHitProd -> DRCAClusterProd
#
#
#
if { ! [info exists trackingDataFull_tcl_loaded] } {
# keep script from multiply loading
    set trackingDataFull_tcl_loaded 1

# Handle problems gracefully
    if [ catch { 

# load configuration
        prod sel DetectorConfigurationProd

#        param DetectorConfiguationProd Override   true
#        param DetectorConfiguationProd UseZDnotSI true

 
# Load DR Geometry-------
	prod sel ADRGeomProd
	prod sel DRGeom

# Load ZD geometry
        prod sel AZDGeomProd
        prod sel ZDGeomProd


# Load Silicon and IR geometry-------
	prod sel ASiStorePro
	prod sel SiGeom
	prod sel IRGeomProd

# Load Raw data---
	prod sel RawDataProd

# Convert Raw cathode hits to DRCACorrectHits----
	prod sel DRCACorrectHitProd

# Cluster cathodes---
	prod sel DRCAClusterProd

# Make PreliminaryDRHits-----
	prod sel PreliminaryDRHitProd

# Make CalibratedDRHits-----
	prod sel CalibratedDRHitProd

# hgs: (not check in) Make PreliminaryZDHits-----
#  no! PreliminaryDRHitProd was a DR specific thing for tracklet bunch finder 
#	prod sel /home/axp/huanggs/cleo3src/shlib/PreliminaryZDHitProd

# hgs: (in lib) Make CalibratedZDHits-----
	prod sel CalibratedZDHitProd

# Make Si Hits-----
#	prod sel CorrectedSVStripProd
#	prod sel SiHitsProd

# Load the magnetic field----
	prod sel MagFieldProd

# CDOffCal
	prod sel CDOffCal

# hgs: ZDDriftFunctionProd
	prod sel ZDDriftFunctionProd

# Get trigger information
	prod sel TriggerL1DataProd
	prod sel TriggerPhaseForDataProd

# Get bunch decision
	prod sel TrackletBunchFinder

# Load Doit
#	prod sel DoitProd
#	param DoitProd ContinueIfHitsMissing true

# Load DRHitCorrections
	prod sel DRHitCorrectorProd
	 param DRHitCorrectorProd ApplyEntranceAngleCorr false           
         param DRHitCorrectorProd ApplyStandardCorrections false   

# hgs: (not check in) Load ZDHitCorrections
	prod sel /home/axp/huanggs/cleo3src/shlib/ZDHitCorrectorProd
	 param ZDHitCorrectorProd ApplyEntranceAngleCorr false           
         param ZDHitCorrectorProd ApplyStandardCorrections false   

# Load Kalman fit
	prod sel KalmanProd
# Set Kalman to only fit pion kaon and proton since that is all
#  we want store from Pass2
	param KalmanProd HyposToFit pion kaon proton
	param KalmanProd OptimizeForAllHypos true
        param KalmanProd HitListFromOneHypo true
	param KalmanProd ScaleCathodeResolution true 
	param KalmanProd CathodeResolutionScale 0.7     

# If someone requests electrons or muons, give them the pion fit
	prod sel ElectronFromPionFitProd
	prod sel MuonFromPionFitProd

# Set up NavTrack
	prod sel NavigationProd
	prod sel TrackDeliveryProd
	prod sel SeedToTRTrackProd


    } resultString ] { # ------------------- tcl try-catch block
	puts stdout "loading failed for a tracking Producer:"
	puts stdout "resultString:"
	puts stderr "$resultString"
    } else {
	puts stdout "Tracking producers loaded successfully"
    }
    
}
