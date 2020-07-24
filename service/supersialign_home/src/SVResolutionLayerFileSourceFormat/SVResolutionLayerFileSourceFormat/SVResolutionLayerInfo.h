#if !defined(SVRESOLUTIONLAYERFILESOURCEFORMAT_SVRESOLUTIONLAYERINFO_H)
#define SVRESOLUTIONLAYERFILESOURCEFORMAT_SVRESOLUTIONLAYERINFO_H
// -*- C++ -*-
//
// Package:     SVResolutionLayerFileSourceFormat
// Module:      SVResolutionLayerInfo
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read SVResolutionLayer constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:14:44 EST 1999
// $Id: SVResolutionLayerInfo.h,v 1.1.1.1 2001/05/22 16:30:01 mccann Exp $
//
// Revision history
//
// $Log: SVResolutionLayerInfo.h,v $
// Revision 1.1.1.1  2001/05/22 16:30:01  mccann
// FileSourceFormat for layer-dependant resolutions in silicon
//
// Revision 1.2  1999/10/10 12:58:25  cdj
// forgot to remove BDLclient from skeleton include
//
// Revision 1.1  1999/03/13 22:44:45  cdj
// First submission
//

// system include files

// user include files
#include "DBSVResolutionLayer.hh"
#include "DataHandler/Stream.h"
#include "C3sv/SiStream.h"

// forward declarations
class DummyFriend;

class SVResolutionLayerInfo
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DBSVResolutionLayer ConstantType;
      
      static const Stream::Type& stream() { return SiStream::kSVResolution; }

      static const char* binderTypeName() { return "svresolutionlayerfile"; }
      static const char* extension() { return ".svresolutionlayer"; }

   private:
      SVResolutionLayerInfo(); //stop default
      SVResolutionLayerInfo( const SVResolutionLayerInfo& ); // stop default
      const SVResolutionLayerInfo& operator=( const SVResolutionLayerInfo& ); // stop default
};

#endif /* SVRESOLUTIONLAYERFILESOURCEFORMAT_SVRESOLUTIONLAYERINFO_H */
