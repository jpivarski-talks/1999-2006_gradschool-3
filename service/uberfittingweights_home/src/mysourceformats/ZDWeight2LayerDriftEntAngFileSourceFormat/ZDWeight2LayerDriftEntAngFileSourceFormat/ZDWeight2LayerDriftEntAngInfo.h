#if !defined(ZDWEIGHT2LAYERDRIFTENTANGFILESOURCEFORMAT_ZDWEIGHT2LAYERDRIFTENTANGINFO_H)
#define ZDWEIGHT2LAYERDRIFTENTANGFILESOURCEFORMAT_ZDWEIGHT2LAYERDRIFTENTANGINFO_H
// -*- C++ -*-
//
// Package:     ZDWeight2LayerDriftEntAngFileSourceFormat
// Module:      ZDWeight2LayerDriftEntAngInfo
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read ZDWeight2LayerDriftEntAng constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:19:44 EDT 2003
// $Id: ZDWeight2LayerDriftEntAngInfo.h,v 1.0 2003/09/22 15:19:44 mccann Exp $
//
// Revision history
//
// $Log: ZDWeight2LayerDriftEntAngInfo.h,v $

// system include files

// user include files
#include "DBZDWeight2LayerDriftEntAng.hh"

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

class ZDWeight2LayerDriftEntAngInfo
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DBZDWeight2LayerDriftEntAng ConstantType;
      
      static const Stream::Type& stream() { return ZdStream::kWeight2LayerDriftEntAng; }

      static const char* binderTypeName() { return "zdweight2layerdriftentangfile"; }
      static const char* extension() { return ".zdweight2layerdriftentang"; }

   private:
      ZDWeight2LayerDriftEntAngInfo(); //stop default
      ZDWeight2LayerDriftEntAngInfo( const ZDWeight2LayerDriftEntAngInfo& ); // stop default
      const ZDWeight2LayerDriftEntAngInfo& operator=( const ZDWeight2LayerDriftEntAngInfo& ); // stop default
};

#endif /* ZDWEIGHT2LAYERDRIFTENTANGFILESOURCEFORMAT_ZDWEIGHT2LAYERDRIFTENTANGINFO_H */
