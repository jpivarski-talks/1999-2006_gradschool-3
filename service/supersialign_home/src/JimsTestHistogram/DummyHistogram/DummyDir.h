#if !defined(DUMMYHISTOGRAM_DUMMYDIR_H)
#define DUMMYHISTOGRAM_DUMMYDIR_H
// -*- C++ -*-
//
// Package:     DummyHistogram
// Module:      DummyDir
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Tue Sep 28 14:13:32 EDT 1999
// $Id: DummyDir.h,v 1.4 2001/07/21 15:44:54 cdj Exp $
//
// Revision history
//
// $Log: DummyDir.h,v $
// Revision 1.4  2001/07/21 15:44:54  cdj
// conform to new HIHistDir interface
//
// Revision 1.3  1999/10/07 18:06:31  hufnagel
// Numbers now use float instead of double
//
// Revision 1.2  1999/10/01 18:43:12  mkl
// fix up memory leaks
//
// Revision 1.1  1999/09/29 17:53:41  cdj
// added histogram directories
//

// system include files
#include "HistogramInterface/HIHistDir.h"

// user include files

// forward declarations
#include "STLUtility/fwd_vector.h"

class DummyDir : public HIHistDir
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DummyDir( const string& iName );
      virtual ~DummyDir();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      // Make a  sub directory
      virtual HIHistDir* makeDir( const string& iName) ;

      // create generic histograms and ntuples
      // 1D histogram
      virtual HIHist1D* makeHistogram( Count          iId, 
				   const string&  iTitle,
				   unsigned int   iNChannels, 
				   float          iLowEdge,
				   float          iHighEdge,
				   float          iBitsPerChannel=0) ;

      // 2D histogram
      virtual HIHist2D* makeHistogram( Count          iId,
				   const string&  iTitle,
				   unsigned int   iNChannelsX, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   unsigned int   iNChannelsY, 
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   float          BitsPerChannel=0) ;
      
      // profile histogram
      virtual HIHistProf* makeProfile( Count          iId,
				   const string&  iTitle,
				   unsigned int   iNChannels, 
				   float iLowEdgeX, 
				   float iHighEdgeX,
				   float iLowEdgeY, 
				   float iHighEdgeY,
				   HIHistProf::DisplayErrorsAs iDisplayErrorsAs ) ;

      // ntuple
      virtual HINtuple* makeNtuple(    Count          iId,
			           const string&  iTitle,
				   unsigned int   iSizeOfEntry,
				   unsigned int   iChunkSize,
				   const char* const* iLabels) ; 

      virtual HINumber* makeNumber( const string& iName,
				float         iValue ) ;

      virtual HINumberWithErrors* makeNumberWithErrors( 
	 const string& iName,
	 float        iValue,
	 float        iPositiveError,
	 float        iNegativeError ) ;

      Count nextId();
      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DummyDir( const DummyDir& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DummyDir& operator=( const DummyDir& ); // stop default

      // ---------- private member functions -------------------
      Count checkValidId( Count iId );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Count m_nextId;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DummyHistogram/Template/DummyDir.cc"
//#endif

#endif /* DUMMYHISTOGRAM_DUMMYDIR_H */
