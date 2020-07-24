#if !defined(DATAHANDLER_FRAMELOGGER_H)
#define DATAHANDLER_FRAMELOGGER_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      FrameLogger
// 
// Description: MessageLog::Logger specialized to the Frame environment
//
// Usage:
//    As with any Logger except setFrame(   Frame* )
//                         and  unSetFrame( Frame* ) are available.
//
// Author:      Brian K. Heltsley
// Created:     Fri Jul 31 16:03:28 EDT 1998
// $Id: FrameLogger.h,v 1.1 1998/09/22 16:44:14 bkh Exp $
//
// Revision history
//
// $Log: FrameLogger.h,v $
// Revision 1.1  1998/09/22 16:44:14  bkh
// moved FrameLogger from ToolBox per mkl request
//
// Revision 1.1  1998/08/14 20:09:48  bkh
// new Logger to print out run,event,stop
//

// system include files
#include <time.h>

// user include files
#include "ToolBox/MessageLog.h"

// forward declarations

class Frame ;

typedef MessageLog::Logger Logger ;

class FrameLogger : public Logger
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      // ---------- Constructors and destructor ----------------
      FrameLogger( Severity level,
		   ostream& out   ) ;
      // : Logger(level), _out(out) {}
      void Log(Severity      severity,
	       const string& facility,
	       const string& logmsg,
	       Messenger*    messenger ) ;

      //virtual ~FrameLogger() ;
      // ---------- member functions ---------------------------

      void setFrame(   Frame* aFramePtr ) ;  // to set a Frame
      void unSetFrame( Frame* aFramePtr ) ;  // to unset a Frame

      // ---------- const member functions ---------------------
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual bool Dumpit(Severity severity);

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      FrameLogger();
      FrameLogger( const FrameLogger& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const FrameLogger& operator=( const FrameLogger& ); // stop default
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------
      // ---------- data members -------------------------------

      ostream& m_out      ;

      time_t   m_oldTime  ;
      UInt32   m_oldRun   ;
      UInt32   m_oldEvt   ;
      string   m_oldStop  ;

      Frame*   m_framePtr ;

      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* DATAHANDLER_FRAMELOGGER_H */
