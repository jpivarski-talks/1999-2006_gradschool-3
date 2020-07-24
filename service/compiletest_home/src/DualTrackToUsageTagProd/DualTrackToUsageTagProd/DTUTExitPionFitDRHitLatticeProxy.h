// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITPIONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITPIONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitPionFitDRHitLatticeProxy
//
/**\class DTUTExitPionFitDRHitLatticeProxy DTUTExitPionFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitPionFitDRHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitPionFitDRHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitPionFitDRHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:55  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations

class DTUTExitPionFitDRHitLatticeProxy : public ProxyBindableTemplate< ExitPionFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitPionFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTExitPionFitDRHitLatticeProxy();
      virtual ~DTUTExitPionFitDRHitLatticeProxy();

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
      DTUTExitPionFitDRHitLatticeProxy( const DTUTExitPionFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitPionFitDRHitLatticeProxy& operator=( const DTUTExitPionFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitPionFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitPionFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitPionFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITPIONFITDRHITLATTICEPROXY_H */
