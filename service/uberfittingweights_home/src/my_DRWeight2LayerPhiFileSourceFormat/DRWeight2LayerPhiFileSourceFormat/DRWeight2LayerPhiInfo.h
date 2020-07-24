#if !defined(DRWEIGHT2LAYERPHIFILESOURCEFORMAT_DRWEIGHT2LAYERPHIINFO_H)
#define DRWEIGHT2LAYERPHIFILESOURCEFORMAT_DRWEIGHT2LAYERPHIINFO_H
// -*- C++ -*-
//
// Package:     DRWeight2LayerPhiFileSourceFormat
// Module:      DRWeight2LayerPhiInfo
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read DRWeight2LayerPhi constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:14:44 EST 1999
// $Id: DRWeight2LayerPhiInfo.h,v 1.2 1999/10/10 12:58:25 cdj Exp $
//
// Revision history
//
// $Log: DRWeight2LayerPhiInfo.h,v $
// Revision 1.2  1999/10/10 12:58:25  cdj
// forgot to remove BDLclient from skeleton include
//
// Revision 1.1  1999/03/13 22:44:45  cdj
// First submission
//

// system include files

// user include files
#include "DBDRWeight2LayerPhi.hh"
#include "DataHandler/Stream.h"
#include "C3dr/DrStream.h"

// forward declarations
class DummyFriend;

class DRWeight2LayerPhiInfo
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DBDRWeight2LayerPhi ConstantType;
      
      static const Stream::Type& stream() { return DrStream::kWeight2LayerPhi; }

      static const char* binderTypeName() { return "drweight2layerphifile"; }
      static const char* extension() { return ".drweight2layerphi"; }

   private:
      DRWeight2LayerPhiInfo(); //stop default
      DRWeight2LayerPhiInfo( const DRWeight2LayerPhiInfo& ); // stop default
      const DRWeight2LayerPhiInfo& operator=( const DRWeight2LayerPhiInfo& ); // stop default
};

#endif /* DRWEIGHT2LAYERPHIFILESOURCEFORMAT_DRWEIGHT2LAYERPHIINFO_H */
