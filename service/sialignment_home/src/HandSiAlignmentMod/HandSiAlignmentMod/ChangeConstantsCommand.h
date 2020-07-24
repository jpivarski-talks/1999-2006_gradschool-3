// -*- C++ -*-
#if !defined(HANDSIALIGNMENTMOD_CHANGECONSTANTSCOMMAND_H)
#define HANDSIALIGNMENTMOD_CHANGECONSTANTSCOMMAND_H
//
// Package:     HandSiAlignmentMod
// Module:      ChangeConstantsCommand
//
/**\class ChangeConstantsCommand ChangeConstantsCommand.h HandSiAlignmentMod/ChangeConstantsCommand.h
 
 Description: 

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri Sep 14 12:59:14 EDT 2001
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

class ChangeConstantsCommand : public Command
{

      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ChangeConstantsCommand( const Command::Name& iName, 
			      HandSiAlignmentMod* target );
      virtual ~ChangeConstantsCommand();

      // ---------- member functions ---------------------------
      // You may override the base class's execute method, but it's
      // easier to use the "Subcommand" class to handle subcommands.
      // (Have a look at the command::ctor for an example.)
      // virtual int execute( int argc, char* argv[] );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual int helpHandler();
      virtual int ladderHandler();
      virtual int sensorHandler();
      virtual int moveyHandler();
      virtual int replotHandler();

      HandSiAlignmentMod* target();
      const HandSiAlignmentMod* target() const;

   private:
      // ---------- Constructors and destructor ----------------
      ChangeConstantsCommand( const ChangeConstantsCommand& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ChangeConstantsCommand& operator=( const ChangeConstantsCommand& ); // stop default

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

#endif /* HANDSIALIGNMENTMOD_CHANGECONSTANTSCOMMAND_H */
