// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 11:04:40 EST 2003
// $Id: DualTrackProd.cc,v 1.3 2003/12/16 22:16:22 mccann Exp $
//
// Revision history
//
// $Log: DualTrackProd.cc,v $
// Revision 1.3  2003/12/16 22:16:22  mccann
// Turned on ZD lattices and fixed a Linux compilation bug.
//
// Revision 1.2  2003/12/15 17:53:11  mccann
// adding hit lattices for dual-constrained residuals
//
// Revision 1.1.1.1  2003/12/11 17:14:28  mccann
// imported DualTrackProd sources
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DualTrackProd/DualTrackProd.h"
#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DualTrackProd.DualTrackProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackProd.cc,v 1.3 2003/12/16 22:16:22 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackProd::DualTrackProd( void )               // anal1
   : Producer( "DualTrackProd" )
   , m_massHypothesis("massHypo", this, "pion")
   , m_virtualPhotonPx("vphoPx", this, 0.015)  // appropriate for early cleo-c runs
   , m_virtualPhotonPy("vphoPy", this, 0.)
   , m_virtualPhotonPz("vphoPz", this, 0.)     // only for asymmetric beams!
   , m_pointConstraint("constrainPoint", this, true)
   , m_ptConstraint("constrainPT", this, true)
   , m_pzConstraint("constrainPZ", this, true)
   , m_printOutP("printOutP", this, false)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   m_massHypothesis.setHelpString("Mass hypothesis to extract for tracks for dual track constraint.\nMust be one of (electron, muon, pion, kaon, proton, exitElectron, exitMuon, exitPion, exitKaon, exitProton).\nThis is irrelevant if you\'re using ChisqFitProd, rather than KalmanProd.");
   m_virtualPhotonPx.setHelpString("X-Momentum of virtual photon == p_x used in PT constraint (ignored if \"constrainPT\" is false).\nThis is non-zero because of the crossing angle.\nTo measure, turn on \"printOutP\".");
   m_virtualPhotonPy.setHelpString("Y-Momentum of virtual photon == p_y used in PT constraint (ignored if \"constrainPT\" is false).\nThis should be zero because the crossing angle is in the X direction.");
   m_virtualPhotonPz.setHelpString("Z-Momentum of virtual photon == p_z used in PZ constraint (ignored if \"constrainPZ\" is false).\nThis should be zero because CESR collides symmetric beams.");
   m_pointConstraint.setHelpString("With this option, DualTrackProd will provide tracks that are forced to originate at the same point in space.\n(Constrains d0 and z0.  May be used with \"constrainPT\" and \"constrainPZ\".)");
   m_ptConstraint.setHelpString("With this option, DualTrackProd will provide tracks that are forced to be back-to-back in RPHI.\n(Constrains phi0 and curvature.  May be used with \"constrainPoint\" and \"constrainPZ\".)");
   m_pzConstraint.setHelpString("With this option, DualTrackProd will provide tracks that are forced to be back-to-back in Z.\n(Constrains cotTheta and curvature.  May be used with \"constrainPoint\" and \"constrainPT\".)");
   m_printOutP.setHelpString("This will print Px, Py, Pz to the INFO stream for each usable event so that you can measure vphoPx, vphoPy, vphoPz for your particular dataset.");

   // Proxy Registration Step 1:
   registerProxyFactory(Stream::kEvent, 
                        new ProxyFactory<DualTrackFitHelixProxy>, 
                        UsageTag());    // or UsageTag("MyUsage")

   registerProxyFactory(Stream::kEvent, 
			new ProxyFactoryFunction<DualTrackProd, DualTrackProxy>(
			   *this, &DualTrackProd::makeDualTrackProxy),
			UsageTag());    // or UsageTag("MyUsage")

   registerProxyFactory(Stream::kEvent, 
			new ProxyFactoryFunction<DualTrackProd, DualTrackDRHitLatticeProxy>(
			   *this, &DualTrackProd::makeDualTrackDRHitLatticeProxy),
			UsageTag());    // or UsageTag("MyUsage")

   registerProxyFactory(Stream::kEvent, 
			new ProxyFactoryFunction<DualTrackProd, DualTrackZDHitLatticeProxy>(
			   *this, &DualTrackProd::makeDualTrackZDHitLatticeProxy),
			UsageTag());    // or UsageTag("MyUsage")
}

// Proxy Registration Step 2:
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory<DualTrackFitHelixProxy>;
#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction<DualTrackProd, DualTrackProxy>;
template class ProxyFactoryFunction<DualTrackProd, DualTrackDRHitLatticeProxy>;
template class ProxyFactoryFunction<DualTrackProd, DualTrackZDHitLatticeProxy>;

DBCandidate::Hypo DualTrackProd::selectedHypo()
{
   DBCandidate::Hypo massHypothesis = DBCandidate::kChargedPion;
   if (m_massHypothesis.value() == string("electron")) massHypothesis = DBCandidate::kElectron;
   else if (m_massHypothesis.value() == string("muon")) massHypothesis = DBCandidate::kMuon;
   else if (m_massHypothesis.value() == string("pion")) massHypothesis = DBCandidate::kChargedPion;
   else if (m_massHypothesis.value() == string("kaon")) massHypothesis = DBCandidate::kChargedKaon;
   else if (m_massHypothesis.value() == string("proton")) massHypothesis = DBCandidate::kProton;
   else if (m_massHypothesis.value() == string("exitElectron")) massHypothesis = DBCandidate::kExitElectron;
   else if (m_massHypothesis.value() == string("exitMuon")) massHypothesis = DBCandidate::kExitMuon;
   else if (m_massHypothesis.value() == string("exitPion")) massHypothesis = DBCandidate::kExitPion;
   else if (m_massHypothesis.value() == string("exitKaon")) massHypothesis = DBCandidate::kExitKaon;
   else if (m_massHypothesis.value() == string("exitProton")) massHypothesis = DBCandidate::kExitProton;
   else {
      report(ERROR, kFacilityString)
	 << "The massHypo \"" << m_massHypothesis.value()
	 << "\" is not one of (electron, muon, pion, kaon, proton, exitElectron, exitMuon, exitPion, exitKaon, exitProton)." << endl
	 << "I will extract \"pion\" hypotheses." << endl;
   }
   return massHypothesis;
}

ProxyBase*
DualTrackProd::makeDualTrackProxy()
{ 
   return new DualTrackProxy(selectedHypo(),
			     HepVector3D(m_virtualPhotonPx.value(),
					 m_virtualPhotonPy.value(),
					 m_virtualPhotonPz.value()),
			     m_pointConstraint.value(),
			     m_ptConstraint.value(),
			     m_pzConstraint.value(),
			     m_printOutP.value());
}

ProxyBase*
DualTrackProd::makeDualTrackDRHitLatticeProxy()
{ 
   return new DualTrackDRHitLatticeProxy(selectedHypo());
}

ProxyBase*
DualTrackProd::makeDualTrackZDHitLatticeProxy()
{ 
   return new DualTrackZDHitLatticeProxy(selectedHypo());
}

// DualTrackProd::DualTrackProd( const DualTrackProd& iDualTrackProd )
// {
//    *this = iDualTrackProd;
// }

DualTrackProd::~DualTrackProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const DualTrackProd& DualTrackProd::operator=( const DualTrackProd& iDualTrackProd )
// {
//   if( this != &iDualTrackProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iDualTrackProd );
//   }
//
//   return *this;
// }

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
DualTrackProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DualTrackProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
DualTrackProd::hist_book( HIHistoManager& )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

//
// const member functions
//

//
// static member functions
//
