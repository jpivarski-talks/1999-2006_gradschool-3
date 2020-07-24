// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavMuonId
// 
// Description: 
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Thu Jun  1 08:59:25 EDT 2000
// $Id: NavMuonId.cc,v 1.3 2002/08/06 17:09:23 cleo3 Exp $
//
// Revision history
//
// $Log: NavMuonId.cc,v $
// Revision 1.3  2002/08/06 17:09:23  cleo3
// removed use of FAItem to decrease dependencies
//
// Revision 1.2  2000/08/11 00:20:58  bkh
// Add operator<< functionality to these classes
//
// Revision 1.1  2000/06/06 18:38:44  bkh
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
#include "Navigation/NavMuonId.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavTkShMatch.h"
#include "C3Mu/MuTrack.h"

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

static const char* const kFacilityString = "Navigation.NavMuonId" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavMuonId.cc,v 1.3 2002/08/06 17:09:23 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

NavMuonId::NavMuonId( const NavTrack& aNavTrack ,
		      const MuTrack*  aMuTrack    ) :
   m_simple1 ( false    ) ,
   m_simple2 ( false    ) ,
   m_depth   ( 0        ) ,
   m_energy  ( 0        ) ,
   m_muTrack ( &(*aMuTrack) )
{
   if( aNavTrack.trackShowerMatch().valid() )
   {
      m_energy = aNavTrack.trackShowerMatch()->matchedEnergy() ;
   }

   if( 0 != m_muTrack )
   {
      if( 0 == m_muTrack->muqual() ) m_depth = m_muTrack->dpthmu() ;
   }

   FAItem< TDKinematicFit > iFit ( aNavTrack.pionFit() ) ;

   if( !iFit.valid() )
   {
      report( WARNING, kFacilityString ) << "No pion fit available!" << endl ;
   }

   const double mom ( !iFit.valid() ? 0 : iFit->momentum().mag() ) ;

   m_simple1 = ( ( 0 == m_energy      ) ||
		 ( 0 == depth()       ) ||
		 ( !muTrack().valid() ) ||
		 ( !iFit.valid()      )     ? false :
		 mom      > 1.0    &&
		 m_energy > 0.1    &&
		 m_energy < 0.6    &&
		 ( ( ( m_depth > 3. ) && ( mom <  1.5 ) ) ||
		   ( ( m_depth > 5. ) && ( mom >= 1.5 ) )    )  ) ;

   m_simple2 = ( ( 0 == m_energy      ) ||
		 ( 0 == depth()       ) ||
		 ( !muTrack().valid() ) ||
		 ( !iFit.valid()      )     ? false :
		 mom      > 1.0    &&
		 m_energy > 0.1    &&
		 m_energy < 0.6    &&
		 ( ( ( m_depth > 3. ) && ( mom <  2. ) ) ||
		   ( ( m_depth > 5. ) && ( mom >= 2. ) )    )  ) ;
}

// NavMuonId::NavMuonId( const NavMuonId& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//NavMuonId::~NavMuonId()
//{
//}

//
// assignment operators
//
// const NavMuonId& NavMuonId::operator=( const NavMuonId& rhs )
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
NavMuonId::simpleMuonId1()    const
{
   return m_simple1 ;
}

DABoolean 
NavMuonId::simpleMuonId2()    const
{
   return m_simple2 ;
}

Real      
NavMuonId::depth()           const
{
   return m_depth ;
}

CcGeV     
NavMuonId::matchedEnergy()   const
{
   return m_energy ;
}

FAItem< MuTrack > 
NavMuonId::muTrack()         const
{
   return m_muTrack ;
}


//
// static member functions
//
