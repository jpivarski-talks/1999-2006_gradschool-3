#if !defined(CONSTANTSDELIVERY_CONSTANTSPROXYDELIVERER_H)
#define CONSTANTSDELIVERY_CONSTANTSPROXYDELIVERER_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      ConstantsProxyDeliverer
// 
// Description: A ProxyDeliverer for DataBase Constants access

// Usage:
//        Create and control proxies for DataBase constants.
//
// Invariants:
//
// Author:      Rich Baker
// Created:     Tues Nov 26 1997
//
// $Id: ConstantsProxyDeliverer.h,v 1.8 2003/09/05 15:13:43 vk Exp $
//
// Revision history
//
// $Log: ConstantsProxyDeliverer.h,v $
// Revision 1.8  2003/09/05 15:13:43  vk
// Split contants.map in two halfs and compile them separetely.
//
// Revision 1.7  1999/07/14 21:22:30  cdj
// now uses bootfile path to find servers
//
// Revision 1.6  1999/07/10 21:03:29  cdj
// now inform user if server is unavailable
//
// Revision 1.5  1999/04/29 18:45:39  cdj
// updated to use new constants implementation
//
// Revision 1.4  1998/11/10 22:12:18  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.3  1998/11/04 15:39:21  baker
// Continuing updates to get this library compiled
//
// Revision 1.2  1998/09/30 20:40:15  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:28:59  baker
// // September 30, Rich Baker
// // Renamed header files from former DataBaseDelivery
// // for new ConstantsDelivery library
//
// Revision 1.2  1998/08/19 03:14:32  cdj
// re-engineered.  This probably won't compile.
//
// Revision 1.1.1.2  1998/01/14 20:56:58  baker
// make DataBaseDelivery library compileable
//
// Revision 1.1.1.1  1997/12/17 20:25:12  cleo3
// imported DataBaseDelivery sources
//


// system include files
#include <string>

// user include files
#include "DataDelivery/ProxyDeliverer.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"
#include "STLUtility/fwd_vector.h"
#include "ConstantsDelivery/DBConstantsSelectionTag.h"

// stl include files

// forward declarations
#include "STLUtility/fwd_map.h"
class DBConstantsProxyFactoryBase;
class DBConstantsRecordInfo;
class ConstantsProxyDeliverer : public ProxyDeliverer
{
      // friend classses and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      ConstantsProxyDeliverer(
	 const string& iBootFilePath,
	 const DBConstantsSelectionTag& iTags );
      virtual ~ConstantsProxyDeliverer();


      virtual void disable( const Stream::Type& aStream );
      virtual void enable( const Stream::Type& aStream );

      DABoolean synchronize( const Stream::Type& iStream,
			     const SyncValue& iSyncTo,
			     SyncValue& oClosestSyncValue );

      void makeProxies( const Stream::Set& iSet );
      void makeProxies( const Stream::Type& iStream );
      
      // const member functions
      DABoolean syncValueOfNextRecord( const Stream::Type& iStream,
				       SyncValue& oSyncValue ) const;

      DABoolean available( const Stream::Type& aStream ) const ;      

      DABoolean sourceAccessible() const;

   protected:
      // protected member functions
      virtual void resetCache( const Stream::Type& aStream );

// 11/9/98 Rich Baker
// The registerProxies method is obsolete.
// commented out:
      virtual void registerProxies( const Stream::Type& aStream ,
				    KeyedProxies& aProxyList );

//      void registerStream( const Stream::Type& aStream );

      void registerProxyFactory( const Stream::Type& iStream,
				 DBConstantsProxyFactoryBase * iFactory );

      void registerProxyFactories();
      void registerProxyFactories1();
      void registerProxyFactories2();
      // protected const member functions

   private:
      // Constructors and destructor
      ConstantsProxyDeliverer() ; // stop default
      ConstantsProxyDeliverer( const ConstantsProxyDeliverer& ) ; // stop default

      // assignment operator(s)
      const ConstantsProxyDeliverer& operator=( 
            const ConstantsProxyDeliverer& ) ; // stop default

      // private member functions

      // private const member functions

      // data members

      STL_VECTOR( DABoolean )& m_available ;
      STL_MAP( Stream::Type, DBConstantsRecordInfo* )& m_streamToRecordInfoMap;
      string m_bootFilePath;
      DBConstantsSelectionTag m_selectionTags;

      STL_MAP( Stream::Type, STL_VECTOR( DBConstantsProxyFactoryBase* ) )& m_streamToFactoryMap;

      // static data members

};

// inline function definitions

#endif /* CONSTANTSDELIVERY_CONSTANTSPROXYDELIVERER_H */

