// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavTrackProxy
// 
// Description: Proxy for providing NavTracks
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Wed Jul  8 15:36:16 EDT 1998
// $Id: NavTrackProxy.cc,v 1.8 2001/01/25 03:07:15 cdj Exp $
//
// Revision history
//
// $Log: NavTrackProxy.cc,v $
// Revision 1.8  2001/01/25 03:07:15  cdj
// fixed memory leak that only happens at end of job
//
// Revision 1.7  2000/10/03 15:47:29  lkg
// tweaks for cxx compatibility
//
// Revision 1.6  2000/09/26 21:28:33  lkg
// An partial implementation of specification of seed track and fit track
// usage and production tags.  For track-only use, the implementation is
// complete.  No effort has been made to enforce the same track tags
// for clusters/rich/muons/etc (or to allow specification of
// prod/usage tag information for that information).
// * add parameters to NavigationProd for specification of the finder and
// fitter usage and production tags
// * add accessor functions to NavigationProd to access the tags
// * pass a pointer to (the calling) NavigationProd to the NavTrackProxy
// constructor, so it may access the tag information
// * create the NavTrack master block using the prod/usage tag information
//
// Revision 1.5  2000/04/17 20:13:37  bkh
// Elim checks of ptr after new
//
// Revision 1.4  1999/12/31 23:15:17  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.3  1999/11/17 20:37:20  marsh
// The master block was being deleted in faultHandler().  This has been
// moved to invalidateCache().
//
// Revision 1.2  1999/09/06 13:28:22  marsh
// Changed return condition for invalid anchor table.
//
// Revision 1.1.1.1  1999/07/19 16:47:46  bkh
// imported NavigationProd sources
//
// Revision 1.4  1998/12/31 18:12:27  marsh
// NavTrack and related files have been moved to new library Navigation.
//
// Revision 1.3  1998/12/29 22:20:57  marsh
// Mods for providing Lattice info.
//
// Revision 1.2  1998/12/19 21:38:32  marsh
// Fixed statements for new proxy system.
//
// Revision 1.1.1.1  1998/07/13 23:26:21  marsh
// New library for user-level analysis objects
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <set>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"

#include "NavigationProd/NavTrackProxy.h"
#include "NavigationProd/NavigationProd.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavTrackMaster.h"

// STL classes
#include <map>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "NavigationProd.NavTrackProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//
NavTrackProxy::NavTrackProxy( NavigationProd* theCallingProducer)
   : m_ptrTable( new value_type ) ,
     m_masterBlock( 0 ),
     m_navigationProd( theCallingProducer )
{
}

// NavTrackProxy::NavTrackProxy( const NavTrackProxy& iproxy )
// {
//   *this = iproxy;
// }

NavTrackProxy::~NavTrackProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
   delete m_masterBlock;
}

//
// assignment operators
//
// const NavTrackProxy& NavTrackProxy::operator=( const NavTrackProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//
void
NavTrackProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
   if ( 0 != m_masterBlock )
   {
      delete m_masterBlock;
      m_masterBlock = 0;
   }
}

const NavTrackProxy::value_type*
NavTrackProxy::faultHandler( const Record& iRecord,
                             const DataKey& iKey )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // ---------- fill FAPtrTable with new items ----------------

   // this typedef here for convenience
   typedef FATable< NavTrack::Anchor > AnchorTable;

   // get the table out of the dictionary
   AnchorTable table;
   extract( iRecord, table ); 

   assert( 0 == m_masterBlock );
   m_masterBlock = new NavTrackMaster( iRecord,
				       m_navigationProd->fitterTags(),
				       m_navigationProd->finderTags() );

   AnchorTable::const_iterator tableEnd( table.end() );
   for ( AnchorTable::const_iterator item( table.begin() ) ;
         item != tableEnd ;
         ++item )
   {
      NavTrack* tmp( new NavTrack( item->identifier() ,
                                   *m_masterBlock ) );

      m_ptrTable->insert( tmp );
   }
   
   return ( m_ptrTable ) ;
}

//
// const member functions
//

//
// static member functions
//


