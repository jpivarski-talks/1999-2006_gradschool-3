// -*- C++ -*-
//
// Package:     C3dr
// Module:      DrStream
// 
// Description: fill stream names
//
// Implementation:
//
// Author:      K.W.McLean
// Created:     Tue Dec 21 14:19:52 EDT 1999
//

#include "Experiment/Experiment.h"

// user include files
#include "C3dr/DrStream.h"

//
// static data member definitions
//

const Stream::Type
DrStream::kDaqCellIDMap( "drdaqcellidmap", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kWireQuality( "drwirequality", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
DrStream::kvarious("drvarious", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
DrStream::kDriftData("drdriftdata", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
DrStream::kDriftDatav2("drdriftdatav2", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
DrStream::kGarfield_DistanceCorrection( "drgarfield_distancecorrection", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kGarfield_TimeCorrection( "drgarfield_timecorrection", 
		       Stream::kCalibrationPriority, 
		       true);


const Stream::Type
DrStream::kWeightDrift( "drweightdrift", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kWeightLayer( "drweightlayer", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kWeightTheta( "drweighttheta", 
			Stream::kCalibrationPriority, 
			true);

const Stream::Type
DrStream::kWeightPhi( "drweightphi", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
DrStream::kWeight2LayerDriftEntAng( "drweight2layerdriftentang",
				    Stream::kCalibrationPriority, 
				    true );

const Stream::Type
DrStream::kWeight2LayerTheta( "drweight2layertheta",
			      Stream::kCalibrationPriority, 
			      true );

const Stream::Type
DrStream::kWeight2LayerPhi( "drweight2layerphi",
			    Stream::kCalibrationPriority, 
			    true );


const Stream::Type
DrStream::kadc2charge( "dradc2charge", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kadcpedestal( "dradcpedestal", 
		       Stream::kCalibrationPriority, 
		       true);


const Stream::Type
DrStream::kt0global( "drt0global", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kt0crate( "drt0crate", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kt0card( "drt0card", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kt0hvcard( "drt0hvcard", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kt0channel( "drt0channel", 
		       Stream::kCalibrationPriority, 
		       true);
const Stream::Type
DrStream::kt0channelv2( "drt0channelv2", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
DrStream::ktdc2time( "drtdc2time", 
		       Stream::kCalibrationPriority, 
		       true);

const Stream::Type
DrStream::kFBUSChannel( "drfbuschannel", 
			Stream::kCalibrationPriority, 
			true);

const Stream::Type
DrStream::kElecMap( "drelecmap", 
		    Stream::kCalibrationPriority, 
		    true);

const Stream::Type
DrStream::kadcsaturation( "dradcsaturation", 
		       Stream::kCalibrationPriority, 
		       true);






