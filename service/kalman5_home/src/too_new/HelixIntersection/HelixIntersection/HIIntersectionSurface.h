#if !defined(HELIXINTERSECTION_HIINTERSECTIONSURFACE_H)
#define HELIXINTERSECTION_HIINTERSECTIONSURFACE_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIIntersectionSurface
// 
// Description: Base class defining the surfaces on which the
//              HIHelixIntersector should check for intersections with its
//              HIHelix.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 18:05:52 EDT 1998
// $Id: HIIntersectionSurface.h,v 1.16 2001/02/22 01:59:27 wsun Exp $
//
// Revision history
//
// $Log: HIIntersectionSurface.h,v $
// Revision 1.16  2001/02/22 01:59:27  wsun
// Added flags to control deletion of condition and operation.
//
// Revision 1.15  2000/11/18 00:08:39  wsun
// Added checkPropagation() flag.
//
// Revision 1.14  2000/09/05 20:34:58  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.13  2000/06/09 23:46:31  wsun
// Moved correctHitsForHelix() to HICalibratedHit.
//
// Revision 1.12  2000/06/09 00:31:26  wsun
// Added correctHitsForHelix().
//
// Revision 1.11  2000/03/02 08:25:23  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.10  2000/02/03 07:15:00  wsun
// Added use of HICalibratedHits.
//
// Revision 1.9  1999/12/15 20:25:35  wsun
// * Sped up arcLengthDerivatives().
// * Modified interface for arcLengthDerivatives, derivatives() and normal()
//   to avoid instantiating temporary HepVectors and HepNormal3Ds.
//
// Revision 1.8  1999/11/21 22:26:49  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.7  1999/08/27 20:16:43  wsun
// Added arcLengthDerivatives() and changed signature for performOperation().
//
// Revision 1.6  1999/08/03 21:52:36  wsun
// Added reset().
//
// Revision 1.5  1999/07/12 06:38:19  wsun
// New convention -- normal() points toward outer material.
//
// Revision 1.4  1999/06/02 23:04:23  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/04/28 22:57:02  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:04:04  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:16  wsun
// First submission.
//

// system include files

// user include files
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "KinematicTrajectory/KTMoveControl.h"

#include "CLHEP/Matrix/Vector.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class CalibratedHit ;
class HIHelix ;
class HepNormal3D ;
class HepPoint3D ;
class HICondition ;
class HIHelixOperation ;
class HIFitInfo ;
class HICalibratedHit ;

class HIIntersectionSurface
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef double Meters ;
      typedef double GeV ;

      static const string kSurfaceType ;

      // ---------- Constructors and destructor ----------------
      HIIntersectionSurface();
      HIIntersectionSurface( const DGConstMaterialPtr& aInnerMaterial,
			     const DGConstMaterialPtr& aOuterMaterial,
			     Meters aRadius,
			     HICondition* aCondition = 0,
			     HIHelixOperation* anOperation = 0,
			     DABoolean aDtorDeletesCondition = true,
			     DABoolean aDtorDeletesOperation = false ) ;
      virtual ~HIIntersectionSurface();

      // ---------- member functions ---------------------------
      void setInnerMaterial( const DGConstMaterialPtr& aMaterial )
      { m_innerMaterial = aMaterial ; }

      void setOuterMaterial( const DGConstMaterialPtr& aMaterial )
      { m_outerMaterial = aMaterial ; }

      void setRadius( Meters aRadius )
      { m_radius = aRadius ; }

      void setCondition( HICondition* aCondition )
      { m_condition = aCondition ; }

      virtual void deleteCondition() ;

      void setOperation( HIHelixOperation* aOperation )
      { m_operation = aOperation ; }

      virtual void deleteOperation() ;

      // Returns original helix if transport fails.  This function is
      // non-const because of HICompositeSurface.
      virtual DABoolean propagateHelix(
	 HIHelix& helixToPropagate,
	 KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) = 0 ;

      virtual void setPrintDiagnostics( DABoolean flag )
      { m_printDiagnostics = flag ; }

      virtual void setDtorDeletesCondition( DABoolean flag )
      { m_dtorDeletesCondition = flag ; }

      virtual void setDtorDeletesOperation( DABoolean flag )
      { m_dtorDeletesOperation = flag ; }

      // Returns false if at the end of the list (or if only one hit).
      // Subclasses should reset the "current" hit to the beginning
      // of the list after the end is reached.
      //
      // The propagated helix is given as an argument because there may
      // need to be small adjustments to the helix position for each
      // hit (e.g. DR hits).
      virtual DABoolean advanceToNextCalibratedHit( HIHelix& )
      { return false ; }

      virtual void reset() {}

      // Clear the stored inward and outward residuals and their errors.
      virtual void resetHICalibratedHits() {}

      // Called by HIHelixIntersector when the global condition is satisfied
      // and the helix has to be retransported to the current surface.
      virtual void revertToPreviousState() {}

      virtual void
      addHICalibratedHitsToVector( STL_VECTOR( HICalibratedHit* )& ) {}

      virtual void setFittingWeight( double aWeight )
      { m_forceFittingWeight = true ; m_fittingWeight = aWeight ; }

      virtual void setUseReverseHitOrder( DABoolean aFlag )
      { m_useReverseHitOrder = aFlag ; }

      // ---------- const member functions ---------------------

      // Each HIIntersectionSurface can be associated with more than one
      // CalibratedHit, e.g. Si planes can have both an rphi and a z hit,
      // or DR hits might be grouped by superlayer and added all at once
      // save time.  These hits are stored in arbitrary order in
      // STL vectors in the relevant subclasses.  Each of the following
      // five functions refers to the "current" CalibratedHit.  By default,
      // this hit is the first one in the list.  To loop through the list,
      // use the function advanceToNextCalibratedHit(), and then call the
      // accessor functions again.

      // Since the surface keeps a pointer (iterator) to the current
      // CalibratedHit, it will be modified by the HIHelixOperations.

      // Information needed for fitting, using the current CalibratedHit
      virtual Meters dca( const HIHelix& ) const
      { return 0. ; }

      // Derivatives of the track-to-hit distance w.r.t. the track parameters.
      // The function first checks that the HepVector to be filled has
      // size HIHelix::kNTrackParameters.
      virtual void derivatives(
	 const HIHelix&, HepVector& outputVector ) const {}

      // Approximate derivatives of the last arc length w.r.t. the track
      // parameters.  The stiff track approximation should be sufficient
      // because these derivatives are important only for non-coaxial
      // cylinders.  These happen to correspond to materials where
      // energy loss and multiple scattering is large.  Since these
      // structures tend to have small r-phi cross sections, the traversed
      // arc length is small compared to the radius of curvature.
      //
      // The function first checks that the HepVector to be filled has
      // size HIHelix::kNTrackParameters.
      virtual void arcLengthDerivatives(
	 const HIHelix& aCurrentHelix,
	 const HIHelix& aLastHelix,
	 const HIIntersectionSurface& aLastSurface,
	 HepVector& outputVector ) const ;

      virtual Meters fittingWeight() const
      { return 0. ; }

      virtual Meters measuredDca() const
      { return 0. ; }

      virtual const CalibratedHit* calibratedHit() const
      { return 0 ; }

      virtual HICalibratedHit* hiCalibratedHit() const
      { return 0 ; }

      virtual int numberOfAssociatedHits() const
      { return 0 ; }

      // The normal should point towards the side with the outer material.
      virtual void normal( const HIHelix&,
			   HepNormal3D& outputNormal ) const = 0 ;
      virtual void normal( const HepPoint3D&,
			   HepNormal3D& outputNormal ) const = 0 ;

      virtual DABoolean isComposite() const { return false ; }

      virtual DABoolean conditionSatisfied( const HIHelix& aHelix ) ;

      virtual DABoolean performOperation(
	 HIHelix& aHelix,
	 const HIHelix& aLastHelix,
	 const HIIntersectionSurface& aLastSurface ) ;

      const DGConstMaterialPtr& innerMaterial() const
      { return m_innerMaterial; }

      const DGConstMaterialPtr& outerMaterial() const
      { return m_outerMaterial; }

      const DGConstMaterialPtr& materialInLastTransport(
	 const HIHelix& ) const ;

      Meters radius() const { return m_radius ; }
      HICondition* condition() const { return m_condition ; }
      HIHelixOperation* operation() const { return m_operation ;}

      DABoolean printDiagnostics() const
      { return m_printDiagnostics ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HIIntersectionSurface::kSurfaceType ; }

      // A flag to tell HIHelixIntersection whether or not to check if
      // the origin was crossed or the path length was too large or small.
      virtual DABoolean checkPropagation() const { return true ; }

      DABoolean useReverseHitOrder() const
      { return m_useReverseHitOrder ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      DABoolean m_forceFittingWeight ;
      double m_fittingWeight ;
      DABoolean m_useReverseHitOrder ;

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIIntersectionSurface( const HIIntersectionSurface& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIIntersectionSurface& operator=( const HIIntersectionSurface& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------
      void calculateTransportMatrix( HIHelix& newHelix,
				     const HIHelix& oldHelix ) const ;


      // ---------- data members -------------------------------
      DGConstMaterialPtr m_innerMaterial ;
      DGConstMaterialPtr m_outerMaterial ;
      Meters m_radius ;
      HICondition* m_condition ;
      HIHelixOperation* m_operation ;
      DABoolean m_printDiagnostics ;
      DABoolean m_dtorDeletesCondition ;
      DABoolean m_dtorDeletesOperation ;

      // ---------- static data members ------------------------

};


// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIIntersectionSurface.cc"
//#endif


// Binary predicate classes needed for sorting pointers of
// HIIntersectionSurface objects.

// class RadiusLessThan
//  : public binary_function< HIIntersectionSurface*,
// 		       HIIntersectionSurface*,
// 		       DABoolean >
// {
//    public:
//       DABoolean operator () ( const HIIntersectionSurface* a,
// 			      const HIIntersectionSurface* b )
//       {
// 	 return ( a->radius() < b->radius() ) ;
//       }
// };

// class RadiusGreaterThan
//  : public binary_function< HIIntersectionSurface*,
// 		       HIIntersectionSurface*,
// 		       DABoolean >
// {
//    public:
//       DABoolean operator () ( const HIIntersectionSurface* a,
// 			      const HIIntersectionSurface* b )
//       {
// 	 return ( a->radius() > b->radius() ) ;
//       }
// };

#endif /* HELIXINTERSECTION_HIINTERSECTIONSURFACE_H */
