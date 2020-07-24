// -*- C++ -*-
#if !defined(DUALTRACKPROD_DUALTRACKDRHITLATTICEPROXY_H)
#define DUALTRACKPROD_DUALTRACKDRHITLATTICEPROXY_H
//
// Package:     <DualTrackProd>
// Module:      DualTrackDRHitLatticeProxy
//
/**\class DualTrackDRHitLatticeProxy DualTrackDRHitLatticeProxy.h DualTrackProd/DualTrackDRHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:41:33 EST 2003
// $Id: DualTrackDRHitLatticeProxy.h,v 1.1 2003/12/15 17:51:39 mccann Exp $
//
// Revision history
//
// $Log: DualTrackDRHitLatticeProxy.h,v $
// Revision 1.1  2003/12/15 17:51:39  mccann
// adding hit lattices for dual-constrained residuals
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "DualTrackProd/DualTrackDRHitLink.h"
#include "CleoDB/DBCandidate.h"

// forward declarations

class DualTrackDRHitLatticeProxy : public ProxyBindableTemplate<DualTrackDRHitLattice>
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DualTrackDRHitLattice value_type;

      // Constructors and destructor
      DualTrackDRHitLatticeProxy(DBCandidate::Hypo hypo);
      virtual ~DualTrackDRHitLatticeProxy();

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
      DualTrackDRHitLatticeProxy( const DualTrackDRHitLatticeProxy& );

      // assignment operator(s)
      const DualTrackDRHitLatticeProxy& operator=( const DualTrackDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DualTrackDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_DualTrackDRHitLattice ;

      DBCandidate::Hypo m_hypo;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackProd/Template/DualTrackDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKPROD_DUALTRACKDRHITLATTICEPROXY_H */
