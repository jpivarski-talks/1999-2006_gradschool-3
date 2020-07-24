#if !defined(HBOOKHISTOGRAM_HBOOKHISTDIR_H)
#define HBOOKHISTOGRAM_HBOOKHISTDIR_H
// -*- C++ -*-
//
// Package:     HbookHistogram
// Module:      HbookHistDir
// 
// Description: Hbook implementation of HIHistDir
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Mon Sep 27 17:16:04 EDT 1999
// $Id: HbookHistDir.h,v 1.7 2001/07/21 15:47:28 cdj Exp $
//
// Revision history
//
// $Log: HbookHistDir.h,v $
// Revision 1.7  2001/07/21 15:47:28  cdj
// conform to new HIHistDir interface
//
// Revision 1.6  2000/03/11 17:49:54  cdj
// HbookHist1D.h
//
// Revision 1.5  2000/01/31 22:56:05  cdj
// now holds a list of Ntuples rather than RZItems
//
// Revision 1.4  1999/10/20 19:31:04  cdj
// pass hbookn_ the full path within the file
//
// Revision 1.3  1999/10/07 18:07:53  hufnagel
// Numbers now use float instead of double
//
// Revision 1.2  1999/10/01 19:23:16  cdj
// plugged memory leaks and histograms without ids works
//
// Revision 1.1  1999/09/29 17:55:54  cdj
// added histogram directories
//

// system include files

// user include files
#include "HistogramInterface/HIHistDir.h"
#include "HbookHistogram/hbook_functions.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
class HbookNtuple;
class HbookFile; 

class HbookHistDir : public HIHistDir
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      /// create root directory, called by HbookManager
      HbookHistDir( HbookFile* iFile );

      virtual ~HbookHistDir();

      // ---------- member functions ---------------------------


      // ---------- const member functions ---------------------
      /// have hbook use the proper memory directory
      void makeDirDefault() const;

      //needed by ntuples
      const string& filePath() const { return m_filePath; }
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      /// called by other HbookHistDirs when creating sub directories
      HbookHistDir( const string& iName, 
		    HbookHistDir* iParent,
		    HbookFile* iFile );

      // ---------- protected const member functions -----------
      /// have hbook use the proper directory in the file
      void makeFileDirDefault() const {
	 hcdir_( m_filePath.c_str(), " ", m_filePath.size(), 1 ); 
	 s_fileDirIsDefault = true;
      }

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
	 float         iValue,
	 float         iPositiveError,
	 float         iNegativeError ) ;
   private:
      // ---------- Constructors and destructor ----------------
      HbookHistDir( const HbookHistDir& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HbookHistDir& operator=( const HbookHistDir& ); // stop default

      // ---------- private member functions -------------------
      Count nextId();
      Count checkValidId( Count iId );
      DABoolean idExists( Count iId ) const;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      ///needed to make NTuples, does not own the memory
      HbookFile* m_file;
      unsigned int m_nextId;
      string m_path;
      string m_filePath;

      // ---------- static data members ------------------------
      static const HbookHistDir* s_defaultDir;
      static DABoolean s_fileDirIsDefault;

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookHistDir.cc"
//#endif

#endif /* HBOOKHISTOGRAM_HBOOKHISTDIR_H */
