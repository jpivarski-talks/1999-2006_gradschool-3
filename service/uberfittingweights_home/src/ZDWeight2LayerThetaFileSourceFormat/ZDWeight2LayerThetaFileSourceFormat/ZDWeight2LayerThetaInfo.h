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
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:14:44 EST 1999
// $Id: ZDWeight2LayerThetaInfo.h,v 1.1.1.1 2003/09/30 17:11:57 mccann Exp $
//
// Revision history
//
// $Log: ZDWeight2LayerThetaInfo.h,v $
// Revision 1.1.1.1  2003/09/30 17:11:57  mccann
// imported ZDWeight2LayerThetaFileSourceFormat sources
//
// Revision 1.2  1999/10/10 12:58:25  cdj
// forgot to remove BDLclient from skeleton include
//
// Revision 1.1  1999/03/13 22:44:45  cdj
// First submission
//

// system include files

// user include files
#include "DBZDWeight2LayerTheta.hh"
#include "DataHandler/Stream.h"
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
