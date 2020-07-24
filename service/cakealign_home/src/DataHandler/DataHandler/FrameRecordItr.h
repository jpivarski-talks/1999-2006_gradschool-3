#if !defined(DATAHANDLER_FRAMERECORDITR_H)
#define DATAHANDLER_FRAMERECORDITR_H
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      FrameRecordItr
// 
/**\class FrameRecordItr FrameRecordItr.h DataHandler/FrameRecordItr.h

 Description: Interator used to iterate through Records in a Frame

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Mon Nov  5 13:35:56 EST 2001
// $Id: FrameRecordItr.h,v 1.1 2001/11/06 18:45:38 cdj Exp $
//
// Revision history
//
// $Log: FrameRecordItr.h,v $
// Revision 1.1  2001/11/06 18:45:38  cdj
// can now iterator over Records in a Frame
//

// system include files
#include <map>

// user include files
#include "DataHandler/Frame.h"

// forward declarations

class FrameRecordItr
{
      // ---------- friend classes and functions ---------------
      friend class Frame;

   public:
      // ---------- constants, enums and typedefs --------------
      typedef Record value_type;
      typedef const Record* pointer;
      typedef const Record& reference;

#if !defined(NO_ITERATOR_TRAITS_BUG)            
      typedef _frame_records_::const_iterator::difference_type difference_type;
      typedef _frame_records_::const_iterator::iterator_category iterator_category;
#endif

      // ---------- Constructors and destructor ----------------
      FrameRecordItr() {}
//      virtual ~FrameRecordItr();


      // ---------- member functions ---------------------------
      FrameRecordItr& operator++() { //prefix
	 ++m_iterator; return *this; }

      FrameRecordItr operator++(int) { //postfix
	 FrameRecordItr temp( *this ); ++m_iterator; return temp; }

      FrameRecordItr& operator--() { //prefix
	 --m_iterator; return *this; }

      FrameRecordItr operator--(int) { //postfix
	 FrameRecordItr temp( *this ); --m_iterator; return temp; }

      // ---------- const member functions ---------------------
      DABoolean operator==( const FrameRecordItr& iRHS ) const {
	 return m_iterator == iRHS.m_iterator; }
      DABoolean operator!=( const FrameRecordItr& iRHS ) const {
	 return m_iterator != iRHS.m_iterator; }

      const Record& operator*() const {
	 return * ((*m_iterator).second); }
      const Record* operator->() const {
	 return  ((*m_iterator).second); }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //FrameRecordItr( const FrameRecordItr& ); // stop default
      FrameRecordItr(const _frame_records_::iterator & iIterator ) :
         m_iterator( iIterator ) { }

      // ---------- assignment operator(s) ---------------------
      //const FrameRecordItr& operator=( const FrameRecordItr& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      _frame_records_::iterator m_iterator;
      
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/FrameRecordItr.cc"
//#endif

#endif /* DATAHANDLER_FRAMERECORDITR_H */
