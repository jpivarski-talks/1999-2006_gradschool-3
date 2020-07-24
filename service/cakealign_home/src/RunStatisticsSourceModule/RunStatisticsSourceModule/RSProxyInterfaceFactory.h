#if !defined(RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACEFACTORY_H)
#define RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACEFACTORY_H
// -*- C++ -*-
//
// Package:     RunStatisticsSourceModule
// Module:      RSProxyInterfaceFactory
// 
/**\class RSProxyInterfaceFactory RSProxyInterfaceFactory.h RunStatisticsSourceModule/RSProxyInterfaceFactory.h

 Description: templated Factory class to create specific instances of class
              that inherit from RSProxy

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Fri Jan 28 08:50:12 EST 2000
// $Id: RSProxyInterfaceFactory.h,v 1.3 2003/03/03 20:47:14 cleo3 Exp $
//
// Revision history
//
// $Log: RSProxyInterfaceFactory.h,v $
// Revision 1.3  2003/03/03 20:47:14  cleo3
// no longer return local value of TypeTag
//
// Revision 1.2  2000/05/08 19:52:14  cdj
// now compiles on OSF with MICO
//
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files

// user include files
#include "RunStatisticsSourceModule/RSProxyInterfaceFactoryBase.h"
#include "DataHandler/RecordMethods.h"

// forward declarations

template< class T>
class RSProxyInterfaceFactory : public RSProxyInterfaceFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef typename T::value_type value_type;

      // ---------- Constructors and destructor ----------------
      RSProxyInterfaceFactory(const char* iFieldName,
			      const value_type& iDefaultValue )
	 : m_fieldName(iFieldName), m_defaultValue(iDefaultValue) {}
      //virtual ~RSProxyInterfaceFactory();

      // ---------- member functions ---------------------------
      virtual RSProxyInterface* make(
	 RSProxyDeliverer::StreamToRSMap::value_type* iStreamRSPair ) {
	   return new T( iStreamRSPair,
			 m_fieldName,
			 m_defaultValue );
      }

      // ---------- const member functions ---------------------
      virtual TypeTag typeTag() const{
	 return RecordMethods<typename T::value_type>::makeKey(UsageTag(),ProductionTag()).type();      
      }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSProxyInterfaceFactory( const RSProxyInterfaceFactory& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSProxyInterfaceFactory& operator=( const RSProxyInterfaceFactory& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const char* m_fieldName;
      value_type m_defaultValue;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSProxyInterfaceFactory.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACEFACTORY_H */
