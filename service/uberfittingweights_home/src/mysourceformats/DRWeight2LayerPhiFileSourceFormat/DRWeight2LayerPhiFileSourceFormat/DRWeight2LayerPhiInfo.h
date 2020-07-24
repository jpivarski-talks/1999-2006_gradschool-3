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
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:19:35 EDT 2003
// $Id: DRWeight2LayerPhiInfo.h,v 1.0 2003/09/22 15:19:35 mccann Exp $
//
// Revision history
//
// $Log: DRWeight2LayerPhiInfo.h,v $

// system include files

// user include files
#include "DBDRWeight2LayerPhi.hh"

// Choose your stream
// #include "C3DRCA/DRCAStream.h"
// #include "C3Dedx/DedxStream.h"
// #include "C3MagFieldMap/MagFieldMapStream.h"
// #include "C3MagFudge/MagFudgeStream.h"
// #include "C3Mu/MuStream.h"
// #include "C3Rich/RichStream.h"
// #include "C3cc/CcStream.h"
#include "C3dr/DrStream.h"
// #include "C3sv/SiStream.h"
// #include "C3trig/TrigStream.h"
// #include "C3zd/ZdStream.h"


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
