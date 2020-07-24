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
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:14:44 EST 1999
// $Id: DRWeight2LayerThetaInfo.h,v 1.2 1999/10/10 12:58:25 cdj Exp $
//
// Revision history
//
// $Log: DRWeight2LayerThetaInfo.h,v $
// Revision 1.2  1999/10/10 12:58:25  cdj
// forgot to remove BDLclient from skeleton include
//
// Revision 1.1  1999/03/13 22:44:45  cdj
// First submission
//

// system include files

// user include files
#include "DBDRWeight2LayerTheta.hh"
#include "DataHandler/Stream.h"
#include "C3dr/DrStream.h"

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
