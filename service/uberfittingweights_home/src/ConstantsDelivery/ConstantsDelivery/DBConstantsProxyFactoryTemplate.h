#if !defined(CONSTANTSDELIVERY_DBCONSTANTSPROXYFACTORYTEMPLATE_H)
#define CONSTANTSDELIVERY_DBCONSTANTSPROXYFACTORYTEMPLATE_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      DBConstantsProxyFactoryTemplate
// 
// Description: Creates a DBConstantsProxy<T>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 22:11:23 EDT 1998
// $Id: DBConstantsProxyFactoryTemplate.h,v 1.6 1999/11/03 03:33:25 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsProxyFactoryTemplate.h,v $
// Revision 1.6  1999/11/03 03:33:25  cdj
// now handles multiple constants per stream
//
// Revision 1.5  1999/07/14 21:22:32  cdj
// now uses bootfile path to find servers
//
// Revision 1.4  1998/11/10 22:12:20  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.3  1998/11/05 18:52:09  baker
// Another batch of updates to get this library finally compiled
// *AND* linkable
//
// Revision 1.2  1998/09/30 20:40:17  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:29:01  baker
// // September 30, Rich Baker
// // Renamed header files from former DataBaseDelivery
// // for new ConstantsDelivery library
//
// Revision 1.1  1998/08/19 03:17:39  cdj
// first submission
//

// system include files

// user include files
#include "ConstantsDelivery/DBConstantsProxyFactoryBase.h"

// forward declarations
template< class T >
class DBConstantsProxyFactoryTemplate : public DBConstantsProxyFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBConstantsProxyFactoryTemplate();
      virtual ~DBConstantsProxyFactoryTemplate();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
// 11/9/98 Rich Baker
// Removed the KeyedProxy from this function (obsolete)
// and changed Info* to Info*&
      void createProxy( const string& iBootFilePath,
			const DBConstantsSelectionTag& iTags,
                        KeyedProxy& oKeyedProxy,
			DBConstantsInfo*& oRecordInfo ) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DBConstantsProxyFactoryTemplate( const DBConstantsProxyFactoryTemplate& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DBConstantsProxyFactoryTemplate& operator=( const DBConstantsProxyFactoryTemplate& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "ConstantsDelivery/Template/DBConstantsProxyFactoryTemplate.cc"
#endif

#endif /* CONSTANTSDELIVERY_DBCONSTANTSPROXYFACTORYTEMPLATE_H */
