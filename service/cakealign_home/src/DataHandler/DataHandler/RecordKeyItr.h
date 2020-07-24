#if !defined(DATAHANDLER_RECORDKEYITR_H)
#define DATAHANDLER_RECORDKEYITR_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      RecordKeyItr
// 
// Description: iterator used by Record to allow user to see all the different
//              Proxies that are in the Record
//
// Usage:
//    <usage>
//
// Author:      Chris D. Jones
// Created:     Wed Sep 30 15:13:23 EDT 1998
// $Id: RecordKeyItr.h,v 1.2 2001/11/06 18:45:09 cdj Exp $
//
// Revision history
//
// $Log: RecordKeyItr.h,v $
// Revision 1.2  2001/11/06 18:45:09  cdj
// added standard iterator typedefs
//
// Revision 1.1  1998/09/30 21:07:50  cdj
// added iterator
//

// system include files

// user include files
#include "DataHandler/DataKey.h" //need for map
#include "STLUtility/fwd_map.h"

// forward declarations
class ProxyBase;

#include <map>

class RecordKeyItr
{
      // ---------- friend classes and functions ---------------
      friend class Record;

   public:
      // ---------- constants, enums and typedefs --------------
      typedef DataKey value_type;
      typedef const DataKey* pointer;
      typedef const DataKey& reference;

#if !defined(NO_ITERATOR_TRAITS_BUG)      
      typedef STL_MAP( DataKey, const ProxyBase* )::iterator::difference_type difference_type;
      typedef STL_MAP( DataKey, const ProxyBase* )::iterator::iterator_category iterator_category;
#endif

      // ---------- Constructors and destructor ----------------
      RecordKeyItr();
      //virtual ~RecordKeyItr();

      // ---------- member functions ---------------------------
      RecordKeyItr& operator++() { //prefix
	 ++m_iterator; return *this; }

      RecordKeyItr operator++(int) { //postfix
	 RecordKeyItr temp( *this ); ++m_iterator; return temp; }

      RecordKeyItr& operator--() { //prefix
	 --m_iterator; return *this; }

      RecordKeyItr operator--(int) { //postfix
	 RecordKeyItr temp( *this ); --m_iterator; return temp; }

      // ---------- const member functions ---------------------
      DABoolean operator==( const RecordKeyItr& iRHS ) const {
	 return m_iterator == iRHS.m_iterator; }
      DABoolean operator!=( const RecordKeyItr& iRHS ) const {
	 return m_iterator != iRHS.m_iterator; }

      const DataKey& operator*() const {
	 return (*m_iterator).first; }
      const DataKey* operator->() const {
	 return & ((*m_iterator).first); }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //RecordKeyItr( const RecordKeyItr& ); // stop default
      RecordKeyItr( const STL_MAP(DataKey, const ProxyBase*)::iterator&  
                          iIterator) : m_iterator( iIterator ) {}

      // ---------- assignment operator(s) ---------------------
      //const RecordKeyItr& operator=( const RecordKeyItr& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      STL_MAP( DataKey, const ProxyBase* )::iterator m_iterator;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/RecordKeyItr.cc"
//#endif

#endif /* DATAHANDLER_RECORDKEYITR_H */
