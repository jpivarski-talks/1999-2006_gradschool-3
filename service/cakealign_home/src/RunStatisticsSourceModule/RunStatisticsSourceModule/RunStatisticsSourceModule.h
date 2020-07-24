#if !defined(RUNSTATISTICSSOURCEMODULE_RUNSTATISTICSSOURCEMODULE_H)
#define RUNSTATISTICSSOURCEMODULE_RUNSTATISTICSSOURCEMODULE_H
// -*- C++ -*-
//
// Package:     RunStatisticsSourceModule
// Module:      RunStatisticsSourceModule
//
/**\class RunStatisticsSourceModule RunStatisticsSourceModule.h RunStatisticsSourceModule/RunStatisticsSourceModule.h
 
 Description: (dynamically-loadable) module

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Tue Jan 25 09:04:45 EST 2000
// $Id: RunStatisticsSourceModule.h,v 1.1.1.1 2000/01/29 20:47:35 cdj Exp $
//
// Revision history
//
// $Log: RunStatisticsSourceModule.h,v $
// Revision 1.1.1.1  2000/01/29 20:47:35  cdj
// imported RunStatisticsSourceModule source
//
//

// system include files

// user include files
#include "CommandPattern/Module.h"
#include "RunStatisticsSourceModule/RunStatisticsSourceModuleCommand.h"

// forward declarations
class StreamSet;

class RunStatisticsSourceModule : public Module
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RunStatisticsSourceModule();
      virtual ~RunStatisticsSourceModule();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      DABoolean openSource( const StreamSet& iBindStreams ) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RunStatisticsSourceModule( const RunStatisticsSourceModule& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RunStatisticsSourceModule& operator=( const RunStatisticsSourceModule& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      // NOTE: you need at least one external command
      //       to be able to call your module;
      //       the number of internal commands is up to you...
      RunStatisticsSourceModuleCommand m_command;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RunStatisticsSourceModule.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RUNSTATISTICSSOURCEMODULE_H */
