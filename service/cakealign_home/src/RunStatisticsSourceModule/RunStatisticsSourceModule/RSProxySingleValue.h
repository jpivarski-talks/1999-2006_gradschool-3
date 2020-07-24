#if !defined(RUNSTATISTICSSOURCEMODULE_RSPROXYSINGLEVALUE_H)
#define RUNSTATISTICSSOURCEMODULE_RSPROXYSINGLEVALUE_H
// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RSProxySingleValue
// 
/**\class RSProxySingleValue RSProxySingleValue.h RunStatisticsSourceModule/RSProxySingleValue.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Fri Jan 28 16:13:26 EST 2000
// $Id: RSProxySingleValue.h,v 1.6 2002/02/15 17:53:50 hubert Exp $
//
// Revision history
//
// $Log: RSProxySingleValue.h,v $
// Revision 1.6  2002/02/15 17:53:50  hubert
// Use methods instead of static variables for type codes to avoid dependency on static constructor run order
//
// Revision 1.5  2000/11/07 03:02:34  lkg
// modifed to handle component names lacking slashes
//
// Revision 1.4  2000/05/08 19:52:14  cdj
// now compiles on OSF with MICO
//
// Revision 1.3  2000/02/29 22:39:26  cdj
// fixed use of strcmp
//
// Revision 1.2  2000/02/02 22:32:15  cdj
// added some commented out debug output
//
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files
#include <string.h>

// user include files
#include "RunStatisticsSourceModule/RSProxy.h"

// forward declarations
template <class ReturnT, class CorbaInfoT >
class RSDefaultConverter
{
   public:
      static ReturnT convert(const typename CorbaInfoT::Type& iValue) { return iValue; } 
};

template <class ReturnT, class CorbaInfoT, class Converter >
class RSProxySingleValue : public RSProxy<ReturnT>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RSProxySingleValue(RSProxyDeliverer::StreamToRSMap::value_type* iPair,
			 const char* iFieldName,
			 const ReturnT& iDefaultValue ) 
	 : RSProxy<ReturnT>( iPair), m_fieldName(iFieldName), m_data(iDefaultValue){}
      //virtual ~RSProxySingleValue();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      void invalidateCache() {
      }

      const ReturnT* implementFaultHandler(const Record&,
					   const DataKey& ) {
	 ReturnT* returnValue = 0;
	 for( int index=0; index < runStatistics()->rsdata.length(); ++index ){
	    //this is useful for debugging
	   //report(INFO, "RSProxySingleValue") << runStatistics()->rsdata[index].name << endl;
	    //first check to see if it is the correct type
	    CORBA::TypeCode_var typeCode = 
	       runStatistics()->rsdata[index].value.type();
	    if( typeCode->equal( CorbaInfoT::typeCode() ) ) {
	       //now see if the name is correct
	       // skip the component and path and go straight to field name
	       const char* fieldName = 
		  strrchr(runStatistics()->rsdata[index].name,'/');
	       if ( fieldName == 0 ) {
		  fieldName = runStatistics()->rsdata[index].name;
	       } else {
		  fieldName += 1;
	       }
	       if( 0 == strcmp(fieldName, m_fieldName) ) {
		  typename CorbaInfoT::Type temp;
		  runStatistics()->rsdata[index].value >>= temp;
		  m_data = Converter::convert( temp );
		  returnValue = &m_data;
		  break;
	       }
	    }
	 }
	 return returnValue;
      }

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSProxySingleValue( const RSProxySingleValue& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSProxySingleValue& operator=( const RSProxySingleValue& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      ReturnT m_data;
      const char* m_fieldName;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSProxySingleValue.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSPROXYSINGLEVALUE_H */
