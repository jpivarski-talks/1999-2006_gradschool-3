#if !defined(NAVIGATION_NAVELECID_H)
#define NAVIGATION_NAVELECID_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavElecId
// 
/**\class NavElecId NavElecId.h Navigation/NavElecId.h

 Description: Presents electron-id-related information

 Usage: through member functions

        The user has at least three options:

        1) use the "simple"
        electron id which has simple straight cuts on
        E/p (calorimeter energy to pion-hypothesis momentum ratio)
        and dedx, which are 0.85 < E/p < 1.1 and eSigma >-2.
        The low end E/p cut eliminates obvious non-electrons,
        and the high-end cut eliminates events in which
        there is an overlap of showers which can only confuse
        identification (the overlapping shower is frequently
        from bremsstrahlung of the track itself, which means
        the measured momentum is likely to be inaccurate).

	2) use the optimized logLik() value which attempts
        to use all detector information in an optimal way.

	3) make your own criteria based on this information.

	Please note that the dedx information is packaged
	as an FAItem<> because sometimes it may not exist
	and there is no reasonable default value in that case.
        That is, there may be a reason that there is no dedx
	information, and this might not be a reason to
        either blindly accept or reject the candidate.
	The simple criterion does require the presence
	of dedx information.

*/
//
// Author:      Brian K. Heltsley
// Created:     Thu Jun  1 08:59:16 EDT 2000
// $Id: NavElecId.h,v 1.4 2002/03/20 15:02:27 ajm36 Exp $
//
// Revision history
//
// $Log: NavElecId.h,v $
// Revision 1.4  2002/03/20 15:02:27  ajm36
// implement lazy evaluation in electron id
//
// Revision 1.3  2001/09/07 18:04:00  cleo3
// removed forward declaration of ostream
//
// Revision 1.2  2000/08/11 00:21:08  bkh
// Add operator<< functionality to these classes
//
// Revision 1.1  2000/06/06 18:38:58  bkh
// Install dedx, electron id, and muon id interfaces
//

// system include files

// this is necessary for using FAItem with a built-in for some compilers
#define DEREF_OP_FOR_BUILTIN_TYPES_BUG

// user include files
#include "FrameAccess/FAItem.h"
#include "C3cc/CcTypes.h"

// forward declarations
#include "C++Std/fwd_ostream.h"

class NavTrack ;

class NavElecId ;

ostream& operator<<( ostream& os, const NavElecId& aEl ) ;

class NavElecId
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum SimpleEnum {kFalse, kTrue, kUninitialized};

      // ---------- Constructors and destructor ----------------
      NavElecId( const NavTrack& aNavTrack ) ;
      virtual ~NavElecId(){}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      DABoolean simpleElecId() const ; // simple, reliable electron id
                                       // "one stop shopping"
                                       // ( 0.85 < E/p < 1.1 ) && eSigma>-2

      Real      logLik()       const ; // sophisticated elec id variable

      CcReal    eOverP()       const ; // 0 if no track-shower match

      FAItem< float > sDedx()  const ; // if invalid, no dedx info avail
                                       // dereference for elec sigma

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavElecId( const NavElecId& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavElecId& operator=( const NavElecId& ); // stop default
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      SimpleEnum m_simple;
      Real      m_logLik ;
      CcReal    m_eOverP ;
      float     m_dedx   ;
      float*    m_dedxPtr;
      const NavTrack& m_track;
      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* NAVIGATION_NAVELECID_H */
