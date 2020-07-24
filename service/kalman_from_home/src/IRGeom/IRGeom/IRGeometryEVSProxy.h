#if !defined(IRGEOM_IRGEOMETRYEVSPROXY_H)
#define IRGEOM_IRGEOMETRYEVSPROXY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      IRGeometryEVSProxy
// 
// Description: Creates the ultimate extra vol specs store for IRGeom
//
// Usage:
//    This proxy combines all EVS stores from separate components. No EVS 
//    defined in this module.
//
// Author:      Aren Jansen
// Created:     Tue Jun 15 17:45:34 EDT 1999
// $Id: IRGeometryEVSProxy.h,v 1.2 2000/11/15 21:58:45 cdj Exp $
//
// Revision history
//
// $Log: IRGeometryEVSProxy.h,v $
// Revision 1.2  2000/11/15 21:58:45  cdj
// proxies no longer inherit from ProxyBindableTemplate to aid static linking
//
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
// First release
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"

// Geant3Interface headers
#include "Geant3Interface/G3iTrackingMedium.h"
#include "Geant3Interface/G3iSensitiveVolume.h"
#include "Geant3Interface/G3iExtraVolSpecs.h"
#include "Geant3Interface/G3iExtraVolSpecsStore.h"
#include "Geant3Interface/G3iTrackingMediaStore.h"
#include "Geant3Interface/G3iTrackingMediaIds.h"

// forward declarations

class IRGeometryEVSProxy : public ProxyTemplate<G3iExtraVolSpecsStore >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef G3iExtraVolSpecsStore value_type;

      // Constructors and destructor
      IRGeometryEVSProxy();
      virtual ~IRGeometryEVSProxy();

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
      IRGeometryEVSProxy( const IRGeometryEVSProxy& );

      // assignment operator(s)
      const IRGeometryEVSProxy& operator=( const IRGeometryEVSProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_G3iExtraVolSpecsStore ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "IRGeom/Template/IRGeometryEVSProxy.cc"
//#endif

#endif /* IRGEOM_IRGEOMETRYEVSPROXY_H */


