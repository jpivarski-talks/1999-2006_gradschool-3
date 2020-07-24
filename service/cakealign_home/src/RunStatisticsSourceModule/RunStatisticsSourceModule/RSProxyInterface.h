#if !defined(RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACE_H)
#define RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACE_H
// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RSProxyInterface
// 
/**\class RSProxyInterface RSProxyInterface.h RunStatisticsSourceModule/RSProxyInterface.h

 Description: Interface used to enable and disable an RSProxy

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Thu Jan 27 10:46:28 EST 2000
// $Id: RSProxyInterface.h,v 1.1.1.1 2000/01/29 20:47:36 cdj Exp $
//
// Revision history
//
// $Log: RSProxyInterface.h,v $
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files

// user include files
#include "RunStatisticsSourceModule/RSProxyDeliverer.h"

// forward declarations

class RSProxyInterface
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RSProxyInterface( RSProxyDeliverer::StreamToRSMap::value_type* 
		     iStreamRSPair, ProxyBase* iProxy ) : 
	 m_enabled(false), 
	 m_pStreamRSPair( iStreamRSPair ),
	 m_proxyBase( iProxy )
         {}
      //virtual ~RSProxyInterface();

      // ---------- member functions ---------------------------
      void enable() { m_enabled = true; }
      void disable() { m_enabled = false; }
      const DBRunStatistics::RunStatistics* runStatistics() {
	 return m_pStreamRSPair->second; }
      ProxyBase* proxyBase() { return m_proxyBase; }

      // ---------- const member functions ---------------------
      DABoolean isEnabled() const { return m_enabled; }
      const Stream::Type& stream() const { return m_pStreamRSPair->first; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSProxyInterface( const RSProxyInterface& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSProxyInterface& operator=( const RSProxyInterface& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_enabled;
      RSProxyDeliverer::StreamToRSMap::value_type* m_pStreamRSPair;
      ProxyBase* m_proxyBase;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSProxyInterface.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACE_H */
