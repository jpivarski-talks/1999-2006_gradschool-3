#if !defined(CONSTANTSPHASE2DELIVERY_CP2PROXYDELIVERER_H)
#define CONSTANTSPHASE2DELIVERY_CP2PROXYDELIVERER_H
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      CP2ProxyDeliverer
// 
// Description: A ProxyDeliverer for DataBase Constants access

// Usage:
//        Create and control proxies for DataBase constants.
//
// Invariants:
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:47:41 EST 2003
//
// $Id: CP2ProxyDeliverer.h,v 1.2 2003/08/29 16:49:51 vk Exp $
//
// Revision history
//
// $Log: CP2ProxyDeliverer.h,v $
// Revision 1.2  2003/08/29 16:49:51  vk
// Come back to original parser. Splitting constants.map now has been done in Makefile. Remove obsolete split_map_file.py. Declare registerProxyFactories1/registerProxyFactories2 in CP2ProxyDeliverer whose implementation auto generated from Template CP2ProxyDeliverer_registerFactories.cc
//
// Revision 1.1.1.1  2003/01/28 21:49:56  vk
// imported ConstantsPhase2Delivery sources
//
//
// First version which works with new DB API.
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
class DBCP2ProxyFactoryBase;
class DBCP2RecordInfo;
class CP2ProxyDeliverer : public ProxyDeliverer
{
      // friend classses and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      CP2ProxyDeliverer(
	 const string& iBootFilePath,
	 const DBConstantsSelectionTag& iTags );
      virtual ~CP2ProxyDeliverer();


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

      virtual void registerProxies( const Stream::Type& aStream ,
				    KeyedProxies& aProxyList );

//      void registerStream( const Stream::Type& aStream );

      void registerProxyFactory( const Stream::Type& iStream,
				 DBCP2ProxyFactoryBase * iFactory );

      void registerProxyFactories();
      void registerProxyFactories1();
      void registerProxyFactories2();
      // protected const member functions

   private:
      // Constructors and destructor
      CP2ProxyDeliverer() ; // stop default
      CP2ProxyDeliverer( const CP2ProxyDeliverer& ) ; // stop default

      // assignment operator(s)
      const CP2ProxyDeliverer& operator=( 
            const CP2ProxyDeliverer& ) ; // stop default

      // private member functions

      // private const member functions

      // data members

      STL_VECTOR( DABoolean )& m_available ;
      STL_MAP( Stream::Type, DBCP2RecordInfo* )& m_streamToRecordInfoMap;
      string m_bootFilePath;
      DBConstantsSelectionTag m_selectionTags;

      STL_MAP( Stream::Type, STL_VECTOR( DBCP2ProxyFactoryBase* ) )& m_streamToFactoryMap;

      // static data members

};

// inline function definitions

#endif /* CONSTANTSPHASE2DELIVERY_CP2PROXYDELIVERER_H */

