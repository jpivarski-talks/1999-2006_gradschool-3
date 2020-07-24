#if !defined(DATAHANDLER_DHSTREAMDIDNOTSYNCEXCEPTION_H)
#define DATAHANDLER_DHSTREAMDIDNOTSYNCEXCEPTION_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      DHStreamDidNotSyncException
// 
/**\class DHStreamDidNotSyncException DHStreamDidNotSyncException.h DataHandler/DHStreamDidNotSyncException.h

 Description: An exception that is thrown when a RecordProvider was unable to
              find a SyncValue for its Record

 Usage:
    Used internal by RecordProvider and FrameProvider

*/
//
// Author:      Chris D Jones
// Created:     Fri Jun  2 20:35:19 EDT 2000
// $Id: DHStreamDidNotSyncException.h,v 1.1 2000/06/03 21:20:55 cdj Exp $
//
// Revision history
//
// $Log: DHStreamDidNotSyncException.h,v $
// Revision 1.1  2000/06/03 21:20:55  cdj
// now issue warning and continue if a Record that is supposed to be in the Frame fails to synchronize
//

// system include files
#include <string>

// user include files
#include "DAException/DAException.h"
#include "DataHandler/Stream.h"

// forward declarations

class DHStreamDidNotSyncException : public DAExceptionBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DHStreamDidNotSyncException( const Stream::Type& iStream )
	 : m_stream( iStream ) {
	    m_message = string("No source has a ") +
	       m_stream.value()+string(" Record for this stop");
      }
      //virtual ~DHStreamDidNotSyncException();

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
      //DHStreamDidNotSyncException( const DHStreamDidNotSyncException& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const DHStreamDidNotSyncException& operator=( const DHStreamDidNotSyncException& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Stream::Type m_stream;
      string m_message;

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* DATAHANDLER_DHSTREAMDIDNOTSYNCEXCEPTION_H */
