#if !defined(DATAHANDLER_DHNORECORDEXCEPTION_H)
#define DATAHANDLER_DHNORECORDEXCEPTION_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      DHNoRecordException
// 
/**\class DHNoRecordException DHNoRecordException.h DataHandler/DHNoRecordException.h

 Description: An exception that is thrown whenever a Frame is asked to retrieve
              a Record it does not have.

 Usage:
   This exception will be thrown if you call the Frame method record() and the
    record type you request does not exist.
    E.g.
    \code
    try {
      extract( iFrame.record( kMyRecord ), myData );
    } catch( DHNoRecordException& iException ) {
       //no record of type kMyRecord found in Frame
       ...
    }
    \endcode  
*/
//
// Author:      Chris D Jones
// Created:     Fri Dec 31 10:31:01 EST 1999
// $Id: DHNoRecordException.h,v 1.1 1999/12/31 18:59:50 cdj Exp $
//
// Revision history
//
// $Log: DHNoRecordException.h,v $
// Revision 1.1  1999/12/31 18:59:50  cdj
// first submission
//

// system include files
#include <string>

// user include files
#include "DAException/DAException.h"
#include "DataHandler/Stream.h"

// forward declarations

class DHNoRecordException : public DAExceptionBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DHNoRecordException(const Stream::Type& iStream )
	 : m_stream( iStream )  {
	    m_message = string("No ") +
	       m_stream.value() +
	       " Record found in the Frame";
      }
      //virtual ~DHNoRecordException();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      const char* what() const {
	 return m_message.c_str();
      }

      const Stream::Type& stream() const { return m_stream; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //DHNoRecordException( const DHNoRecordException& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const DHNoRecordException& operator=( const DHNoRecordException& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Stream::Type m_stream;
      string m_message;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/DHNoRecordException.cc"
//#endif

#endif /* DATAHANDLER_DHNORECORDEXCEPTION_H */
