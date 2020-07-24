// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITELECTRONFITDRHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITELECTRONFITDRHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitElectronFitDRHitLatticeProxy
//
/**\class DTUTExitElectronFitDRHitLatticeProxy DTUTExitElectronFitDRHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitElectronFitDRHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitElectronFitDRHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:54 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitElectronFitDRHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:54  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations

class DTUTExitElectronFitDRHitLatticeProxy : public ProxyBindableTemplate< ExitElectronFitDRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitElectronFitDRHitLattice value_type;

      // Constructors and destructor
      DTUTExitElectronFitDRHitLatticeProxy();
      virtual ~DTUTExitElectronFitDRHitLatticeProxy();

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
      DTUTExitElectronFitDRHitLatticeProxy( const DTUTExitElectronFitDRHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitElectronFitDRHitLatticeProxy& operator=( const DTUTExitElectronFitDRHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitElectronFitDRHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitElectronFitDRHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitElectronFitDRHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITELECTRONFITDRHITLATTICEPROXY_H */
