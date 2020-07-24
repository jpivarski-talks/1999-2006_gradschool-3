// -*- C++ -*-
//
// Package:     <DRHitCorrectorProd>
// Module:      DRHitCorrectorProd
// 
// Description: 
//
// Implementation:
//
//
// Author:      Ken McLean
// Created:     Wed Jul 28 16:40:55 EDT 1999
// $Id: DRHitCorrectorProd.cc,v 1.9 2001/02/26 00:57:19 cdj Exp $
//
// Revision history
//
// $Log: DRHitCorrectorProd.cc,v $
// Revision 1.9  2001/02/26 00:57:19  cdj
// extract MagneticField from StartRun so move Proxy to StartRun as well
//
// Revision 1.8  2000/12/13 22:33:12  urner
// change default of standard correction parameter and entrance angle parameter to true
//
// Revision 1.7  2000/10/31 22:56:27  mclean
// CorrectedDRHit* used to "make" CorrectedDRHit's, we never actually used
// these, relying on storing the corrected values in link data, so now
// its gone forever!
//
// Deleted sigPropBeta parameter from DRHitCorrectorProd,HitCorrector
// and HitCorrectorProxy, this now comes from CDOffCal via the call to
// ZCorr added to HitCorrector.
//
// In HitCorrector.cc, correctedDriftTimeAndDist is overloaded with
// one additional argument the resolution, if this form of the
// function is called by the track fitter then the corrected resolution
// will be returned (fitting weight implimentation).
//
// The code is slightly reorganized and the user is blessed with rare
// informational messages about  which corrections are being applied.
//
// Additional error checking for failed track swims is included.
// The old drift function call is replaced by time2SignedDrift which
// will, if available return values for a two sided drift function.
// Less arguments are required since CDOffCal access the geometry from
// the frame internally.
//
// If, as when entrance angle corrections are enabled,  a 4th and 5th argument
// is added (entrance angle and zhit) then entrance angle corrections are added
// inside CDOffCal by overloading time2SignedDrift.
//
// Revision 1.6  2000/08/16 20:38:17  wsun
// Moved HitCorrector proxy from drcalibration to beginrun stream.
//
// Revision 1.5  2000/06/29 00:04:24  wsun
// Added parameter for signal propagation speed in DR wires.
//
// Revision 1.4  2000/06/27 03:16:07  wsun
// Added parameters for controlling each correction individually.
//
// Revision 1.3  2000/06/22 14:27:42  cdj
// removed TBThreeVector/Point, made constant variables nanames begin with k and removed unnecessary includes
//
// Revision 1.2  1999/08/21 14:37:03  mclean
// Add corrections for flight time and signal propagation over sense wires.
//
//
#include "Experiment/Experiment.h"
//
// system include files
//
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
// 
// user include files
//
#include "DRHitCorrectorProd/DRHitCorrectorProd.h"
#include "Experiment/report.h"
#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "DataHandler/ProxyFactoryFunction.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "DRHitCorrectorProd/HitCorrector.h"
#include "DRHitCorrectorProd/HitCorrectorProxy.h"
//
// STL classes
//
#include <vector>
#include <map>
//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DRHitCorrectorProd.DRHitCorrectorProd" ;
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRHitCorrectorProd.cc,v 1.9 2001/02/26 00:57:19 cdj Exp $";
static const char* const kTagString = "$Name:  $";
//---------------------------------------------------------------------------
// Constructor
//  (Default definition of standard corrections is given in HitCorrector.cc)
//
DRHitCorrectorProd::DRHitCorrectorProd( void )
   : Producer( "DRHitCorrectorProd" ),
     m_applyStandardCorrections( "ApplyStandardCorrections"  , this, true  ),
     m_disableAllCorrections   ( "DisableAllCorrections"     , this, false ),
     m_applySignalPropCorr     ( "ApplySignalPropagationCorr", this, true  ),
     m_applyTimeOfFlightCorr   ( "ApplyTimeOfFlightCorr"     , this, true  ),
     m_applyPulseHeightCorr    ( "ApplyPulseHeightCorr"      , this, true  ),
     m_applyEntranceAngleCorr  ( "ApplyEntranceAngleCorr"    , this, true  )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   m_applyStandardCorrections.setHelpString(
      string("ApplyStandardCorrections overrides all other parameters\n") +
      string("EXCEPT DisableAllCorrections. I.e. if\n")+
      string("ApplyStandardCorrections is true, then ONLY the standard\n") +
      string("corrections will be applied, unless DisableAllCorrections is\n")+
      string("also true, in which case, no corrections will be applied.\n") ) ;

   m_disableAllCorrections.setHelpString(
      string("DisableAllCorrections overrides ALL other parameters.  I.e.\n") +
      string("despite how the other parameters are set, no corrections\n") +
      string("will be applied if DisableAllCorrections is true.\n") ) ;

   m_applySignalPropCorr.setHelpString(
      string("This parameter will be ignored unless DisableAllCorrections\n")+
      string("and ApplyStandardCorrections are BOTH false!!!\n") ) ;
   m_applyTimeOfFlightCorr.setHelpString(
      string("This parameter will be ignored unless DisableAllCorrections\n")+
      string("and ApplyStandardCorrections are BOTH false!!!\n") ) ;
   m_applyPulseHeightCorr.setHelpString(
      string("This parameter will be ignored unless DisableAllCorrections\n")+
      string("and ApplyStandardCorrections are BOTH false!!!\n") ) ;
   m_applyEntranceAngleCorr.setHelpString(
      string("This parameter will be ignored unless DisableAllCorrections\n")+
      string("and ApplyStandardCorrections are BOTH false!!!\n") ) ;

   registerProxyFactory( Stream::kStartRun, 
			 new ProxyFactoryFunction<
			    DRHitCorrectorProd,HitCorrectorProxy >
			 (*this,&DRHitCorrectorProd::makeProxy),UsageTag());
}
//
// template instantiations for proxy factories--------------------------------
//
#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< DRHitCorrectorProd, HitCorrectorProxy >;
//
// ProxyFactoryFunction Producer Method---------------------------------------
//
ProxyBase* DRHitCorrectorProd::makeProxy()
{  report( DEBUG, kFacilityString ) << " makeProxy" << endl;
   return ( m_HitCorrectorProxy = 
	   new HitCorrectorProxy( this ) ); }
//	   new HitCorrectorProxy( m_CorrectionFlags.value() ) ); }
//
// Destructor-----------------------------------------------------------------
//
DRHitCorrectorProd::~DRHitCorrectorProd()                    
{ report( DEBUG, kFacilityString ) << "destructor" << endl; }
//
// ~anal1----------------------------------------------------------------------
// operations after new set of parameters before instantiation by proxy
//
void DRHitCorrectorProd::init( void )
{ report( DEBUG, kFacilityString ) << " init" << endl; }
//
// ~anal5----------------------------------------------------------------------
// operations before new set of parameters
//
void DRHitCorrectorProd::terminate( void )
{ report( DEBUG, kFacilityString ) << "here in terminate()" << endl; }
//
// book histograms-------------------------------------------------------------
//
void DRHitCorrectorProd::hist_book( TBHistoManager& )
{ report( DEBUG, kFacilityString ) << "here in hist_book()" << endl; }
//
//-----------------------------------------------------------------------------
//
