#if !defined(CONSTANTSMODULE_CONSTANTSMODULECOMMAND_H)
#define CONSTANTSMODULE_CONSTANTSMODULECOMMAND_H
// -*- C++ -*-
//
// Package:     <ConstantsModule>
// Module:      ConstantsModuleCommand
// 
// Description: Command executer for the "constants" command
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Apr 21 10:20:17 EDT 1999
// RC(Id)
//
// Revision history
//
// $Log: ConstantsModuleCommand.h,v $
// Revision 1.3  2000/04/27 21:17:21  cdj
// now works on OSF using MICO ORB
//
// Revision 1.2  2000/04/22 19:33:20  cdj
// added streams_exclude sub-command
//
// Revision 1.1  1999/04/29 19:20:53  cdj
// rewritten to allow multiple databases at the same time
//
//

// system include files
#include <set>

// user include files
#include "CommandPattern/Command.h"
#include "CLEORB.hxx" //defines CLEORB_CLIENT_HH
#include CLEORB_CLIENT_HH(DBVersion)

// forward declarations
class ConstantsModule;
class StreamSet;
class DBConstantsSelectionTag;
#include "STLUtility/fwd_set.h"

class ConstantsModuleCommand : public Command
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ConstantsModuleCommand( const Command::Name& iName, 
			      ConstantsModule* target );
      virtual ~ConstantsModuleCommand();

      // ---------- member functions ---------------------------
      virtual int execute( int argc, char* argv[] );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual int helpHandler();
      virtual int sourceHandler();

      ConstantsModule* target();
      const ConstantsModule* target() const;

   private:
      // ---------- Constructors and destructor ----------------
      ConstantsModuleCommand( const ConstantsModuleCommand& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ConstantsModuleCommand& operator=( const ConstantsModuleCommand& ); // stop default

      // ---------- private member functions -------------------
      int parseTags( int iIndex, 
		     DBVersion::SelectType iSelectType,
		     const STL_SET(string)& iEndStrings,
		     DBConstantsSelectionTag& oTag ) const;

      // ---------- private const member functions -------------
      void parseStreams( StreamSet& iStreams, int iIndex ) const;

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsModule/Template/ConstantsModuleCommand.cc"
//#endif

#endif /* CONSTANTSMODULE_CONSTANTSMODULECOMMAND_H */
