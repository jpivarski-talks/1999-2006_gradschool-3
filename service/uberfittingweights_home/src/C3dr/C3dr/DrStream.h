#if !defined(C3DR_DRSTREAM_H)
#define C3DR_DRSTREAM_H
// -*- C++ -*-
//
// Package:     C3dr
// Module:      DrStream
// 
// Description: Drift Chamber Calibration Streams
//
// Usage:
//
// Author:      Kenneth McLean
// Created:     Tue Dec 21 14:19:49 EDT 1999
// user include files
#include "DataHandler/Stream.h"

// forward declarations

class dummyDrStreamFriend ;

class DrStream
{
      // ---------- friend classes and functions ---------------
      friend class dummyDrStreamFriend ; // to suppress warnings

      // ---------- static data members ------------------------
   public:

      static const Stream::Type kvarious          ;
      static const Stream::Type kDriftData        ;
      static const Stream::Type kDriftDatav2      ;
      static const Stream::Type kWireQuality      ;
      static const Stream::Type ktdc2time         ;
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
      static const Stream::Type kt0channelv2      ;
      static const Stream::Type kGarfield_TimeCorrection      ;
      static const Stream::Type kGarfield_DistanceCorrection  ;
      static const Stream::Type kDaqCellIDMap     ;
      static const Stream::Type kadcpedestal      ;
      static const Stream::Type kFBUSChannel      ;
      static const Stream::Type kElecMap          ;
      static const Stream::Type kadcsaturation    ;

   private:
      // ---------- Constructors and destructor ----------------
      DrStream();
      virtual ~DrStream();
      DrStream( const DrStream& ); // stop default
      const DrStream& operator=( const DrStream& ); // stop default
};
#endif /* C3DR_DRSTREAM_H */


