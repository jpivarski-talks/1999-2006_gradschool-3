#if !defined(DRWEIGHT2LAYERTHETAFILESOURCEFORMAT_DRWEIGHT2LAYERTHETAINFO_H)
#define DRWEIGHT2LAYERTHETAFILESOURCEFORMAT_DRWEIGHT2LAYERTHETAINFO_H
// -*- C++ -*-
//
// Package:     DRWeight2LayerThetaFileSourceFormat
// Module:      DRWeight2LayerThetaInfo
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read DRWeight2LayerTheta constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      Jim Pivarski
// Created:     Mon Sep 22 15:19:30 EDT 2003
// $Id: DRWeight2LayerThetaInfo.h,v 1.0 2003/09/22 15:19:30 mccann Exp $
//
// Revision history
//
// $Log: DRWeight2LayerThetaInfo.h,v $

// system include files

// user include files
#include "DBDRWeight2LayerTheta.hh"

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

class DRWeight2LayerThetaInfo
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DBDRWeight2LayerTheta ConstantType;
      
      static const Stream::Type& stream() { return DrStream::kWeight2LayerTheta; }

      static const char* binderTypeName() { return "drweight2layerthetafile"; }
      static const char* extension() { return ".drweight2layertheta"; }

   private:
      DRWeight2LayerThetaInfo(); //stop default
      DRWeight2LayerThetaInfo( const DRWeight2LayerThetaInfo& ); // stop default
      const DRWeight2LayerThetaInfo& operator=( const DRWeight2LayerThetaInfo& ); // stop default
};

#endif /* DRWEIGHT2LAYERTHETAFILESOURCEFORMAT_DRWEIGHT2LAYERTHETAINFO_H */
