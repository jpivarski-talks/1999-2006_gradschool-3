// -*- C++ -*-
#if !defined(HANDSIALIGNMENTMOD_PICKLADDERCOMMAND_H)
#define HANDSIALIGNMENTMOD_PICKLADDERCOMMAND_H
//
// Package:     HandSiAlignmentMod
// Module:      PickLadderCommand
//
/**\class PickLadderCommand PickLadderCommand.h HandSiAlignmentMod/PickLadderCommand.h
 
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

// user include files

// forward declarations

class HandSiAlignmentMod;

class PickLadderCommand : public Command
{

      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      PickLadderCommand( const Command::Name& iName, 
			 HandSiAlignmentMod* target );
      virtual ~PickLadderCommand();

      // ---------- member functions ---------------------------
      // You may override the base class's execute method, but it's
      // easier to use the "Subcommand" class to handle subcommands.
      // (Have a look at the command::ctor for an example.)
      virtual int execute( int argc, char* argv[] );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual int helpHandler();

      HandSiAlignmentMod* target();
      const HandSiAlignmentMod* target() const;

   private:
      // ---------- Constructors and destructor ----------------
      PickLadderCommand( const PickLadderCommand& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const PickLadderCommand& operator=( const PickLadderCommand& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "FrameIterate/Template/FrameIteratorModuleBaseCommand.cc"
//#endif

#endif /* HANDSIALIGNMENTMOD_PICKLADDERCOMMAND_H */