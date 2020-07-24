#if !defined(CONSTANTSMODULE_CONSTANTSMODULE_H)
#define CONSTANTSMODULE_CONSTANTSMODULE_H
// -*- C++ -*-
//
// Package:     ConstantsModule
// Module:      ConstantsModule
// 
// Description: module to create constant's sources
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Apr 21 10:20:17 EDT 1999
// $Id: ConstantsModule.h,v 1.5 2000/04/22 19:33:20 cdj Exp $
//
// Revision history
//
// $Log: ConstantsModule.h,v $
// Revision 1.5  2000/04/22 19:33:20  cdj
// added streams_exclude sub-command
//
// Revision 1.4  1999/04/29 19:20:53  cdj
// rewritten to allow multiple databases at the same time
//
//

// system include files

// user include files
#include "CommandPattern/Module.h"

// forward declarations
class SourceManager;
class StreamSet;
class ConstantsModuleCommand;
class DBConstantsSelectionTag;

class ConstantsModule : public Module
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ConstantsModule(); //for dynamic loading
      ConstantsModule( SourceManager& iSourceManager);
      virtual ~ConstantsModule();

      // ---------- member functions ---------------------------
      DABoolean openSource( const string& iFDBName,
			    const DBConstantsSelectionTag& iTag,
			    const StreamSet& iBindStreams,
			    const StreamSet& iExcludeStreams );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ConstantsModule( const ConstantsModule& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ConstantsModule& operator=( const ConstantsModule& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      // NOTE: you need at least one external command
      //       to be able to call your module;
      //       the number of internal commands is up to you...
      ConstantsModuleCommand& m_command;

      SourceManager& m_sourceManager;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsModule/Template/ConstantsModule.cc"
//#endif

#endif /* CONSTANTSMODULE_CONSTANTSMODULE_H */
