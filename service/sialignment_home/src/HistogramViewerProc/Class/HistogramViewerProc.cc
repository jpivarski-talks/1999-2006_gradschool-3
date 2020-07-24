// -*- C++ -*-
//
// Package:     HistogramViewerProc
// Module:      HistogramViewerProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Jul 20 09:24:38 EDT 2001
// $Id: HistogramViewerProc.cc,v 1.1.1.1 2001/07/30 14:11:45 cdj Exp $
//
// Revision history
//
// $Log: HistogramViewerProc.cc,v $
// Revision 1.1.1.1  2001/07/30 14:11:45  cdj
// importing
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "qapplication.h"
#include "qtimer.h"

#include "HistogramViewerProc/HistogramViewerProc.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "HistogramViewerProc/HVPWindow.h"

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
static const char* const kFacilityString = "Processor.HistogramViewerProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HistogramViewerProc.cc,v 1.1.1.1 2001/07/30 14:11:45 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HistogramViewerProc::HistogramViewerProc( void )               // anal1
   : Processor( "HistogramViewerProc" ),
     m_window(0),
     m_app(0),
     m_numberStopsSinceLastUpdate(0)
{
   //report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &HistogramViewerProc::action,    Stream::kEvent );
   //bind( &HistogramViewerProc::beginRun, Stream::kBeginRun );
   //bind( &HistogramViewerProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

HistogramViewerProc::~HistogramViewerProc()                    // anal5
{
   //report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
HistogramViewerProc::init( void )          // anal1 "Interactive"
{
   //report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
HistogramViewerProc::terminate( void )     // anal5 "Interactive"
{
   //report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
static int argc=1;
static char*  argv[] ={"HistogramViewer"};

void
HistogramViewerProc::hist_book( HIHistoManager& iHistoManager )
{
   //report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_app = new QApplication(argc, &(argv[0]) );
   m_window = new HVPWindow(&iHistoManager);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
HistogramViewerProc::action( Frame& iFrame )          // anal3 equiv.
{
   //report( DEBUG, kFacilityString ) << "here in event()" << endl;
   ++m_numberStopsSinceLastUpdate;

   if( 0 == m_app->mainWidget() ) {
      m_app->setMainWidget( m_window );
      m_window->show();
      m_app->exec();
   } else {
     
     if(m_numberStopsSinceLastUpdate >= m_window->frequency()) {
         m_numberStopsSinceLastUpdate = 0;
         m_window->updateInfo();
         QTimer* timer = new QTimer(m_window);
         QObject::connect( timer, SIGNAL(timeout()),
	                m_app, SLOT(quit()));
//      timer->start(50,TRUE);
         timer->start(0,TRUE);
         m_app->exec();

         if( m_window->interactionRequested() ) {
	    m_app->exec();
         }
     }
   }
   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
HistogramViewerProc::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
HistogramViewerProc::endRun( Frame& iFrame )         // anal4 equiv.
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
