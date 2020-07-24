#if !defined(CONSTANTSDELIVERY_DBCONSTANTSPROXY_H)
#define CONSTANTSDELIVERY_DBCONSTANTSPROXY_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      DBConstantsProxy
// 
// Description: Proxy that delivers CLEOConstants objects
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 13:46:03 EDT 1998
// $Id: DBConstantsProxy.h,v 1.13 2000/07/27 18:19:52 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsProxy.h,v $
// Revision 1.13  2000/07/27 18:19:52  cdj
// now much faster when reading from multiple runs in one job
//
// Revision 1.12  1999/12/10 16:26:21  cdj
// updated to work with extract exceptions
//
// Revision 1.11  1999/11/03 03:33:24  cdj
// now handles multiple constants per stream
//
// Revision 1.10  1999/07/14 21:22:31  cdj
// now uses bootfile path to find servers
//
// Revision 1.9  1999/07/10 21:03:30  cdj
// now inform user if server is unavailable
//
// Revision 1.8  1999/04/29 18:45:41  cdj
// updated to use new constants implementation
//
// Revision 1.7  1998/11/19 18:09:44  baker
// Again changed include path for DBConstants.hxx
//
// Revision 1.6  1998/11/18 19:09:52  baker
// Change include path for DBConstants.hxx
//
// Revision 1.5  1998/11/10 22:12:18  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.4  1998/11/05 18:52:08  baker
// Another batch of updates to get this library finally compiled
// *AND* linkable
//
// Revision 1.3  1998/09/30 20:40:16  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.2  1998/08/19 14:25:25  cdj
// needed some additional member functions
//
// Revision 1.1  1998/08/19 03:17:38  cdj
// first submission
//

// system include files

// user include files
// 11/5/98 Rich Baker
// Changed from ProxyDict/IfdDataProxyTemplate.hh to
// DataHandler/ProxyTemplate.h
#include "DataDelivery/DataSourceProxy.h" // superclass
// 11/18/98 Rich Baker
// Change include path for DBConstants.hxx
#include "DBConstants.hxx"
#include "ConstantsDelivery/DBConstantsSelectionTag.h"
#include "ConstantsDelivery/DBConstantsInfo.h"

// forward declarations

template < class T >
class DBConstantsProxy : public DataSourceProxy< CLEOConstants<T> >, 
			 public DBConstantsInfo
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef CLEOConstants< T > value_type; 
      // ---------- Constructors and destructor ----------------
      DBConstantsProxy( const string& iBootFilePath,
			const DBConstantsSelectionTag& iTag );
      virtual ~DBConstantsProxy();

      // ---------- member functions ---------------------------
      virtual void invalidateCache() ;
// 11/5/98 Changed from ProxyDict to ProxyTemplate implementation
      virtual const value_type* faultHandler( const Record& aRecord ,
					      const DataKey& aKey );


      // ---------- const member functions ---------------------
      virtual DABoolean syncValueOfNextRecord( SyncValue& oSyncValue ) const;
      virtual DABoolean closestSyncValueTo( const SyncValue& iSyncValue, 
					    SyncValue& oClosestSyncValue ) const ;
      virtual SyncValue presentSyncValue() const;

      virtual DABoolean foundServer() const;

      virtual const char* nameOfConstant() const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      class ValidRange {
	 public: 
	    ValidRange() {}
	    ValidRange( const SyncValue& iBegin,
			const SyncValue& iEnd ) :
	       m_begin(iBegin), m_end(iEnd) {}
	    const SyncValue& begin() const { return m_begin; }
	    const SyncValue& end() const { return m_end; }
	 private:
	    SyncValue m_begin;
	    SyncValue m_end;
      };

      // ---------- Constructors and destructor ----------------
      DBConstantsProxy(); //stop default
      DBConstantsProxy( const DBConstantsProxy& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DBConstantsProxy& operator=( const DBConstantsProxy& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DBConstants< T > m_constants;
      ValidRange m_validRange;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "ConstantsDelivery/Template/DBConstantsProxy.cc"
#endif

#endif /* CONSTANTSDELIVERY_DBCONSTANTSPROXY_H */
