#if !defined(DUMMYHISTOGRAM_DUMMYHISTOMANAGER_H)
#define DUMMYHISTOGRAM_DUMMYHISTOMANAGER_H
// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyHistoManager
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 22:30:34 EST 1999
// $Id: DummyHistoManager.h,v 1.6 1999/10/07 18:06:32 hufnagel Exp $
//
// Revision history
//
// $Log: DummyHistoManager.h,v $
// Revision 1.6  1999/10/07 18:06:32  hufnagel
// Numbers now use float instead of double
//
// Revision 1.5  1999/10/06 20:26:18  cdj
// added setDefaultDir to Manager
//
// Revision 1.4  1999/09/29 17:53:42  cdj
// added histogram directories
//
// Revision 1.3  1999/09/10 01:11:57  mkl
// now allow booking of histograms with automatic id allocation
//
// Revision 1.2  1999/09/06 21:47:28  mkl
// move to new histogramming interface: dummy implementation
//
// Revision 1.1.1.1  1999/05/12 00:10:12  billm
// imported DummyHistogram sources
//

// system include files

// user include files
#include "HistogramInterface/HIHistoManager.h"

// forward declarations
class DummyDir;

class DummyHistoManager : public HIHistoManager
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DummyHistoManager();
      virtual ~DummyHistoManager();

      // ---------- member functions ---------------------------
      // Make a  sub directory
      virtual HIHistDir* mkdir( const string& iName) ;

      // create Hbook histograms and Ntuples
      // 1D histogram
      virtual HIHist1D* histogram( Count          iId,
				   const string&  iTitle,
				   unsigned int   NChannels,
				   float          iLowEdge,
				   float          iHighEdge,
				   float          iBitsPerChannel=0 );

      // 1D histogram with automatic id assignment
      virtual HIHist1D* histogram( const string&  iTitle,
				   unsigned int   NChannels,
				   float          iLowEdge,
				   float          iHighEdge,
				   float          iBitsPerChannel=0 );

      // 2D histogram
      virtual HIHist2D* histogram( Count          iId,
				   const string&  iTitle,
				   unsigned int   iNChannelsX, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   unsigned int   iNChannelsY, 
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   float          BitsPerChannel=0 );
      
      // 2D histogram with automatic id assignment
      virtual HIHist2D* histogram( const string&  iTitle,
				   unsigned int   iNChannelsX, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   unsigned int   iNChannelsY, 
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   float          BitsPerChannel=0 );

      // profile histogram
      virtual HIHistProf* profile( Count          iId,
				   const string&  iTitle,
				   unsigned int   iNChannels, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   HIHistProf::DisplayErrorsAs iDisplayErrorsAs );

      // profile histogram with automatic id assignment
      virtual HIHistProf* profile( const string&  iTitle,
				   unsigned int   iNChannels, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   HIHistProf::DisplayErrorsAs iDisplayErrorsAs );

      // ntuple
      virtual HINtuple* ntuple(    Count          iId,
			           const string&  iTitle,
				   unsigned int   iNumberOfVariables,
				   unsigned int   iChunkSize,
				   const char* const* iLabels ) ;

      // ntuple with automatic id assignment
      virtual HINtuple* ntuple(    const string&  iTitle,
				   unsigned int   iNumberOfVariables,
				   unsigned int   iChunkSize,
				   const char* const* iLabels ) ;

      virtual HINumber* number( const string& iName,
				float         iValue );

      virtual HINumberWithErrors* numberWithErrors( 
	 const string& iName,
	 float         iValue,
	 float         iPositiveError,
	 float         iNegativeError );

      // Determines which histogram to fill corresponding
      // to a particular directory
      //virtual HIDirSwitch* mkSwitch();        

      virtual void setDefaultDir( const string& iDirectoryName ) ;

      // ---------- const member functions ---------------------
      // Directory information
      virtual HIHistDir* defaultDir() const; // gets the default directory
      virtual HIHistDir* rootDir() const;    // gets the root directory */

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DummyHistoManager( const DummyHistoManager& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DummyHistoManager& operator=( const DummyHistoManager& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DummyDir* m_root;
      HIHistDir* m_default;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DummyHistogram/Template/DummyHistoManager.cc"
//#endif

#endif /* DUMMYHISTOGRAM_DUMMYHISTOMANAGER_H */
