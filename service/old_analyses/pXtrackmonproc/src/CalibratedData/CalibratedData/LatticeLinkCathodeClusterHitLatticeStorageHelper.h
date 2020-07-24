#if !defined(LATTICELINKCathodeClusterHitLatticeSTORAGEHELPER_H)
#define LATTICELINKCathodeClusterHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     
// Module:      LatticeLinkCathodeClusterHitLatticeStorageHelper
// 
// Description: Storage helper to store a DefaultLinkData which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkCathodeClusterHitLatticeStorageHelper.h,v 1.1 2000/03/03 15:14:04 lyon Exp $
//
// Revision history
//
// $Log: LatticeLinkCathodeClusterHitLatticeStorageHelper.h,v $
// Revision 1.1  2000/03/03 15:14:04  lyon
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
#include "CalibratedData/CathodeClusterHitLattice.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

class LatticeLinkCathodeClusterHitLatticeStorageHelper :
   public SMStorageHelper< 
           SMLatticeLinkInfoBase< CathodeClusterHitLattice > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef SMLatticeLinkInfoBase< CathodeClusterHitLattice > StoreType;

      // ---------- Constructors and destructor ----------------
      LatticeLinkCathodeClusterHitLatticeStorageHelper();
      virtual ~LatticeLinkCathodeClusterHitLatticeStorageHelper();

      // ---------- member functions ---------------------------
      virtual StoreType* deliverV1( SMSourceStream& iSource );

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
      LatticeLinkCathodeClusterHitLatticeStorageHelper( const LatticeLinkCathodeClusterHitLatticeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkCathodeClusterHitLatticeStorageHelper& operator=( const LatticeLinkCathodeClusterHitLatticeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
   void registerVersion( StoreType* (LatticeLinkCathodeClusterHitLatticeStorageHelper::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/LatticeLinkCathodeClusterHitLatticeStorageHelper.cc"
//#endif

#endif /* LATTICELINKCathodeClusterHitLatticeSTORAGEHELPER_H */
