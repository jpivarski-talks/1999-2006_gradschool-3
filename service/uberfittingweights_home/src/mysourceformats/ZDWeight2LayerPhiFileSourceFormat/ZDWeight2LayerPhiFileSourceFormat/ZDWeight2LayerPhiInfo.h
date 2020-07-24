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
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:20:01 EDT 2003
// $Id: ZDWeight2LayerPhiInfo.h,v 1.0 2003/09/22 15:20:01 mccann Exp $
//
// Revision history
//
// $Log: ZDWeight2LayerPhiInfo.h,v $

// system include files

// user include files
#include "DBZDWeight2LayerPhi.hh"

// Choose your stream
// #include "C3DRCA/DRCAStream.h"
// #include "C3Dedx/DedxStream.h"
// #include "C3MagFieldMap/MagFieldMapStream.h"
// #include "C3MagFudge/MagFudgeStream.h"
// #include "C3Mu/MuStream.h"
// #include "C3Rich/RichStream.h"
// #include "C3cc/CcStream.h"
// #include "C3dr/DrStream.h"
// #include "C3sv/SiStream.h"
// #include "C3trig/TrigStream.h"
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
