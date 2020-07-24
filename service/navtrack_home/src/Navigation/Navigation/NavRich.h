#if !defined(NAVIGATION_NAVRICH_H)
#define NAVIGATION_NAVRICH_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRich
// 
/**\class NavRich NavRich.h Navigation/NavRich.h

 Description: User-level navigation object for rich.

 T.S. 7/26/00, 12/30/02

 Usage:
    \c NavRich provides access to all track related Rich information.
    Most of the users will be only interested in the first few
    functions. The documentation and the declarations are structured
    to separate most commonly used functions and functions intended
    primarily for experts. 

    All \c NavRich functions comply with standard CLEO-III units
    (i.e. meters for length).
    Please be warned that if you access length quantities in
    other Rich classes, they are in centimeters rather than meters.
    All Rich code, but \c NavRich , use centimeters for historical
    reasons.

 \par Accessing NavRich information:

 Usual way is to loop over \c NavTracks and using its \c richInfo()
 function to get a pointer (i.e. FAItem<NavRich>) to \c NavRich. 
 It is important to check validity of that pointer since there are
 often a few tracks which were skipped in Rich analysis.   

\code
#include "Navigation/NavTrack.h"
#include "Navigation/NavRich.h"
  FATable<NavTrack> navTracks;
  extract( iFrame.record(Stream::kEvent),  navTracks );
  FATable<NavTrack>::const_iterator tableEnd( navTracks.end() );
  for( FATable<NavTrack>::const_iterator item( navTracks.begin() ) ; 
                                         item != tableEnd ; 
                                       ++item ) {
     FAItem<NavRich> navRichPtr = item->richInfo();
     // if this pointer is not valid, the track was skipped in Rich analysis
     // it may also mean that Rich producers were not configured properly
     if( navRichPtr.valid() ){ 
          const NavRich& navRich = *navRichPtr;
     }
  } 
\endcode

 If you run a special analysis in which track input to Rich analysis
 is not taken from tracking system (e.g. when using 
 \c RichTracksFromRichProd instead of \c  RichTracksProd) you should 
 extract and loop over \c NavRich \c FATable.

\code
#include "Navigation/NavRich.h"
  FATable<NavRich> navRichTracks;
  extract( iFrame.record(Stream::kEvent),  navRichTracks );
  FATable<NavRich>::const_iterator tableEndR( navRichTracks.end() );
  for( FATable<NavRich>::const_iterator item( navRichTracks.begin() ) ; 
                                        item != tableEndR ; 
                                      ++item ) {
     const NavRich& navRich = *item;
  }
\endcode

 The second way will, of course, work also when
 tracks are input to Rich from the tracking system.
 In that case you can get to \c NavTrack from \c NavRich via
 \c navTrack() function. \c trackId() returns NavTrack identifier
 without extracting \c NavTrack object. 
  
 \par Particle identification tools

 Identification algorithms are still in development, thus
 this section is likely to be expanded/modified in the future.
 I recommend using log-likelihoods to do particle discrimination
 for now. In addition require some minimum number of photons to be 
 used in the track average angle for the hypothesis of your
 choice (e.g. > 4 ). You may also want to cut on track momentum to make
 sure it is above radiation threshold for your hypothesis:

\code
                       electron   muon    pion     kaon   proton
 threshold             0.0004     0.093   0.124   0.438   0.832   GeV
 50% of photon
   yield reached at    0.0006     0.132   0.175   0.619   1.177   GeV
\endcode    

 Many \c NavRich functions take a mass hypothesis as input parameter.
 The mass hypotheses are defined in C3Rich/RichHypotheses.h.
 Use \c RichHypotheses::kElectron, \c RichHypotheses::kMuon, 
     \c RichHypotheses::kPion, \c RichHypotheses::kKaon and 
     \c RichHypotheses::kProton 
 to dial specific mass hypothesis. 
 To loop over all do
\code
  #include "C3Rich/RichTypes.h" 
  for(RichHypothesisIDType iHyp=RichHypotheses::kElectron; 
                           iHyp<RichHypotheses::kHypMax;
                         ++iHyp){ ... }
\endcode
 Some, but not all, \c NavRich functions have also variants
 in which mass hypothesis is specified in the function name
 (see below).

 \c hypLogLikelihood(iHyp) returns -2*log(likelihood) for given 
 mass hypothesis. The same information is accessible also via
 \c electronLogLikelihood(), \c muonLogLikelihood(), etc. 
 The log-likelihoods are chisquare-like quantities.
 Unfortunately at present their absolute values are 
 meaningless and only the chisquare difference between
 two hypotheses is meaningful. 

 An example:
\code
 double pionKaonChisquareDifference = 
           navRich.hypLogLikelihood(RichHypotheses::kPion) -
           navRich.hypLogLikelihood(RichHypotheses::kKaon);
\endcode
 or
\code
 double pionKaonChisquareDifference = 
           navRich.pionLogLikelihood() - 
           navRich.kaonLogLikelihood();
\endcode 
 With the \c pionKaonChisquareDifference defined below,
 pions will tend to have negative values, whereas kaons
 will tend to have positive values. 
\code 
 pionKaonChisquareDifference > 0.0 
\endcode 
 will select tracks which are more likely to be a kaon than
 a pion.     
\code 
 pionKaonChisquareDifference > 9.0 
\endcode 
 should give 3 sigma rejection of pions.
 Hypotheses below the radiation threshold are assigned a large
 chisquare, thus use these chisquare differences with care for
 low momentum tracks.

 User is also advised to check that given mass hypothesis was
 analyzed before using any other quantities for that hypothesis. 
 This can be accomplished using \c hypWasAnalyzed(iHyp) 
 or \c pionHypWasAnalyzed() etc. functions.

 Here is a complete example of using RICH for separating 
 kaons from pions:
\code
  FATable<NavTrack> navTracks;
  extract( iFrame.record(Stream::kEvent),  navTracks );
  FATable<NavTrack>::const_iterator tableEnd( navTracks.end() );
  for( FATable<NavTrack>::const_iterator item( navTracks.begin() ) ; 
                                         item != tableEnd ; 
                                       ++item ) {
     //  is track momentum high enough to use RICH K/pi likelihoods ?
     FAItem< TDKinematicFit > kaon ( item->kaonFit() );     
     if( kaon->momentum().mag() > 0.6  ){
       // was this track subject to RICH analysis ?  
       FAItem<NavRich> navRichPtr ( item->richInfo() );
       if( navRichPtr.valid() ){ 
          // were both pion and kaon hypotheses analyzed ?
          const NavRich& navRich = *navRichPtr;
          if( navRich.pionHypWasAnalyzed() && navRich.kaonHypWasAnalyzed() ){
              // at least 5 photons matched kaon hypothesis ?
              if( navRich.kaonNumberOfPhotonsInAverage() > 4 ){
                  // pion chisquared larger than kaon chisquared ? 
                  if( navRich.pionLogLikelihood() > navRich.kaonLogLikelihood() ){
                            // this is a kaon candidate
                  }
              }
          }
       }
     }
  } 
\endcode
 

 \c hypWithMaxLikelihood() returns hypothesis index with maximum
 likelihood (i.e. lowest log-likelihood).
 Selection of type:
\code
 if( RichHypotheses::kKaon == navRich.hypWithMaxLikelihood() ){ ... }
\endcode
 will be inefficient for high momentum kaons, since fluctuations
 can easily make other hypotheses more likely (Rich particle ID
 separation decreases with increasing track momentum).

 \c hypNumberOfPhotonsInAverage(iHyp) function (and related particle
 specific calls \c electronNumberOfPhotonsInAverage() ) 
 returns a number of photons with Cherenkov angle within +-3sigma cut
 of the expected angle for given hypothesis. For example, number
 of photons for a kaon hypothesis can be obtained from:
\code
 int ng = navRich.hypNumberOfPhotonsInAverage(RichHypotheses::kKaon);
\endcode
 or simply as
\code
 int ng = navRich.kaonNumberOfPhotonsInAverage();
\endcode
 \c mostLikelyNumberOfPhotonsInAverage() returns number of photons
 for the most likely hypothesis.

 \c hypAverageAngleStandardDeviation(iHyp) returns a standard deviations of average
 Cherenkov angle per track from the expected angle for given hypothesis.
 Again functions of type \c kaonAverageAngleStandardDeviation() can be used
 instead. You should not think about these deviations as "SGKADI"like since 
 different hypotheses may have different number of photons assigned.
 Furthermore, photons used in one hypothesis may be different from the
 ones used in the other hypothesis. 
 Thus, e.g. a hypothesis with single assigned photon may have a better
 \c hypAverageAngleStandardDeviation(iHyp) than one with 10 photons assigned.
 Clearly the latter is more likely to be correct in many situations.
 Proper use of these deviations is e.g. in consistency cuts:
\code
 if( abs( navRich.kaonAverageAngleStandardDeviation() )<3.0 ){ ... }
\endcode
 gives loosely preselected kaons. Inconsistency cuts are likely to kill
 all your efficiency, and I do not recommend them.
 Hypotheses with no photons have a deviation of zero. 

 \par ------------------------------------------------------------------
 \par MOST OF PEOPLE SHOULD SKIP THE REST OF DOCUMENTATION WHICH
 \par DEALS WITH FUNCTIONS WHICH ARE UNLIKELY TO BE OF GENERAL INTEREST 
 \par ------------------------------------------------------------------

 \c hypAverageAngleStandardDeviation(iHyp) can be turned into
 deviation in radians by multiplying it by \c hypAverageAngleTotalError(iHyp)

 \c RichTrack's producer (usually \c RichTrackPrototypeProd+RichTracksProd ) 
 can be configured to skip certain mass hypotheses. You can check if the hypothesis was
 not analyzed by using \c hypWasAnalyzed(iHyp) function. 
 By default all hypotheses are analyzed. However, in some past versions
 of pass2 releases some hypotheses were not analyzed since
 the associated outwards fits contained erroneous values
 for the tracking errors.
 
 \par Information on track matching to Rich hit 

 (i.e. connected region of pads that fired) can be accessed via
 \c richTrackCRegionMatch() function (full access).
 Non-experts are advised to use shortcuts:
 \c matchedRichConnectedRegionPulseHeight(iHyp) gives pulse height
 of the matched hit. Zero indicates that no hit was matched.
 Even when a hit is matched, it may be coincidental. Good track hits
 have large pulse heights 
 ( > 700.0 for early CLEO-III post-engineering data)
 \c richConnectedRegion(iHyp) returns FAItem<RichConnectedRegion> to the
 matched connected region (check validity before dereferencing !).
 \c HepPoint3D matchedRichConnectedRegionPosition(iHyp ) gives 
 Rich hit coordinates in CLEO reference frame. This, and the
 following function, are evaluated on a fly and need Rich geometry
 to be available in suez. 
 \c HepSymMatrix matchedRichConnectedRegionPositionCov(iHyp) returns
 corresponding error matrix. Both are all zeros if no hit was matched.
 \c trackRichHitDeltaZ(iHyp) returns track - rich hit residual along
 rich chamber axis which is parallel to the z-axis. 
 \c trackRichHitDeltaZError(iHyp) returns error on it.
 There are also \c trackRichHitDeltaRphi(iHyp) and
 \c trackRichHitDeltaRphiError(iHyp) for the perpendicular
 direction (despite their confusing name, the residual is
 evaluated on rich pad chamber plane, not on a cylinder).

 \par Information on track intersection with Rich radiators

 is saved in \c RichTrackRadiators class accessible via
 \c richTrackRadiators(). Non-experts should use short-cut
 functions: \c numberOfRadiators(iHyp), \c sigmasToAnEdge(iHyp)
 (track distance to any radiator edge in standard deviations)
 \c radiatorId(iHyp) (for the most likely radiator if more
 than one) \c radLength(iHyp) (effective track length in the
 radiators).
 \c HepPoint3D \c trackRadiatorIntersect(iHyp) and  
 \c HepVector3D \c trackDirectionAtRadiatorIntersect(iHyp) 
 give mean emission point and track direction in local
 coordinate of the radiator (corresponding to \c radiatorId()). 
 These two functions require Rich geometry to be loaded into
 suez.  

 \par Information on track-input to Rich analysis (RichTrackPrototype and RichTrack classes)

 \c RichTrackPrototype class returned via \c richTrackPrototype() function
 lays out which tracking information was used as input to the Rich reconstruction code. 
 It stores the track identifier and the information on which helix fits were 
 used as input for various mass hypotheses. 
 \c RichTrack has similar structure to \c RichTrackPrototype
 except that it also contains helix parameters copied from tracking 
 objects and possibly modified. \c NavRich table is build on top of
 \c RichTrackPrototype table, thus extraction of \c NavRich does
 not trigger extraction of helices. Only \c RichTrackPrototype
 can be saved in storage. When accessing Rich information from
 storage, \c RichTrack table is recreated from RichTrackPrototype's
 which triggers extractions of all needed helices. Since some of
 them may not be stored in easily accessible medium all \c NavRich
 function depedent on \c RichTrack extractions should be used with care.
 Full access to \c RichTrack is provided via \c richTrack(). 
 Shortcut to an input helix (mass hypothesis dependent):
 \c FAItem<KTHelix> richTrajectory(iHyp).
 Usually this helix is a copy (possibly modified) of one of the helix fits belonging
 to \c TDTrack (accessible via \c NavTrack). Which fit was input for given 
 hypothesis depends on \c RichTrack producer (see e.g. parameters of
 \c RichTrackPrototypesProd).

 \par Info on track possibly "improved" by use of matched Rich hit 

 Knowledge of trajectory through the Rich radiator can be improved if the track
 leaves a hit in Rich pad chamber (information on matched Rich hit i.e. 
 RichConnectedRegion can be found via NavRich functions described below).
 Some tracks miss active region of Rich chambers. If the track is actually
 improved depends on the producer of \c RichImprovedTrack's (see e.g. \c RichImprovedTracksProd).    
 To find out if trajectory was improved use \c DABoolean \c trajectoryWasImproved(iHyp) function
 Full access to the improved track is provided via \c FAItem<RichImprovedTrack> \c richImprovedTrack() function.
 There is also a shortcut to \c FAItem<KTHelix> via \c richImprovedTrajectory(iHyp).
 Notice that improved track is used only in Cherenkov angles analysis. 
 Track-rich hit matching is done using tracks before improvement.

 \par List of individual photons linked to the track

 is stored in \c RichTrackPhotons class accessible
 via \c richTrackPhotons() functions.
 For convenience shortcut functions 
 returning STL vector of pointers to \c RichPhotonAngles
 \c RichRayAngle, \c RichPhoton, \c RichBump and \c RichCalibratedHit
 are provided. Only calibrated hits for the central pad in
 the rich bump are returned by \c richCalibratedHitPtrs().
 The last three functions take optional additional two arguments 
 which specify range of Cherenkov angle deviation for
 entities to be included in the list (by default -5,+5 
 corresponding to preselection cuts). 
 \c numberOfPhotonsLinked(iHyp) returns number of all linked
 photons (do not assume this equals to size of any returned
 list - use size() function of the STL vector instead).

 \par Printing NavRich content with << operator. 

 NavRich knows how to print its content to an output stream.
 This should be used with care, since many NavRich functions can
 trigger some proxies, which in turn may try to print information
 which leads to errors in output. Thus, if you are going to
 use this operator call \c richTrackAveAngles() function
 BEFORE you use this NavRich with the << operator (this will
 trigger complete chain of Rich analysis before you try to print
 the results). If you are confused by these hints, 
 just don't use this operator.
 
*/
// Description: Navigation object for Rich
//
// Usage:      
//
//
// Author:      Tomasz Skwarnicki
// Created:     Tue Jul 18 10:46:34 EDT 2000
// $Id: NavRich.h,v 1.4 2002/12/30 20:58:44 ts Exp $
//
// Revision history
//
// $Log: NavRich.h,v $
// Revision 1.4  2002/12/30 20:58:44  ts
// change in documentation part only, advertise use of hypWasAnalyzed()
//
// Revision 1.3  2001/09/07 18:04:03  cleo3
// removed forward declaration of ostream
//
// Revision 1.2  2000/11/20 00:05:59  ts
// build NavRich on top of RichTrackPrototype
//
// Revision 1.1  2000/07/27 01:32:05  ts
// NavRich added
//
//

// system include files

// user include files

#include "C3Rich/RichTypes.h"
#include "C3Rich/RichHypotheses.h"
#include "RichTrackPrototypesProd/RichTrackPrototype.h"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Geometry/Vector3D.h"


// forward declarations

class NavRichServer;
class KTHelix;
class RichTrack;
class RichImprovedTrack;
class NavTrack;
class RichTrackAveAngles;
class RichTrackChamberCoord;
class RichTrackCRegionMatch;
class RichCRegionChamberCoord;
class RichConnectedRegion;
class RichTrackRadiators;
class RichTrajectoryRadiator;
class RichTrackPhotons;
class RichPhotonAngles;
class RichRayAngle;
class RichPhoton;
class RichBump;
class RichCalibratedHit;

template <class T> class FAItem         ;
template <class T> class FATable        ;

#include "STLUtility/fwd_vector.h"
#include "C++Std/fwd_ostream.h"

class NavRich ;

ostream& operator<<( ostream& , const NavRich& ) ;

class NavRich
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------

      typedef RichTrackPrototype::Identifier          Identifier     ;

      typedef float Radians;
      typedef float Meters;
      typedef float ADCcounts;
      typedef float Unitless;
      typedef RichDFloatType ChisquareWithArbitraryOffset;

      typedef STL_VECTOR( const RichPhotonAngles* ) RichPhotonAnglesTable;
      typedef STL_VECTOR( const RichRayAngle* ) RichRayAngleTable;
      typedef STL_VECTOR( const RichPhoton* ) RichPhotonTable;
      typedef STL_VECTOR( const RichBump* ) RichBumpTable;
      typedef STL_VECTOR( const RichCalibratedHit* ) RichCalibratedHitTable;

      // ---------- Constructors and destructor ----------------

      NavRich( const RichTrackPrototype&  aRichTrackPrototype,
	       const NavRichServer& aServer   ) ;
      NavRich();

      virtual ~NavRich();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------


      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      //     functions of common interest first       (functions for experts later)
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


            // Warning! NavRich identifiers do not coincide with NavTrack identifiers
            // use NavRich.navTrack() and NavTrack.richInfo() to cross-reference

            // (NavRich identifiers are the same as RichTrack identifiers)                 

      Identifier         identifier() const; 
                            
            // .....................................................................
            // ............   corresponding NavTrack ............................... 
            // .....................................................................

                  // pointer to corresponding NavTrack (it may be invalid if RichTrack's were
                  //                                    not made out of tracking system tracks)
      FAItem<NavTrack>  navTrack() const; 

                  // track identifier (usually NavTrack identifier); does not extract NavTrack
      RichTrackIDType   trackId()  const; 


            // .....................................................................
            // .............   per track likelihoods ................................
            // .....................................................................

                  // -2(log-likelihood) for each hypothesis
      ChisquareWithArbitraryOffset hypLogLikelihood( RichHypothesisIDType iHyp ) const;     
      ChisquareWithArbitraryOffset electronLogLikelihood() const { return hypLogLikelihood(RichHypotheses::kElectron); } 
      ChisquareWithArbitraryOffset muonLogLikelihood()     const { return hypLogLikelihood(RichHypotheses::kMuon); } 
      ChisquareWithArbitraryOffset pionLogLikelihood()     const { return hypLogLikelihood(RichHypotheses::kPion); } 
      ChisquareWithArbitraryOffset kaonLogLikelihood()     const { return hypLogLikelihood(RichHypotheses::kKaon); } 
      ChisquareWithArbitraryOffset protonLogLikelihood()   const { return hypLogLikelihood(RichHypotheses::kProton); } 

                  // hypothesis with maximal likelihood (i.e. minimal "chisquare")
      RichHypothesisIDType hypWithMaxLikelihood() const;  

            // .....................................................................
            // .............   per track average-Cherenkov-angles ..................
            // .....................................................................

                 // number-of-photons included in average Cherenkov angle
      RichPhotonIDType  hypNumberOfPhotonsInAverage( RichHypothesisIDType iHyp ) const;  
      RichPhotonIDType    electronNumberOfPhotonsInAverage() const { return hypNumberOfPhotonsInAverage(RichHypotheses::kElectron); }
      RichPhotonIDType    muonNumberOfPhotonsInAverage()     const { return hypNumberOfPhotonsInAverage(RichHypotheses::kMuon); }
      RichPhotonIDType    pionNumberOfPhotonsInAverage()     const { return hypNumberOfPhotonsInAverage(RichHypotheses::kPion); }
      RichPhotonIDType    kaonNumberOfPhotonsInAverage()     const { return hypNumberOfPhotonsInAverage(RichHypotheses::kKaon); }
      RichPhotonIDType    protonNumberOfPhotonsInAverage()   const { return hypNumberOfPhotonsInAverage(RichHypotheses::kProton); }

      RichPhotonIDType    mostLikelyNumberOfPhotonsInAverage() const { return hypNumberOfPhotonsInAverage(hypWithMaxLikelihood()); }

               // deviation of average angle from the expected one in standard deviations 
               // (these will not work well for "inconsistency cuts" - see docs above)
      Unitless hypAverageAngleStandardDeviation( RichHypothesisIDType iHyp ) const ; 
      Unitless   electronAverageAngleStandardDeviation()  const { return hypAverageAngleStandardDeviation(RichHypotheses::kElectron); }
      Unitless   muonAverageAngleStandardDeviation()      const { return hypAverageAngleStandardDeviation(RichHypotheses::kMuon); }
      Unitless   pionAverageAngleStandardDeviation()      const { return hypAverageAngleStandardDeviation(RichHypotheses::kPion); }
      Unitless   kaonAverageAngleStandardDeviation()      const { return hypAverageAngleStandardDeviation(RichHypotheses::kKaon); }
      Unitless   protonAverageAngleStandardDeviation()    const { return hypAverageAngleStandardDeviation(RichHypotheses::kProton); }


      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      //     functions which are unlikely to be of general interest 
      //     ( ordered according to decreasing likelihood of being useful for non-expert)
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


      // total expected error on average angle deviation (in radians)
      Radians hypAverageAngleTotalError( RichHypothesisIDType iHyp ) const ; 
      Radians electronAverageAngleTotalError()  const { return hypAverageAngleTotalError(RichHypotheses::kElectron); }
      Radians muonAverageAngleTotalError()      const { return hypAverageAngleTotalError(RichHypotheses::kMuon); }
      Radians pionAverageAngleTotalError()      const { return hypAverageAngleTotalError(RichHypotheses::kPion); }
      Radians kaonAverageAngleTotalError()      const { return hypAverageAngleTotalError(RichHypotheses::kKaon); }
      Radians protonAverageAngleTotalError()    const { return hypAverageAngleTotalError(RichHypotheses::kProton); }

      // it is possible to limit Rich producers to a certain subset of mass hypotheses
      // you can find out which were inactive from below functions 
      // (if analysis failed all will be false)
      DABoolean hypWasAnalyzed( RichHypothesisIDType iHyp ) const; 
      DABoolean electronHypWasAnalyzed() const {return hypWasAnalyzed(RichHypotheses::kElectron); }
      DABoolean muonHypWasAnalyzed()     const {return hypWasAnalyzed(RichHypotheses::kMuon); }
      DABoolean pionHypWasAnalyzed()     const {return hypWasAnalyzed(RichHypotheses::kPion); }
      DABoolean kaonHypWasAnalyzed()     const {return hypWasAnalyzed(RichHypotheses::kKaon); }
      DABoolean protonHypWasAnalyzed()   const {return hypWasAnalyzed(RichHypotheses::kProton); }

      //  pulse height of a Rich hit attributed to the track ionizing rich pad chamber
      //  (zero if none, if small could be coincidental match)
      ADCcounts matchedRichConnectedRegionPulseHeight( RichHypothesisIDType iHyp ) const;

      //  track-rich hit residuals
      Meters trackRichHitDeltaZ( RichHypothesisIDType iHyp ) const;
      Meters trackRichHitDeltaZError( RichHypothesisIDType iHyp ) const;
      Meters trackRichHitDeltaRphi( RichHypothesisIDType iHyp ) const;
      Meters trackRichHitDeltaRphiError( RichHypothesisIDType iHyp ) const;

      //    position and error matrix of the matched Rich hit in master reference system 
      //    (you will need to load Rich geometry into suez to use these two)
      HepPoint3D   matchedRichConnectedRegionPosition( RichHypothesisIDType iHyp ) const;
      HepSymMatrix matchedRichConnectedRegionPositionCov( RichHypothesisIDType iHyp ) const;


      // number of intersected radiators
      RichSmallCountType numberOfRadiators( RichHypothesisIDType iHyp ) const; 

      // number of standard deviations to any radiator edge
      Unitless sigmasToAnEdge( RichHypothesisIDType iHyp ) const;  

      // largest intersect radiator ID 
      RichRadiatorIDType radiatorId( RichHypothesisIDType iHyp ) const;  

      // effective track length in the radiators
      Meters radLength( RichHypothesisIDType iHyp ) const;     
      
      // position (in m) and 
      // direction cosines of the track intersect (i.e. mean emission point) 
      // with the radiator (of longest intersect) in local coordinates of the radiator  
      //    (you will need to load Rich geometry into suez to use these two)
      HepPoint3D trackRadiatorIntersect( RichHypothesisIDType iHyp ) const;  
      HepVector3D trackDirectionAtRadiatorIntersect( RichHypothesisIDType iHyp ) const;  

      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      //     functions which are essentially for experts only
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      // access to truncated info on track-parameters input to Rich analysis 
      //     no helix info is extracted by this call
      FAItem<RichTrackPrototype>  richTrackPrototype() const;  

      // access to full info on track-parameters input to Rich analysis
      FAItem<RichTrack>  richTrack() const;  

      //  shortcut to trajectory info in richTrack()
      FAItem<KTHelix> richTrajectory( RichHypothesisIDType iHyp ) const;

      // access to full info on rich-hit-improved track-parameters
      FAItem<RichImprovedTrack>  richImprovedTrack() const;

      // shortcut to improved trajectory
      FAItem<KTHelix> richImprovedTrajectory( RichHypothesisIDType iHyp ) const;

      // will tell you if track was improved 
      //    this call will cause extraction of helix information
      DABoolean trajectoryWasImproved( RichHypothesisIDType iHyp ) const;

      //    access to full info on per track likelihoods and average angles
      FAItem<RichTrackAveAngles> richTrackAveAngles() const;

      //    access to full info on track projections onto Rich chambers
      FAItem<RichTrackChamberCoord>   richTrackChamberCoord() const;
      //    access to full info on track - rich hit match
      FAItem<RichTrackCRegionMatch>   richTrackCRegionMatch() const;
      //    access to full info on rich cluster position in Rich chamber coordinates
      FAItem<RichCRegionChamberCoord> richCRegionChamberCoord( RichHypothesisIDType iHyp ) const;
      //    access to full info on track projections onto Rich chambers
      FAItem<RichConnectedRegion>     richConnectedRegion( RichHypothesisIDType iHyp ) const;
      //    access to full info on track-rich radiator intersections
      FAItem<RichTrackRadiators>  richTrackRadiators() const;
                  // largest intersect radiator
      FAItem<RichTrajectoryRadiator> richTrajectoryRadiator( RichHypothesisIDType iHyp ) const; 
      //    access to full info on track-Cherenkov photons info
      FAItem<RichTrackPhotons>  richTrackPhotons() const;
      //    shortcuts:
      RichPhotonIDType numberOfPhotonsLinked( RichHypothesisIDType iHyp ) const;
          // pointers to RichPhotonAngles
      RichPhotonAnglesTable  richPhotonAnglesPtrs( RichHypothesisIDType iHyp ) const; 
          // pointers to RichRayAngle for the most likely optical path
      RichRayAngleTable      richRayAnglePtrs( RichHypothesisIDType iHyp ) const;     
          // pointers to RichPhoton's for given cut on Cherenkov angle deviation 
      RichPhotonTable        richPhotonPtrs( RichHypothesisIDType iHyp, const Unitless minStdDev= -5.0, const Unitless maxStdDev= +5.0 ) const; 
          // pointers to RichBump's for given cut on Cherenkov angle deviation 
      RichBumpTable          richBumpPtrs(   RichHypothesisIDType iHyp, const Unitless minStdDev= -5.0, const Unitless maxStdDev= +5.0 ) const;
          // pointers to RichCalibratedHit's with maximal pulse in the bump for given cut on Cherenkov angle deviation 
      RichCalibratedHitTable richCalibratedHitPtrs( RichHypothesisIDType iHyp, const Unitless minStdDev= -5.0, const Unitless maxStdDev= +5.0 ) const;

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavRich( const NavRich& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavRich& operator=( const NavRich& ); // stop default
      // ---------- private member functions -------------------

      void getCRHypXyz(RichHypothesisIDType iHyp ) const;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      const RichTrackPrototype& m_RichTrack;
      const NavRichServer&      m_server   ;

      //  cache ....................................... 

      //             for RichTrackAveAngles quantities
      RichPhotonIDType*   m_HypNumberOfPhotonsInAverage;
      Unitless*           m_HypAverageAngleStandardDeviation;
      Radians*            m_HypAverageAngleTotalError;
      DABoolean*          m_HypWasAnalyzed;

      //             for track-rich hit match
      RichHypothesisIDType       m_CRHypInCache;
      RichConnectedRegionIDType  m_CRInCache;

      HepPoint3D*   m_MatchedRichConnectedRegionPosition;
      HepSymMatrix* m_MatchedRichConnectedRegionPositionCov;

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* NAVIGATION_NAVRICH_H */
