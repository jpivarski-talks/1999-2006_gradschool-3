*
* $Id: cfhita.F,v 1.4 2003/03/05 17:58:19 dpp Exp $
*
* $Log: cfhita.F,v $
* Revision 1.4  2003/03/05 17:58:19  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.3  2001/11/19 23:54:21  dpp
*      -> delete unused include file
*
* Revision 1.2  2000/08/18 18:52:54  dpp
*      -> comment
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
*
#include "sys/CLEO_machine.h"
*-- Author :
      SUBROUTINE CFHITA
C.......................................................................
C.
C. CFHITA - HITs are Added to the track that are not used in the fit

C!!  this routine is called from TF3FIT which still uses the old TF
C!!  scatters. If this routine is called from somewhere else or
C!!  TF3FIT scatters are updated, then the scattering radius in the call to
C!!  TFPHTR must change.

C.
C. COMMON    : ?
C. CALLS     : CFCFIL FINDHW TFPHTR
C. CALLED    : TF3FIT
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.10
C. CREATED   : ?
C. LAST MOD  : 25-May-92
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"

      INTEGER YESFIT(KLYRTF)
      INTEGER IHWADR(NHITDM),ILYR,IER,NHW,IHW,IADRS
      INTEGER IFIT,IAMB,GOTONE
      REAL XT,YT,PHIT,ARCFUN
      REAL DEL,RESNOW,RESMIN

      EXTERNAL ARCFUN

* ----------Executable code starts here---------------------

C set up the list of layers really in the fit

      CALL VZERO(YESFIT,KLYRTF)
      IF(NFIT.LE.0)GO TO 119
      DO 117 IFIT=1,NFIT
          ILYR=IPLFIT(IFIT)
          IF(ILYR.LE.0)GO TO 117
          IF(ILYR.GT.KLYRTF)GO TO 117
          YESFIT(ILYR)=1
117   CONTINUE
119   CONTINUE

C loop over CF layers looking for other hits in the DR

      DO 157 ILYR = LLYRCF, MLYRCF
          IF(NFIT.GE.NLR2TF)GO TO 159
          IF(IDVCTF(ILYR).NE.IDRFT)GO TO 157
          IF(YESFIT(ILYR).EQ.1)GO TO 157
              CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RSCATF(IKN6TF),
     2                    RSQTF(ILYR),+1,XT,YT,PHIT,IER )
              IF(IER.NE.0)GO TO 157

C set up array, IHWADR, with addresses of hit wires
C NHITDM is used for number of hits allowed, NHW is number found

                  CALL FINDHW(ILYR+0,PHIT,0.0,IHWADR,NHITDM,NHW)
                  IF(NHW.LE.0 )GO TO 157

C put the best hit in the fit list
C RESMIN is preset to the maximum allowed, 1.0 cells
C loop over wires found
C loop over possible ambiguities
C DEL is the distance (track-wire)

                  RESMIN=CELLTF(ILYR)*RTF(ILYR)
                  GOTONE=0
                  DO 137 IHW = 1,NHW
                      IADRS= IHWADR(IHW)
                      DEL= KAPCF*RSQTF(ILYR) + ALPCF*EvntHit_X(IADRS)
     +                   + BETCF*EvntHit_Y(IADRS) + GAMCF
                  DO 127 IAMB=-1,1,2
                      RESNOW=ABS( DEL-IAMB*EvntHit_Drift(IADRS) )
                      IF(RESNOW.GE.RESMIN)GO TO 127

C need only save IADFIT,IPLFIT,SFIT; everything else is generated in TF3FIT

                      IF(GOTONE.EQ.1)GO TO 125
                      GOTONE=1
                      NFIT=NFIT+1
                      YESFIT(ILYR)=-1
                      IPLFIT(NFIT)=-ILYR
                      SFIT(NFIT)=ARCFUN(KAPCF,ALPCF,BETCF,XT,YT,XICF)
 125                  IADFIT(NFIT)=IAMB*IADRS
                      RESMIN=RESNOW
 127  CONTINUE
C  ^---end of loop over sign of the drift distance
 137  CONTINUE
C  ^---end of loop over hits in the layer
 157  CONTINUE
C  ^---end of loop over layers
 159  CONTINUE
C  ^---break point if fit list is filled

      RETURN
      END
