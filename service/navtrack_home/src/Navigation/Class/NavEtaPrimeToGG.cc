// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaPrimeToGG
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:08:15 EDT 1999
// $Id: NavEtaPrimeToGG.cc,v 1.1 2001/05/10 19:55:03 bkh Exp $
//
// Revision history
//
// $Log: NavEtaPrimeToGG.cc,v $
// Revision 1.1  2001/05/10 19:55:03  bkh
// Addition of new etaPrime class
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
#include "Navigation/NavEtaPrimeToGG.h"
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

static const char* const kReport = "Navigation.NavEtaPrimeToGG" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavEtaPrimeToGG.cc,v 1.1 2001/05/10 19:55:03 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

NavEtaPrimeToGG::NavEtaPrimeToGG( const PhdEtaPrimeToGG* aEtaPrime  ,
				  const NavShower*       aHiEn      ,
				  const NavShower*       aLoEn        ) :
   m_etaPrime ( aEtaPrime  ) ,
   m_hi       ( aHiEn      ) ,
   m_lo       ( aLoEn      )
{
   assert( 0 != m_etaPrime ) ;
   assert( 0 != m_hi  ) ;
   assert( 0 != m_lo  ) ;
}

// NavEtaPrimeToGG::NavEtaPrimeToGG( const NavEtaPrimeToGG& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavEtaPrimeToGG::~NavEtaPrimeToGG()
{
}

//
// assignment operators
//
// const NavEtaPrimeToGG& NavEtaPrimeToGG::operator=( const NavEtaPrimeToGG& rhs )
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

NavEtaPrimeToGG::Identifier       
NavEtaPrimeToGG::identifier() const { return m_etaPrime->identifier() ; }

const PhdEtaPrimeToGG&    
NavEtaPrimeToGG::etaPrime()   const { return *m_etaPrime ; }

const NavShower& 
NavEtaPrimeToGG::hiEnShower() const { return *m_hi ; }

const NavShower& 
NavEtaPrimeToGG::loEnShower() const { return *m_lo ; }

//
// static member functions
//


ostream& operator<<( ostream& os, const NavEtaPrimeToGG& aEtaPrime ) 
{
   const PhdEtaPrimeToGG& phd ( aEtaPrime.etaPrime() ) ;
   os << phd 
      << "  " << aEtaPrime.hiEnShower().attributes()
      << "  " << aEtaPrime.loEnShower().attributes() ;
   return os ;
}
