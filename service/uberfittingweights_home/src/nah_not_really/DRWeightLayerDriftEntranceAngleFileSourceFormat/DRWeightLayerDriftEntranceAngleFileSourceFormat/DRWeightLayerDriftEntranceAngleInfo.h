#if !defined(PACKAGE_DRWEIGHTLAYERDRIFTENTRANCEANGLEINFO_H)
#define PACKAGE_DRWEIGHTLAYERDRIFTENTRANCEANGLEINFO_H
// -*- C++ -*-
//
// Package:     DRWeightLayerDriftEntranceAngleFileSourceFormat
// Module:      DRWeightLayerDriftEntranceAngleInfo
// 
/**\class DRWeightLayerDriftEntranceAngleInfo DRWeightLayerDriftEntranceAngleInfo.h package/DRWeightLayerDriftEntranceAngleInfo.h

 Description: Used by the ConstantsFileDelivery templated class to
              create a source that can read DRWeightLayer constants from
              a file

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Sep 12 14:52:19 EDT 2003
// $Id$
//
// Revision history
//
// $Log$

// system include files

// user include files

#include "DBDRWeightLayerDriftEntranceAngle.hh"
#include "C3dr/DrStream.h"

// forward declarations
class DummyFriend;

class DRWeightLayerDriftEntranceAngleInfo
{
      // ---------- friend classes and functions ---------------
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      // ---------- constants, enums and typedefs --------------
      typedef DBDRWeightLayerDriftEntranceAngle ConstantType;

      // ---------- Constructors and destructor ----------------
      virtual ~DRWeightLayerDriftEntranceAngleInfo();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

      static const Stream::Type& stream() { return DrStream::kWeightLayerDriftEntranceAngle; }
      
      static const char* binderTypeName() { return "drweightlayerdriftentranceanglefile"; }
      static const char* extension() { return ".drweightlayerdriftentranceangle"; }

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DRWeightLayerDriftEntranceAngleInfo(); // stop default
      DRWeightLayerDriftEntranceAngleInfo( const DRWeightLayerDriftEntranceAngleInfo& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DRWeightLayerDriftEntranceAngleInfo& operator=( const DRWeightLayerDriftEntranceAngleInfo& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/DRWeightLayerDriftEntranceAngleInfo.cc"
//#endif

#endif /* PACKAGE_DRWEIGHTLAYERDRIFTENTRANCEANGLEINFO_H */
