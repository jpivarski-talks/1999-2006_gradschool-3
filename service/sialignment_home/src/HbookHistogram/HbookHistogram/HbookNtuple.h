#if !defined(HBOOKHISTOGRAM_HBOOKNTUPLE_H)
#define HBOOKHISTOGRAM_HBOOKNTUPLE_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      HIHbookNtuple
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Thu Feb  4 15:00:01 EST 1999
// $Id: HbookNtuple.h,v 1.2 1999/09/29 17:55:56 cdj Exp $
//
// Revision history
//
// $Log: HbookNtuple.h,v $
// Revision 1.2  1999/09/29 17:55:56  cdj
// added histogram directories
//
// Revision 1.1.1.1  1999/09/06 22:02:31  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

// system include files

// user include files
#include "HistogramInterface/HINtuple.h"
#include "HbookHistogram/HbookRZItem.h"
#include "HbookHistogram/HbookManager.h" 
#include "HbookHistogram/HbookFile.h"
#include "HbookHistogram/HbookDirContents.h"

// forward declarations
class HbookFile ;

class HbookNtuple : public HINtuple , public HbookRZItem, private HbookDirContents
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HbookNtuple( HbookHistDir* iParent, 
		   HbookFile&    iNtupleFile,
		   unsigned int  iId,
		   const string& iTitle,
		   unsigned int  iNumberOfVariables,
		   unsigned int  iChunkSize,
		   const char* const* iLabels ) ;

      ~HbookNtuple( void );

      // ---------- member functions ---------------------------
      virtual void fill( const float* iEntry ) ; 
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookNtuple( const HbookNtuple& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HbookNtuple& operator=( const HbookNtuple& ); // stop default

      // ---------- private member functions -------------------
      virtual void book ( const string& iTitle,
			  const string& iTopDir,
			  unsigned int  iNumberOfVariables,
			  unsigned int  iChunkSize,
			  const char* const* iLabels ) ;
      virtual void close() ;
      // ---------- private const member functions -------------

      // ---------- Data members -------------------------------

      // ---------- Static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookNtuple.cc"
//#endif

#endif /* HBOOKHISTOGRAM_HBOOKNTUPLE_H */





