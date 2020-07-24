// -*- C++ -*-
//
// Package:     <ZDDriftFunctionProd>
// Module:      ZDDriftFunction
// 
// Description: This is the root class for ZDDriftFunction classes
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDDriftFunction.cc,v 1.4 2003/10/13 16:47:45 mccann Exp $
//
// Revision history
//
// For the ZD, we have only one version ZDt0channel, and this is an
// equivalent of DRt0channelv2. We don't need ZDtdc2time, since the
// slope is same for all TDC channels.
//
// $Log: ZDDriftFunction.cc,v $
// Revision 1.4  2003/10/13 16:47:45  mccann
// Linux g++ doesn't like 'double variable(value)' initializers--- I switched to equal signs
//
// Revision 1.3  2003/10/10 18:09:00  mccann
// fixed syntax errors introduced by my code merge
//
// Revision 1.2  2003/10/10 17:36:39  mccann
// Totally vamped the fittingweights representation and delivery to use the new ZDWeight2* constants
//
// Revision 1.1.1.1  2003/06/26 19:02:50  huanggs
// imported ZDDriftFunctionProd source
//
// Revision 1.1  2003/04/24 18:08:10  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:50  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//

// compiler bug flags
#include "Experiment/Experiment.h"

// system include files
#include "C++Std/fstream.h"
#include "C++Std/iostream.h"

// compiler bug fixes 
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif  
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ZDDriftFunctionProd/ZDDriftFunctionProd.h"
#include "ZDDriftFunctionProd/ZDDriftFunction.h"
#include "ZDDriftFunctionProd/ZDDriftFunctionProxy.h"
#include "ZDDriftFunctionProd/ZDHitQuality.h"
#include "ZDDriftFunctionProd/ZDStatistics.h"
#include "CalibratedData/Tseries.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "AZDGeom/AZDSenseWireStore.h"

#include <vector>
#include <map>
#include <set>

static const char* const kFacilityString   = "ZDDriftFunctionProd.ZDDriftFunction" ;
const double ZDDriftFunction::kdefault_MinTightTime =  -30000. ;
const double ZDDriftFunction::kdefault_MaxTightTime =  350000. ;
const double ZDDriftFunction::kdefault_MinLooseTime =  -50000. ;
const double ZDDriftFunction::kdefault_MaxLooseTime =  600000. ;
const double ZDDriftFunction::kdefault_GlobalT0     =       0. ;
const double ZDDriftFunction::kdefault_TDCQuantum   =    -500. ;
const double ZDDriftFunction::kdefault_Resolution   = 0.000123 ; // 1/123microns**2
const double ZDDriftFunction::kdefault_SignalProp   = 0.5      ; 
const double ZDDriftFunction::kclight        = 3.0e-4      ;
const UInt16 ZDDriftFunction::kHighTDC       = 16384       ; 

//----------------------------------------------------------------------------
//
ZDDriftFunction::ZDDriftFunction(ZDDriftFunctionProd* ZDDriftFunctionProd,
			     ZDDFDataHolder* iDataHolder) :
   m_dataHolder( iDataHolder ),
   m_ZDDriftFunctionProd( ZDDriftFunctionProd )
{
  report( DEBUG, kFacilityString )  << "Constructor Starts " << endl;
  //-----------------------------------------------------------------------------------------------------------
  //
  // These are now used as function calls to the producer,
  // this allows parameters to be changed "on the fly"
  // 
  //m_Mintime    = ZDDriftFunctionProd->getMinimumTime();
  //m_Maxtime    = ZDDriftFunctionProd->getMaximumTime();
  //m_TDCQuantum = ZDDriftFunctionProd->getTDCQuantum();
  //m_GlobalT0   = ZDDriftFunctionProd->getGlobalT0();

  //-----------------------------------------------------------------------------------------------------------
  //
  // This enables the set of statistics counters
  // these have been mostly commented out because
  // the code is slow because of the way it 
  // handles the character arguments
  //
  m_statistics = new ZDStatistics;
  m_statistics->counter(0,"ZDDriftFunctionProd Calibration Statistics"); 

  //-----------------------------------------------------------------------------------------------------------
  //
  // Oldstyle drift functions (NaiveDriftFunction and
  // HePrDriftFunction) are obsolete and deleted.
  //
  if ( m_ZDDriftFunctionProd->getType() < ZDDriftFunction::kDFType_HePr_data1 ) 
    { 
      report( EMERGENCY, kFacilityString )  << "Invalid DriftFunctionType Parameter: " 
					    <<  m_ZDDriftFunctionProd->getType() 
					    << endl;
      assert( false );
      ::exit( 1 ); 
    }
  //-----------------------------------------------------------------------------------------------------------
  // rather than have many dual function calls, parse this
  // to local member data
  for(int level =  0 ;
          level < 20 ;
        ++level      )
    {
      debugLevel[level] = ( m_ZDDriftFunctionProd->getDebugLevel() == -level || 
	                    m_ZDDriftFunctionProd->getDebugLevel() >=  level  ) ;
    }
  //-----------------------------------------------------------------------------------------------------------
  // Unpack Fitting Weights 
  //
  report( DEBUG, kFacilityString )  << "Trying to get Fitting Weights  " << endl;
  report( INFO , kFacilityString )  << "Global scale (user parameter): " << m_ZDDriftFunctionProd->getResolution() << endl;
  //

     const float kFavoriteNumber = -2648.0126;

     // The main ZD weight constants type is now ZDWeight2LayerDriftEntAng
     // Information in "layer zero" is formatted this way:
     //     layer    driftBin  EntAngBin  Resolution
     //       0       1            0         0.0 (min absDrift value)
     //       0      15 (max)      0         1.5 (max absDrift value)
     //       0       0          1          -1.0 (min sinalpha value)
     //       0       0         20 (max)     1.0 (max sinalpha value)

     // It is a show-stopping error to include bins outside of the
     // defined range, to include a bin more than once, to not
     // completely fill the defined range, or to not completely fill
     // all layers.

     // First loop to get layer 0 information
     DABoolean gotDriftMin, gotDriftMax, gotEntAngMin, gotEntAngMax;
     gotDriftMin = false;  gotDriftMax = false;  gotEntAngMin = false;  gotEntAngMax = false;
     FANonSeqConstants<DBZDWeight2LayerDriftEntAng>::const_iterator lde_iter = m_dataHolder->LayerDriftEntAngWeight().begin();
     FANonSeqConstants<DBZDWeight2LayerDriftEntAng>::const_iterator lde_end = m_dataHolder->LayerDriftEntAngWeight().end();

     for (;  lde_iter != lde_end;  ++lde_iter) {
	if (lde_iter->get_layer() == 0) {
	   if (lde_iter->get_entangbin() == 0) {

	      // Set min and max drift value and number of drift bins
	      if (lde_iter->get_driftbin() == 1) {
		 if (!gotDriftMin) {
		    m_WeightDriftMin = lde_iter->get_resolution();
		    gotDriftMin = true;
		 }
		 else {
		    report(EMERGENCY, kFacilityString)
		       << "Illegal line in ZDWeight2LayerDriftEntAng: layer = " << lde_iter->get_layer()
		       << " driftbin = " << lde_iter->get_driftbin()
		       << " entangbin = " << lde_iter->get_entangbin()
		       << " resolution = " << lde_iter->get_resolution()
		       << " (drift min has already been set!)" << endl;
		    assert(false);
		 }
	      }
	      else {
		 if (!gotDriftMax) {
		    m_WeightDriftNBins = lde_iter->get_driftbin();
		    m_WeightDriftMax = lde_iter->get_resolution();
		    gotDriftMax = true;
		 }
		 else {
		    report(EMERGENCY, kFacilityString)
		       << "Illegal line in ZDWeight2LayerDriftEntAng: layer = " << lde_iter->get_layer()
		       << " driftbin = " << lde_iter->get_driftbin()
		       << " entangbin = " << lde_iter->get_entangbin()
		       << " resolution = " << lde_iter->get_resolution()
		       << " (drift max has already been set!)" << endl;
		    assert(false);
		 }
	      }

	   } 
	   else { // (entangbin is NOT zero)

	      // Possible error condition: layer == 0, and both entangbin and driftbin are nonzero
	      if (lde_iter->get_driftbin() != 0) {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerDriftEntAng: layer = " << lde_iter->get_layer()
		       << " driftbin = " << lde_iter->get_driftbin()
		       << " entangbin = " << lde_iter->get_entangbin()
		       << " resolution = " << lde_iter->get_resolution()
		       << " (you can't set drift range and entangrange at the same time!)" << endl;
		 assert(false);
	      }

	      if (lde_iter->get_entangbin() == 1) {
		 if (!gotEntAngMin) {
		    m_WeightEntAngMin = lde_iter->get_resolution();
		    gotEntAngMin = true;
		 }
		 else {
		    report(EMERGENCY, kFacilityString)
		       << "Illegal line in ZDWeight2LayerDriftEntAng: layer = " << lde_iter->get_layer()
		       << " driftbin = " << lde_iter->get_driftbin()
		       << " entangbin = " << lde_iter->get_entangbin()
		       << " resolution = " << lde_iter->get_resolution()
		       << " (entang min has already been set!)" << endl;
		    assert(false);
		 }
	      }
	      else {
		 if (!gotEntAngMax) {
		    m_WeightEntAngNBins = lde_iter->get_entangbin();
		    m_WeightEntAngMax = lde_iter->get_resolution();
		    gotEntAngMax = true;
		 }
		 else {
		    report(EMERGENCY, kFacilityString)
		       << "Illegal line in ZDWeight2LayerDriftEntAng: layer = " << lde_iter->get_layer()
		       << " driftbin = " << lde_iter->get_driftbin()
		       << " entangbin = " << lde_iter->get_entangbin()
		       << " resolution = " << lde_iter->get_resolution()
		       << " (entang max has already been set!)" << endl;
		    assert(false);
		 }
	      }

	   } // end "which are we setting: drift params or entang params?"
	} // endif layer == 0

	if (gotDriftMin && gotDriftMax && gotEntAngMin && gotEntAngMax) break;
     } // end param-setting loop

     if (!gotDriftMin || !gotDriftMax || !gotEntAngMin || !gotEntAngMax) {
	report(EMERGENCY, kFacilityString)
	   << "Badly formatted ZDWeight2LayerDriftEntAng: something's missing."
	   << " gotDriftMin = " << gotDriftMin
	   << " gotDriftMax = " << gotDriftMax
	   << " gotEntAngMin = " << gotEntAngMin
	   << " gotEntAngMax = " << gotEntAngMax << endl;
	assert(false);
     }

     // Allocation block
     report(DEBUG, kFacilityString)
	<< "About to allocate m_LayerDriftEntAngWeight as a "
	<< kmaxZDlayer << " x " << m_WeightDriftNBins << " x " << m_WeightEntAngNBins << " array of floats." << endl;
     m_LayerDriftEntAngWeight = new floatPtrPtr[kmaxZDlayer];
     if (true) { // to protect the "int l" declaration on some compilers
	for (int l = 0;  l < kmaxZDlayer;  l++) {
	   m_LayerDriftEntAngWeight[l] = new floatPtr[m_WeightDriftNBins];
	   for (int d = 0;  d < m_WeightDriftNBins;  d++) {
	      m_LayerDriftEntAngWeight[l][d] = new float[m_WeightEntAngNBins];
	      for (int ea = 0;  ea < m_WeightEntAngNBins;  ea++) {
		 m_LayerDriftEntAngWeight[l][d][ea] = kFavoriteNumber;
	      }
	   }
	}
     }
     report(DEBUG, kFacilityString) << "Allocated." << endl;

     for (lde_iter = m_dataHolder->LayerDriftEntAngWeight().begin();  lde_iter != lde_end;  ++lde_iter) {
	if (lde_iter->get_layer() != 0) {
	   int l = lde_iter->get_layer() - 1;
	   int d = lde_iter->get_driftbin() - 1;
	   int ea = lde_iter->get_entangbin() - 1;

	   if (l < 0   ||  l >= kmaxZDlayer           ||
	       d < 0   ||  d >= m_WeightDriftNBins    ||
	       ea < 0  ||  ea >= m_WeightEntAngNBins     ) {
	      report(EMERGENCY, kFacilityString)
		 << "Illegal line in ZDWeight2LayerDriftEntAng: layer = " << lde_iter->get_layer()
		 << " driftbin = " << lde_iter->get_driftbin()
		 << " entangbin = " << lde_iter->get_entangbin()
		 << " resolution = " << lde_iter->get_resolution()
		 << " (kmaxZDlayer = " << kmaxZDlayer
		 << " m_WeightDriftNBins = " << m_WeightDriftNBins
		 << " m_WeightEntAngNBins = " << m_WeightEntAngNBins << ")" << endl;
	      assert(false);
	   }

	   if (m_LayerDriftEntAngWeight[l][d][ea] != kFavoriteNumber) {
	    report(EMERGENCY, kFacilityString)
		 << "Illegal line in ZDWeight2LayerDriftEntAng: layer = " << lde_iter->get_layer()
		 << " driftbin = " << lde_iter->get_driftbin()
		 << " entangbin = " << lde_iter->get_entangbin()
		 << " resolution = " << lde_iter->get_resolution()
		 << " (this bin has already been filled with " << m_LayerDriftEntAngWeight[l][d][ea] << ")" << endl;
	      assert(false);
	   }  

	   if (lde_iter->get_resolution() <= 0.) {
	    report(EMERGENCY, kFacilityString)
		 << "Illegal line in ZDWeight2LayerDriftEntAng: layer = " << lde_iter->get_layer()
		 << " driftbin = " << lde_iter->get_driftbin()
		 << " entangbin = " << lde_iter->get_entangbin()
		 << " resolution = " << lde_iter->get_resolution()
		 << " (resolution must be positive!)" << endl;
	      assert(false);
	   }  

	   m_LayerDriftEntAngWeight[l][d][ea] = lde_iter->get_resolution();
	}
     }

     if (true) { // to protect the "int l" declaration on some compilers
	report(VERIFY, kFacilityString) << "ZFW ================= ZDWeight2LayerDriftEntAng =================" << endl;
	report(VERIFY, kFacilityString) << "FW " << m_WeightDriftNBins << " "
					<< (m_WeightDriftMin >= 0. ? "absolute, " : "")
					<< "normalized drift bins ranging from "
					<< m_WeightDriftMin << " to " << m_WeightDriftMax << endl;
	report(VERIFY, kFacilityString) << "FW " << m_WeightEntAngNBins << " sin(entrance angle) bins ranging from "
					<< m_WeightEntAngMin << " to " << m_WeightEntAngMax << endl;
	for (int l = 0;  l < kmaxZDlayer;  l++) {
	   char lname[3];
	   sprintf(lname, "%02d", l+1);
	   report(VERIFY, kFacilityString) << "ZFW Layer " << lname << endl;
	   for (int d = 0;  d < m_WeightDriftNBins;  d++) {
	      char dname[3];
	      sprintf(dname, "%02d", d+1);
	      report(VERIFY, kFacilityString) << "ZFW          D " << dname;
	      for (int ea = 0;  ea < m_WeightEntAngNBins;  ea++) {
		 if (m_LayerDriftEntAngWeight[l][d][ea] == kFavoriteNumber) {
		    report(VERIFY, kFacilityString) << "  ??.??" << endl;
		    report(EMERGENCY, kFacilityString)
		       << "Ack!  We're missing a resolution value at layer "
		       << l+1 << " driftbin " << d+1 << " entangbin " << ea+1 << endl;
		    assert(false);
		 }

		 char fwname[8];
		 sprintf(fwname, "  %5.2f", m_LayerDriftEntAngWeight[l][d][ea]);
		 report(VERIFY, kFacilityString) << fwname << " ";
	      }
	      report(VERIFY, kFacilityString) << endl;
	   }
	}
     }

     ////////////////////////////////////////////////////////////////////////////////////////////////////////

     // One can put in theta-dependent corrections using ZDWeight2LayerTheta
     // Information in "layer zero" is formatted this way:
     //     layer    thetaBin  Resolution
     //       0       1          -1.0 (min cosTheta value)
     //       0      20 (max)     1.0 (max cosTheta value)

     // Alternatively, if the file is formatted this way (with only
     // one element or with this element ANYWHERE in the file)
     //     layer    thetaBin  Resolution
     //       0       0           0.0
     // only the standard correction of (1 - cos(theta)^2)^(-1/4) is applied.
     // (This is what you would normally want.  The rest is here for
     // future disasters and backward compatibility with CLEO III data.)

     // It is a show-stopping error to include bins outside of the
     // defined range, to include a bin more than once, to not
     // completely fill the defined range, or to not completely fill
     // all layers.

     FANonSeqConstants<DBZDWeight2LayerTheta>::const_iterator lt_iter = m_dataHolder->LayerThetaWeight().begin();
     FANonSeqConstants<DBZDWeight2LayerTheta>::const_iterator lt_end = m_dataHolder->LayerThetaWeight().end();
     DABoolean gotThetaStdCor, gotThetaMin, gotThetaMax;
     gotThetaStdCor = false;  gotThetaMin = false;  gotThetaMax = false;
     for (;  lt_iter != lt_end;  ++lt_iter) {
	if (lt_iter->get_layer() == 0) {
	   if (lt_iter->get_thetabin() == 0) {
	      gotThetaStdCor = true;
	   }
	   else if (lt_iter->get_thetabin() == 1) {
	      if (!gotThetaMin) {
		 m_WeightThetaMin = lt_iter->get_resolution();
		 gotThetaMin = true;
	      }
	      else {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerTheta: layer = " << lt_iter->get_layer()
		    << " thetabin = " << lt_iter->get_thetabin()
		    << " resolution = " << lt_iter->get_resolution()
		    << " (theta min has already been set!)" << endl;
		 assert(false);
	      }
	   }
	   else {
	      if (!gotThetaMax) {
		 m_WeightThetaNBins = lt_iter->get_thetabin();
		 m_WeightThetaMax = lt_iter->get_resolution();
		 gotThetaMax = true;
	      }
	      else {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerTheta: layer = " << lt_iter->get_layer()
		    << " thetabin = " << lt_iter->get_thetabin()
		    << " resolution = " << lt_iter->get_resolution()
		    << " (theta max has already been set!)" << endl;
		 assert(false);
	      }
	   }
	}

	if (gotThetaStdCor || (gotThetaMin && gotThetaMax)) break;
     } // end param-setup loop

     if (gotThetaStdCor) {
	m_LayerThetaWeight = NULL;
	report(VERIFY, kFacilityString) << "ZFW ================= ZDWeight2LayerTheta =================" << endl;
	report(VERIFY, kFacilityString) << "ZFW Applying standard correction of (1 - cos(theta)^2)^(-1/4) only." << endl;
     }
     else {
	if (!gotThetaMin || !gotThetaMax) {
	   report(EMERGENCY, kFacilityString)
	      << "Badly formatted ZDWeight2LayerTheta: something's missing."
	      << " gotThetaStdCor = " << gotThetaStdCor
	      << " gotThetaMin = " << gotThetaMin
	      << " gotThetaMax = " << gotThetaMax << endl;
	   assert(false);
	}

	// Allocation block
	report(DEBUG, kFacilityString)
	   << "About to allocate m_LayerThetaWeight as a "
	   << kmaxZDlayer << " x " << m_WeightThetaNBins << " array of floats." << endl;
	m_LayerThetaWeight = new floatPtr[kmaxZDlayer];
	if (true) { // to protect the "int l" declaration on some compilers
	   for (int l = 0;  l < kmaxZDlayer;  l++) {
	      m_LayerThetaWeight[l] = new float[m_WeightThetaNBins];
	      for (int t = 0;  t < m_WeightThetaNBins;  t++) {
		 m_LayerThetaWeight[l][t] = kFavoriteNumber;
	      }
	   }
	}
	report(DEBUG, kFacilityString) << "Allocated." << endl;

	for (lt_iter = m_dataHolder->LayerThetaWeight().begin();  lt_iter != lt_end;  ++lt_iter) {
	   if (lt_iter->get_layer() != 0) {
	      int l = lt_iter->get_layer() - 1;
	      int t = lt_iter->get_thetabin() - 1;
	      
	      if (l < 0  ||  l >= kmaxZDlayer         ||
		  t < 0  ||  t >= m_WeightThetaNBins     ) {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerTheta: layer = " << lt_iter->get_layer()
		    << " thetabin = " << lt_iter->get_thetabin()
		    << " resolution = " << lt_iter->get_resolution()
		    << " (kmaxZDlayer = " << kmaxZDlayer
		    << " m_WeightThetaNBins = " << m_WeightThetaNBins << ")" << endl;
		 assert(false);
	      }

	      if (m_LayerThetaWeight[l][t] != kFavoriteNumber) {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerTheta: layer = " << lt_iter->get_layer()
		    << " thetabin = " << lt_iter->get_thetabin()
		    << " resolution = " << lt_iter->get_resolution()
		    << " (this bin has already been filled with " << m_LayerThetaWeight[l][t] << ")" << endl;
		 assert(false);
	      }

	      if (lt_iter->get_resolution() <= 0.) {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerTheta: layer = " << lt_iter->get_layer()
		    << " thetabin = " << lt_iter->get_thetabin()
		    << " resolution = " << lt_iter->get_resolution()
		    << " (resolution must be positive!)" << endl;
		 assert(false);
	      }

	      m_LayerThetaWeight[l][t] = lt_iter->get_resolution();
	   }
	}

	if (true) { // to protect the "int l" declaration on some compilers
	   report(VERIFY, kFacilityString) << "ZFW ================= ZDWeight2LayerTheta =================" << endl;
	   report(VERIFY, kFacilityString) << "FW " << m_WeightThetaNBins << " cos(theta) bins ranging from "
					   << m_WeightThetaMin << " to " << m_WeightThetaMax << endl;
	   for (int l = 0;  l < kmaxZDlayer;  l++) {
	      char lname[3];
	      sprintf(lname, "%02d", l+1);
	      report(VERIFY, kFacilityString) << "ZFW Layer " << lname;
	      for (int t = 0;  t < m_WeightThetaNBins;  t++) {
		 if (m_LayerThetaWeight[l][t] == kFavoriteNumber) {
		    report(VERIFY, kFacilityString) << "  ??.??" << endl;
		    report(EMERGENCY, kFacilityString)
		       << "Ack!  We're missing a resolution value at layer "
		       << l+1 << " thetabin " << t+1 << endl;
		    assert(false);
		 }

		 char fwname[8];
		 sprintf(fwname, "  %5.2f", m_LayerThetaWeight[l][t]);
		 report(VERIFY, kFacilityString) << fwname << " ";
	      }
	      report(VERIFY, kFacilityString) << endl;
	   }
	}
     } // end applying-layertheta-weights-from-constants block (not the standard correction)

     ////////////////////////////////////////////////////////////////////////////////////////////////////////

     // One can put in phi-dependent corrections using ZDWeight2LayerPhi
     // Information in "layer zero" is formatted this way:
     //     layer    phiBin    Resolution
     //       0      20 (max)      0.0

     // Alternatively, if the file is formatted this way (with only
     // one element or with this element ANYWHERE in the file)
     //     layer    phiBin  Resolution
     //       0       0           0.0
     // no correction is applied.
     // (This is what you would normally want.  The rest is here for
     // future disasters only.)

     // It is a show-stopping error to include bins outside of the
     // defined range, to include a bin more than once, to not
     // completely fill the defined range, or to not completely fill
     // all layers.

     FANonSeqConstants<DBZDWeight2LayerPhi>::const_iterator lp_iter = m_dataHolder->LayerPhiWeight().begin();
     FANonSeqConstants<DBZDWeight2LayerPhi>::const_iterator lp_end = m_dataHolder->LayerPhiWeight().end();
     DABoolean gotPhiStdCor, gotPhiMax;
     gotPhiStdCor = false;  gotPhiMax = false;
     for (;  lp_iter != lp_end;  ++lp_iter) {
	if (lp_iter->get_layer() == 0) {
	   if (lp_iter->get_phibin() == 0) {
	      gotPhiStdCor = true;
	   }
	   else {
	      if (!gotPhiMax) {
		 m_WeightPhiNBins = lp_iter->get_phibin();
		 gotPhiMax = true;
	      }
	      else {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerPhi: layer = " << lp_iter->get_layer()
		    << " phibin = " << lp_iter->get_phibin()
		    << " resolution = " << lp_iter->get_resolution()
		    << " (phi max has already been set!)" << endl;
		 assert(false);
	      }
	   }
	}

	if (gotPhiStdCor || gotPhiMax) break;
     } // end param-setup loop

     if (gotPhiStdCor) {
	m_LayerPhiWeight = NULL;
	report(VERIFY, kFacilityString) << "ZFW ================= ZDWeight2LayerPhi =================" << endl;
	report(VERIFY, kFacilityString) << "ZFW Resolutions will have no phi dependence." << endl;
     }
     else {
	if (!gotPhiMax) {
	   report(EMERGENCY, kFacilityString)
	      << "Badly formatted ZDWeight2LayerPhi: something's missing."
	      << " gotPhiStdCor = " << gotPhiStdCor
	      << " gotPhiMax = " << gotPhiMax << endl;
	   assert(false);
	}

	// Allocation block
	report(DEBUG, kFacilityString)
	   << "About to allocate m_LayerPhiWeight as a "
	   << kmaxZDlayer << " x " << m_WeightPhiNBins << " array of floats." << endl;
	m_LayerPhiWeight = new floatPtr[kmaxZDlayer];
	if (true) { // to protect the "int l" declaration on some compilers
	   for (int l = 0;  l < kmaxZDlayer;  l++) {
	      m_LayerPhiWeight[l] = new float[m_WeightPhiNBins];
	      for (int t = 0;  t < m_WeightPhiNBins;  t++) {
		 m_LayerPhiWeight[l][t] = kFavoriteNumber;
	      }
	   }
	}
	report(DEBUG, kFacilityString) << "Allocated." << endl;

	for (lp_iter = m_dataHolder->LayerPhiWeight().begin();  lp_iter != lp_end;  ++lp_iter) {
	   if (lp_iter->get_layer() != 0) {
	      int l = lp_iter->get_layer() - 1;
	      int t = lp_iter->get_phibin() - 1;
	      
	      if (l < 0  ||  l >= kmaxZDlayer         ||
		  t < 0  ||  t >= m_WeightPhiNBins     ) {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerPhi: layer = " << lp_iter->get_layer()
		    << " phibin = " << lp_iter->get_phibin()
		    << " resolution = " << lp_iter->get_resolution()
		    << " (kmaxZDlayer = " << kmaxZDlayer
		    << " m_WeightPhiNBins = " << m_WeightPhiNBins << ")" << endl;
		 assert(false);
	      }

	      if (m_LayerPhiWeight[l][t] != kFavoriteNumber) {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerPhi: layer = " << lp_iter->get_layer()
		    << " phibin = " << lp_iter->get_phibin()
		    << " resolution = " << lp_iter->get_resolution()
		    << " (this bin has already been filled with " << m_LayerPhiWeight[l][t] << ")" << endl;
		 assert(false);
	      }

	      if (lp_iter->get_resolution() <= 0.) {
		 report(EMERGENCY, kFacilityString)
		    << "Illegal line in ZDWeight2LayerPhi: layer = " << lp_iter->get_layer()
		    << " phibin = " << lp_iter->get_phibin()
		    << " resolution = " << lp_iter->get_resolution()
		    << " (resolution must be positive!)" << endl;
		 assert(false);
	      }

	      m_LayerPhiWeight[l][t] = lp_iter->get_resolution();
	   }
	}

	if (true) { // to protect the "int l" declaration on some compilers
	   report(VERIFY, kFacilityString) << "ZFW ================= ZDWeight2LayerPhi =================" << endl;
	   report(VERIFY, kFacilityString) << "FW " << m_WeightPhiNBins << " phi bins ranging from 0 to 2 pi";
	   for (int l = 0;  l < kmaxZDlayer;  l++) {
	      char lname[3];
	      sprintf(lname, "%02d", l+1);
	      report(VERIFY, kFacilityString) << "ZFW Layer " << lname;
	      for (int t = 0;  t < m_WeightPhiNBins;  t++) {
		 if (m_LayerPhiWeight[l][t] == kFavoriteNumber) {
		    report(VERIFY, kFacilityString) << "  ??.??" << endl;
		    report(EMERGENCY, kFacilityString)
		       << "Ack!  We're missing a resolution value at layer "
		       << l+1 << " phibin " << t+1 << endl;
		    assert(false);
		 }

		 char fwname[8];
		 sprintf(fwname, "  %5.2f", m_LayerPhiWeight[l][t]);
		 report(VERIFY, kFacilityString) << fwname << " ";
	      }
	      report(VERIFY, kFacilityString) << endl;
	   }
	}
     } // end applying-layerphi-weights-from-constants block (not the standard correction)

     ////////////////////////////////////////////////////////////////////////////////////////////////////////

     // m_LayerSummary and m_WeightMiddleEntAngBin are provided in
     // case the user wants a rough hit resolution with no knowledge
     // of the track

     // translations of absDrift and sinalpha into bin numbers
     double sbDriftMin = sbDrift(0.);
     double sbDriftMax = sbDrift(1.);
     m_WeightMiddleEntAngBin = 0;
     if (true) { // to keep narrow the scope of some variables
	double sbEntAngMiddle(sbEntAng(0.));
	for (int ea = 1;  ea < m_WeightEntAngNBins;  ea++) {
	   if (fabs(double(ea) - sbEntAngMiddle) < fabs(double(m_WeightMiddleEntAngBin) - sbEntAngMiddle)) {
	      m_WeightMiddleEntAngBin = ea;
	   }
	}
     }

     report(DEBUG, kFacilityString)
	<< "About to allocate m_LayerSummary as a " << kmaxZDlayer << "-component array of floats." << endl;
     m_LayerSummary = new float[kmaxZDlayer];
     report(DEBUG, kFacilityString) << "Allocated." << endl;

     if (true) { // to protect the "int l" declaration on some compilers
	for (int l = 0;  l < kmaxZDlayer;  l++) {
	   double driftAve(0.);
	   int driftN(0);

	   for (int d = 0;  d < m_WeightDriftNBins;  d++) {
	      if (d >= sbDriftMin  &&  d <= sbDriftMax) {
		 driftAve += double(m_LayerDriftEntAngWeight[l][d][m_WeightMiddleEntAngBin]);
		 driftN++;
	      }
	   }

	   if (driftN == 0) {
	      report(EMERGENCY, kFacilityString)
		 << "Your ZDWeight2LayerDriftEntAng constants don't have any drift bins"
		 << " that lie within the drift cell!  sbDriftMin = " << sbDriftMin
		 << " sbDriftMax = " << sbDriftMax
		 << " and m_WeightDriftNBins = " << m_WeightDriftNBins << endl;
	      assert(false);  // ... and this CAN'T be what you actually want.
	   }

	   m_LayerSummary[l] = float(driftAve / double(driftN));

	}
     }

  // Unpack T0s 
  //
  int i;
  int imagic  = 123456789 ;
  double magic ;
  memset(&magic,imagic,sizeof(magic));  // unique number for initialization 
                                         // to test if constants unpack has
                                         // set all channels 

  report( DEBUG, kFacilityString )  << "Trying to get T0s" << endl;
  report( INFO , kFacilityString )  << "Global T0 offset 1 (user parameter): " 
				    << m_ZDDriftFunctionProd->getGlobalT0() << endl;
  report( INFO , kFacilityString )  << "Global T0 offset 2 (constants)     : " 
				    << m_dataHolder->T0Global()[1].get_tzero() << endl;

//  report( INFO , kFacilityString )  << "hgs: sizeof(m_channelT0)=" 
//                                    << sizeof(m_channelT0) << endl;

  memset ( m_channelT0, imagic, sizeof(m_channelT0) ) ;

//  report( INFO , kFacilityString )  << "hgs: memset finished" << endl;

  for(FAConstants<DBZDt0channel>::const_iterator 
	t0loop =m_dataHolder->T0Channel().begin();
        t0loop!=m_dataHolder->T0Channel().end()  ;
      ++t0loop ) 
    {

//  report( INFO , kFacilityString )  << "hgs: inside loop:" << t0loop << endl;
//  report( INFO , kFacilityString )  << "hgs: wire# ="
//                                    << (*t0loop).get_indexNumber() << endl;

      int layer = m_dataHolder->Geometry()->layerWire( (*t0loop).get_indexNumber()).first ;

//  report( INFO , kFacilityString )  << "hgs: layer=" << layer << endl;

/*report(INFO,kFacilityString )<< "hgs,a:"<< (*t0loop).get_t0CommonMode()<<endl;
report(INFO,kFacilityString )<< "hgs,b:"<< (*t0loop).get_t0Channel()<<endl;
report(INFO,kFacilityString )<< "hgs,c:"<<
       m_dataHolder->T0Crate()[(*t0loop).get_crate ()].get_tzero() <<endl;
report(INFO,kFacilityString )<< "hgs,d:"<<
       m_dataHolder->T0Card  ()[(*t0loop).get_card  ()].get_tzero() << endl;
report(INFO,kFacilityString )<< "hgs,e:"<<
       m_dataHolder->T0HVCard()[(*t0loop).get_hvcard()].get_tzero() << endl;
report(INFO,kFacilityString )<< "hgs,f:"<<
       m_dataHolder->T0Global()[1].get_tzero() << endl;
report(INFO,kFacilityString )<< "hgs,g:"<<m_ZDDriftFunctionProd->getGlobalT0() << endl;
report(INFO,kFacilityString )<< "hgs,h:"<<ZShift ( layer ) << endl;
report(INFO,kFacilityString )<< "hgs,i:"<<
       ( m_dataHolder->Geometry()->radiusNominal( layer ) / kclight ) 
       << "hgs#" <<endl;
*/

      m_channelT0[(*t0loop).get_indexNumber()-1] 
        = (*t0loop).get_t0CommonMode()
	+ (*t0loop).get_t0Channel   ()
	+ m_dataHolder->T0Crate ()[(*t0loop).get_crate ()].get_tzero()
	+ m_dataHolder->T0Card  ()[(*t0loop).get_card  ()].get_tzero()
	+ m_dataHolder->T0HVCard()[(*t0loop).get_hvcard()].get_tzero()
	+ m_dataHolder->T0Global()[1].get_tzero()
	+ m_ZDDriftFunctionProd->getGlobalT0() 
	+ ZShift ( layer ) 
	- ( m_dataHolder->Geometry()->radiusNominal( layer ) / kclight ) ;
    }

  int nt0err = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_channelT0[i] == magic )
	{ 
	  nt0err++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of ZDt0channel" << endl;
	} 
    }

  if ( nt0err == 0 ) 
    { 
      report( INFO, kFacilityString )  << "t0s loaded OK" 
				       << endl; 
    }
  else
    {
      report( ERROR, kFacilityString )  << "t0s loaded, some channels missing " 
					<< nt0err << endl; 
      assert(nt0err==0);
    }

  
  
  //-------------------------------------------------------------------------------------------------------------------------------------------
  //
  // Unpack Charge Dependent T0 shifts
  //
  int qcount = 0 ;
  memset ( m_Qcorr, 0, sizeof(m_Qcorr) )  ;
  for(FAConstants<DBZDt0global>::const_iterator 
      t0_qcorr  = m_dataHolder->T0Global().begin() ;
      t0_qcorr != m_dataHolder->T0Global().end() ;
    ++t0_qcorr )
    {
      int qbin = (*t0_qcorr).get_index() - 2 ; 
      if ( qbin >= 0 ) 
	{ 
	  if ( qbin >= kmaxQcorrBins ) 
	    {
	      report( ERROR, kFacilityString )  << " overflow in pulseheight-dependent t0 shifts " 
						<< endl ;
	    }
	  else
	    {
	      m_Qcorr[qbin] = (*t0_qcorr).get_tzero();
              qcount++ ;
	    }
	}
    }
  report( INFO, kFacilityString )  << " Found " 
				   << qcount
				   << " Charge Dependent T0 Shifts " 
				   << endl; 
  

  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack ADC information (pedestals only for now)
  //
  report( DEBUG, kFacilityString )  << "Trying to get ADC pedestals" << endl;

  memset ( m_channelPed, imagic, sizeof(m_channelPed) ) ;

  for(FAConstants<DBZDadcpedestal>::const_iterator 
	adcloop =m_dataHolder->ADCped().begin();
        adcloop!=m_dataHolder->ADCped().end()  ;
      ++adcloop ) 
    {
//report(INFO, kFacilityString) << "hgs:" << (*adcloop).get_CellID()-1 << " "
//                              << (*adcloop).get_pedestal() << endl;

      m_channelPed   [(*adcloop).get_CellID()-1]
	=(*adcloop).get_pedestal();
      m_channelPedRMS[(*adcloop).get_CellID()-1]
	=(*adcloop).get_pedestal_RMS();
    }

  int npederr = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_channelPed[i] == magic ) 
	{ 
	  npederr++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of ZDadcpedestal" << endl;
	} 
    }

  if ( npederr == 0 ) 
    {
      report( INFO , kFacilityString )  << "Pedestals loaded OK" 
					<< endl;
    }
  else
    {
      report( ERROR, kFacilityString )  << "Pedestals loaded, some channels missing "
					<< npederr 
					<< endl;
      assert(npederr==0);
    }
  
  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack WireQuality information
  //
  report( DEBUG, kFacilityString )  << "Trying to get WireQuality from frame" << endl;
  
  memset ( m_quality, imagic, sizeof(m_quality) ) ;
  
  for(FAConstants<DBZDWireQuality>::const_iterator 
	qloop =m_dataHolder->WireQuality().begin();
        qloop!=m_dataHolder->WireQuality().end()  ;
      ++qloop ) 
    {	  
      m_quality[(*qloop).get_CellID()-1]=(*qloop).get_Quality();
    }

  int nqerr = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_quality[i] == imagic ) 
	{ 
	  nqerr++ ; 
	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of ZDWireQuality" << endl;
	} 
    }

  if ( nqerr == 0 ) 
    {
      report( INFO , kFacilityString )  << "WireQuality Info loaded OK" 
					<< endl ; 
      assert(nqerr==0);
    }
  else
    {
      report( ERROR, kFacilityString )  << "WireQuality Info loaded, some channels missing  " 
					<< nqerr 
					<< endl;
    }
  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack MisCabling information
  //
  report( DEBUG, kFacilityString )  << "Trying to get Miscabling Information" << endl;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      m_daqmap[i]    = kTotalZDWires+1 ; 
      m_invdaqmap[i] = kTotalZDWires+1 ; 
    }

  for(FAConstants<DBZDDaqCellIDMap>::const_iterator 
	wloop =m_dataHolder->WireMap().begin();
        wloop!=m_dataHolder->WireMap().end();
      ++wloop ) 
    {
         m_daqmap[(*wloop).get_DaqId() -1]=(*wloop).get_CellId();
      m_invdaqmap[(*wloop).get_CellId()-1]=(*wloop).get_DaqId();
    }

  int nwerr = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_daqmap[i] == kTotalZDWires+1 ) 
	{ 
	  nwerr++ ; 
	} 
    }

  int nwerr2 = 0 ;
  for ( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_invdaqmap[i] == kTotalZDWires+1 ) 
	{ 
	  nwerr2++ ; 
	} 
    }

  if ( nwerr == 0 ) 
    { 
      report( INFO , kFacilityString )  << "WireMap loaded OK" 
					<< endl; 
    }
  else
    { 
      report( ERROR, kFacilityString )  << "WireMap loaded, some channels missing  " 
					<< nwerr << " " 
					<< nwerr2 
					<< endl; 
    }
  //-------------------------------------------------------------------------------------------------------------------------------------------
  // Unpack Wire ADC Gain Information 
  //
  report( DEBUG, kFacilityString )  << "Trying to get ADC Gains" << endl;
 
  memset ( m_adcgain, imagic, sizeof(m_adcgain) ) ;
 
  for(FAConstants<DBZDadc2charge>::const_iterator 
	gloop =m_dataHolder->ADCpar().begin();
        gloop!=m_dataHolder->ADCpar().end()  ;
      ++gloop ) 
    {
      m_adcgain   [(*gloop).get_CellID()-1]=(*gloop).get_slope();
      m_adcgainRMS[(*gloop).get_CellID()-1]=(*gloop).get_slope_error();
    }

  int ngerr = 0 ;
  for( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_adcgain[i] == magic ) 
	{ 
	  ngerr++ ;
 	  report(ERROR,kFacilityString) << " could not load wire " << (i+1) << " of ZDadc2charge" << endl;
	} 
    } 

  if ( ngerr == 0 ) 
    { 
      report( INFO, kFacilityString )  << "ADC Gains loaded OK" 
				       << endl; 
    }
  else
    { 
      report( ERROR, kFacilityString )  << "adcgains, some channels missing> "
					<< ngerr 
					<< endl; 
      assert(ngerr==0);
    }

  int ngerr2 = 0 ;
  for( i=0; i<kTotalZDWires; ++i ) 
    { 
      if ( m_adcgain[i] == 0 ) 
	{ 
	  ngerr2++ ; 
	  m_adcgain[i] = 1. ; 
	} 
    } 

  if ( ngerr2 == 0 ) 
    { 
      report( INFO, kFacilityString )  << "No Zero ADC Gains" 
				       << endl; 
    }
  else
    { 
      report( ERROR, kFacilityString )  << "adcgains, some channels have zero gain, set to 1> "
					<< ngerr2 
					<< endl; 
    }

  //-------------------------------------------------------------------------------------------------------------------------------------------
  //
  // Set Wire TDC Slope Information rather than unpack from database
  //
  report( INFO, kFacilityString )  << "Trying to set TDC Slopes" << endl;

  for( i=0; i<kTotalZDWires; ++i ) 
    { 
       m_tdcslope[i] = -500. ; //hgs: naive, in ps
       m_tdcslopeRMS[i] = 0. ; //hgs: naive
       m_tdcped[i] = 0. ;      //hgs: naive
       m_tdcpedRMS[i] = 1.2 ;  //hgs: naive
    } 

  report( INFO, kFacilityString )  << "Constructor Ends " << endl;
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::~ZDDriftFunction()
{
  report( DEBUG, kFacilityString )  << "Destructor Starts " << endl;
  delete m_statistics;

  report( DEBUG, kFacilityString )  << "possibly about to delete m_LayerDriftEntAngWeight = " << m_LayerDriftEntAngWeight << endl;
  if (m_LayerDriftEntAngWeight != NULL) {
     for (int l = 0;  l < kmaxZDlayer;  l++) {
	for (int d = 0;  d < m_WeightDriftNBins;  d++) {
	   report( DEBUG, kFacilityString )  << "about to delete layer " << l << " driftbin " << d << " array of entang fittingweights" << endl;
	   delete [] m_LayerDriftEntAngWeight[l][d];
	}
	report( DEBUG, kFacilityString )  << "about to delete layer " << l << " array of driftbin pointers" << endl;
	delete [] m_LayerDriftEntAngWeight[l];
     }
     report( DEBUG, kFacilityString )  << "about to delete array of layer pointers" << endl;
     delete [] m_LayerDriftEntAngWeight;
     report( DEBUG, kFacilityString )  << "WHEW!  We survived!" << endl;
  }

  report( DEBUG, kFacilityString )  << "possibly about to delete m_LayerThetaWeight = " << m_LayerThetaWeight << endl;
  if (m_LayerThetaWeight != NULL) {
     for (int l = 0;  l < kmaxZDlayer;  l++) {
	report( DEBUG, kFacilityString ) << "about to delete layer " << l << " array of theta fittingweights" << endl;
	delete [] m_LayerThetaWeight[l];
     }
     report( DEBUG, kFacilityString )  << "about to delete array of layer pointers" << endl;
     delete [] m_LayerThetaWeight;
     report( DEBUG, kFacilityString )  << "WHEW!  We survived!" << endl;
  }

  report( DEBUG, kFacilityString )  << "possibly about to delete m_LayerPhiWeight = " << m_LayerPhiWeight << endl;
  if (m_LayerPhiWeight != NULL) {
     for (int l = 0;  l < kmaxZDlayer;  l++) {
	report( DEBUG, kFacilityString ) << "about to delete layer " << l << " array of phi fittingweights" << endl;
	delete [] m_LayerPhiWeight[l];
     }
     report( DEBUG, kFacilityString )  << "about to delete array of layer pointers" << endl;
     delete [] m_LayerPhiWeight;
     report( DEBUG, kFacilityString )  << "WHEW!  We survived!" << endl;
  }

  report( DEBUG, kFacilityString )  << "possibly about to delete m_LayerSummary = " << m_LayerSummary << endl;
  if (m_LayerSummary != NULL) delete [] m_LayerSummary;

  report( DEBUG, kFacilityString )  << "Destructor Ends " << endl;
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge 
ZDDriftFunction::adcpedestal(UInt16 wire) const 
{ 
//report(INFO, kFacilityString ) << "hgs: wire=" << wire << endl;
//report(INFO, kFacilityString ) << "hgs: m_channelPed=" << m_channelPed << endl;
  return m_channelPed[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge 
ZDDriftFunction::adcpedestalRMS(UInt16 wire) const 
{ 
  return m_channelPedRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge
ZDDriftFunction::adcgain(UInt16 wire) const 
{ 
  return m_adcgain[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge
ZDDriftFunction::adcgainRMS(UInt16 wire) const 
{ 
  return m_adcgainRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time
ZDDriftFunction::tdcslope(UInt16 wire) const 
{ 
  return m_tdcslope[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time
ZDDriftFunction::tdcslopeRMS(UInt16 wire) const 
{ 
  return m_tdcslopeRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time
ZDDriftFunction::tdcped(UInt16 wire) const 
{ 
  return m_tdcped[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time
ZDDriftFunction::tdcpedRMS(UInt16 wire) const 
{ 
  return m_tdcpedRMS[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time 
ZDDriftFunction::tdctzero(UInt16 wire) const 
{ 
  return m_channelT0[wire-1];
}
//
//----------------------------------------------------------------------------
//

double ZDDriftFunction::soft_bin(double value, double min, double max, int nbins) const
{
   return (value - min) / (max - min) * double(nbins) - 0.5;
}

double ZDDriftFunction::sbDrift(double absDrift) const
{
   return soft_bin(absDrift, m_WeightDriftMin, m_WeightDriftMax, m_WeightDriftNBins);
}

double ZDDriftFunction::sbEntAng(double sinalpha) const
{
   return soft_bin(sinalpha, m_WeightEntAngMin, m_WeightEntAngMax, m_WeightEntAngNBins);
}

double ZDDriftFunction::sbTheta(double cosTheta) const
{
   return soft_bin(cosTheta, m_WeightThetaMin, m_WeightThetaMax, m_WeightThetaNBins);
}

double ZDDriftFunction::sbPhi(double phi) const
{
   double rightphi = fmod(phi + 12.*M_PI, 2.*M_PI);
   report(DEBUG, kFacilityString) << "ZDDriftFunction::sbPhi: rightphi = " << rightphi << endl;
   assert(rightphi >= 0.);
   assert(rightphi < 2.*M_PI);
   return soft_bin(rightphi, 0., 2.*M_PI, m_WeightPhiNBins);
}

int ZDDriftFunction::close_low(double sb, int nbins) const
{
   assert(nbins > 1);
   if (sb < 0.) return 0;
   if (sb >= double(nbins-1)) return nbins-2;
   if (sb == floor(sb)) return int(sb);
   return int(floor(sb));
}

int ZDDriftFunction::clDrift(double sb) const
{
   return close_low(sb, m_WeightDriftNBins);
}

int ZDDriftFunction::clEntAng(double sb) const
{
   return close_low(sb, m_WeightEntAngNBins);
}

int ZDDriftFunction::clTheta(double sb) const
{
   return close_low(sb, m_WeightThetaNBins);
}

double ZDDriftFunction::interp(double sb, int cl, double fl, double fh) const
{
   // sb == soft bin
   // cl == close low; close high is close low + 1
   // fl == function low (value of function at close low)
   // fh == function high
   return (fh - fl)*(sb - double(cl)) + fl;
}

double ZDDriftFunction::intEntAng(int l, int d, double sinalpha) const
{
   double sb = sbEntAng(sinalpha);
   int cl = clEntAng(sb);
   int ch = cl + 1;
   double fl = m_LayerDriftEntAngWeight[l][d][cl];
   double fh = m_LayerDriftEntAngWeight[l][d][ch];
   return interp(sb, cl, fl, fh);
}

double ZDDriftFunction::intDriftEntAng(int l, double absDrift, double sinalpha) const
{
   double sb = sbDrift(absDrift);
   int cl = clDrift(sb);
   int ch = cl + 1;
   double fl = intEntAng(l, cl, sinalpha);
   double fh = intEntAng(l, ch, sinalpha);
   return interp(sb, cl, fl, fh);
}

double ZDDriftFunction::intDriftOnly(int l, double absDrift) const
{
   double sb = sbDrift(absDrift);
   int cl = clDrift(sb);
   int ch = cl + 1;
   double fl = m_LayerDriftEntAngWeight[l][cl][m_WeightMiddleEntAngBin];
   double fh = m_LayerDriftEntAngWeight[l][ch][m_WeightMiddleEntAngBin];
   return interp(sb, cl, fl, fh);
}

double ZDDriftFunction::standardTheta(double cosTheta) const
{
   return pow(1 - sqr(cosTheta), -0.25);
}

double ZDDriftFunction::intTheta(int l, double cosTheta) const
{
   double sb = sbTheta(cosTheta);
   int cl = clTheta(sb);
   int ch = cl + 1;
   double fl = m_LayerThetaWeight[l][cl];
   double fh = m_LayerThetaWeight[l][ch];
   return interp(sb, cl, fl, fh);
}

double ZDDriftFunction::intPhi(int l, double phi) const
{
   const int nbins = m_WeightPhiNBins;

   double sb = sbPhi(phi);
   if (sb >= 0.  &&  sb < double(nbins-1)) {
      int cl = int(floor(sb));
      int ch = cl + 1;
      double fl = m_LayerPhiWeight[l][cl];
      double fh = m_LayerPhiWeight[l][ch];
      return interp(sb, cl, fl, fh);
   }

   double fl = m_LayerPhiWeight[l][nbins-1];
   double fh = m_LayerPhiWeight[l][0];
   if (sb < 0.) return interp(sb, -1, fl, fh);
   else return interp(sb, nbins-1, fl, fh);
}

ZDDriftFunction::Resolution
ZDDriftFunction::resolution() const 
{ 
  return m_ZDDriftFunctionProd->getResolution();
}

ZDDriftFunction::Resolution
ZDDriftFunction::resolution(int layer) const 
{ 
   return double(m_LayerSummary[layer-1]) * resolution();
}

ZDDriftFunction::Resolution
ZDDriftFunction::resolution(int layer,Drift drift) const 
{ 
   const double layerCells = double(m_dataHolder->Geometry()->numberOfWiresLyr(layer));
   const double layerRadius = m_dataHolder->Geometry()->radiusNominal(layer);
   // "abs"Drift may be signed if m_WeightDriftMin is negative
   double absDrift = drift * layerCells / (M_PI * layerRadius);
   if (m_WeightDriftMin >= 0.) absDrift = fabs(absDrift);

   return intDriftOnly(layer-1, absDrift) * resolution();
}

ZDDriftFunction::Resolution
ZDDriftFunction::resolution(int layer,Drift drift,double costheta,double phi,double sinalpha) const 
{ 
   const double layerCells = double(m_dataHolder->Geometry()->numberOfWiresLyr(layer));
   const double layerRadius = m_dataHolder->Geometry()->radiusNominal(layer);
   // "abs"Drift may be signed if m_WeightDriftMin is negative
   double absDrift = drift * layerCells / (M_PI * layerRadius);
   if (m_WeightDriftMin >= 0.) absDrift = fabs(absDrift);

   double res = intDriftEntAng(layer-1, absDrift, sinalpha);

   if (m_LayerThetaWeight != NULL) res *= intTheta(layer-1, costheta);
   else res *= standardTheta(costheta);

   if (m_LayerPhiWeight != NULL) res *= intPhi(layer-1, phi);

//    cout << "ZRES layer " << layer << " drift " << drift << " costheta " << costheta
// 	<< " phi " << phi << " res " << res << endl;

   return res * resolution();
}

//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time ZDDriftFunction::badwiremap(UInt16 wire) const 
{ 
  return m_quality[wire-1]; 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge ZDDriftFunction::adc2Charge( UInt16 wire, 
						 ZDDatum::DigiType adc) const 
{ 
  return Charge ( m_adcgain[wire-1] * ( adc - m_channelPed[wire-1] ) ); 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Charge ZDDriftFunction::adcwithped( UInt16 wire, 
						 ZDDatum::DigiType adc) const 
{ 
  //just pedestal correction
  return Charge ( adc - m_channelPed[wire-1] ); 
}
//
//----------------------------------------------------------------------------
//
ZDDriftFunction::Time ZDDriftFunction::tdc2Time( UInt16 wire, 
					     ZDDatum::DigiType tdc ) const
{  
  // verify TDC slope is OK at constants unpack
  return Time ( m_tdcslope[wire-1] * double ( tdc ) ) + m_channelT0[wire-1];
}
//
//----------------------------------------------------------------------------
//
ZDDatum::DigiType ZDDriftFunction::charge2adc( UInt16 wire, 
					     Charge PulseHeight ) const
{ 
  return ZDDatum::DigiType ( ( PulseHeight / m_adcgain[wire-1] ) + m_channelPed[wire-1] ); 
}
//
//----------------------------------------------------------------------------
//
ZDDatum::DigiType ZDDriftFunction::time2tdc( UInt16 wire, 
					   Time time) const
{  
  // verify TDC slope is OK at constants unpack
  return  ( ( time - m_channelT0[wire-1] ) / m_tdcslope[wire-1] );
}
ZDDatum::DigiType ZDDriftFunction::time2tdc( UInt16 wire,
                                           Time time,
                                           double radiusOfLayer ) const
{ return time2tdc( wire, time ) ; }
//
//----------------------------------------------------------------------
//
// daqID2CellID( daqID )
//
//  Convert a daqId to a CellID (take care of miscablings, trivial right now)
//
UInt16 ZDDriftFunction::daqID2CellID( UInt16 daqID ) const
{ 
  return m_daqmap[daqID-1]; 
}
//---------------------------------------------------------------------------
//
// CellID2daqID( CellID )
//
//  Convert a CellID2daqId (take care of miscablings, trivial right now)
//
UInt16 ZDDriftFunction::CellID2daqID( UInt16 CellID ) const
{ 
  return m_invdaqmap[CellID-1]; 
}
//---------------------------------------------------------------------------
//
// ZCorr( )
//
ZDDriftFunction::Time ZDDriftFunction::Zcorr( int layer, float dz ) const
{
  float z = dz ;
  if ( m_dataHolder->Geometry()->layer( layer ).readoutEnd() == 
       AEWireLayer::kReadoutEast ) 
    { 
      z *= -1.;
    }
//report(INFO,kFacilityString )<< "hgs: in Zcorr" << endl;
//report(INFO,kFacilityString )<< "hgs: in Zcorr "
//  << m_ZDDriftFunctionProd->signalPropBeta_Val() << " " << kclight << endl;

  return (z/(m_ZDDriftFunctionProd->signalPropBeta_Val()*kclight));
  //
  // If this was my world I would define the geometry so one could
  // simply do:
  /*
    return (dz*m_dataHolder->Geometry()->layer( layer ).readoutSign()/
            (m_ZDDriftFunctionProd->signalPropBeta_Val()*kclight));
  */
  // but nooooooooooooo
  //
}
//---------------------------------------------------------------------------
//
// ZShift( )
//
//  Check parameters, determine appropriate Z-shift
//
ZDDriftFunction::Time ZDDriftFunction::ZShift( int layer ) const
{
//report(INFO,kFacilityString )<< "hgs: in ZShift:a, layer=" << layer << endl;
  // Shift time to z=0 AFTER the raw time has been checked above.
  if( m_ZDDriftFunctionProd->shiftTimeToCenter() )
    {
//report(INFO,kFacilityString )<< "hgs: in ZShift:b" << endl;
      // This is the time for a signal to travel from the endplate
      // at readout to the center of the chamber, z=0.  This time
      // is ADDED to a time at the endplate to shift it to z=0.
      
      // West is +z
      Meters dz ;
      if( m_dataHolder->Geometry()->layer( layer ).readoutEnd() ==
	  AEWireLayer::kReadoutEast )
	{
//report(INFO,kFacilityString )<< "hgs: in ZShift:c" << endl;
	  dz = m_dataHolder->Geometry()->layer( layer ).zMin() ;
//report(INFO,kFacilityString )<< "hgs: in ZShift:c2" << endl;
	}
      else
	{
//report(INFO,kFacilityString )<< "hgs: in ZShift:d" << endl;
	  dz = m_dataHolder->Geometry()->layer( layer ).zMax()  ;
	}
//report(INFO,kFacilityString )<< "hgs: in ZShift:e" << endl;
      return Zcorr ( layer , dz ) ;
    }
  else
    {
      return 0 ;
    }
}
//---------------------------------------------------------------------------
// makeCalibratedHit - convert a raw hit into a calibrated hit
//
//        Note tOffset has default of 0.0 ps
//
//        See .h file for more info and warnings
//
CalibratedZDHit* ZDDriftFunction::makeCalibratedHit( const CollatedRawZDHit& rawhit, 
						   int id, 
						   const AZDSenseWireStore* geom, 
						   double tOffset ) const
{      
   DABoolean singleTDC(true)   ;
   ZDDatum::DigiType tdc(0)    ;
   Time         time(0)        ;
   UInt16       wTdc(0)        ;
   Charge       charge(-1)     ;
   ZDHitQuality       qhit     ; // default is "bad" 
   double       weight(1.)     ;    
   Tseries      dva(0.,0.,0.)  ;   
   unsigned int  nTDCs(0)      ;
   Time earlyGap(-1.)          ;
   Time lateGap(-1.)           ;
   int    layer(0)             ;
   int    wireInLayer(0)       ;
   UInt16 wire(0)              ;
   int wintype(0)              ;

   //higher channels are electronic test channels (counts from 1)
   if ( rawhit.daqId() <= kTotalZDWires )
     {
       UInt16 cellID = daqID2CellID(rawhit.daqId()); 
       AZDSenseWireStore::LayerWire coord = geom->layerWire( cellID );
       layer       = coord.first       ;
       wireInLayer = coord.second      ;
       wire        = cellID            ;
       STL_VECTOR(ZDDatum::DigiType)::const_iterator itdc = rawhit.digiTimes().begin();   
       nTDCs     = rawhit.digiTimes().size()  ;
       //m_statistics->counter(2,"normal hits"); 
       if (rawhit.isCharge()) 
	 {
	   charge = adc2Charge(wire,rawhit.digiCharge());
	 }
       else                   
	 { 
	   //m_statistics->counter(3,"no charge info"); 
	   qhit.set_nocharge()                    ;
	 }
       if ( nTDCs == 0 )  
	 { 
	   //m_statistics->counter(6,"no TDCS in hit"); 
	   qhit.set_notime(); 
	 } 
       else
	 {
           int i;

           UInt16 tdcTight,tdcLoose,tdcHigh,tdcLow;

	   int wTdcTight = -1;
           int wTdcLoose = -1;
           int wTdcHigh  = -1;
           int wTdcLow   = -1;

           Time timeTight = m_ZDDriftFunctionProd->getMaxTightTime();
           Time timeLoose = m_ZDDriftFunctionProd->getMaxLooseTime();
           Time timeHigh  = m_ZDDriftFunctionProd->getMaxLooseTime() + 1.e12;
           Time timeLow   = m_ZDDriftFunctionProd->getMinLooseTime() - 1.e12;
           for ( i=0 ; i<nTDCs ; ++i )
	     {
	       ZDDatum::DigiType   newtdc  = rawhit.digiTimes()[i];
	       Time newtime = tdc2Time(wire,newtdc)-tOffset; 
	       if ( newtime > m_ZDDriftFunctionProd->getMinTightTime() && newtime < m_ZDDriftFunctionProd->getMaxTightTime() ) 
		 { if ( newtime < timeTight ) { timeTight = newtime ; tdcTight = newtdc ; wTdcTight = i; } }
               else 
		 {
		   if ( newtime > m_ZDDriftFunctionProd->getMinLooseTime() && newtime < m_ZDDriftFunctionProd->getMaxLooseTime() ) 
		     { if ( newtime < timeLoose ) { timeLoose = newtime ; tdcLoose = newtdc ; wTdcLoose = i; } } 
		   else
		     {
		       if ( newtime > m_ZDDriftFunctionProd->getMaxLooseTime() ) 
			 { if ( newtime < timeHigh ) { timeHigh  = newtime ; tdcHigh = newtdc ; wTdcHigh = i; } }
		       else
			 {
			   if ( newtime < m_ZDDriftFunctionProd->getMinLooseTime() ) 
			     { if ( newtime > timeLow ) { timeLow = newtime ; tdcLow = newtdc ; wTdcLow = i; } }
			 }
		     }
		 }
	     }
	   if ( wTdcTight > -1 ) 
	     { 
	       time = timeTight; 
	       tdc  = tdcTight; 
	       wTdc = wTdcTight; 
	       qhit.set_windowNarrow() ;
               wintype = 1 ; 
               // m_statistics->counter(7,"Narrow Time"); 
	     }
	   else
	     {
	       if ( wTdcLoose > -1 ) 
		 { 
		   time = timeLoose; 
		   tdc  = tdcLoose; 
		   wTdc = wTdcLoose; 
		   qhit.set_windowWide() ; 
                   wintype = 2 ; 
		   //m_statistics->counter(8,"Wide Time");
		 }
	       else
		 {
		   if ( wTdcHigh > -1 ) 
		     { 
		       time = timeHigh; 
		       tdc  = tdcHigh; 
		       wTdc = wTdcHigh;
		       qhit.set_windowHigh() ; 
                       wintype = 3 ; 
		       //m_statistics->counter(9,"High Time");
		     }
		   else
		     {
		       if ( wTdcLow > -1 ) 
			 { 
			   time = timeLow; 
			   tdc  = tdcLow; 
			   wTdc = wTdcLow;
			   qhit.set_windowLow() ; 
                           wintype = 4 ; 
			   //m_statistics->counter(10,"Low Time");
			 }
		       else
			 {			  
                           //m_statistics->counter(11,"Time Window Error");
			   report( ERROR, kFacilityString )  << "Error: no valid window decision!" << endl;
			 }
		     }
		 }
	     }
           if ( charge > 0 && nTDCs == 1 ) 
	     {
               time += charge2TimeCorr(time, charge) ;
	     }
	   dva  = time2DriftSeries(time, layer) ;
	   qhit.set_good() ;
	   weight   =  resolution(layer,dva.distance());
           weight   =  1. / (weight*weight);

	   // Determine the earlier and later time
 	   for( i=0 ; i<nTDCs ; ++i ) 
	   {  

	     // If this is the TDC we're using for the hit, skip
	     if ( i == wTdc ) continue;
	     
	     // Get the time of this TDC
	     ZDDatum::DigiType   newtdc  = rawhit.digiTimes()[i];
	     Time newtime = tdc2Time(wire,newtdc)-tOffset;

	     // Compare to the time of the TDC we're using for the hit
	     float delta = time - newtime ;

	     // Is this hit earlier than the "standard" hit?
	     if ( delta > 0 )
	     { 
	       if ( delta < earlyGap || earlyGap < 0 ) 
	       { 
		 earlyGap = delta; 
	       } 
	     }
	     else 
	     { 
	       if ( -delta < lateGap || lateGap  < 0 ) 
	       { 
		 lateGap = -delta; 
	       } 
	     } // if ( delta > 0 )

	   }  // for ( over TDC's )

	 } // else from (if nTDC's == 0 )

     }  // if ( raw DAQId is ok )
   else
     {
       //m_statistics->counter(12,"test hits"); 
       qhit.set_testchannel();
     }
   if ( debugLevel[2] )
     {
       report( DEBUG, kFacilityString )  << "Wire#    " << wire           << endl
					 << "WireLyr  " << wireInLayer    << endl
					 << "Layer    " << layer          << endl   
					 << "Flag     " << qhit.value()   << endl
					 << "#TDC     " << nTDCs          << endl   
					 << "TDCused  " << tdc            << endl
					 << "charge   " << charge         << endl
					 << "time     " << time           << endl
					 << "drift    " << dva.distance() << endl
					 << "weight   " << weight         << endl
					 << "bunch    " << tOffset        << endl
					 << "wintype  " << wintype        << endl
					 << "earlyGap " << earlyGap       << endl
					 << "lateGap  " << lateGap        << endl
	                                 << "TDC#     " << wTdc           << endl;
     }
   return new CalibratedZDHit(wire, layer, wireInLayer, charge, time, dva, 
			      weight,qhit.value(), nTDCs, earlyGap, lateGap, wTdc, id);
}
//---------------------------------------------------------------------------
ZDDriftFunction::Drift ZDDriftFunction::time2Drift( Time time, 
						int layer ) const 
{
  return time2Drift ( time,  layer, kLRaveraged ) ;
}
//--------------------------------------------------------------------------
ZDDriftFunction::Time ZDDriftFunction::drift2Time( Drift drift, 
					       int layer ) const
{ 
  return drift2Time ( drift, layer, kLRaveraged ) ;
}
//---------------------------------------------------------------------------
Tseries ZDDriftFunction::time2DriftSeries( Time time, 
					 int layer ) const
{
  //
  // For now drift velocity and curvature are not set
  //
  Tseries DVA(time2Drift(time,layer,kLRaveraged),0.0,0.0);
  return DVA;
}
//---------------------------------------------------------------------------
ZDDriftFunction::Drift ZDDriftFunction::time2SignedDrift( Time time, 
						      int layer, 
						      int ambiguity,
						      float eAngle,
						      float zHit ) const
{
  if ( ambiguity < 0 )
    { 
      return -time2Drift( time, layer, krightside )
             -time2DriftCorr  ( time, layer, 0, eAngle, zHit ) ;
    }
  else
    { 
      return +time2Drift( time, layer, kleftside  )
             -time2DriftCorr  ( time, layer, 1, eAngle, zHit ) ;
    }
}
//---------------------------------------------------------------------------
ZDDriftFunction::Drift ZDDriftFunction::time2SignedDrift( Time time, 
						      int layer, 
						      int ambiguity) const
{
  if ( ambiguity < 0 )
    { 
      return -time2Drift( time, layer, krightside ) ; 
    }
  else
    { 
      return  time2Drift( time, layer, kleftside  ) ; 
    }
}
//--------------------------------------------------------------------------
ZDDriftFunction::Time ZDDriftFunction::signeddrift2Time( Drift drift, 
						     int layer,
						     float eAngle,
						     float zHit ) const
{
  return signeddrift2Time( drift, layer ) - 
         drift2TimeCorr  ( drift, layer, eAngle, zHit ) ;
}
//--------------------------------------------------------------------------
ZDDriftFunction::Time ZDDriftFunction::signeddrift2Time( Drift drift, 
						     int layer ) const 
{
  if ( drift < 0 )
    { return drift2Time( -drift, layer, krightside ) ; }
  else
    { return drift2Time( drift, layer, kleftside  ) ; }
}
