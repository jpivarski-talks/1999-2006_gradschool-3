#if !defined(SIGEOM_SIGEOMEVSPROXY_H)
#define SIGEOM_SIGEOMEVSPROXY_H
// -*- C++ -*-
//
// Package:     SiGeom
// Module:      SiGeomEVSProxy
// 
// Description: Proxy to create a G3iExtraVolSpecsStore for SiGeom
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Mon Jun 14 10:02:48 EDT 1999
// $Id: SiGeomEVSProxy.h,v 1.3 2000/11/15 21:53:16 cdj Exp $
//
// Revision history
//
// $Log: SiGeomEVSProxy.h,v $
// Revision 1.3  2000/11/15 21:53:16  cdj
// now inherits from ProxyTemplate instead of ProxyBindableTemplate
//
// Revision 1.2  1999/07/27 22:37:04  lkg
// tweaks for SunOs/CC compilation
//
// Revision 1.1  1999/06/29 19:42:18  lkg
// Add Si3 Geant3 extra volume spec's information and associated proxy
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"

// Geant 3 Interface headers
#include "Geant3Interface/G3iTrackingMedium.h"
#include "Geant3Interface/G3iSensitiveVolume.h"
#include "Geant3Interface/G3iExtraVolSpecs.h"
#include "Geant3Interface/G3iExtraVolSpecsStore.h"
#include "Geant3Interface/G3iTrackingMediaStore.h"
#include "Geant3Interface/G3iTrackingMediaIds.h"

// forward declarations

class SiGeomEVSProxy : public ProxyTemplate<G3iExtraVolSpecsStore>
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef G3iExtraVolSpecsStore value_type;

      // Constructors and destructor
      SiGeomEVSProxy();
      virtual ~SiGeomEVSProxy();

      // member functions

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
      SiGeomEVSProxy( const SiGeomEVSProxy& );

      // assignment operator(s)
      const SiGeomEVSProxy& operator=( const SiGeomEVSProxy& );

      // private member functions

      // private member functions
      G3iExtraVolSpecsStore&
      buildEVSS(const G3iTrackingMediaStore& aMedia);

      // data members
      value_type* m_G3iExtraVolSpecsStore ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "package/Template/SiGeomEVSProxy.cc"
//#endif

#endif /* SIGEOM_SIGEOMEVSPROXY_H */


