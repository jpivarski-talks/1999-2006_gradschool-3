// -*- C++ -*-
#if !defined(HANDSIALIGNMENTMOD_RESTRICTSENSORCOMMAND_H)
#define HANDSIALIGNMENTMOD_RESTRICTSENSORCOMMAND_H
//
// Package:     HandSiAlignmentMod
// Module:      RestrictSensorCommand
//
/**\class RestrictSensorCommand RestrictSensorCommand.h HandSiAlignmentMod/RestrictSensorCommand.h
 
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
#include <vector>

// user include files

// forward declarations

class HandSiAlignmentMod;

class RestrictSensorCommand : public Command
{

      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RestrictSensorCommand( const Command::Name& iName, 
			     HandSiAlignmentMod* target );
      virtual ~RestrictSensorCommand();

      // ---------- member functions ---------------------------
      // You may override the base class's execute method, but it's
      // easier to use the "Subcommand" class to handle subcommands.
      // (Have a look at the command::ctor for an example.)
      virtual int execute( int argc, char* argv[] );
      DABoolean sensormin( int ladder );
      DABoolean sensormax( int ladder );
      DABoolean restrict( int sensor );
      
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual int helpHandler();

      HandSiAlignmentMod* target();
      const HandSiAlignmentMod* target() const;

   private:
      // ---------- Constructors and destructor ----------------
      RestrictSensorCommand( const RestrictSensorCommand& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RestrictSensorCommand& operator=( const RestrictSensorCommand& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      int m_sensormin[61];
      int m_sensormax[61];

      DABoolean m_restrict;
      vector< int > m_sensors;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "FrameIterate/Template/FrameIteratorModuleBaseCommand.cc"
//#endif

#endif /* HANDSIALIGNMENTMOD_RESTRICTSENSORCOMMAND_H */
