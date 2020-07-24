#if !defined(HBOOKHISTOGRAM_RZITEM_H)
#define HBOOKHISTOGRAM_RZITEM_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookRZItem
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Wed Feb  3 15:11:07 EST 1999
// $Id: HbookRZItem.h,v 1.1.1.1 1999/09/06 22:02:31 mkl Exp $
//
// Revision history
//
// $Log: HbookRZItem.h,v $
// Revision 1.1.1.1  1999/09/06 22:02:31  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

// system include files

// user include files

// forward declarations

class HbookFile;
template <class T> class SingleHook;
typedef SingleHook <HbookFile> FileHook;

class HbookRZItem
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HbookRZItem( HbookFile* pFile);
      virtual ~HbookRZItem();
     
      // ---------- member functions ---------------------------
      DABoolean hookUpFile( HbookFile* pFile );
      DABoolean unHookFile( HbookFile* pFile );
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      //Accessors
      virtual void close ( void );
      DABoolean isOpen ( void );
      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookRZItem( const HbookRZItem& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HbookRZItem& operator=( const HbookRZItem& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      FileHook* m_linkToFile;
      DABoolean m_open;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookRZItem.cc"
//#endif

#endif /* HbookHistogram_RZITEM_H */
