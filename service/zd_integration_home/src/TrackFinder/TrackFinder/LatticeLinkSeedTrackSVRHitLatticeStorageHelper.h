#if !defined(LATTICELINKSeedTrackSVRHitLatticeSTORAGEHELPER_H)
#define LATTICELINKSeedTrackSVRHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     
// Module:      LatticeLinkSeedTrackSVRHitLatticeStorageHelper
// 
// Description: Storage helper to store a DefaultLinkData which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkSeedTrackSVRHitLatticeStorageHelper.h,v 1.2 2001/10/23 19:12:58 cdj Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackSVRHitLatticeStorageHelper.h,v $
// Revision 1.2  2001/10/23 19:12:58  cdj
// modified StorageHelpers so they store individual identifiers rather than lists
//
// Revision 1.1  2000/02/08 18:28:24  lyon
// Storage helpers
//
// Revision 1.1  1999/10/18 21:30:19  cdj
// renamed files
//
// Revision 1.1  1999/10/11 22:00:37  cdj
// added Lattice storage code
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "Lattice/LatticeLink.h"
#include "SeedTrackSVRHitLattice.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

class LatticeLinkSeedTrackSVRHitLatticeStorageHelper :
   public SMStorageHelper< 
           SMLatticeLinkInfoBase< SeedTrackSVRHitLattice > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef SMLatticeLinkInfoBase< SeedTrackSVRHitLattice > StoreType;

      // ---------- Constructors and destructor ----------------
      LatticeLinkSeedTrackSVRHitLatticeStorageHelper();
      virtual ~LatticeLinkSeedTrackSVRHitLatticeStorageHelper();

      // ---------- member functions ---------------------------
      virtual StoreType* deliverV1( SMSourceStream& iSource );
      virtual StoreType* deliverV2( SMSourceStream& iSource );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const StoreType& iData );

      // ---------- protected const member functions -----------
      virtual const StoreType& getDefault() const;
      virtual DABoolean compare( const StoreType* iNewData, 
				 const StoreType& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      LatticeLinkSeedTrackSVRHitLatticeStorageHelper( const LatticeLinkSeedTrackSVRHitLatticeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkSeedTrackSVRHitLatticeStorageHelper& operator=( const LatticeLinkSeedTrackSVRHitLatticeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
   void registerVersion( StoreType* (LatticeLinkSeedTrackSVRHitLatticeStorageHelper::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/LatticeLinkSeedTrackSVRHitLatticeStorageHelper.cc"
//#endif

#endif /* LATTICELINKSeedTrackSVRHitLatticeSTORAGEHELPER_H */
