// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_PIONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_PIONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTPionFitDRHitLatticeProxy
//
/**\class DTUTPionFitDRHitLatticeProxy DTUTPionFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTPionFitDRHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTPionFitDRHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:54 mccann Exp $
//
// Revision history
//
// $Log: DTUTPionFitDRHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:54  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations

class DTUTPionFitDRHitLatticeProxy : public ProxyBindableTemplate< PionFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef PionFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTPionFitDRHitLatticeProxy();
      virtual ~DTUTPionFitDRHitLatticeProxy();

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
      DTUTPionFitDRHitLatticeProxy( const DTUTPionFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTPionFitDRHitLatticeProxy& operator=( const DTUTPionFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTPionFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_PionFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTPionFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_PIONFITDRHITLATTICEPROXY_H */
