#if !defined(HBOOKHISTOGRAMMODULE_HBOOKHISTOGRAMMODULECOMMAND_H)
#define HBOOKHISTOGRAMMODULE_HBOOKHISTOGRAMMODULECOMMAND_H
// -*- C++ -*-
//
// Package:     <HbookHistogramModule>
// Module:      HbookHistogramModuleCommand
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Fri Nov 20 01:16:34 EST 1998
// RC(Id)
//
// Revision history
//
// $Log: HbookHistogramModuleCommand.h,v $
// Revision 1.1.1.1  1999/09/06 22:05:31  mkl
// imported HbookHistogramModule sources
//
// Revision 1.1.1.1  1998/11/20 07:08:26  mkl
// new HbookHistogramModule
//
//

// system include files

// user include files
#include "HistogramModule/HistogramModuleCommand.h"

// forward declarations
class HbookHistogramModule;

class HbookHistogramModuleCommand : public HistogramModuleCommand
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HbookHistogramModuleCommand( const Command::Name& iName, 
		      HbookHistogramModule* target );
      virtual ~HbookHistogramModuleCommand();

      // ---------- member functions ---------------------------
      //virtual int execute( int argc, char* argv[] );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual int helpHandler();

      virtual int fileHandler();
      virtual int recordLengthHandler();
      virtual int logicalUnitHandler();

      HbookHistogramModule* target();
      const HbookHistogramModule* target() const;

   private:
      // ---------- Constructors and destructor ----------------
      HbookHistogramModuleCommand( const HbookHistogramModuleCommand& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HbookHistogramModuleCommand& operator=( const HbookHistogramModuleCommand& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogramModule/Template/HbookHistogramModuleCommand.cc"
//#endif

#endif /* HBOOKHISTOGRAMMODULE_HBOOKHISTOGRAMMODULECOMMAND_H */
