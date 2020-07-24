#if !defined(NAVIGATION_NAVTYPESTORAGEHELPER_H)
#define NAVIGATION_NAVTYPESTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTypeStorageHelper
// 
// Description: StorageHelper to store a NavType
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: NavTypeStorageHelper.h,v 1.1 2003/09/02 01:53:14 cdj Exp $
//
// Revision history
//
// $Log: NavTypeStorageHelper.h,v $
// Revision 1.1  2003/09/02 01:53:14  cdj
// can now store NavKs and NavShowers using templated storage helper
//
// Revision 1.1  2003/08/31 23:25:36  cdj
// added the ability to store NavTypes
//
// Revision 1.1  1999/10/18 18:13:05  cdj
// first submission
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "FrameAccess/FATable.h"

// forward declarations

template <class T>
class NavTypeStorageHelper : public SMStorageHelper<T>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      NavTypeStorageHelper() ;
      //virtual ~NavTypeStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const T& iData );

      //functions to deliver specific storage versions of T
      T* deliverV1( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const T& getDefault() const;
      virtual DABoolean compare( const T* iNewData, 
				 const T& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      NavTypeStorageHelper( const NavTypeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const NavTypeStorageHelper& operator=( const NavTypeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( T* (NavTypeStorageHelper<T>::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      unsigned long m_recordCache;
      FATable<T> m_masterList;

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* NAVIGATION_NAVTYPESTORAGEHELPER_H */
