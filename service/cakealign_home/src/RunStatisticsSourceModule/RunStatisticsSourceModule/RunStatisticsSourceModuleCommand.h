#if !defined(RUNSTATISTICSSOURCEMODULE_RUNSTATISTICSSOURCEMODULECOMMAND_H)
#define RUNSTATISTICSSOURCEMODULE_RUNSTATISTICSSOURCEMODULECOMMAND_H
// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RunStatisticsSourceModuleCommand
//
/**\class RunStatisticsSourceModuleCommand RunStatisticsSourceModuleCommand.h RunStatisticsSourceModule/RunStatisticsSourceModuleCommand.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Tue Jan 25 09:04:46 EST 2000
// RC(Id)
//
// Revision history
//
// $Log: RunStatisticsSourceModuleCommand.h,v $
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//
//

// system include files

// user include files
#include "CommandPattern/Command.h"

// forward declarations
class RunStatisticsSourceModule;
class StreamSet;

class RunStatisticsSourceModuleCommand : public Command
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RunStatisticsSourceModuleCommand( const Command::Name& iName, 
		      RunStatisticsSourceModule* target );
      virtual ~RunStatisticsSourceModuleCommand();

      // ---------- member functions ---------------------------
      // You may override the base class's execute method, but it's
      // easier to use the "Subcommand" class to handle subcommands.
      // (Have a look at the command::ctor for an example.)
      //virtual int execute( int argc, char* argv[] );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual int helpHandler();
      virtual int sourceHandler();

      RunStatisticsSourceModule* target();
      const RunStatisticsSourceModule* target() const;

   private:
      // ---------- Constructors and destructor ----------------
      RunStatisticsSourceModuleCommand( const RunStatisticsSourceModuleCommand& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RunStatisticsSourceModuleCommand& operator=( const RunStatisticsSourceModuleCommand& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------
      void parseStreams( StreamSet& iStreams, int iIndex ) const;

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RunStatisticsSourceModuleCommand.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RUNSTATISTICSSOURCEMODULECOMMAND_H */
