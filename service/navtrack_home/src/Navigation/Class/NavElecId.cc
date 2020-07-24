// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavElecId
// 
// Description: 
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Thu Jun  1 08:59:30 EDT 2000
// $Id: NavElecId.cc,v 1.3 2002/03/21 20:06:45 ajm36 Exp $
//
// Revision history
//
// $Log: NavElecId.cc,v $
// Revision 1.3  2002/03/21 20:06:45  ajm36
// fix const bug in NavElecId::sDedx
//
// Revision 1.2  2002/03/20 15:02:24  ajm36
// implement lazy evaluation in electron id
//
// Revision 1.1  2000/06/06 18:38:43  bkh
// Install dedx, electron id, and muon id interfaces
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavElecId.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavTkShMatch.h"
#include "DedxInfo/DedxInfo.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavElecId" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavElecId.cc,v 1.3 2002/03/21 20:06:45 ajm36 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavElecId::NavElecId( const NavTrack& aNavTrack ) :
   m_simple ( kUninitialized ) ,
   m_logLik ( -99.  ) ,
   m_eOverP ( -1    ) ,
   m_dedxPtr( 0     ) ,
   m_track  ( aNavTrack )
{
}

// NavElecId::NavElecId( const NavElecId& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//
// assignment operators
//
// const NavElecId& NavElecId::operator=( const NavElecId& rhs )
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

DABoolean 
NavElecId::simpleElecId() const
{
   if( m_simple == kUninitialized)
   {
      //rely on short circuit evaluation here
      //if E/p cut fails (including no shower match), we won't ask for
      //dE/dx, and might avoid ever asking for electron fits
      DABoolean temp = ( ( eOverP() > 0.85 )  && 
                         ( eOverP() < 1.1  )  &&
                         ( sDedx().valid()  ) &&
                         ( (*sDedx()) > -2. )   );
      const_cast<NavElecId*>(this)->m_simple = ( temp ? kTrue : kFalse );
   }
		
   return m_simple;
} 

Real      
NavElecId::logLik()       const
{
   return m_logLik ;
}

CcReal    
NavElecId::eOverP()       const
{
   if( m_eOverP < 0 )
   {
      if( m_track.trackShowerMatch().valid() )
      {
         const_cast<NavElecId*>(this)->m_eOverP =
            (*(m_track.trackShowerMatch())).eOverP() ;
      }
      else
      {
         const_cast<NavElecId*>(this)->m_eOverP = 0;
      }
   }
   return m_eOverP ;
}

FAItem< float > 
NavElecId::sDedx()  const
{
   if( m_dedxPtr == 0 )
   {
      if( m_track.dedxInfo().valid() )
      {
         const_cast<NavElecId*>(this)->m_dedx =
            (*(m_track.dedxInfo())).eSigma();
         const_cast<NavElecId*>(this)->m_dedxPtr = 
	    &(const_cast<NavElecId*>(this)->m_dedx);
      }
   }
   return FAItem< float > ( m_dedxPtr ) ;
}

//
// static member functions
//
