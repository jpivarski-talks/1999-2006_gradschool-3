// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrack
// 
// Description: User-level navigation object for tracks.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:31:23 EDT 1998
// $Id: NavTrack.cc,v 1.18 2003/10/08 16:23:04 urner Exp $
//
// Revision history
//
// $Log: NavTrack.cc,v $
// Revision 1.18  2003/10/08 16:23:04  urner
// included ZD lattice links
//
// Revision 1.17  2003/02/04 18:13:08  bkh
// Separate off record function
//
// Revision 1.16  2003/02/03 20:16:18  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.15  2002/11/07 17:42:03  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.14  2002/08/06 17:34:58  cleo3
// NavTrackMaster rather than NavTrack now owns any memory inorder to decrease dependencies seen on Solaris 8
//
// Revision 1.13  2002/03/19 20:25:05  cdj
// simplified memory handling for NavTrack
//
// Revision 1.12  2001/11/09 20:26:40  bkh
// Added in shower-->pi0  functionality
//
// Revision 1.11  2001/10/26 21:53:54  bkh
// Add features to showers and connected region objects
//
// Revision 1.10  2000/06/07 21:53:02  llh14
// Deleted 'm_seed' member data in destructor to plug memory leaks
//
// Revision 1.9  2000/06/06 18:38:44  bkh
// Install dedx, electron id, and muon id interfaces
//
// Revision 1.8  2000/04/05 15:42:59  marsh
// Split class implementations into separate files based on closely-related
// functions.  This will greatly reduce the amount of linking that most
// users will have to do.
//
// Revision 1.7  1999/11/17 20:34:19  marsh
// Lattice information is now returned with pointers instead of references.
// Track-shower matches are returned as FAItems instead of pointers.
// There is no more hypothesis faking for missing data.
//
// Revision 1.6  1999/10/06 22:16:20  marsh
// Removed "void" from argument lists.
//
// Revision 1.5  1999/09/16 20:38:11  bkh
// Add track shower match info
//
// Revision 1.4  1999/05/08 00:45:13  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.3  1999/01/19 21:27:58  marsh
// Added tests on caches before returning.  If the caches are still
// empty, return references to empty tables.
//
// Revision 1.2  1998/12/31 22:27:46  marsh
// Release memory that was dynamically allocated.
//
// Revision 1.1.1.1  1998/12/31 18:18:31  marsh
// New library for navigation objects.
//
// Revision 1.2  1998/12/29 22:20:55  marsh
// Mods for providing Lattice info.
//
// Revision 1.1.1.1  1998/07/13 23:26:21  marsh
// New library for user-level analysis objects
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavMuonId.h"
#include "Navigation/NavElecId.h"
#include "Navigation/NavTrackMaster.h"
#include "DataHandler/Record.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrack" ;

//
// static data member definitions
//

//
// constructors and destructor
//
#define ZEROARRAY(_array_) memset(_array_,0,sizeof(_array_))
NavTrack::NavTrack( const NavTrack::Identifier& aId ,
                    const NavTrackMaster& aMaster ) :
   TDTrack( aId , aMaster ) ,
   m_seedDRHitLinks( 0 ) ,             // fill the cache with 0
   m_seedZDHitLinks( 0 ) ,             // fill the cache with 0
   m_seedSVRHitLinks( 0 ) ,            // fill the cache with 0
   m_seedSVZHitLinks( 0 ) ,            // fill the cache with 0
   m_seedDRHitTable( 0 ) ,             // fill the cache with 0
   m_seedZDHitTable( 0 ) ,             // fill the cache with 0
   m_seedSVRHitTable( 0 ) ,            // fill the cache with 0
   m_seedSVZHitTable( 0 ),             // fill the cache with 0
   m_master ( aMaster ) ,
   m_elecId ( 0 ) ,
   m_muonId ( 0 ) ,
   m_bremShowers ( 0 ) ,
   m_navKs ( 0 ) ,
   m_navLambda ( 0 )
{
   ZEROARRAY(m_drHitLinks);
   ZEROARRAY(m_zdHitLinks);
   ZEROARRAY(m_svrHitLinks);
   ZEROARRAY(m_svzHitLinks);
   ZEROARRAY(m_drHitTables);
   ZEROARRAY(m_zdHitTables);
   ZEROARRAY(m_svrHitTables);
   ZEROARRAY(m_svzHitTables);
}

template<class T>
inline
void
const_delete(const T* iPtr)
{
   delete const_cast<T*>(iPtr);
}

template<class T>
inline
void
deletememory(T* iBegin, T* iEnd)
{
   for( T* iter = iBegin; iter != iEnd; ++iter) {
      if( 0 != *iter) {
	 const_delete( *iter );
      }
   }
}
#define CLEARARRAY(_array_) \
  deletememory(_array_, _array_+sizeof(_array_)/sizeof(void*))

NavTrack::~NavTrack()
{
   CLEARARRAY(m_drHitLinks);
   CLEARARRAY(m_zdHitLinks);
   CLEARARRAY( m_svrHitLinks );
   CLEARARRAY( m_svzHitLinks );
   CLEARARRAY( m_drHitTables );
   CLEARARRAY( m_zdHitTables );
   CLEARARRAY( m_svrHitTables );
   CLEARARRAY( m_svzHitTables );

   // cast away `const'
   delete (SeedDRHitLinkTable*)(m_seedDRHitLinks);
   delete (SeedZDHitLinkTable*)(m_seedZDHitLinks);
   delete (SeedSVRHitLinkTable*)(m_seedSVRHitLinks);
   delete (SeedSVZHitLinkTable*)(m_seedSVZHitLinks);
   delete (DRHitTable*)(m_seedDRHitTable);
   delete (ZDHitTable*)(m_seedZDHitTable);
   delete (SVRHitTable*)(m_seedSVRHitTable);
   delete (SVZHitTable*)(m_seedSVZHitTable);

   delete m_elecId ;
   delete m_muonId ;
}
