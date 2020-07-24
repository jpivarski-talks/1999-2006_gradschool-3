#if !defined(DATAHANDLER_DURABLEDATAKEY_H)
#define DATAHANDLER_DURABLEDATAKEY_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      DurableDataKey
// 
/**\class DurableDataKey DurableDataKey.h DataHandler/DurableDataKey.h

 Description: A DataKey that owns the strings held by UsageTag and
              ProductionTag

 Usage:
    Rule: 
    Use a DurableDataKey instead of a simple DataKey when you are
    pairing a DataKey with a Proxy that you own.  So all ProxyProviders
    should use a DurableDataKey (since they own their Proxies) but a
    Record should just use a DataKey.

    Reason: 
    The reason a Record should use a DataKey is for efficiency.  DataKeys 
    are much faster to construct than a DurableDataKey because no
    strings have to be copied.  But this means that object must own the
    memory for the strings and we have decided that that object should be
    the object that also owns the Proxies.  Therefore, if you own Proxies
    you should use a DurableDataKey.

*/
//
// Author:      Chris D Jones
// Created:     Fri Nov 12 10:45:04 EST 1999
// $Id: DurableDataKey.h,v 1.3 2001/05/31 19:42:47 cdj Exp $
//
// Revision history
//
// $Log: DurableDataKey.h,v $
// Revision 1.3  2001/05/31 19:42:47  cdj
// added operator== and operator!= to DurableDataKey
//
// Revision 1.2  2000/03/02 20:04:09  mkl
// DurableDataKeys now handle blank strings properly
//
// Revision 1.1  1999/11/12 18:47:12  cdj
// first submission
//

// system include files
#include <string.h>

// user include files
#include "DataHandler/DataKey.h"

// forward declarations

class DurableDataKey : public DataKey
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DurableDataKey() : DataKey() {}

      DurableDataKey( const DataKey& iDataKey ) :
	 DataKey( iDataKey ) 
      {
	 makeMemory();
      }

      DurableDataKey( const DurableDataKey& iKey ) :
	 DataKey( iKey ) 
      {
	 makeMemory(); 
      }

      const DataKey& operator=( const DataKey& iKey ) {
	 if( this != &iKey) {
	    deleteMemory();

	    //because of compiler problems I can't do 
	    // DataKey::operator=( iKey );
	    *(static_cast<DataKey*>(this)) = iKey;
	    
	    makeMemory();
	 }
	 return *this;
      }
      
      const DurableDataKey& operator=( const DurableDataKey& iKey ) {
	 operator=( static_cast< const DataKey& >( iKey ) );
	 return *this;
      }

      ~DurableDataKey() {
	 deleteMemory();
      }

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      DABoolean operator==( const DurableDataKey& iRHS ) const {
      	return DataKey::operator==(iRHS); 
      }
      DABoolean operator!=( const DurableDataKey& iRHS ) const {
      	return ! DataKey::operator==(iRHS); 
      }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      void makeMemory() {
	 static const char kBlank = '\0';

	 char* pUsage      = const_cast<char*>(&kBlank);
	 char* pProduction = const_cast<char*>(&kBlank);
	 if( kBlank != usage().value()[0] ) {
	    pUsage = new char[ strlen( usage().value() ) + 1];
	    strcpy( pUsage, usage().value() );
	 }
	 if( kBlank != production().value()[0] ) {
	    pProduction = new char[ strlen( production().value() ) + 1];
	    strcpy( pProduction, production().value() );	    
	 }
	 //because of compiler problems I can't do 
	 // DataKey::operator=( DataKey(...) );
	 *(static_cast<DataKey*>(this)) = DataKey(type(), IdTags( pUsage, pProduction ) ) ;
      }

      void deleteMemory() {
	 static const char kBlank = '\0';

	 if( kBlank != usage().value()[0] ) {
	    delete [] const_cast<char*>(usage().value());
	 }
	 if( kBlank != production().value()[0] ) {
	    delete [] const_cast<char*>(production().value());
	 }
      }
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/DurableDataKey.cc"
//#endif

#endif /* DATAHANDLER_DURABLEDATAKEY_H */
