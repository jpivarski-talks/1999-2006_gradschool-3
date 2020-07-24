#if !defined(ADRGEOMPROD_ADRCATHODEPROXY_H)
#define ADRGEOMPROD_ADRCATHODEPROXY_H
// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRCathodeProxy
// 
// Description: <one line class summary>
//
// Usage:       
//    <usage>
//
// Author:      Inga Karliner
// Created:     5/10/99
//
// Revision history
// 
// First submission
// 12/1/99 I.Karliner New library ADRGeomProd (was in ADRGeom)
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DataHandler/DataField.h"
#include "ADRGeom/ADRCathodeStore.h"
#include "ADRGeomProd/ADRCathodeStoreFactory.h"

// forward declarations

class ADRCathodeProxy : public DataField< ADRCathodeStore >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ADRCathodeStore value_type;



      // Constructors and destructor
      ADRCathodeProxy();
      virtual ~ADRCathodeProxy();

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
      ADRCathodeProxy( const ADRCathodeProxy& );

      // assignment operator(s)
      const ADRCathodeProxy& operator=( const ADRCathodeProxy& );

      // private member functions

      // private const member functions

      // data members
      ADRCathodeStoreFactory* m_ADRCathodeStoreFactory ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeomProd/Template/ADRCathodeProxy.cc"
//#endif

#endif /* ADRGEOMPROD_ADRCathodeProxy_H */

