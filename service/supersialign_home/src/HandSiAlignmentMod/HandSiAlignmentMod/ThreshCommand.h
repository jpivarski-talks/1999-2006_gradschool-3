// -*- C++ -*-
#if !defined(HANDSIALIGNMENTMOD_THRESHCOMMAND_H)
#define HANDSIALIGNMENTMOD_THRESHCOMMAND_H
//
// Package:     HandSiAlignmentMod
// Module:      ThreshCommand
//
/**\class ThreshCommand ThreshCommand.h HandSiAlignmentMod/ThreshCommand.h
 
 Description: 

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Wed Apr 10 16:01:15 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

#include "CommandPattern/Command.h"
#include "HistogramViewer/HVCommunicate.h"

// user include files

// forward declarations

class HandSiAlignmentMod;

class ThreshCommand : public Command
{

      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef enum {
	 kNeither,
	 kPosThresh,
	 kNegThresh
      } WhichThresh;

      // ---------- Constructors and destructor ----------------
      ThreshCommand( const Command::Name& iName, 
		     HandSiAlignmentMod* target,
		     double thresh,
		     WhichThresh which );
      virtual ~ThreshCommand();

      // ---------- member functions ---------------------------
      // You may override the base class's execute method, but it's
      // easier to use the "Subcommand" class to handle subcommands.
      // (Have a look at the command::ctor for an example.)
      virtual int execute( int argc, char* argv[] );

      // ---------- const member functions ---------------------

      void setValue( double thresh ) { m_thresh = thresh; };
      const double value() const { return m_thresh; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual int helpHandler();

      HandSiAlignmentMod* target();
      const HandSiAlignmentMod* target() const;

   private:
      // ---------- Constructors and destructor ----------------
      ThreshCommand( const ThreshCommand& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ThreshCommand& operator=( const ThreshCommand& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      HVCommunicate m_hv;
      WhichThresh m_which;
      double m_thresh;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "FrameIterate/Template/FrameIteratorModuleBaseCommand.cc"
//#endif

#endif /* HANDSIALIGNMENTMOD_THRESHCOMMAND_H */
