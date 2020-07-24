// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      NavKs
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      David Urner
// Created:     Tue Mar 20 10:50:08 EST 2001
// $Id: NavKs.cc,v 1.2 2002/11/07 17:42:01 bkh Exp $
//
// Revision history
//
// $Log: NavKs.cc,v $
// Revision 1.2  2002/11/07 17:42:01  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.1  2001/03/26 20:13:45  urner
// added Nav Ks
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
#include "Navigation/NavKs.h"
#include "Navigation/NavTrack.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavKs" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavKs.cc,v 1.2 2002/11/07 17:42:01 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavKs::NavKs(const VXFitVeeKShort* aKs ,
	     const NavTrack* aPiPlus ,
	     const NavTrack* aPiMinus   ) :
  m_Ks      ( aKs     ) ,
  m_PiPlus  ( aPiPlus ) ,
  m_PiMinus ( aPiMinus) ,
  m_mcTag   ( 0       )
{
   assert( 0 != m_Ks     ) ;
   assert( 0 != m_PiPlus ) ;
   assert( 0 != m_PiMinus) ;
}

// NavKs::NavKs( const NavKs& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavKs::~NavKs()
{
}

//
// assignment operators
//
// const NavKs& NavKs::operator=( const NavKs& rhs )
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

NavKs::Identifier       
NavKs::identifier() const { return m_Ks->identifier() ; }

const VXFitVeeKShort&    
NavKs::kShort()        const { return *m_Ks ; }

const NavTrack& 
NavKs::piPlus() const { return *m_PiPlus ; }

const NavTrack& 
NavKs::piMinus() const { return *m_PiMinus ; }


//
// const member functions
//

//
// static member functions
//
ostream& operator<<( ostream& os, const NavKs& aKs ) 
{
   const VXFitVeeKShort& kshort ( aKs.kShort() ) ;
   os << aKs ;
   //   os << "  " << aKs.piPlus().attributes() ;
   //   os << "  " << aKs.piMinus().attributes() ;
   return os ;
}
