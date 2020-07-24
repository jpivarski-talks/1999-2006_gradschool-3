#if !defined(DUALTRACKPROD_DUALTRACKFITHELIX_H)
#define DUALTRACKPROD_DUALTRACKFITHELIX_H
// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackFitHelix
// 
/**\class DualTrackFitHelix DualTrackFitHelix.h DualTrackProd/DualTrackFitHelix.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 15:13:40 EST 2003
// $Id: DualTrackFitHelix.h,v 1.1 2003/12/15 17:51:42 mccann Exp $
//
// Revision history
//
// $Log: DualTrackFitHelix.h,v $
// Revision 1.1  2003/12/15 17:51:42  mccann
// adding hit lattices for dual-constrained residuals
//

// system include files

// user include files
#include "HelixIntersection/HIFitHelix.h"

// forward declarations

class DualTrackFitHelix : public HIFitHelix
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DualTrackFitHelix();
      DualTrackFitHelix(unsigned int id, const KTHelix& copy_helix);
      DualTrackFitHelix(const DualTrackFitHelix& copy_helix);
      virtual ~DualTrackFitHelix();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------
      const DualTrackFitHelix& operator=( const DualTrackFitHelix& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DualTrackProd/Template/DualTrackFitHelix.cc"
//#endif

#endif /* DUALTRACKPROD_DUALTRACKFITHELIX_H */
