// -*- C++ -*-
//
// Package:     MyProc
// Module:      MyProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Oct 12 15:48:03 EDT 2000
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "MyProc/MyProc.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "ADRGeom/ADRSenseWireStore.h"
#include "DRGeom/DRGeometryParams.h"

// STL classes
// You may have to uncomment some of these or other stl headers 
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Processor.MyProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.16 2000/08/21 16:13:51 marsh Exp $";
static const char* const kTagString = "$Name: v03_02_03 $";

//
// static data member definitions
//

//
// constructors and destructor
//
MyProc::MyProc( void )               // anal1
   : Processor( "MyProc" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &MyProc::geometry,    Stream::kDRAlignment );
   //bind( &MyProc::beginRun, Stream::kBeginRun );
   //bind( &MyProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

MyProc::~MyProc()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
MyProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
MyProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
MyProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
// ActionBase::ActionResult
// MyProc::event( Frame& iFrame )          // anal3 equiv.
// {
//    report( DEBUG, kFacilityString ) << "here in event()" << endl;

//    return ActionBase::kPassed;
// }


ActionBase::ActionResult
MyProc::geometry( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in geometry()" << endl;

   FAItem< ADRSenseWireStore > wireStore ;
   extract( iFrame.record( Stream::kDRAlignment ), wireStore ) ;

   // Keep count of wires.
   unsigned int nSenseTotal = 0 ;
   unsigned int nFieldTotal = 0 ;

   // Probability of passing through a wire's worth of each of these materials.
   double probAl = 0. ;
   double probAu = 0. ;
   double probW  = 0. ;

   static Meters WDiam = 2.e-5 ;
   static Meters AlDiam = 1.1e-4 ;
   static Meters AuDiam = 1.115e-4 ;

   for( int iLayer = ADRSenseWireStore::kFirstLayer ;
	iLayer < ADRSenseWireStore::kFirstLayer +
	   ADRSenseWireStore::kNumberOfLayers ;
	++iLayer )
   {
      const AEWireLayerCylindrical& layer = wireStore->layer( iLayer ) ;
      Meters senseRadius = layer.radiusNominal() ;
      Meters fieldRadius = senseRadius - layer.radialExtentMinus() ;
      unsigned int nSense = layer.numberOfWires() ;

//       cout << "Layer " << iLayer
// 	   << " r_sense " << senseRadius
// 	   << " r_field " << fieldRadius
// 	   << " n_sense " << nSense << endl ;

      // In a sense layer, # field wires = # sense wires.
      // In a field layer, # field wires = # (sense+field) in adjacent
      //    sense layer with higher radius.
      // Except in Layer 0.5, where #field = #sense in Layer 1.

      // There are 2 field layers between 16 and 17.  The first one has
      // the same number of wires in 16, the second has same number of
      // wires in 17.  Use radialExtentPlus for Layer 16 only.

      unsigned int nField = 2 * nSense ;
      if( iLayer == 1 )
      {
	 nField = nSense ;
      }

      // Probability of strinking a wire's worth of each material separately.
      // The sense wires are 20 micron diam tungsten.
      // The field wires are 110 micron diam Al-5056 coated with a gold layer
      // 0.75 microns thick.
      // [ Al-5056 = 0.12% Mn, 5%, Mg, 0.12% Cr, rest Al.]

      probW  += nSense * WDiam / (  2. * M_PI * senseRadius ) ;
      probAl += nSense * AlDiam / ( 2. * M_PI * senseRadius ) +
	 nField * AlDiam / ( 2. * M_PI * fieldRadius ) ;
      probAu += nSense * AuDiam / ( 2. * M_PI * senseRadius ) +
	 nField * AuDiam / ( 2. * M_PI * fieldRadius ) ;

      nSenseTotal += nSense ;
      nFieldTotal += nField + nSense ;

      if( iLayer == 16 )
      {
	 Meters fieldRadius2 = senseRadius + layer.radialExtentPlus() ;
	 probAl += nField * AlDiam / ( 2. * M_PI * fieldRadius2 ) ;
	 probAu += nField * AuDiam / ( 2. * M_PI * fieldRadius2 ) ;
	 nFieldTotal += nField ;
      }
   }

   cout << endl << "prob W " << probW << " Al " << probAl
	<< " Au " << probAu << endl << endl ;

   // Assuming uniform illumination, the average path length through a
   // cylinder is area/diameter = pi * radius / 2
   static Meters avgPathW  = M_PI * WDiam / 4. ;
   static Meters avgPathAl = M_PI * AlDiam / 4. ;
   static Meters avgPathAu =
      M_PI * ( sqr( AuDiam/2. ) - sqr( AlDiam/2. ) ) / AuDiam ;

   cout << "Avg path/wire W " << avgPathW << " Al " << avgPathAl
	<< " Au " << avgPathAu << endl ;
   cout << "Path/track W " << avgPathW * probW
	<< " Al " << avgPathAl * probAl
	<< " Au " << avgPathAu * probAu << endl << endl ;

//    double drArea = M_PI * ( sqr( DRGeometryParams::DROuterRadius ) -
// 			    sqr( DRGeometryParams::DRInnerRadius ) ) ;
//    double wireArea = M_PI * (
//       nFieldTotal * sqr( AuDiam/2. ) + nSenseTotal * sqr( WDiam/2. ) ) ;

   cout << "DR inner rad " << DRGeometryParams::kDRInnerRadius
	<< " outer rad " << DRGeometryParams::kDROuterRadius << endl ;
// 	<< " DR area " << drArea << endl ;
//    cout << "NSense " << nSenseTotal << " NField " << nFieldTotal
// 	<< " wireArea " << wireArea << endl ;

   Meters drPathLength =
      DRGeometryParams::kDROuterRadius - DRGeometryParams::kDRInnerRadius ;

   double WScale = avgPathW * probW / drPathLength ;
   double AlScale = avgPathAl * probAl / drPathLength ;
   double AuScale = avgPathAu * probAu / drPathLength ;

   cout << "Scale factor W " << WScale
	<< " Al " << AlScale << " Au " << AuScale << endl << endl ;

   double overallScale = 1. + WScale + AlScale + AuScale ;

   cout << "Vaporized Wire Gas: "
	<< endl << "   W: " << WScale/overallScale
	<< endl << "  Al: " << AlScale/overallScale
	<< endl << "  Au: " << AuScale/overallScale << endl << endl ;

   return ActionBase::kPassed;
}


/*
ActionBase::ActionResult
MyProc::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

//
// const member functions
//

//
// static member functions
//
