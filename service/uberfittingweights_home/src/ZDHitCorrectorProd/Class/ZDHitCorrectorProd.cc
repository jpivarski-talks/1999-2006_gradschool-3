// -*- C++ -*-
//
// Package:     <ZDHitCorrectorProd>
// Module:      ZDHitCorrectorProd
// 
// Description: 
//
// Implementation:
//
//
// Author:      Guangshun Huang
// Created:     Thu Feb 27 2003
// $Id: ZDHitCorrectorProd.cc,v 1.1.1.1 2003/08/30 00:17:08 wsun Exp $
//
// Revision history
//
// $Log: ZDHitCorrectorProd.cc,v $
// Revision 1.1.1.1  2003/08/30 00:17:08  wsun
// Imported ZDHitCorrectorProd sources.
//
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
#include "ZDHitCorrectorProd/ZDHitCorrectorProd.h"
#include "Experiment/report.h"
#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "DataHandler/ProxyFactoryFunction.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "ZDHitCorrectorProd/ZDHitCorrector.h"
#include "ZDHitCorrectorProd/ZDHitCorrectorProxy.h"
//
// STL classes
//
#include <vector>
#include <map>
//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ZDHitCorrectorProd.ZDHitCorrectorProd" ;
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ZDHitCorrectorProd.cc,v 1.1.1.1 2003/08/30 00:17:08 wsun Exp $";
static const char* const kTagString = "$Name:  $";
//---------------------------------------------------------------------------
// Constructor
//  (Default definition of standard corrections is given in HitCorrector.cc)
//
ZDHitCorrectorProd::ZDHitCorrectorProd( void )
   : Producer( "ZDHitCorrectorProd" ),
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
			    ZDHitCorrectorProd,ZDHitCorrectorProxy >
			 (*this,&ZDHitCorrectorProd::makeProxy),UsageTag());
}
//
// template instantiations for proxy factories--------------------------------
//
#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< ZDHitCorrectorProd, ZDHitCorrectorProxy >;
//
// ProxyFactoryFunction Producer Method---------------------------------------
//
ProxyBase* ZDHitCorrectorProd::makeProxy()
{  report( DEBUG, kFacilityString ) << " makeProxy" << endl;
   return ( m_ZDHitCorrectorProxy = 
	   new ZDHitCorrectorProxy( this ) ); }
//	   new ZDHitCorrectorProxy( m_CorrectionFlags.value() ) ); }
//
// Destructor-----------------------------------------------------------------
//
ZDHitCorrectorProd::~ZDHitCorrectorProd()                    
{ report( DEBUG, kFacilityString ) << "destructor" << endl; }
//
// ~anal1----------------------------------------------------------------------
// operations after new set of parameters before instantiation by proxy
//
void ZDHitCorrectorProd::init( void )
{ report( DEBUG, kFacilityString ) << " init" << endl; }
//
// ~anal5----------------------------------------------------------------------
// operations before new set of parameters
//
void ZDHitCorrectorProd::terminate( void )
{ report( DEBUG, kFacilityString ) << "here in terminate()" << endl; }
//
// book histograms-------------------------------------------------------------
//
void ZDHitCorrectorProd::hist_book( TBHistoManager& )
{ report( DEBUG, kFacilityString ) << "here in hist_book()" << endl; }
//
//-----------------------------------------------------------------------------
//
