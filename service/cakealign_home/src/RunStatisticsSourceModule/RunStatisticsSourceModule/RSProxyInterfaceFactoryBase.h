#if !defined(RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACEFACTORYBASE_H)
#define RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACEFACTORYBASE_H
// -*- C++ -*-
//
// Package:     RunStatisticsSourceModule
// Module:      RSProxyInterfaceFactoryBase
// 
/**\class RSProxyInterfaceFactoryBase RSProxyInterfaceFactoryBase.h RunStatisticsSourceModule/RSProxyInterfaceFactoryBase.h

 Description: Base class for RSProxyInterfaceFactories

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Fri Jan 28 08:50:28 EST 2000
// $Id: RSProxyInterfaceFactoryBase.h,v 1.2 2003/03/03 20:47:14 cleo3 Exp $
//
// Revision history
//
// $Log: RSProxyInterfaceFactoryBase.h,v $
// Revision 1.2  2003/03/03 20:47:14  cleo3
// no longer return local value of TypeTag
//
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files

// user include files
#include "RunStatisticsSourceModule/RSProxyDeliverer.h"

// forward declarations
class RSProxyInterface;

class RSProxyInterfaceFactoryBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RSProxyInterfaceFactoryBase() {}
      //virtual ~RSProxyInterfaceFactoryBase() ;
	 
      // ---------- member functions ---------------------------
      virtual RSProxyInterface* make(
	 RSProxyDeliverer::StreamToRSMap::value_type* iStreamRSPair ) = 0;

      // ---------- const member functions ---------------------
      virtual TypeTag typeTag() const = 0;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSProxyInterfaceFactoryBase( const RSProxyInterfaceFactoryBase& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSProxyInterfaceFactoryBase& operator=( const RSProxyInterfaceFactoryBase& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSProxyInterfaceFactoryBase.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSPROXYINTERFACEFACTORYBASE_H */
