#if !defined(RUNSTATISTICSSOURCEMODULE_RSPROXYDELIVERER_H)
#define RUNSTATISTICSSOURCEMODULE_RSPROXYDELIVERER_H
// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RSProxyDeliverer
// 
/**\class RSProxyDeliverer RSProxyDeliverer.h RunStatisticsSourceModule/RSProxyDeliverer.h

 Description: ProxyDeliverer used to access info from RunStatistics

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Jan 26 10:46:30 EST 2000
// $Id: RSProxyDeliverer.h,v 1.2 2000/05/08 19:52:13 cdj Exp $
//
// Revision history
//
// $Log: RSProxyDeliverer.h,v $
// Revision 1.2  2000/05/08 19:52:13  cdj
// now compiles on OSF with MICO
//
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files
#include <map>

// user include files
#include "DataDelivery/ProxyDeliverer.h"

#include "CLEORB.hxx" //defines CLEORB_CLIENT_HH
#include CLEORB_CLIENT_HH(DBRunStatistics)

// forward declarations
#include "STLUtility/fwd_map.h"
#include "STLUtility/fwd_multimap.h"
class RSProxyInterfaceFactoryBase;
class RSProxyInterface;

class RSProxyDeliverer : public ProxyDeliverer
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef STL_MAP(Stream::Type, const DBRunStatistics::RunStatistics* ) 
	 StreamToRSMap;
      typedef pair<RSProxyInterfaceFactoryBase*, UsageTag> FactoryTagPair;
      typedef STL_MULTIMAP(Stream::Type, FactoryTagPair )
	 StreamToFactoryMap;
      typedef STL_MULTIMAP(Stream::Type, RSProxyInterface* ) 
	 StreamToProxyMap;

      // ---------- Constructors and destructor ----------------
      RSProxyDeliverer();
      virtual ~RSProxyDeliverer();

      // ---------- member functions ---------------------------
      void use( const DBRunStatistics::RunStatistics* iRS,
		const SyncValue& iSyncValue );

      virtual void disable( const Stream::Type& aStream ) ;
      virtual void enable( const Stream::Type& aStream ) ;

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void resetCache( const Stream::Type& aStream ) ;
      virtual void registerProxies( const Stream::Type& aStream ,
                                    KeyedProxies& aProxyList ) ;

      void registerProxyFactory( const Stream::Type& iStream,
				 RSProxyInterfaceFactoryBase* iFactory,
				 const UsageTag& iTag );
      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSProxyDeliverer( const RSProxyDeliverer& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSProxyDeliverer& operator=( const RSProxyDeliverer& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      StreamToRSMap m_streamToRSMap;
      StreamToFactoryMap m_streamToFactoryMap;
      StreamToProxyMap m_streamToProxyMap;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSProxyDeliverer.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSPROXYDELIVERER_H */
