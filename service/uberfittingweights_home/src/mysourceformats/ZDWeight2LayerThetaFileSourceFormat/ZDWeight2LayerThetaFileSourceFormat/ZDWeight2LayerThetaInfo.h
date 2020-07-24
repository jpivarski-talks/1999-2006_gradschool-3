#if !defined(ZDWEIGHT2LAYERTHETAFILESOURCEFORMAT_ZDWEIGHT2LAYERTHETAINFO_H)
#define ZDWEIGHT2LAYERTHETAFILESOURCEFORMAT_ZDWEIGHT2LAYERTHETAINFO_H
// -*- C++ -*-
//
// Package:     ZDWeight2LayerThetaFileSourceFormat
// Module:      ZDWeight2LayerThetaInfo
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read ZDWeight2LayerTheta constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:19:50 EDT 2003
// $Id: ZDWeight2LayerThetaInfo.h,v 1.0 2003/09/22 15:19:50 mccann Exp $
//
// Revision history
//
// $Log: ZDWeight2LayerThetaInfo.h,v $

// system include files

// user include files
#include "DBZDWeight2LayerTheta.hh"

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

class ZDWeight2LayerThetaInfo
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DBZDWeight2LayerTheta ConstantType;
      
      static const Stream::Type& stream() { return ZdStream::kWeight2LayerTheta; }

      static const char* binderTypeName() { return "zdweight2layerthetafile"; }
      static const char* extension() { return ".zdweight2layertheta"; }

   private:
      ZDWeight2LayerThetaInfo(); //stop default
      ZDWeight2LayerThetaInfo( const ZDWeight2LayerThetaInfo& ); // stop default
      const ZDWeight2LayerThetaInfo& operator=( const ZDWeight2LayerThetaInfo& ); // stop default
};

#endif /* ZDWEIGHT2LAYERTHETAFILESOURCEFORMAT_ZDWEIGHT2LAYERTHETAINFO_H */
