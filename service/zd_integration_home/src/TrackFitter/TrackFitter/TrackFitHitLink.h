#if !defined(TRACKFITTER_TRACKFITHITLINK_H)
#define TRACKFITTER_TRACKFITHITLINK_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      TrackFitHitLink
// 
/**\class TrackFitHitLink TrackFitHitLink.h TrackFitter/TrackFitHitLink.h

 Description: Link data base class for fit track \<--\> hit Lattices.

 Usage:
    This defines the link data base class between fit tracks and hits.

    \sa LatticeLink

*/
//
// Author:      Michael Marsh
// Created:     Mon Apr 26 15:42:00 EDT 1999
// $Id: TrackFitHitLink.h,v 1.7 2002/06/12 20:39:17 cleo3 Exp $
//
// Revision history
//
// $Log: TrackFitHitLink.h,v $
// Revision 1.7  2002/06/12 20:39:17  cleo3
// let the compiler automatically write what member functions it can
//
// Revision 1.6  2000/08/16 19:33:52  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.5  2000/02/24 17:34:09  wsun
// Updated for storage helpers.
//
// Revision 1.4  1999/10/07 18:00:42  marsh
// Added doxygen-style comments.
//
// Revision 1.3  1999/08/27 19:58:57  wsun
// Added set functions for data members.
//
// Revision 1.2  1999/05/09 21:32:06  wsun
// Implemented operator=()
//
// Revision 1.1  1999/04/26 20:35:21  marsh
// Made link data classes inherit a common base class.
//
//

// system include files

// user include files

// forward declarations

class TrackFitHitLink
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      TrackFitHitLink( DABoolean used,
                       double residual,
                       double residualError,
//                       double curvature,
                       double momentum );
      TrackFitHitLink();
      //TrackFitHitLink( const TrackFitHitLink& );
      //const TrackFitHitLink& operator=( const TrackFitHitLink& );
      virtual ~TrackFitHitLink() {}

      DABoolean operator==( const TrackFitHitLink& aLinkData ) const ;
      DABoolean operator!=( const TrackFitHitLink& aLinkData ) const ;

      // member functions
      void setUsed( DABoolean aUsed )        { m_used = aUsed ; }
      void setResidual( Meters aResidual )   { m_residual = aResidual ; }
      void setResidualError( Meters aErr )   { m_residualError = aErr ; }
//      void setCurvature( double aCurvature ) { m_curvature = aCurvature ; }
      void setMomentum( GeV aMomentum )      { m_momentum = aMomentum ; }

      // const member functions
      /**
       * \return true if the hit was used in the fit.
       */
      DABoolean used()          const { return m_used; }
      Meters    residual()      const { return m_residual; }
      Meters    residualError() const { return m_residualError; }
//       /**
//        * \return curvature of track at this hit.
//        */
//       double    curvature()     const { return m_curvature; }
      /**
       * \return total momentum of track at this hit.
       */
      GeV       momentum()      const { return m_momentum; }

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor (stop defaults) -- default ok

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members
      DABoolean  m_used;
      double     m_residual;
      double     m_residualError;
//      double     m_curvature;
      double     m_momentum;

      // static data members

};

// inline function definitions

#endif /* TRACKFITTER_TRACKFITHITLINK_H */
