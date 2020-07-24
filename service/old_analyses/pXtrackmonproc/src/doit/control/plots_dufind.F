*
* $Id: plots_dufind.F,v 1.1 2001/11/19 22:58:49 dpp Exp $
*
* $Log: plots_dufind.F,v $
* Revision 1.1  2001/11/19 22:58:49  dpp
*      -> NEW ROUTINE
*      -> hbook plots of track variables
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D Peterson
      SUBROUTINE PLOTS_DUFIND
C.......................................................................
C.
C. PLOTS_DUFIND - diagnostic plots at end of DUFIND
C.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.0
C. CREATED   : 16-11-2001
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tftrakcd.inc"


      LOGICAL LFIRST_HBOOK
      SAVE LFIRST_HBOOK
      DATA LFIRST_HBOOK/.TRUE./

      INTEGER IFIT
 
c      REAL CFIX
c      COMMON/FIXER/CFIX(250)

* ----------Executable code starts here---------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
         
      IF(LFIRST_HBOOK)THEN
        CALL HBOOK1(151,'CURVA ',100,-1.0,1.0,0.)
        CALL HBOOK1(152,'TANDLF',100,-1.5,1.5,0.)
        CALL HBOOK1(153,'Z0    ',100,-.10,.10,0.)
        CALL HBOOK1(154,'CHICF ',100,0.,50.,0.)
        CALL HBOOK1(155,'CURVA ',100,-1.0,1.0,0.)
        CALL HBOOK1(156,'RES ALL  ',100,-.05,.05,0.)
        CALL HBOOK1(161,'RES -1.05',100,-.05,.05,0.)
        CALL HBOOK1(162,'RES -0.70',100,-.05,.05,0.)
        CALL HBOOK1(163,'RES -0.35',100,-.05,.05,0.)
        CALL HBOOK1(164,'RES  0   ',100,-.05,.05,0.)
        CALL HBOOK1(165,'RES +0.35',100,-.05,.05,0.)
        CALL HBOOK1(166,'RES +0.70',100,-.05,.05,0.)
        CALL HBOOK1(167,'RES +1.05',100,-.05,.05,0.)
        LFIRST_HBOOK=.FALSE.
      ENDIF
            
      IF(
     1     (ABS(TANDLF).LT. 1.225).AND.
     1     (ABS(Y0LF).LT. .03).AND.
     3     (CHICF.LT.15.)
     9     )THEN
        CALL HF1(151,KAPCF,1.)
      ENDIF
      
      IF(
     1     (ABS(KAPCF).GT.  0.04).AND.
     2     (ABS(KAPCF).LT.  0.16).AND.
     1     (ABS(Y0LF).LT. .03).AND.
     2     (CHICF.LT.15.)
     9     )THEN
        CALL HF1(152,TANDLF,1.)
      ENDIF
      
      IF(
     1     (ABS(KAPCF).GT.  0.04).AND.
     2     (ABS(KAPCF).LT.  0.16).AND.
     2     (ABS(TANDLF).LT. 1.225).AND.
     3     (CHICF.LT.15.)
     9     )THEN
        CALL HF1(153,Y0LF,1.)
      ENDIF
      
         
      IF(
     1     (ABS(KAPCF).GT.  0.04).AND.
     2     (ABS(KAPCF).LT.  0.16).AND.
     2     (ABS(TANDLF).LT. 1.225).AND.
     1     (ABS(Y0LF).LT. .03)
     9     )THEN
        CALL HF1(154,CHICF,1.)
      ENDIF
      
      
      IF(
     1     (ABS(KAPCF).GT.  0.04).AND.
     2     (ABS(KAPCF).LT.  0.16).AND.
     2     (ABS(TANDLF).LT. 1.225).AND.
     1     (ABS(Y0LF).LT. .03).AND.
     3     (CHICF.LT.15.)
     9     )THEN
        CALL HF1(155,KAPCF,1.)
        
        DO 711 IFIT=1,NFIT
          IF(IPLFIT(IFIT).EQ.56)THEN
            CALL HF1(156,PULFIT(IFIT),1.)
            IF(
     1           (TANDLF.GE.-1.225).AND.
     2           (TANDLF.LT.-0.875))THEN
              CALL HF1(161,PULFIT(IFIT),1.)
            ELSEIF(
     1             (TANDLF.GE.-0.875).AND.
     2             (TANDLF.LT.-0.525))THEN
              CALL HF1(162,PULFIT(IFIT),1.)
            ELSEIF(
     1             (TANDLF.GE.-0.525).AND.
     2             (TANDLF.LT.-0.175))THEN
              CALL HF1(163,PULFIT(IFIT),1.)
            ELSEIF(
     1             (TANDLF.GE.-0.175).AND.
     2             (TANDLF.LT. 0.175))THEN
              CALL HF1(164,PULFIT(IFIT),1.)
            ELSEIF(
     1             (TANDLF.GE. 0.175).AND.
     2             (TANDLF.LT. 0.525))THEN
              CALL HF1(165,PULFIT(IFIT),1.)
            ELSEIF(
     1             (TANDLF.GE. 0.525).AND.
     2             (TANDLF.LT. 0.875))THEN
              CALL HF1(166,PULFIT(IFIT),1.)
            ELSEIF(
     1             (TANDLF.GE. 0.875).AND.
     2             (TANDLF.LT. 1.225))THEN
              CALL HF1(167,PULFIT(IFIT),1.)
            ENDIF
            
          ENDIF
 711    CONTINUE
      ENDIF

      RETURN
      END

