// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITPROTONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITPROTONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitProtonFitZDHitLatticeProxy
//
/**\class DTUTExitProtonFitZDHitLatticeProxy DTUTExitProtonFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitProtonFitZDHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitProtonFitZDHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitProtonFitZDHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:55  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitZDHitLink.h"

// forward declarations

class DTUTExitProtonFitZDHitLatticeProxy : public ProxyBindableTemplate< ExitProtonFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitProtonFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTExitProtonFitZDHitLatticeProxy();
      virtual ~DTUTExitProtonFitZDHitLatticeProxy();

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
      DTUTExitProtonFitZDHitLatticeProxy( const DTUTExitProtonFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitProtonFitZDHitLatticeProxy& operator=( const DTUTExitProtonFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitProtonFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitProtonFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitProtonFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITPROTONFITZDHITLATTICEPROXY_H */
