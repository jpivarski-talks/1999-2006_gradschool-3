// -*- C++ -*-
//
// Package:     C3zd
// Module:      ZdStream
// 
// Description: fill stream names
//
// Implementation:
//
// Author:      Guangshun Huang
// Created:     Wed Jan 15 2003
//

#include "Experiment/Experiment.h"

// user include files
#include "C3zd/ZdStream.h"

//
// static data member definitions
//

const Stream::Type
ZdStream::kDaqCellIDMap( "zddaqcellidmap", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
ZdStream::kWireQuality( "zdwirequality", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
ZdStream::kWirePropagation("zdwirepropagation", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
ZdStream::kDriftData("zddriftdata", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
ZdStream::kGarfield_DistanceCorrection( "zdgarfield_distancecorrection", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
ZdStream::kGarfield_TimeCorrection( "zdgarfield_timecorrection", 
		       Stream::kCalibrationPriority, 
		       true);


// const Stream::Type
// ZdStream::kWeightDrift( "zdweightdrift", 
// 		       Stream::kCalibrationPriority, 
// 		       true);
// const Stream::Type
// ZdStream::kWeightLayer( "zdweightlayer", 
// 		       Stream::kCalibrationPriority, 
// 		       true);
// const Stream::Type
// ZdStream::kWeightTheta( "zdweighttheta", 
// 			Stream::kCalibrationPriority, 
// 			true);

// const Stream::Type
// ZdStream::kWeightPhi( "zdweightphi", 
// 		       Stream::kCalibrationPriority, 
// 		       true);

const Stream::Type
ZdStream::kWeight2LayerDriftEntAng( "zdweight2layerdriftentang",
				    Stream::kCalibrationPriority,
				    true);
const Stream::Type
ZdStream::kWeight2LayerTheta( "zdweight2layertheta",
			      Stream::kCalibrationPriority,
			      true);
const Stream::Type
ZdStream::kWeight2LayerPhi( "zdweight2layerphi",
			    Stream::kCalibrationPriority,
			    true);

const Stream::Type
ZdStream::kadc2charge( "zdadc2charge", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
ZdStream::kadcpedestal( "zdadcpedestal", 
		       Stream::kCalibrationPriority, 
		       true);


const Stream::Type
ZdStream::kt0global( "zdt0global", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
ZdStream::kt0crate( "zdt0crate", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
ZdStream::kt0card( "zdt0card", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
ZdStream::kt0hvcard( "zdt0hvcard", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
ZdStream::kt0channel( "zdt0channel", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
ZdStream::kFBUSChannel( "zdfbuschannel", 
			Stream::kCalibrationPriority, 
			true);

const Stream::Type
ZdStream::kElecMap( "zdelecmap", 
		    Stream::kCalibrationPriority, 
		    true);

const Stream::Type
ZdStream::kadcsaturation( "zdadcsaturation", 
		       Stream::kCalibrationPriority, 
		       true);
