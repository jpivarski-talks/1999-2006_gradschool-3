// -*- C++ -*-
#if !defined(DUALTRACKPROD_DUALTRACKFITHELIXPROXY_H)
#define DUALTRACKPROD_DUALTRACKFITHELIXPROXY_H
//
// Package:     <DualTrackProd>
// Module:      DualTrackFitHelixProxy
//
/**\class DualTrackFitHelixProxy DualTrackFitHelixProxy.h DualTrackProd/DualTrackFitHelixProxy.h
 
 Description: Proxy to "handle" DualTrackFitHelix

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:18:32 EST 2003
// $Id: DualTrackFitHelixProxy.h,v 1.1 2003/12/15 17:51:43 mccann Exp $
//
// Revision history
//
// $Log: DualTrackFitHelixProxy.h,v $
// Revision 1.1  2003/12/15 17:51:43  mccann
// adding hit lattices for dual-constrained residuals
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "DualTrackProd/DualTrackFitHelix.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DualTrackFitHelixProxy : public ProxyBindableTemplate< FAPtrTable< DualTrackFitHelix > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< DualTrackFitHelix > value_type ;

      // Constructors and destructor
      DualTrackFitHelixProxy();
      virtual ~DualTrackFitHelixProxy();

      // member functions

      // use this form of a function when calling 'bind'
      // void boundMethod( const Record& iRecord );

      // const member functions

      // static member functions

   protected:
      // protected member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // protected const member functions

   private:
      // Constructors and destructor
      DualTrackFitHelixProxy( const DualTrackFitHelixProxy& );

      // assignment operator(s)
      const DualTrackFitHelixProxy& operator=( const DualTrackFitHelixProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DualTrackFitHelixProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackProd/Template/DualTrackFitHelixProxy.cc"
//#endif

#endif /* DUALTRACKPROD_DUALTRACKFITHELIXPROXY_H */
