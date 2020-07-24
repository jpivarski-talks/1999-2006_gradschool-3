#if !defined(ZDWEIGHT2LAYERPHIFILESOURCEFORMAT_ZDWEIGHT2LAYERPHIINFO_H)
#define ZDWEIGHT2LAYERPHIFILESOURCEFORMAT_ZDWEIGHT2LAYERPHIINFO_H
// -*- C++ -*-
//
// Package:     ZDWeight2LayerPhiFileSourceFormat
// Module:      ZDWeight2LayerPhiInfo
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read ZDWeight2LayerPhi constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:14:44 EST 1999
// $Id: ZDWeight2LayerPhiInfo.h,v 1.2 1999/10/10 12:58:25 cdj Exp $
//
// Revision history
//
// $Log: ZDWeight2LayerPhiInfo.h,v $
// Revision 1.2  1999/10/10 12:58:25  cdj
// forgot to remove BDLclient from skeleton include
//
// Revision 1.1  1999/03/13 22:44:45  cdj
// First submission
//

// system include files

// user include files
#include "DBZDWeight2LayerPhi.hh"
#include "DataHandler/Stream.h"
#include "C3zd/ZdStream.h"

// forward declarations
class DummyFriend;

class ZDWeight2LayerPhiInfo
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DBZDWeight2LayerPhi ConstantType;
      
      static const Stream::Type& stream() { return ZdStream::kWeight2LayerPhi; }

      static const char* binderTypeName() { return "zdweight2layerphifile"; }
      static const char* extension() { return ".zdweight2layerphi"; }

   private:
      ZDWeight2LayerPhiInfo(); //stop default
      ZDWeight2LayerPhiInfo( const ZDWeight2LayerPhiInfo& ); // stop default
      const ZDWeight2LayerPhiInfo& operator=( const ZDWeight2LayerPhiInfo& ); // stop default
};

#endif /* ZDWEIGHT2LAYERPHIFILESOURCEFORMAT_ZDWEIGHT2LAYERPHIINFO_H */
