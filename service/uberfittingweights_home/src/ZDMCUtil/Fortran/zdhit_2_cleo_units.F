C
C  $Id: zdhit_2_cleo_units.F,v 1.4 2003/06/26 18:12:30 huanggs Exp $
C
C  $Log: zdhit_2_cleo_units.F,v $
C  Revision 1.4  2003/06/26 18:12:30  huanggs
C  ZD codes workable
C

      SUBROUTINE ZDHIT_2_CLEO_UNITS( GeantHitsArray,
     &                               iGeantHit,
     &                               WireNumGeantHit,
     &                               XGeantHit,
     &                               YGeantHit,
     &                               ZGeantHit,
     &                               cosZGeantHit,
     &                               pathLengthGeantHit, 
     &                               ElossGeantHit,
     &                               TOFGeantHit,
     &                               DriftDistGeantHit,
     &                               sinXangleGeantHit,
     &                               IonDistGeantHit,
     &                               LR_ambiguity,
     &                               Cotangent )

C 
C  begin_doc
C 
C   Documentation for subroutine zdhit_2_cleo_units
C 
C   Purpose: Convert ZD Geant Hit to CLEO units
C   --------
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C     GeantHitsArray   - contains Geant hits
C     iGeantHit     - loop index for Geant hits

C   Output Parameters: (Name - Meaning)
C   -----------------
C     WireNumGeantHit    - wire # that has geant hit
C     LR_ambiguity       - Left/Right ambiguity of drift distance (+/- 1)
C     ZGeantHit          - Z position of geant hit 
C     cosZGeantHit       - cos in z direction of geant hit, aka dip angle
C     pathLengthGeantHit - pathLength of geant hit 
C     ElossGeantHit      - Energy loss for geant hit 
C     TOFGeantHit        - TOF for geant hit 
C     DriftDistGeantHit  - drift distance of geant hit 
C     sinXangleGeantHit  - sin of crossing angle wrt layer of geant hit 
C     IonDistGeantHit    - distance based on ion statistics of geant hit 
C     Cotangent          - Cotangent of ACOS(CZ)
C     PaveGeantHit       - Average momentum at the length of the hit in drift cell 
      
C 
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:
C   ------
C 
C   Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
C   -------
C 
C   Detailed description:
C   ---------------------
C 
C   Major revisions:
C   ----------------
C      
C 
C  end_doc
C 
      IMPLICIT NONE
 
C  begin_inc
C   Include files :
C   ---------------------
C   Cleo3 Message Reporter
#include "ToolBox/report.inc"

C   Offsets into Geant hit bank
#include "ZDMCUtil/Fortran/geant_zd_hit_def.inc"

C   Cleog includes
#include "Cleog3_suez/cg3cdes/zddspa.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      real GeantHitsArray( MXZDNH, kMaxHit)
      integer iGeantHit 

      integer WireNumGeantHit, LR_ambiguity
      real XGeantHit, YGeantHit, ZGeantHit, cosZGeantHit,
     &     pathLengthGeantHit, ElossGeantHit, TOFGeantHit,
     &     DriftDistGeantHit, sinXangleGeantHit, IonDistGeantHit,
     &     Cotangent, PaveGeantHit

C 
C   Local User defined variables:
C   -----------------------------
C     kCMTOM - convert cm to meters
C     kSTOPSEC - convert seconds to picoseconds
      real kCM_TO_M, kS_TO_PSEC
      parameter ( kCM_TO_M = 100.0 )
      parameter ( kS_TO_PSEC = 1.0E12 )

 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zdhit_2_cleo_units.F,v $')
      parameter(chrcsr='$Revision: 1.4 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/06/26 18:12:30 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zdhit_2_cleo_units' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine zdhit_2_cleo_units:
#if WRITE_RCS_INFO
      if(rcsini)then
        rcsini = .false.
        write(6,*)'-------- CVSinfo --------'
        write(6,*)chrcss
        write(6,*)chrcsr//' '//chrcst
        write(6,*)chrcsd//' '//chrcsa
      endif
#endif
C ----6----------------------------------------------------------------72
 
      if (lfirst) then
        chmess=
     &  'Convert ZD Geant Hit to CLEO units: cm->meters,s->picoseconds'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

      
C     Copy hit info from HITS array into local variables
C     and use CLEO units: cm->meters and seconds->picoseconds.
      WireNumGeantHit    = GeantHitsArray(kWIRE_GEANTHIT,iGeantHit)

      XGeantHit          = GeantHitsArray(kX_GEANTHIT,iGeantHit)
     &                    / kCM_TO_M

      YGeantHit          = GeantHitsArray(kY_GEANTHIT,iGeantHit)
     &                    / kCM_TO_M

      ZGeantHit          = GeantHitsArray(kZ_GEANTHIT,iGeantHit)
     &                    / kCM_TO_M

      cosZGeantHit       = GeantHitsArray(kCZ_GEANTHIT,iGeantHit)

      pathLengthGeantHit = GeantHitsArray(kS_GEANTHIT,iGeantHit)
     &                    / kCM_TO_M

      PaveGeantHit       = GeantHitsArray(kP_GEANTHIT,iGeantHit)

      ElossGeantHit      = GeantHitsArray(kELOS_GEANTHIT,iGeantHit)

      TOFGeantHit        = GeantHitsArray(kTOF_GEANTHIT,iGeantHit)
     &                    * kS_TO_PSEC

      DriftDistGeantHit  = GeantHitsArray(kDIST_GEANTHIT,iGeantHit)
     &                    / kCM_TO_M

      sinXangleGeantHit  = GeantHitsArray(kSINA_GEANTHIT,iGeantHit)

      IonDistGeantHit    = GeantHitsArray(kIOND_GEANTHIT,iGeantHit)
     &                    / kCM_TO_M

      LR_ambiguity       = IFIX(SIGN(1.0,DriftDistGeantHit))


      if ( ABS(cosZGeantHit) .LT. 1. ) then
        Cotangent = 1./TAN(ACOS(cosZGeantHit))
      elseif ( cosZGeantHit .GE. 1. ) then
        Cotangent =  10000000.
      else
        Cotangent = -10000000.
      endif


#if defined(DEBUG_RESPONSE)
      chmess='--------------------------------------------'
      call report(MS_INFO, crname, chmess)
      chmess='Dumping Geant ZD Hit from zdhit_2_cleo_units'
      call report(MS_INFO, crname, chmess)
      chmess='--------------------------------------------'
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'iGeantHit =', iGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'WireNumGeantHit=', WireNumGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'XGeantHit=', XGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'YGeantHit=', YGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'ZGeantHit=', ZGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'cosZGeantHit=', cosZGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'pathLengthGeantHit=', pathLengthGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'ElossGeantHit=', ElossGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'TOFGeantHit=', TOFGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'DriftDistGeantHit=', DriftDistGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'sinXangleGeantHit=', sinXangleGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'IonDistGeantHit=', IonDistGeantHit
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'LR_ambiguity=', LR_ambiguity
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'Cotangent=', Cotangent
      call report(MS_INFO, crname, chmess)

      write(chmess,*) 'PaveGeantHit=', PaveGeantHit
      call report(MS_INFO, crname, chmess)

      chmess='============================================'
      call report(MS_INFO, crname, chmess)
#endif

      return
      end
