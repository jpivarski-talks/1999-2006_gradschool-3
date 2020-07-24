#if !defined(DUALTRACKHELICES_DUALTRACKHELICESSTORAGEHELPER_H)
#define DUALTRACKHELICES_DUALTRACKHELICESSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     DualTrackHelices
// Module:      DualTrackHelicesStorageHelper
// 
// Description: StorageHelper to store a DualTrackHelices
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: DualTrackHelicesStorageHelper.h,v 1.1.1.1 2001/01/30 23:57:19 llh14 Exp $
//
// Revision history
//
// $Log: DualTrackHelicesStorageHelper.h,v $
// Revision 1.1.1.1  2001/01/30 23:57:19  llh14
// imported DualTrackHelices sources
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
#include "DualTrackHelices/DualTrackHelices.h"

// forward declarations

class DualTrackHelicesStorageHelper : public SMStorageHelper<DualTrackHelices>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DualTrackHelicesStorageHelper() ;
      //virtual ~DualTrackHelicesStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const DualTrackHelices& iData );

      //functions to deliver specific storage versions of DualTrackHelices
      DualTrackHelices* deliverV1( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const DualTrackHelices& getDefault() const;
      virtual DABoolean compare( const DualTrackHelices* iNewData, 
				 const DualTrackHelices& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      DualTrackHelicesStorageHelper( const DualTrackHelicesStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DualTrackHelicesStorageHelper& operator=( const DualTrackHelicesStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( DualTrackHelices* (DualTrackHelicesStorageHelper::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* DUALTRACKHELICES_DUALTRACKHELICESSTORAGEHELPER_H */
