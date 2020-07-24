#if !defined(DRWEIGHT2LAYERDRIFTENTANGFILESOURCEFORMAT_DRWEIGHT2LAYERDRIFTENTANGINFO_H)
#define DRWEIGHT2LAYERDRIFTENTANGFILESOURCEFORMAT_DRWEIGHT2LAYERDRIFTENTANGINFO_H
// -*- C++ -*-
//
// Package:     DRWeight2LayerDriftEntAngFileSourceFormat
// Module:      DRWeight2LayerDriftEntAngInfo
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read DRWeight2LayerDriftEntAng constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:02:26 EDT 2003
// $Id: DRWeight2LayerDriftEntAngInfo.h,v 1.0 2003/09/22 15:02:26 mccann Exp $
//
// Revision history
//
// $Log: DRWeight2LayerDriftEntAngInfo.h,v $

// system include files

// user include files
#include "DBDRWeight2LayerDriftEntAng.hh"

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

class DRWeight2LayerDriftEntAngInfo
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DBDRWeight2LayerDriftEntAng ConstantType;
      
      static const Stream::Type& stream() { return DrStream::kWeight2LayerDriftEntAng; }

      static const char* binderTypeName() { return "drweight2layerdriftentangfile"; }
      static const char* extension() { return ".drweight2layerdriftentang"; }

   private:
      DRWeight2LayerDriftEntAngInfo(); //stop default
      DRWeight2LayerDriftEntAngInfo( const DRWeight2LayerDriftEntAngInfo& ); // stop default
      const DRWeight2LayerDriftEntAngInfo& operator=( const DRWeight2LayerDriftEntAngInfo& ); // stop default
};

#endif /* DRWEIGHT2LAYERDRIFTENTANGFILESOURCEFORMAT_DRWEIGHT2LAYERDRIFTENTANGINFO_H */
