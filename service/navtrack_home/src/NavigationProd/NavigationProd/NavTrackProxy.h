#if !defined(NAVIGATIONPROD_NAVTRACKPROXY_H)
#define NAVIGATIONPROD_NAVTRACKPROXY_H
// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavTrackProxy
// 
// Description: Proxy to "handle" NavTrack
//
// Usage:
//    <usage>
//
// Author:      Michael Marsh
// Created:     Wed Jul  8 15:36:15 EDT 1998
// $Id: NavTrackProxy.h,v 1.2 2000/09/26 21:28:44 lkg Exp $
//
// Revision history
//
// $Log: NavTrackProxy.h,v $
// Revision 1.2  2000/09/26 21:28:44  lkg
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
// Revision 1.1.1.1  1999/07/19 16:47:45  bkh
// imported NavigationProd sources
//
// Revision 1.3  1998/12/31 18:12:18  marsh
// NavTrack and related files have been moved to new library Navigation.
//
// Revision 1.2  1998/12/19 21:38:54  marsh
// Fixed statements for new proxy system.
//
// Revision 1.1.1.1  1998/07/13 23:26:20  marsh
// New library for user-level analysis objects
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "Navigation/NavTrack.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */

class NavigationProd;

class NavTrackProxy : public ProxyTemplate< FAPtrTable< NavTrack > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< NavTrack > value_type ;

      // Constructors and destructor
      NavTrackProxy( NavigationProd* theCallingProducer );
      virtual ~NavTrackProxy();

      // member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      NavTrackProxy( const NavTrackProxy& );

      // assignment operator(s)
      const NavTrackProxy& operator=( const NavTrackProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type*      m_ptrTable ;
      NavTrackMaster*  m_masterBlock ;
      NavigationProd*  m_navigationProd;

      // static data members

};

// inline function definitions

#endif /* NAVIGATIONPROD_NAVTRACKPROXY_H */
