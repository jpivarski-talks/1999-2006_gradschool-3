#if !defined(RUNSTATISTICSSOURCEMODULE_RSPROXY_H)
#define RUNSTATISTICSSOURCEMODULE_RSPROXY_H
// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RSProxy
// 
/**\class RSProxy RSProxy.h RunStatisticsSourceModule/RSProxy.h

 Description: Base class for RunStatistics Proxies

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Thu Jan 27 10:46:34 EST 2000
// $Id: RSProxy.h,v 1.1.1.1 2000/01/29 20:47:36 cdj Exp $
//
// Revision history
//
// $Log: RSProxy.h,v $
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files

// user include files
#include "DataDelivery/DataSourceProxy.h"
#include "RunStatisticsSourceModule/RSProxyInterface.h"

// forward declarations

template <class T>
class RSProxy : public DataSourceProxy<T>, public RSProxyInterface
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RSProxy( RSProxyDeliverer::StreamToRSMap::value_type* iPair) :
	 RSProxyInterface( iPair, this ) {}
      //virtual ~RSProxy();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual const T* faultHandler( const Record& iRecord , 
                                     const DataKey& iKey ) {
	 cout << "RSProxy.h::faultHandler this is " << this
	      << " record stream " << iRecord.stream()
	      << " run/event/time " << iRecord.syncValue().runNumber() << " / " 
	      << iRecord.syncValue().runNumber() << iRecord.syncValue().time()
	      << " datakey type/usage/production " << iKey.type().value() << " / "
	      << iKey.usage().value() << " / " << iKey.production().value() << endl;
	 if( isEnabled() ) {
	    return implementFaultHandler( iRecord, iKey ); 
	 } 
	 return static_cast<T*>( 0 );
      }

      virtual const T* implementFaultHandler( const Record& iRecord , 
					      const DataKey& iKey ) = 0 ;
      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSProxy( const RSProxy& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSProxy& operator=( const RSProxy& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSProxy.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSPROXY_H */
