#if !defined(NAVIGATION_NAVCONREG_H)
#define NAVIGATION_NAVCONREG_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavConReg
// 
/**\class NavConReg NavConReg.h Navigation/NavConReg.h
*/
// Description: Calorimeter "connected region" object
//
// Usage:
//
// Author:      Brian K. Heltsley
// Created:     Wed Jun 23 14:40:06 EDT 1999
// $Id: NavConReg.h,v 1.9 2003/02/03 20:16:37 bkh Exp $
//
// Revision history
//
// $Log: NavConReg.h,v $
// Revision 1.9  2003/02/03 20:16:37  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.8  2002/03/25 11:53:06  bkh
// various optimizations for speed
//
// Revision 1.7  2001/10/26 21:54:01  bkh
// Add features to showers and connected region objects
//
// Revision 1.6  2001/02/02 20:46:10  bkh
// Fix bug that showed up only on solaris: const in front of pointer to list
//
// Revision 1.5  2001/02/02 19:34:21  bkh
// New constructor and new way of traversing lattice
//
// Revision 1.4  2000/10/26 16:01:05  bkh
// Bring up to date with CcConRegAttributes changes
//
// Revision 1.3  2000/03/15 22:02:59  bkh
// add minimal doxygen trigger
//
// Revision 1.2  1999/12/06 18:13:05  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.1  1999/08/09 16:25:36  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:58  bkh
// New classes associated with analysis-level shower object
//

// system include files

// user include files

#include "C3cc/CcConRegShowerLattice.h"

// forward declarations

template <class T> class FAConstPtrTable ;
template <class T> class FATable         ;

class Hep3Vector      ;
class NavShower       ;
class NavConRegServer ;

class NavConReg
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef CcConRegAttributes::Identifier Identifier   ;
      typedef CcShowerAttributes::Identifier ShowerId     ;
      typedef STL_VECTOR( ShowerId )         ShowerIdList ;
      typedef CcConRegShowerLattice::Link    LatticeLink  ;

      // ---------- Constructors and destructor ----------------
      NavConReg( const NavConReg::Identifier    aId  ,
		 const CcConRegShowerLattice&   aLattice ,
		 const NavConRegServer&         aServer   ) ;

      virtual ~NavConReg();
      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      Identifier           identifier() const ;
      const Hep3Vector&    momentum()   const ;
      CcGeV                energy()     const ;
      CcGeV                mass()       const ;

      const ShowerIdList&  showerIds()  const ;
      FATable< NavShower > showers()    const ;

      DABoolean            noTrackMatchConReg() const ; // no shower matched
      FAItem< NavShower >  matchedShower() const ; // invalid if not matched

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavConReg();
      NavConReg( const NavConReg& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavConReg& operator=( const NavConReg& );
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------

      const CcConRegAttributes& atts() const ;

      // ---------- data members -------------------------------

      Identifier                    m_id            ;
      const CcConRegShowerLattice*  m_lattice       ;
      const NavConRegServer&        m_server        ;
      const CcConRegAttributes*     m_attsPtr       ;
      FAConstPtrTable< NavShower >* m_showers       ;

      DABoolean                     m_doneTrackMatch ;
      DABoolean                     m_noTrackMatch   ;
      const NavShower*              m_matchedShower  ;

      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* NAVIGATION_NAVCONREG_H */
