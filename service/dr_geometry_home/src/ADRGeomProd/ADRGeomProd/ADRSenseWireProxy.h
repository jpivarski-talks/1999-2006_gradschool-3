#if !defined(ADRGEOMPROD_ADRSENSEWIREPROXY_H)
#define ADRGEOMPROD_ADRSENSEWIREPROXY_H
// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRSenseWireProxy
// 
// Description: <one line class summary>
//
// Usage:       
//    <usage>
//
// Author:      Inga Karliner
// Created:     9/22/98
//
// Revision history
//
// First submission
//
//

// system include files

// user include files
#include "DataHandler/DataField.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "ADRGeomProd/ADRSenseWireStoreFactory.h"

// forward declarations

class ADRSenseWireProxy : public DataField< ADRSenseWireStore >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ADRSenseWireStore value_type;

      // Constructors and destructor
      ADRSenseWireProxy();
      virtual ~ADRSenseWireProxy();

      // member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const IfdKey& aKey );

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ADRSenseWireProxy( const ADRSenseWireProxy& );

      // assignment operator(s)
      const ADRSenseWireProxy& operator=( const ADRSenseWireProxy& );

      // private member functions

      // private const member functions

      // data members
  //value_type* m_ADRSenseWireStore ;
      ADRSenseWireStoreFactory* m_ADRSenseWireStoreFactory ;
      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeomProd/Template/ADRSenseWireProxy.cc"
//#endif

#endif /* ADRGEOMPROD_ADRSenseWireProxy_H */

