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
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:14:44 EST 1999
// $Id: DRWeight2LayerDriftEntAngInfo.h,v 1.1.1.1 2003/09/30 17:09:49 mccann Exp $
//
// Revision history
//
// $Log: DRWeight2LayerDriftEntAngInfo.h,v $
// Revision 1.1.1.1  2003/09/30 17:09:49  mccann
// imported DRWeight2LayerDriftEntAngFileSourceFormat sources
//
// Revision 1.2  1999/10/10 12:58:25  cdj
// forgot to remove BDLclient from skeleton include
//
// Revision 1.1  1999/03/13 22:44:45  cdj
// First submission
//

// system include files

// user include files
#include "DBDRWeight2LayerDriftEntAng.hh"
#include "DataHandler/Stream.h"
#include "C3dr/DrStream.h"

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
