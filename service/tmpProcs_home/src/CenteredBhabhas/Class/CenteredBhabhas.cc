// -*- C++ -*-
//
// Package:     CenteredBhabhas
// Module:      CenteredBhabhas
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Mar 22 05:24:07 EST 2004
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "CenteredBhabhas/CenteredBhabhas.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"


#include "Navigation/NavShower.h"
#include "C3cc/CcShowerAttributes.h"


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
static const char* const kFacilityString = "Processor.CenteredBhabhas" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.35 2004/03/08 02:40:17 cdj Exp $";
static const char* const kTagString = "$Name: v06_09_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
CenteredBhabhas::CenteredBhabhas( void )               // anal1
   : Processor( "CenteredBhabhas" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &CenteredBhabhas::event,    Stream::kEvent );
   //bind( &CenteredBhabhas::beginRun, Stream::kBeginRun );
   //bind( &CenteredBhabhas::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

CenteredBhabhas::~CenteredBhabhas()                    // anal5
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
CenteredBhabhas::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CenteredBhabhas::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
CenteredBhabhas::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
CenteredBhabhas::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;



   FATable< NavShower > showerTable;
   extract( iFrame.record( Stream::kEvent ) , showerTable );

   int goodshowers = 0;

   FATable< NavShower >::const_iterator showerBegin = showerTable.begin();
   FATable< NavShower >::const_iterator showerEnd = showerTable.end();
   for (FATable< NavShower >::const_iterator showerItr = showerBegin; 
        showerItr != showerEnd ; ++showerItr )
   {
      if (showerItr->attributes().energy() > 1.  &&
	  fabs(cos(showerItr->attributes().theta())) < 0.1)
	 goodshowers++;

   }

   if (goodshowers == 2) return ActionBase::kPassed;
   else return ActionBase::kFailed;
}

/*
ActionBase::ActionResult
CenteredBhabhas::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
CenteredBhabhas::endRun( Frame& iFrame )         // anal4 equiv.
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
