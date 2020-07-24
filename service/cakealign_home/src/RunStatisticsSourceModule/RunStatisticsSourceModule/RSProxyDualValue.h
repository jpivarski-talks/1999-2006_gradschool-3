#if !defined(RUNSTATISTICSSOURCEMODULE_RSPROXYDUALVALUE_H)
#define RUNSTATISTICSSOURCEMODULE_RSPROXYDUALVALUE_H
// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RSProxyDualValue
// 
/**\class RSProxyDualValue RSProxyDualValue.h RunStatisticsSourceModule/RSProxyDualValue.h

 Description: A Proxy that retrieves two values from RunStatistics

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Tue Feb 29 08:56:18 EST 2000
// $Id: RSProxyDualValue.h,v 1.3 2002/02/15 17:53:50 hubert Exp $
//
// Revision history
//
// $Log: RSProxyDualValue.h,v $
// Revision 1.3  2002/02/15 17:53:50  hubert
// Use methods instead of static variables for type codes to avoid dependency on static constructor run order
//
// Revision 1.2  2000/05/08 19:52:13  cdj
// now compiles on OSF with MICO
//
// Revision 1.1  2000/02/29 22:39:53  cdj
// first submission
//

// system include files
#include <string.h>

// user include files
#include "RunStatisticsSourceModule/RSProxy.h"

// forward declarations

template <class ReturnT, class CorbaInfoT, class Converter >
class RSProxyDualValue : public RSProxy<ReturnT>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RSProxyDualValue(RSProxyDeliverer::StreamToRSMap::value_type* iPair,
		       const char* iField1Name,
		       const char* iField2Name,
		       const ReturnT& iDefaultValue ) :
	 RSProxy<ReturnT>(iPair), 
	 m_field1Name(iField1Name),
	 m_field2Name(iField2Name),
	 m_data(iDefaultValue){}
      //virtual ~RSProxyDualValue();

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
	 //temporary variables to hold values
	 typename CorbaInfoT::Type field1Value;
	 typename CorbaInfoT::Type field2Value;
	 unsigned char numberOfFoundFields = 0;

	 for( int index=0; index < runStatistics()->rsdata.length(); ++index ){
//this is useful for debugging
//	    report(INFO, "RSProxySingleValue") << runStatistics()->rsdata[index].name << endl;
	    //first check to see if it is the correct type
	    CORBA::TypeCode_var typeCode = 
	       runStatistics()->rsdata[index].value.type();
	    if( typeCode->equal( CorbaInfoT::typeCode() ) ) {

	       //now see if the name is correct
	       // skip the component and path and go straight to field name
	       const char* fieldName = 
		  strrchr(runStatistics()->rsdata[index].name,'/')+1;
	       if( 0 == strcmp(fieldName, m_field1Name) ) {
		  runStatistics()->rsdata[index].value >>= field1Value;
		  if( 2 == ++numberOfFoundFields ) {
		     break;
		  }
	       }
	       if( 0 == strcmp(fieldName, m_field2Name) ) {
		  runStatistics()->rsdata[index].value >>= field2Value;
		  if( 2 == ++numberOfFoundFields ) {
		     break;
		  }
	       }
	    }
	 }
	 if( 2 == numberOfFoundFields ) {
	    m_data = ReturnT( Converter::convert( field1Value ),
			      Converter::convert( field2Value ) );
	    returnValue = &m_data;
	 }
	 return returnValue;
      }

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSProxyDualValue( const RSProxyDualValue& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSProxyDualValue& operator=( const RSProxyDualValue& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      ReturnT m_data;
      const char* m_field1Name;
      const char* m_field2Name;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSProxyDualValue.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSPROXYDUALVALUE_H */
