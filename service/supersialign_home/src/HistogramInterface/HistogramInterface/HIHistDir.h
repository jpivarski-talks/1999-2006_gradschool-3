#if !defined(HISTOGRAMINTERFACE_HIHISTDIR_H)
#define HISTOGRAMINTERFACE_HIHISTDIR_H
// -*- C++ -*-
//
// Package:     HistogramInterface
// Module:      HIHistDir
// 
// Description: Abstract interface for directories containing histograms
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Tue Dec  1 15:34:52 EST 1998
// $Id: HIHistDir.h,v 1.6 2001/07/21 15:43:48 cdj Exp $
//
// Revision history
//
// $Log: HIHistDir.h,v $
// Revision 1.6  2001/07/21 15:43:48  cdj
// can now iterate through HIHistDir's contents
//
// Revision 1.5  1999/10/07 18:05:05  hufnagel
// Numbers now use float instead of double
//
// Revision 1.4  1999/09/29 17:52:03  cdj
// added histogram directories
//// Revision 1.3  1999/09/07 21:20:46  mkl
// make cxx happy
//
// Revision 1.2  1999/09/07 18:53:29  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:03  cdj
// first submission
//

// system include files
#include <string>
#include "STLUtility/fwd_vector.h"
#include "HistogramInterface/HIHistProf.h"
// user include files
class HIHist1D;
class HIHist2D;
class HINtuple;
class HIHistProf;
class HINumber;
class HINumberWithErrors;

// forward declarations
#define HIDIR_FUNCS(_type_, _prefix_) \
public:\
const_ ## _prefix_ ## _iterator _prefix_ ## _begin() const { \
   return reinterpret_cast<const_ ## _prefix_ ## _iterator >(m_ ## _prefix_ ## List.begin()); } \
const_ ## _prefix_ ## _iterator _prefix_ ## _end() const { \
   return reinterpret_cast<const_ ## _prefix_ ## _iterator> (m_ ## _prefix_ ## List.end()); } \
protected: \
_prefix_ ## _iterator _prefix_ ## _begin() { \
   return reinterpret_cast< _prefix_ ## _iterator >(m_ ## _prefix_ ## List.begin()); } \
_prefix_ ## _iterator _prefix_ ## _end() { \
   return reinterpret_cast< _prefix_ ## _iterator> (m_ ## _prefix_ ## List.end()); } \
void _prefix_ ## Empty(); \
private: \
_type_* insert( _type_ * iEntry ) { m_ ## _prefix_ ## List.push_back(iEntry); return iEntry; }\
STL_VECTOR(void*) m_ ## _prefix_ ##List; \
public:


class HIHistDir 
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef STL_VECTOR(HIHist1D*)::const_iterator const_h1d_iterator;
      typedef STL_VECTOR(HIHist2D*)::const_iterator const_h2d_iterator;
      typedef STL_VECTOR(HINtuple*)::const_iterator const_nt_iterator;
      typedef STL_VECTOR(HIHistProf*)::const_iterator const_prof_iterator;
      typedef STL_VECTOR(HINumber*)::const_iterator const_n_iterator;
      typedef STL_VECTOR(HINumberWithErrors*)::const_iterator const_ne_iterator;
      typedef STL_VECTOR(HIHistDir*)::const_iterator const_dir_iterator;

      typedef STL_VECTOR(HIHist1D*)::iterator h1d_iterator;
      typedef STL_VECTOR(HIHist2D*)::iterator h2d_iterator;
      typedef STL_VECTOR(HINtuple*)::iterator nt_iterator;
      typedef STL_VECTOR(HIHistProf*)::iterator prof_iterator;
      typedef STL_VECTOR(HINumber*)::iterator n_iterator;
      typedef STL_VECTOR(HINumberWithErrors*)::iterator ne_iterator;
      typedef STL_VECTOR(HIHistDir*)::iterator dir_iterator;
      // ---------- Constructors and destructor ----------------     
      HIHistDir( const string& iName ) : m_name( iName ) {}
      virtual ~HIHistDir();

      // ---------- member functions ---------------------------

      /// Make a  sub directory with name iName
      virtual HIHistDir* mkdir( const string& iName) {
	 return insert(makeDir(iName) ); }

      // create generic histograms and ntuples
      /// 1D histogram

      HIHist1D* histogram( Count          iId, 
			   const string&  iTitle,
			   unsigned int   iNChannels, 
			   float          iLowEdge,
			   float          iHighEdge,
			   float          iBitsPerChannel=0){ 
	 return insert(makeHistogram(iId, iTitle, iNChannels, iLowEdge, iHighEdge, iBitsPerChannel)); }
      
      /// 1D histogram with automatic id assignment
      HIHist1D* histogram( const string&  iTitle,
			   unsigned int   iNChannels, 
			   float          iLowEdge,
			   float          iHighEdge,
			   float          iBitsPerChannel=0) { 
			   return insert(makeHistogram(nextId(),iTitle, iNChannels, iLowEdge, iHighEdge, iBitsPerChannel)); }

      /// 2D histogram
      HIHist2D* histogram( Count          iId,
			   const string&  iTitle,
			   unsigned int   iNChannelsX, 
			   float          iLowEdgeX, 
			   float          iHighEdgeX,
			   unsigned int   iNChannelsY, 
			   float          iLowEdgeY, 
			   float          iHighEdgeY,
			   float          iBitsPerChannel=0) { 
	 return insert(makeHistogram(iId, iTitle, iNChannelsX, iLowEdgeX, iHighEdgeX, iNChannelsY, iLowEdgeY, iHighEdgeY, iBitsPerChannel)); }
      
      /// 2D histogram with automatic id assignment
      HIHist2D* histogram( const string&  iTitle,
			   unsigned int   iNChannelsX, 
			   float          iLowEdgeX, 
			   float          iHighEdgeX,
			   unsigned int   iNChannelsY, 
			   float          iLowEdgeY, 
			   float          iHighEdgeY,
			   float          iBitsPerChannel=0) { 
	 return insert(makeHistogram(nextId(),iTitle, iNChannelsX, iLowEdgeX, iHighEdgeX, iNChannelsY, iLowEdgeY, iHighEdgeY, iBitsPerChannel)); }
      
      /// profile histogram
      HIHistProf* profile( Count          iId,
			   const string&  iTitle,
			   unsigned int   iNChannels, 
			   float iLowEdgeX, 
			   float iHighEdgeX,
			   float iLowEdgeY, 
			   float iHighEdgeY,
			   HIHistProf::DisplayErrorsAs iDisplayErrorsAs ) { 
	 return insert(makeProfile(iId, iTitle, iNChannels, iLowEdgeX, iHighEdgeX, iLowEdgeY, iHighEdgeY, iDisplayErrorsAs)); }

      /// profile histogram with automatic id assignment
      virtual HIHistProf* profile( const string&  iTitle,
				   unsigned int   iNChannels, 
				   float iLowEdgeX, 
				   float iHighEdgeX,
				   float iLowEdgeY, 
				   float iHighEdgeY,
				   HIHistProf::DisplayErrorsAs iDisplayErrorsAs ) { 
	 return insert(makeProfile(nextId(),iTitle, iNChannels, iLowEdgeX, iHighEdgeX, iLowEdgeY, iHighEdgeY, iDisplayErrorsAs)); }

      /// ntuple
      HINtuple* ntuple(    Count          iId,
			   const string&  iTitle,
			   unsigned int   iSizeOfEntry,
			   unsigned int   iChunkSize,
			   const char* const* iLabels) { 
	 return insert(makeNtuple(iId, iTitle, iSizeOfEntry, iChunkSize, iLabels)); }
 
      /// ntuple histogram with automatic id assignment
      HINtuple* ntuple(    const string&  iTitle,
			   unsigned int   iSizeOfEntry,
			   unsigned int   iChunkSize,
			   const char* const* iLabels) { 
	 return insert(makeNtuple(nextId(),iTitle, iSizeOfEntry, iChunkSize, iLabels)); }

      /// create an updatable number (needed for Online)
      HINumber* number( const string& iName,
			float        iValue ) { 
	 return insert(makeNumber(iName, iValue)); }

      /// create an updatable number with errors (needed for Online)
      HINumberWithErrors* numberWithErrors( 
	 const string& iName,
	 float        iValue,
	 float        iPositiveError,
	 float        iNegativeError ) { 
	 return insert(makeNumberWithErrors(iName, iValue, iPositiveError, iNegativeError)); }
      
      // ---------- const member functions ---------------------
      const string& name() const { return m_name; }

      HIDIR_FUNCS(HIHist1D, h1d)
      HIDIR_FUNCS(HIHist2D, h2d)
      HIDIR_FUNCS(HINtuple, nt)
      HIDIR_FUNCS(HIHistProf, prof)
      HIDIR_FUNCS(HINumber, n)
      HIDIR_FUNCS(HINumberWithErrors, ne)
      HIDIR_FUNCS(HIHistDir, dir)

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // Make a  sub directory with name iName
      virtual HIHistDir* makeDir( const string& iName) = 0;

      // create generic histograms and ntuples
      virtual HIHist1D* makeHistogram( Count          iId, 
				       const string&  iTitle,
				       unsigned int   iNChannels, 
				       float          iLowEdge,
				       float          iHighEdge,
				       float          iBitsPerChannel=0) =0;
            
      virtual HIHist2D* makeHistogram( Count          iId,
				       const string&  iTitle,
				       unsigned int   iNChannelsX, 
				       float          iLowEdgeX, 
				       float          iHighEdgeX,
				       unsigned int   iNChannelsY, 
				       float          iLowEdgeY, 
				       float          iHighEdgeY,
				       float          BitsPerChannel=0) =0;
            
      virtual HIHistProf* makeProfile( Count          iId,
				       const string&  iTitle,
				       unsigned int   iNChannels, 
				       float iLowEdgeX, 
				       float iHighEdgeX,
				       float iLowEdgeY, 
				       float iHighEdgeY,
				       HIHistProf::DisplayErrorsAs iDisplayErrorsAs ) =0;
      
      virtual HINtuple* makeNtuple(    Count          iId,
				       const string&  iTitle,
				       unsigned int   iSizeOfEntry,
				       unsigned int   iChunkSize,
				       const char* const* iLabels) =0; 
            
      virtual HINumber* makeNumber( const string& iName,
				    float        iValue ) =0;
      
      virtual HINumberWithErrors* makeNumberWithErrors( 
	 const string& iName,
	 float        iValue,
	 float        iPositiveError,
	 float        iNegativeError ) =0;

      virtual Count nextId() = 0;
      // ---------- protected const member functions -----------
      
   private:
      // ---------- Constructors and destructor ----------------
      HIHistDir( const HIHistDir& ); // stop default
      
      // ---------- assignment operator(s) ---------------------
      const HIHistDir& operator=( const HIHistDir& ); // stop default
      
      // ---------- private member functions -------------------
      
      // ---------- private const member functions -------------
      
      // ---------- data members -------------------------------
      string m_name;
      
      // ---------- static data members ------------------------
      
};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HISTOGRAMINTERFACE/Template/HIHistDir.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHistDIR_H */
