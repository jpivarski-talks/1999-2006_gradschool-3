#if !defined(RUNSTATISTICSSOURCEMODULE_RSPROXYDUALFACTORY_H)
#define RUNSTATISTICSSOURCEMODULE_RSPROXYDUALFACTORY_H
// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RSProxyDualFactory
// 
/**\class RSProxyDualFactory RSProxyDualFactory.h RunStatisticsSourceModule/RSProxyDualFactory.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Tue Feb 29 12:45:57 EST 2000
// $Id: RSProxyDualFactory.h,v 1.3 2003/03/03 20:47:13 cleo3 Exp $
//
// Revision history
//
// $Log: RSProxyDualFactory.h,v $
// Revision 1.3  2003/03/03 20:47:13  cleo3
// no longer return local value of TypeTag
//
// Revision 1.2  2000/05/08 19:52:13  cdj
// now compiles on OSF with MICO
//
// Revision 1.1  2000/02/29 22:39:52  cdj
// first submission
//

// system include files

// user include files
#include "RunStatisticsSourceModule/RSProxyInterfaceFactoryBase.h"
#include "DataHandler/RecordMethods.h"

// forward declarations

template< class T>
class RSProxyDualFactory : public RSProxyInterfaceFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef typename T::value_type value_type;

      // ---------- Constructors and destructor ----------------
      RSProxyDualFactory(const char* iField1Name,
			 const char* iField2Name,
			 const value_type& iDefaultValue ) :
	 m_field1Name( iField1Name ),
	 m_field2Name( iField2Name ),
	 m_defaultValue(iDefaultValue) {}
      //virtual ~RSProxyDualFactory();

      // ---------- member functions ---------------------------
      virtual RSProxyInterface* make(
	 RSProxyDeliverer::StreamToRSMap::value_type* iStreamRSPair ) {
	   return new T( iStreamRSPair,
			 m_field1Name,
			 m_field2Name,
			 m_defaultValue );
      }
      
      // ---------- const member functions ---------------------
      virtual TypeTag typeTag() const{
	 return RecordMethods<typename T::value_type>::makeKey("","").type();      
      }
      
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSProxyDualFactory( const RSProxyDualFactory& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSProxyDualFactory& operator=( const RSProxyDualFactory& ); // stop default

      // ---------- private member functions -------------------
      
      // ---------- private const member functions -------------
      
      // ---------- data members -------------------------------
      const char* m_field1Name;
      const char* m_field2Name;
      value_type m_defaultValue;
      
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSProxyDualFactory.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSPROXYDUALFACTORY_H */
