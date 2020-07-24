#if !defined(HBOOKHISTOGRAM_HBOOKFILE_H)
#define HBOOKHISTOGRAM_HBOOKFILE_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookFile
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Tue Feb  2 14:36:48 EST 1999
// $Id: HbookFile.h,v 1.1.1.1 1999/09/06 22:02:30 mkl Exp $
//
// Revision history
//
// $Log: HbookFile.h,v $
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

// system include files
#include "Experiment/fortran_types.h"
// user include files

// forward declarations
class HbookRZItem;
template <class T> class MultipleHook;
typedef MultipleHook <HbookRZItem> HbookRZItemHooks;

class HbookFile
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum { kMaxItemsInFile = 16 };

      // ---------- Constructors and destructor ----------------
      HbookFile(
		   const char* pRequestedFileName,
		   const char* pRequestedTopDir,
		   SmallCount  iLogicalUnitNumber,
		   SmallCount  iRecordLength );
      ~HbookFile();
      //
      // Modifiers
      void      close ( void );
      DABoolean hookUpHbookRZItem   ( HbookRZItem* pItem );
      DABoolean unHookHbookRZItem   ( HbookRZItem* pItem );
      void      storeHistogram ( Count iId );
      //
      // Accessors
      const char* getTopDir ( void ) const;
      const char* name ( void ) const;
      //
      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookFile( const HbookFile& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HbookFile& operator=( const HbookFile& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      // Storage
      FChar* m_name;
      FChar* m_topdir;
      FInteger m_logicalUnitNumber;
      HbookRZItemHooks* m_itemsInFile;
      DABoolean m_open;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookFile.cc"
//#endif

#endif /* HbookHistogram_HBOOKFILE_H */
