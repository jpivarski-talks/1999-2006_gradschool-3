#if !defined(KALMANFILTER_KSURFACEVECTOR_H)
#define KALMANFILTER_KSURFACEVECTOR_H
// -*- C++ -*-
//
// Package:     <PivarskiFilter>
// Module:      KSurfaceVector
// 
/**\class KSurfaceVector KSurfaceVector.h PivarskiFilter/KSurfaceVector.h

 Description: Helper class to make it easier to manage a list of Surfaces

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Apr 23 09:20:30 EDT 2003
// $Id: KSurfaceVector.h,v 1.1 2003/04/23 16:01:45 cdj Exp $
//
// Revision history
//
// $Log: KSurfaceVector.h,v $
// Revision 1.1  2003/04/23 16:01:45  cdj
// fixed double deletion bug by creating new class, KSurfaceVector, to manage the memory
//

// system include files

// user include files
#include "STLUtility/fwd_vector.h"

// forward declarations
class HIIntersectionSurface;

class KSurfaceVector : public STL_VECTOR(HIIntersectionSurface*)
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      //NOTE: making this virtual would not help since vector's destructor
      // is not virtual.  Therefore we can NOT use this class as a
      // drop in replacement for a vector where we want to delete the
      // vector
      ~KSurfaceVector();

      // ---------- member functions ---------------------------
      void clearSurfaces();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //KSurfaceVector( const KSurfaceVector& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const KSurfaceVector& operator=( const KSurfaceVector& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "PivarskiFilter/Template/KSurfaceVector.cc"
//#endif

#endif /* KALMANFILTER_KSURFACEVECTOR_H */
