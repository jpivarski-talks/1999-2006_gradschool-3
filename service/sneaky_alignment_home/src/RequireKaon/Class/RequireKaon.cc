// -*- C++ -*-
//
// Package:     RequireKaon
// Module:      RequireKaon
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Mar 20 14:49:46 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "RequireKaon/RequireKaon.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "MCInfo/MCDecayTree/MCDecayTree.h"
#include "MCInfo/MCDecayTree/MCParticle.h"
#include "MCInfo/MCParticleProperty/MCParticleProperty.h"
#include "MCInfo/MCParticleProperty/MCParticlePropertyStore.h"

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
static const char* const kFacilityString = "Processor.RequireKaon" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
RequireKaon::RequireKaon( void )               // anal1
   : Processor( "RequireKaon" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &RequireKaon::event,    Stream::kEvent );
   //bind( &RequireKaon::beginRun, Stream::kBeginRun );
   //bind( &RequireKaon::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

RequireKaon::~RequireKaon()                    // anal5
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
RequireKaon::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
RequireKaon::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
RequireKaon::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
RequireKaon::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<MCDecayTree> tree;
   extract(iFrame.record(Stream::kEvent), tree, "Generator");
   MCDecayTree::const_pIterator tree_end = tree->pEnd();

   report( DEBUG, kFacilityString ) << "got tree" << endl;

   const MCParticlePropertyStore& mcppstore =
     tree->topParticle().properties().store();
   int kshort_id = mcppstore.nameToQQId("K0S");
   int piplus_id = mcppstore.nameToQQId("PI+");
   int piminus_id = mcppstore.nameToQQId("PI-");
 
   report( DEBUG, kFacilityString ) << "got mcppstore" << endl;

   DABoolean seeUsefulKaon(false);

   for( MCDecayTree::const_pIterator part_iter = tree->pBegin();
        part_iter != tree_end;
	++part_iter ) {
      report( DEBUG, kFacilityString ) << "  for a given leaf..." << endl;

      if ( part_iter->properties().QQId() == kshort_id ) {
	 report( DEBUG, kFacilityString ) << "  this leaf is a Kshort..." << endl;

	 seeUsefulKaon = true;

      } // end if the particle is a kaon
   } // end loop over the whole tree
   
   if ( seeUsefulKaon ) {
      report( DEBUG, kFacilityString ) << "event passes because I saw a useful kaon, after all!" << endl;
      return ActionBase::kPassed;
   }
   else return ActionBase::kFailed;
}

/*
ActionBase::ActionResult
RequireKaon::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
RequireKaon::endRun( Frame& iFrame )         // anal4 equiv.
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
