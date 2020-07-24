#if !defined(HBOOKHISTOGRAMMODULE_HBOOKHISTOGRAMMODULE_H)
#define HBOOKHISTOGRAMMODULE_HBOOKHISTOGRAMMODULE_H
// -*- C++ -*-
//
// Package:     HbookHistogramModule
// Module:      HbookHistogramModule
// 
// Description: (dynamically-loadable) module
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Fri Nov 20 01:16:34 EST 1998
// $Id: HbookHistogramModule.h,v 1.6 2002/01/25 15:34:13 cdj Exp $
//
// Revision history
//
// $Log: HbookHistogramModule.h,v $
// Revision 1.6  2002/01/25 15:34:13  cdj
// ntuple command can now specify full DataKey as well as buffer size
//
// Revision 1.5  2001/12/20 19:12:19  ajm36
// add cdj's FAXXXX->ntuple facility
//
// Revision 1.4  2001/09/07 19:09:16  cleo3
// fixed problems found with Solaris 8
//
// Revision 1.3  2001/04/02 21:45:28  ajm36
// made record length a 32 bit integer
//
// Revision 1.2  2001/02/17 21:39:42  cleo3
// found typo in non-used #ifdef section
//
// Revision 1.1.1.1  1999/09/06 22:05:31  mkl
// imported HbookHistogramModule sources
//
// Revision 1.1.1.1  1998/11/20 07:08:26  mkl
// new HbookHistogramModule
//
//

// system include files

// user include files
#include "HistogramModule/HistogramModule.h"
#include "HbookHistogramModule/HbookHistogramModuleCommand.h"
//must include to get covariant return type.
#include "HbookHistogram/HbookManager.h"  
#include <vector>
#include "STLUtility/fwd_vector.h"

// forward declarations
class HbookManager;
class StreamSet;

class HbookHistogramModule : public HistogramModule
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HbookHistogramModule();
      virtual ~HbookHistogramModule();

      // ---------- member functions ---------------------------
      virtual HIHistoManager* newHistogramManager();
      virtual void closeHistogramManager();

#if !defined(NO_COVARIANT_RETURN_TYPE_BUG)
      virtual HbookManager* manager();
#else
      virtual HIHistoManager* manager();
#endif /* NO_COVARIANT_RETURN_TYPE_BUG */

      // ---------- const member functions ---------------------
#if !defined(NO_COVARIANT_RETURN_TYPE_BUG)
      virtual const HbookManager* manager() const;
#else
      virtual const HIHistoManager* manager() const;
#endif /* NO_COVARIANT_RETURN_TYPE_BUG */

      virtual string statusInfo() const;

      // specific to Hbook implementation
      void setFileName( const string& iFileName );
      void setLogicalUnitNumber( SmallCount iLogicalUnitNumber );
      void setRecordLength( SmallCount iRecordLength );

      DABoolean createNtupleSink( const string& iFileName,
                                SmallCount iLogicalUnitNumber,
                                SmallCount iRecordLength,
                                unsigned long iBufferSize,
                                const STL_VECTOR(string) & iKeyNames,
                                const StreamSet& iStreams );

      void listUsableStorageHelpers();
                                
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookHistogramModule( const HbookHistogramModule& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HbookHistogramModule& operator=( const HbookHistogramModule& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      // NOTE: you need at least one external command
      //       to be able to call your module;
      //       the number of internal commands is up to you...
      HbookHistogramModuleCommand m_command;

      // specific to hbook
      string m_fileName;
      SmallCount m_logicalUnitNumber;
      UInt32 m_recordLength;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogramModule/Template/HbookHistogramModule.cc"
//#endif

#endif /* HBOOKHISTOGRAMMODULE_HBOOKHISTOGRAMMODULE_H */
