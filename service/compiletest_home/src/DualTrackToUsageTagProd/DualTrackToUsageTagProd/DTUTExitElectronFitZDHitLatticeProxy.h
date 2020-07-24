// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_EXITELECTRONFITZDHITLATTICEPROXY_H)
#define DUALTRACKTOUSAGETAGPROD_EXITELECTRONFITZDHITLATTICEPROXY_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitElectronFitZDHitLatticeProxy
//
/**\class DTUTExitElectronFitZDHitLatticeProxy DTUTExitElectronFitZDHitLatticeProxy.h DualTrackToUsageTagProd/DTUTExitElectronFitZDHitLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitElectronFitZDHitLatticeProxy.h,v 1.1.1.1 2003/12/17 21:00:54 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitElectronFitZDHitLatticeProxy.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:54  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFitter/TrackFitZDHitLink.h"

// forward declarations

class DTUTExitElectronFitZDHitLatticeProxy : public ProxyBindableTemplate< ExitElectronFitZDHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ExitElectronFitZDHitLattice value_type;

      // Constructors and destructor
      DTUTExitElectronFitZDHitLatticeProxy();
      virtual ~DTUTExitElectronFitZDHitLatticeProxy();

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
      DTUTExitElectronFitZDHitLatticeProxy( const DTUTExitElectronFitZDHitLatticeProxy& );

      // assignment operator(s)
      const DTUTExitElectronFitZDHitLatticeProxy& operator=( const DTUTExitElectronFitZDHitLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DTUTExitElectronFitZDHitLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ExitElectronFitZDHitLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackToUsageTagProd/Template/DTUTExitElectronFitZDHitLatticeProxy.cc"
//#endif

#endif /* DUALTRACKTOUSAGETAGPROD_EXITELECTRONFITZDHITLATTICEPROXY_H */