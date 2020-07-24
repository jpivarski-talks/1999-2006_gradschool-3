// -*- C++ -*-
//
// Package:     DataHandler
// Module:      FrameLogger
// 
// Description: Provide extra Frame-specific information when logging msgs
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Jul 31 16:03:35 EDT 1998
// $Id: FrameLogger.cc,v 1.2 2003/01/27 20:07:25 cdj Exp $
//
// Revision history
//
// $Log: FrameLogger.cc,v $
// Revision 1.2  2003/01/27 20:07:25  cdj
// get iomanip.h from C++Std package
//
// Revision 1.1  1998/09/22 16:44:00  bkh
// moved FrameLogger from ToolBox per mkl request
// typed a nonTyped variable in StreamType.cc (noticed as a warning msg)
//
// Revision 1.3  1998/08/19 21:10:07  bkh
// fix string problem for g++
//
// Revision 1.2  1998/08/14 20:51:09  bkh
// forgot to implement unSetFrame()
//
// Revision 1.1  1998/08/14 20:09:43  bkh
// new Logger to print out run,event,stop
//

#include "Experiment/Experiment.h"

// system include files
#include "C++Std/iomanip.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/FrameLogger.h"

#include "DataHandler/Frame.h"

// STL classes
#include <string>

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FrameLogger::FrameLogger( Severity level ,
			  ostream& out     ) :
   Logger     ( level       ) ,
   m_out      ( out         ) ,
   m_oldTime  ( ::time( 0 ) ) ,
   m_oldRun   ( 0           ) ,
   m_oldEvt   ( 0           ) ,
   m_oldStop  ( " "         ) ,
   m_framePtr ( 0           )
{
}

// FrameLogger::FrameLogger( const FrameLogger& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//FrameLogger::~FrameLogger()
//{
//}

//
// assignment operators
//
// const FrameLogger& FrameLogger::operator=( const FrameLogger& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

void 
FrameLogger::setFrame( Frame* aFramePtr )
{
   m_framePtr = aFramePtr ;
}

void 
FrameLogger::unSetFrame( Frame* aFramePtr )
{
   if( m_framePtr == aFramePtr )
   {
      m_framePtr = 0 ;
   }
}

//
// FrameLogger -- by default, DEBUG streams always dump, also dump for
// ALERT or worse.
//

bool
FrameLogger::Dumpit( Severity severity )
{
   // IF this is a debug stream OR this is a very serious message,
   // then log a dump (if we can).
   return Logit(DEBUG) || severity <= ALERT;
}

//
// Log method for the FrameLogger class.
//

void
FrameLogger::Log(Severity      severity ,
		 const string& facility ,
		 const string& logmsg   ,
		 Messenger*    messenger  )
{
   if( Logit( severity ) ) 
   {
      time_t now = ::time(0);

      const DABoolean ptrOK ( 0 != m_framePtr ) ;
      const UInt32 run  ( ptrOK ? m_framePtr->syncValue().runNumber()  :0 ) ;
      const UInt32 evt  ( ptrOK ? m_framePtr->syncValue().eventNumber():0 ) ;
      const string stop ( ptrOK ? m_framePtr->stopStream().value() : 
			  string(" ") ) ;

      const DABoolean newFrame ( run  != 0 &&  // log only if any have changed
				 ( ( run  != m_oldRun  )  ||
				   ( evt  != m_oldEvt  )  ||
				   !( stop == m_oldStop )     ) ) ;
     
      m_oldRun  = run  ;
      m_oldEvt  = evt  ;
      m_oldStop = stop ;

      if( newFrame )
      {
	 const string chartime ( ::ctime( &now ) ) ; // with embedded "\n"
	 const string timeNoCR ( chartime.substr(0, chartime.length()-1 ) ) ;

	 m_out << "\n >> " 
	       << timeNoCR ; // no carriage-return

	 m_out << " Run:" 
	       << setw(8) << run ;

	 if( ( 0 != evt ) && ( "event" == stop ) )
	 {
	    m_out << " Event:"
		  << setw(8) << evt ;
	 }

	 m_out << " Stop: "
	       << stop ;
      }

      if( newFrame )
      {
	 m_out << " << \n" ; // terminate time + frame info line
      }

      m_out << " %% "                       // begin next line
	    << SeverityString( severity ) 
	    << "-" 
	    << facility 
	    << ": "
	    << logmsg ;

// Messenger not currently used (sjp)
//      if (messenger != 0 && Dumpit(severity)) {
//	 messenger->Dump(m_out);
//      }
//
      m_out << flush ;
   }
}

//
// const member functions
//

//
// static member functions
//
