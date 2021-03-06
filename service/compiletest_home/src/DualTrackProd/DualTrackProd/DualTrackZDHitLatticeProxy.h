// -*- C++ -*-
#if !defined(DUALTRACKPROD_DUALTRACKZDHITLATTICEPROXY_H)
#define DUALTRACKPROD_DUALTRACKZDHITLATTICEPROXY_H
//
// Package:     <DualTrackProd>
// Module:      DualTrackZDHitLatticeProxy
//
/**\class DualTrackZDHitLatticeProxy DualTrackZDHitLatticeProxy.h DualTrackProd/DualTrackZDHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:41:33 EST 2003
// $Id: DualTrackZDHitLatticeProxy.h,v 1.1 2003/12/15 17:51:44 mccann Exp $
//
// Revision history
//
// $Log: DualTrackZDHitLatticeProxy.h,v $
// Revision 1.1  2003/12/15 17:51:44  mccann
// adding hit lattices for dual-constrained residuals
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "DualTrackProd/DualTrackZDHitLink.h"
#include "CleoDB/DBCandidate.h"

// forward declarations

class DualTrackZDHitLatticeProxy : public ProxyBindableTemplate<DualTrackZDHitLattice>
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DualTrackZDHitLattice value_type;

      // Constructors and destructor
      DualTrackZDHitLatticeProxy(DBCandidate::Hypo hypo);
      virtual ~DualTrackZDHitLatticeProxy();

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
      DualTrackZDHitLatticeProxy( const DualTrackZDHitLatticeProxy& );

      // assignment operator(s)
      const DualTrackZDHitLatticeProxy& operator=( const DualTrackZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DualTrackZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_DualTrackZDHitLattice ;

      DBCandidate::Hypo m_hypo;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackProd/Template/DualTrackZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKPROD_DUALTRACKZDHITLATTICEPROXY_H */
