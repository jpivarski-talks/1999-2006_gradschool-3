// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaToGG
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:08:15 EDT 1999
// $Id: NavEtaToGG.cc,v 1.2 2003/02/03 20:16:15 bkh Exp $
//
// Revision history
//
// $Log: NavEtaToGG.cc,v $
// Revision 1.2  2003/02/03 20:16:15  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
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
#include "Navigation/NavEtaToGG.h"
#include "Navigation/NavShower.h"

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

static const char* const kReport = "Navigation.NavEtaToGG" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavEtaToGG.cc,v 1.2 2003/02/03 20:16:15 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

NavEtaToGG::NavEtaToGG( const PhdEtaToGG* aEta  ,
			const NavShower*  aHiEn ,
			const NavShower*  aLoEn   ) :
   m_eta ( aEta  ) ,
   m_hi  ( aHiEn ) ,
   m_lo  ( aLoEn ) ,
   m_mcTag ( 0 ) 
{
   assert( 0 != m_eta ) ;
   assert( 0 != m_hi  ) ;
   assert( 0 != m_lo  ) ;
}

// NavEtaToGG::NavEtaToGG( const NavEtaToGG& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavEtaToGG::~NavEtaToGG()
{
}

//
// assignment operators
//
// const NavEtaToGG& NavEtaToGG::operator=( const NavEtaToGG& rhs )
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

NavEtaToGG::Identifier       
NavEtaToGG::identifier() const { return m_eta->identifier() ; }

const PhdEtaToGG&    
NavEtaToGG::eta()        const { return *m_eta ; }

const NavShower& 
NavEtaToGG::hiEnShower() const { return *m_hi ; }

const NavShower& 
NavEtaToGG::loEnShower() const { return *m_lo ; }

//
// static member functions
//


ostream& operator<<( ostream& os, const NavEtaToGG& aEta ) 
{
   const PhdEtaToGG& phd ( aEta.eta() ) ;
   os << phd 
      << "  " << aEta.hiEnShower().attributes()
      << "  " << aEta.loEnShower().attributes() ;
   return os ;
}
