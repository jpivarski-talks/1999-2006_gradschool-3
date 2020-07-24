#if !defined(DATAHANDLER_RECORDMETHODS_H)
#define DATAHANDLER_RECORDMETHODS_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      RecordMethods
// 
/**\class RecordMethods RecordMethods.h DataHandler/RecordMethods.h

 Description: Helper class to stand in for Record's templated member
              functions

 Usage:
    These methods are used by 'extract' and 'insert' which are found
    in the FrameAccess package.

*/
//
// Author:      Chris D. Jones
// Created:     Thu Sep 24 10:08:58 EDT 1998
// $Id: RecordMethods.h,v 1.3 1999/10/09 21:51:14 cdj Exp $
//
// Revision history
//
// $Log: RecordMethods.h,v $
// Revision 1.3  1999/10/09 21:51:14  cdj
// updated to doxygen style comments
//
// Revision 1.2  1999/01/19 23:29:53  mkl
// uncommented template .cc inclusion part
//
// Revision 1.1  1998/09/25 19:31:51  cdj
// no longer use ProxyDict
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"

// forward declarations
class DummyFriend;

class Record;
class UsageTag;
class ProductionTag;

template< class T >
class RecordMethods
{
      // ---------- friend classes and functions ---------------
      friend class DummyFriend; //stops complaint about now 
                                // accessible constructors
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      // ---------- static member functions --------------------
      static DataKey makeKey( const UsageTag& iUsage, 
			      const ProductionTag& iProduction );

      static ProxyTemplate< T >* extractProxy(const Record& iRecord,
					      const UsageTag& iUsage,
					      const ProductionTag& iProduction );
      static DABoolean insertProxy(Record& iRecord,
				   ProxyTemplate< T >* iProxy,
				   const UsageTag& iUsage,
				   const ProductionTag& iProduction );

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RecordMethods( const RecordMethods& ); // stop default
      RecordMethods();
      //virtual ~RecordMethods();

      // ---------- assignment operator(s) ---------------------
      const RecordMethods& operator=( const RecordMethods& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "DataHandler/Template/RecordMethods.cc"
#endif

#endif /* DATAHANDLER_RECORDMETHODS_H */
