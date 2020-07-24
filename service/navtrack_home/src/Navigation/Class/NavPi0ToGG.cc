// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavPi0ToGG
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:08:15 EDT 1999
// $Id: NavPi0ToGG.cc,v 1.4 2002/11/07 17:42:01 bkh Exp $
//
// Revision history
//
// $Log: NavPi0ToGG.cc,v $
// Revision 1.4  2002/11/07 17:42:01  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.3  2001/11/13 16:05:34  bkh
// Separated off pi0 pieces to avoid making everyone link to PhotonDecays
//
// Revision 1.2  1999/08/20 17:26:47  bkh
// Fix up operator<< for pi0's
//
// Revision 1.1  1999/08/09 16:25:08  bkh
// Massive renaming
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavPi0ToGG.h"
#include "Navigation/NavShower.h"
#include "PhotonDecays/PhdPi0.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kReport = "Navigation.NavPi0ToGG" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavPi0ToGG.cc,v 1.4 2002/11/07 17:42:01 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

NavPi0ToGG::NavPi0ToGG( const PhdPi0*    aPi0  ,
			const NavShower* aHiEn ,
			const NavShower* aLoEn   ) :
   m_pi0 ( aPi0  ) ,
   m_hi  ( aHiEn ) ,
   m_lo  ( aLoEn ) ,
   m_mcTag ( 0 ) 
{
   assert( 0 != m_pi0 ) ;
   assert( 0 != m_hi  ) ;
   assert( 0 != m_lo  ) ;
}

// NavPi0ToGG::NavPi0ToGG( const NavPi0ToGG& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavPi0ToGG::~NavPi0ToGG()
{
}

//
// assignment operators
//
// const NavPi0ToGG& NavPi0ToGG::operator=( const NavPi0ToGG& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

NavPi0ToGG::Identifier       
NavPi0ToGG::identifier() const { return m_pi0->identifier() ; }

const PhdPi0&    
NavPi0ToGG::pi0()        const { return *m_pi0 ; }


const NavShower& 
NavPi0ToGG::hiEnShower() const { return *m_hi ; }

const NavShower& 
NavPi0ToGG::loEnShower() const { return *m_lo ; }

//
// static member functions
//


ostream& operator<<( ostream& os, const NavPi0ToGG& aPi0 ) 
{
   const PhdPi0& phd ( aPi0.pi0() ) ;
   os << phd ;
   os << "  " << aPi0.hiEnShower().attributes() ;
   os << "  " << aPi0.loEnShower().attributes() ;
   return os ;
}
