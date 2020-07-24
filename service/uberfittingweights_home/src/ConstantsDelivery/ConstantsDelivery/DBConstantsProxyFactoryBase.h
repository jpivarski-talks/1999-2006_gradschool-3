#if !defined(CONSTANTSDELIVERY_DBCONSTANTSPROXYFACTORYBASE_H)
#define CONSTANTSDELIVERY_DBCONSTANTSPROXYFACTORYBASE_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      DBConstantsProxyFactoryBase
// 
// Description: Abstract interface for Factories that build a DBConstantsProxy
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 22:04:06 EDT 1998
// $Id: DBConstantsProxyFactoryBase.h,v 1.7 1999/11/03 03:33:25 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsProxyFactoryBase.h,v $
// Revision 1.7  1999/11/03 03:33:25  cdj
// now handles multiple constants per stream
//
// Revision 1.6  1999/07/14 21:22:32  cdj
// now uses bootfile path to find servers
//
// Revision 1.5  1998/11/10 22:12:19  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.4  1998/11/05 18:52:08  baker
// Another batch of updates to get this library finally compiled
// *AND* linkable
//
// Revision 1.3  1998/11/04 18:55:51  baker
// Fix stupid errors so that this library actually compiles (finally!)
//
// Revision 1.2  1998/09/30 20:40:17  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:29:00  baker
// // September 30, Rich Baker
// // Renamed header files from former DataBaseDelivery
// // for new ConstantsDelivery library
//
// Revision 1.1  1998/08/19 03:17:38  cdj
// first submission
//

// system include files

// user include files
#include "DataHandler/KeyedProxy.h"

// forward declarations
class DBConstantsSelectionTag;
class DBConstantsInfo;
#include "ConstantsDelivery/DBConstantsSelectionTag.h"

class DBConstantsProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBConstantsProxyFactoryBase() {}
      virtual ~DBConstantsProxyFactoryBase() {}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
// 11/9/98 Rich Baker
// Removed the KeyedProxy from this function (obsolete)
// and changed Info* to Info*&
      virtual void createProxy( const string& iBootFilePath,
				const DBConstantsSelectionTag& iTags,
				KeyedProxy& oKeyedProxy,
				DBConstantsInfo*& iRecordInfo) const = 0;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DBConstantsProxyFactoryBase( const DBConstantsProxyFactoryBase& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DBConstantsProxyFactoryBase& operator=( const DBConstantsProxyFactoryBase& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsDelivery/Template/DBConstantsProxyFactoryBase.cc"
//#endif

#endif /* CONSTANTSDELIVERY_DBCONSTANTSPROXYFACTORYBASE_H */
