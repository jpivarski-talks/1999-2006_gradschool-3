#if !defined(DATAHANDLER_DHPROXYERROREXCEPTION_H)
#define DATAHANDLER_DHPROXYERROREXCEPTION_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      DHProxyErrorException
// 
/**\class DHProxyErrorException DHProxyErrorException.h DataHandler/DHProxyErrorException.h

 Description: An exception that is thrown whenever a Proxy had a problem with
              its algorithm.

 Usage:
    This exception will be thrown automatically if a a class that inherits from
    ProxyTemplate<> returns 0 from its fault handler.

    If you wish to explain the reason for the error, you can throw a 
    DHProxyErrorException from within your Proxy
    E.g.
    \code
    if( outOfBoundsValue ) {
      throw DHProxyErrorException<value_type>(string(kFacilityString) +
                                              " value out of bounds",
                                              iRecord.stream(),
					      iDataKey );
    }
    \endcode

*/
//
// Author:      Chris D Jones
// Created:     Tue Dec  7 13:54:09 EST 1999
// $Id: DHProxyErrorException.h,v 1.1 1999/12/10 14:10:09 cdj Exp $
//
// Revision history
//
// $Log: DHProxyErrorException.h,v $
// Revision 1.1  1999/12/10 14:10:09  cdj
// first submission
//

// system include files
#include <string>

// user include files
#include "DAException/DAException.h"
#include "DataHandler/Stream.h"
#include "DataHandler/DurableDataKey.h"
#include "HeteroContainer/HCTypeTagTemplate.h"

// forward declarations

template <class T>
class DHProxyErrorException : public DAExceptionBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DHProxyErrorException( const Stream::Type& iStream,
			 const DataKey& iDataKey ) : 
	 m_stream( iStream ),
	 m_dataKey( iDataKey ),
	 m_message(),
         m_builtMessage(false){}
      DHProxyErrorException( const string& iMessage, 
			     const Stream::Type& iStream,
			     const DataKey& iDataKey ) : 
	 m_stream( iStream ),
	 m_dataKey( iDataKey ),
	 m_message(iMessage),
         m_builtMessage(false){}

      //virtual ~DHProxyErrorException();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual const char* what () const { 
	 if( ! m_builtMessage ) {
	    const_cast<DHProxyErrorException<T>*>(this)->m_builtMessage = true;
	    //should make m_message mutable
	    string& message = 
	       const_cast<DHProxyErrorException<T>*>(this)->m_message;

	    message = string("Proxy error while extracting ") 
	       +string("\"")
	         +string(HCTypeTagTemplate<T,DataKey>::className()) 
	       +string("\" ")
	       +string("\"")
	         +string(m_dataKey.usage().value() )
	       +string("\" ")
	       +string("\"")
	         +string(m_dataKey.production().value() )
	       +string("\" ")
	       +string("in Record ")
	       +string(m_stream.value())
	       +"\n" + m_message;
	 }
	 return m_message.c_str();
      }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //DHProxyErrorException( const DHProxyErrorException& ); // allow default

      // ---------- assignment operator(s) ---------------------
      //const DHProxyErrorException& operator=( const DHProxyErrorException& ); // allow default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Stream::Type m_stream;
      DurableDataKey m_dataKey;
      string m_message;
      DABoolean m_builtMessage;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/DHProxyErrorException.cc"
//#endif

#endif /* DATAHANDLER_DHPROXYERROREXCEPTION_H */
