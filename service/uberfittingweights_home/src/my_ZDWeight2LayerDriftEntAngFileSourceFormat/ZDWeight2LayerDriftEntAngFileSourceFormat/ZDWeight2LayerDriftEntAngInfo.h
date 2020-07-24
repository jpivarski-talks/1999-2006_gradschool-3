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
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:14:44 EST 1999
// $Id: ZDWeight2LayerDriftEntAngInfo.h,v 1.2 1999/10/10 12:58:25 cdj Exp $
//
// Revision history
//
// $Log: ZDWeight2LayerDriftEntAngInfo.h,v $
// Revision 1.2  1999/10/10 12:58:25  cdj
// forgot to remove BDLclient from skeleton include
//
// Revision 1.1  1999/03/13 22:44:45  cdj
// First submission
//

// system include files

// user include files
#include "DBZDWeight2LayerDriftEntAng.hh"
#include "DataHandler/Stream.h"
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
