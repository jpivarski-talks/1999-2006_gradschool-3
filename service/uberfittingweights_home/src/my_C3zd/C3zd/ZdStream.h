#if !defined(C3ZD_ZDSTREAM_H)
#define C3ZD_ZDSTREAM_H
// -*- C++ -*-
//
// Package:     C3zd
// Module:      ZdStream
// 
// Description: ZD Calibration Streams
//
// Usage:
//
// Author:      Guangshun Huang
// Created:     Wed Jan 15 2003
// user include files
#include "DataHandler/Stream.h"

// forward declarations

class dummyZdStreamFriend ;

class ZdStream
{
      // ---------- friend classes and functions ---------------
      friend class dummyZdStreamFriend ; // to suppress warnings

      // ---------- static data members ------------------------
   public:

      static const Stream::Type kWirePropagation  ;
      static const Stream::Type kDriftData        ;
      static const Stream::Type kWireQuality      ;
      static const Stream::Type kadc2charge       ;
      static const Stream::Type kWeightLayer      ;
      static const Stream::Type kWeightDrift      ;
      static const Stream::Type kWeightTheta      ;
      static const Stream::Type kWeightPhi        ;
      static const Stream::Type kWeight2LayerDriftEntAng ;
      static const Stream::Type kWeight2LayerTheta       ;
      static const Stream::Type kWeight2LayerPhi         ;
      static const Stream::Type kt0global         ;
      static const Stream::Type kt0crate          ;
      static const Stream::Type kt0card           ;
      static const Stream::Type kt0hvcard         ;
      static const Stream::Type kt0channel        ;
      static const Stream::Type kGarfield_TimeCorrection      ;
      static const Stream::Type kGarfield_DistanceCorrection  ;
      static const Stream::Type kDaqCellIDMap     ;
      static const Stream::Type kadcpedestal      ;
      static const Stream::Type kFBUSChannel      ;
      static const Stream::Type kElecMap          ;
      static const Stream::Type kadcsaturation    ;

   private:
      // ---------- Constructors and destructor ----------------
      ZdStream();
      virtual ~ZdStream();
      ZdStream( const ZdStream& ); // stop default
      const ZdStream& operator=( const ZdStream& ); // stop default
};
#endif /* C3ZD_ZDSTREAM_H */
