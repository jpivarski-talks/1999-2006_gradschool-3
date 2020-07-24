// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      NavLambda
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      David Urner
// Created:     Tue Mar 20 10:50:08 EST 2001
// $Id: NavLambda.cc,v 1.2 2001/11/05 18:13:50 ajm36 Exp $
//
// Revision history
//
// $Log: NavLambda.cc,v $
// Revision 1.2  2001/11/05 18:13:50  ajm36
// use pion charge to determine lambda flavor instead of proton
//
// Revision 1.1  2001/09/07 21:50:56  eckhart
// Created File
//
// Revision 1.1  2001/03/26 20:13:45  urner
// added Nav Lambda
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#include <assert.h>

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavLambda.h"
#include "Navigation/NavTrack.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavLambda" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavLambda.cc,v 1.2 2001/11/05 18:13:50 ajm36 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavLambda::NavLambda(const VXFitVeeLambda* aLambda ,
	     const NavTrack* aProton ,
	     const NavTrack* aPion   ) :
  m_Lambda      ( aLambda     ) ,
  m_Proton  ( aProton ) ,
  m_Pion ( aPion)
{
   assert( 0 != m_Lambda     ) ;
   assert( 0 != m_Proton ) ;
   assert( 0 != m_Pion) ;
   m_isbar = ( aPion->pionFit()->charge() == +1 );
}

// NavLambda::NavLambda( const NavLambda& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavLambda::~NavLambda()
{
}

//
// assignment operators
//
// const NavLambda& NavLambda::operator=( const NavLambda& rhs )
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

NavLambda::Identifier       
NavLambda::identifier() const { return m_Lambda->identifier() ; }

const VXFitVeeLambda&    
NavLambda::lambda()        const { return *m_Lambda ; }

const NavTrack& 
NavLambda::proton() const { return *m_Proton ; }

const NavTrack& 
NavLambda::pion() const { return *m_Pion ; }


//
// const member functions
//

//
// static member functions
//
ostream& operator<<( ostream& os, const NavLambda& aLambda ) 
{
   const VXFitVeeLambda& Lambda ( aLambda.lambda() ) ;
   os << aLambda ;
   //   os << "  " << aLambda.Poton().attributes() ;
   //   os << "  " << aLambda.Pion().attributes() ;
   return os ;
}


















