// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITPIONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITPIONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitPionFitZDHitLatticeProxy
//
/**\class DTUTExitPionFitZDHitLatticeProxy DTUTExitPionFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitPionFitZDHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitPionFitZDHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitPionFitZDHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:55  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitZDHitLink.h"

// forward declarations

class DTUTExitPionFitZDHitLatticeProxy : public ProxyBindableTemplate< ExitPionFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitPionFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTExitPionFitZDHitLatticeProxy();
      virtual ~DTUTExitPionFitZDHitLatticeProxy();

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
      DTUTExitPionFitZDHitLatticeProxy( const DTUTExitPionFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitPionFitZDHitLatticeProxy& operator=( const DTUTExitPionFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitPionFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitPionFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitPionFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITPIONFITZDHITLATTICEPROXY_H */
