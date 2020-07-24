#if !defined(ASISTOREPRO_ASISTOREPROXY_H)
#define ASISTOREPRO_ASISTOREPROXY_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStoreProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Alexander Undrus
// Created:     Sat Jan  9 13:40:26 EST 1999
// $Id: ASiStoreProxy.h,v 1.2 1999/07/03 22:27:17 cleo3 Exp $
//
// Revision history
//
// $Log: ASiStoreProxy.h,v $
// Revision 1.2  1999/07/03 22:27:17  cleo3
//  ASiSensorAddress is transferred from ASiSensor package, ASiStorePro becomes producer, all references to ASiSensor are eliminated
//
// Revision 1.1.1.1  1999/01/23 02:28:03  undrus
// imported c3tr sources
//
//

// system include files

// user include files
#include "DataHandler/DataField.h"
#include "DataHandler/ProxyTemplate.h"
#include "ASiStorePro/ASiStorePro.h"
#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiStrip.h"
#include "ASiStorePro/ASiLayer.h"
#include "ASiStorePro/ASiLadder.h"
#include "ASiStorePro/ASiHybrid.h"
#include "ASiStorePro/ASiSensorInfo.h"
#include "ASiStorePro/ASiSensorAddress.h"

// forward declarations
class ASiStorePro;

class ASiStoreProxy : public ProxyTemplate< ASiStore >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ASiStore value_type;

      // Constructors and destructor
      ASiStoreProxy(const ASiStorePro& iProducer );
      virtual ~ASiStoreProxy();

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
      ASiStoreProxy( const ASiStoreProxy& );

      // assignment operator(s)
      const ASiStoreProxy& operator=( const ASiStoreProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_SiStore ;
      const ASiStorePro& m_producer;

      // static data members

};

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "package/Template/ASiStoreProxy.cc"
//#endif

#endif /* PACKAGE_ASISTOREPROXY_H */
