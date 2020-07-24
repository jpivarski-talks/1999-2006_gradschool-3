#if !defined(CP2DELIVERY_CP2SOURCEBINDER_H)
#define CP2DELIVERY_CP2SOURCEBINDER_H
// -*- C++ -*-
//
// Package:     ConstantsPhase2Delivery
// Module:      CP2SourceBinder
// 
// Description: 

// Usage:

// Invariants:
//
// Author:      Valentine Kouznetsov
// Created:     Wed Jan 15 13:48:33 EST 2003
//
// $Id: CP2SourceBinder.h,v 1.1.1.1 2003/01/28 21:49:56 vk Exp $
//
// Revision history
//
// $Log: CP2SourceBinder.h,v $
// Revision 1.1.1.1  2003/01/28 21:49:56  vk
// imported ConstantsPhase2Delivery sources
//
//
// First working version of code with new DB API
// Using CLEORB_CLIENT_HH(DBVersionManager)

// system include files

// user include files
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"
#include "DataDelivery/DataSourceBinder.h"
#include "ConstantsDelivery/ConstantsSourceBinder.h"
#include "ConstantsDelivery/DBConstantsSelectionTag.h"

#include "CLEORB.hxx" //defines CLEORB_CLIENT_HH
#include CLEORB_CLIENT_HH(DBVersionManager)

// stl include files

// forward declarations

class CP2SourceBinder : public DataSourceBinder
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      CP2SourceBinder( const string& fdbName,
		       const DBConstantsSelectionTag& tag,
		       const Stream::Set& iBindStreams );

      virtual ~CP2SourceBinder();

      // member functions

      // const member functions

      virtual DataSourceController*
        createDataSourceController() const;

      const DBConstantsSelectionTag& selTag( void ) const;
      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      CP2SourceBinder( const CP2SourceBinder& );

      // assignment operator(s)
      const CP2SourceBinder& operator=( const CP2SourceBinder& );

      // private member functions
      DBVersionManager::StringList* retrieveTypeList(const char* tag);

      // private const member functions
      Stream::Set retrieveStreams(DBVersionManager::StringList* type) const;

      // data members
      DBConstantsSelectionTag mSelectTag;

      // static data members

};

// inline function definitions

#endif /* CP2DELIVERY_CP2SOURCEBINDER_H */

