*
* $Id: roadhits.F,v 1.4 2001/09/14 21:17:02 dpp Exp $
*
* $Log: roadhits.F,v $
* Revision 1.4  2001/09/14 21:17:02  dpp
*      -> fill pointer to tit coming from same wire
*
* Revision 1.3  2001/03/30 00:04:46  dpp
*     -> new argument for FINDHS
*      -> load EFFILR, efficiency for the layer
*
* Revision 1.2  2000/12/19 23:57:59  dpp
*      -> use local arrays to receive data from FINDHW to
*         allow using HTWADR etc. for a recall of FAST_TRAK
*      -> fill HTWADR etc for a recall of FAST_TRAK
*      -> counters of number of DR layers of types in ROAD
*
* Revision 1.1  2000/11/15 16:55:47  dpp
*      ->  NEW name, replaces CDHITS
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
*
* Revision 1.11  2000/10/24 22:46:09  dpp
*      -> set UPTODATE_LYR flag .FALSE.
*      -> add new argument for FINDHS
*
* Revision 1.10  2000/10/02 20:43:49  dpp
*      -> test error flag returned from TFPHTR
*      -> pass entrace angle to FINDHW
*
* Revision 1.9  2000/08/18 18:49:10  dpp
*      -> change compilation switch to ROADHITSDIAG to limit outout
*
* Revision 1.8  2000/05/26 16:48:05  dpp
*      -> identify SV layers and hybrids that have hits
*
* Revision 1.7  2000/04/26 14:58:37  dpp
*      -> modifications for silicon
*      -> tfphtr can be used for silicon; remove tfinsv
*
* Revision 1.6  1999/12/09 21:13:33  dpp
* change call to hit_to_fit to use parameter directive
*
* Revision 1.5  1999/11/09 21:40:37  dpp
* remove the temporary assignment of the cathode geometry
*
* Revision 1.4  1999/11/03 22:20:19  dpp
* eliminate clustering; to be done in separate process
* set ILLRLR
* set UPTODATE_HITS to false to force full ROADHITS_COORD
* fill in missing cathode geometry, in diag only
*
* Revision 1.3  1999/09/12 22:25:33  dpp
* call HIT_TO_FIT to delete pointers on entry
*
* Revision 1.2  1999/04/27 20:16:19  dpp
* correct for Z position when predicting wire number
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :    Dan Riley   06/09/88
      SUBROUTINE ROADHITS(LVL_TYP,HIT_TYP,FIT_TYP)
C.......................................................................
C.
C. ROADHITS - Calculate the hits that can belong to a seed r-phi track.
C.
C. COMMON    : ?
C. CALLS     : ? CDUSE
C. CALLED    : CFHITS LFHITS
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by D. Riley
C.
C. VERSION   : 1.10
C. CREATED   : ?
C. LAST MOD  : 02-Oct-88
C.
C. Modification Log.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tftime.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/svgeometry.inc"
#include "doit/duseq/svscratch.inc"

      INTEGER LVL_TYP,HIT_TYP,FIT_TYP

      LOGICAL LFLIP, LCURL

      INTEGER NLRMAX
      PARAMETER (NLRMAX = 2*KLYRDM+1)
      INTEGER IPLTRY(NLRMAX),WFTRY(NLRMAX)
      REAL    XTRY(NLRMAX),YTRY(NLRMAX),PHITRY(NLRMAX)
      REAL    ZTRY(NLRMAX),STRY(NLRMAX)
      REAL    COSTRY,SINTRY
      REAL    KAPRAD
      INTEGER NHWMAX,LLYR,MLYR,NTRY,ITRY,ILYR,IER
      INTEGER NHTWIR_RET,HTWADR_RET(NHITDM)
      REAL    EFF_RET
      INTEGER JTRY,MAMB,LHW,MHW,IHWD,IHW,IAMB,IA
      INTEGER IHIT,IADTMP,IFIT
      REAL D0,RRCURL,ARCFUN,ATN2PI,TCMAX,QMIN,QMAX

      REAL XLIST(8),YLIST(8)
      INTEGER WFLIST(8),NINLIST
      LOGICAL FILL_HTWADR

      integer ip

* ----------Executable code starts here---------------------

      CALL BEGCLK(KTIMES+ITHITS)

C store the input conditions for called subroutines

      SAV_LVL_TYP=LVL_TYP
      SAV_HIT_TYP=HIT_TYP
      SAV_FIT_TYP=FIT_TYP

C for now, always force full execution of ROADHITS and decendents
      
      UPTODATE_HIT=.FALSE.
      CALL VFILL(UPTODATE_LYR,KLYRDM,.FALSE.)

#if defined(CLEO_ROADHITSDIAG)
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
c temporary stuff to display the cathode geometry
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      if(LCLSTR_LAYER(ILYR))then
      print 9901
      do 101 ip=1,nlyrtf
      print 9902,ip,nwirtf(ip),nwrztf(ip),
     2          phiftf(ip),celltf(ip),indxtf(ip),
     4          SEARCH_LAYER(ILYR,SAV_HIT_TYP,SAV_FIT_TYP)
 101  continue

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C             12345678901234 23456 2345 2345678 2345678 234567 2345
 9901 format(' ROADHITS: layer  nwir wirZ  PHIFTF  CELLTF INDXTF Swid ')
 9902 format(I14,I6,I5,F8.3,F8.4,I7,I5)
      endif

c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif

C Find the first layer for the track
      LLYR = 1
      D0   = 2.0*ABS(GAMCF)/(1.0 + XICF)
 10   IF (RTF(LLYR).LE.D0 .AND. LLYR.LT.NLYRTF) THEN
         LLYR = LLYR + 1
         GOTO 10
      ENDIF

C Find the last layer for the track
      MLYR   = NLYRTF
      RRCURL = 0.5*(1.0 + XICF)/AMAX1(ABS(KAPCF),1.E-9)
      LCURL  = RRCURL.LT.RTF(MLYR)
 20   IF (RTF(MLYR).GE.RRCURL .AND. MLYR.GT.LLYR) THEN
         MLYR = MLYR - 1
         GOTO 20
      ENDIF

C TEMP !!!! Switch off using return half hits !!!!  (temp indeed)
      LCURL = .FALSE.

C zero out the pointers from hits to fit, and zero the fit status
      CALL HIT_TO_FIT(H2F_ZERO_POINTER,0,0)

C limit number of wires 
      NHWMAX = NHITDM/2

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C fill the arrays flagging which SV hybrids are used in the fit
C IADFIT <- IADRHT <- HTWADR_RET <- SV_HIT 
      SV_HYBRID_USE_VALID=.FALSE.
      IF(
     1     (SAV_LVL_TYP.EQ.USE_TFXFIT).AND.
     2     (SAV_FIT_TYP.EQ.USE_LF))THEN
         CALL VFILL(SV_HYBRID_USE,NUM_SV_HYBRIDS,.FALSE.)
         CALL VFILL(SV_HYBRID_IDENTIFY,NUM_SV_LAYERS,.FALSE.)
         SV_HYBRID_USE_VALID=.TRUE.
         IF(NFIT.GE.1)THEN
            DO 29 IFIT=1,NFIT
               ILYR=IPLFIT(IFIT)
               IF(ILYR.GT.0)THEN
                  IF(IDVCTF(ILYR).EQ.ISVX1)THEN
                     SV_HYBRID_IDENTIFY(SV_LAYER(IADFIT(IFIT)))=.TRUE.
                     SV_HYBRID_USE(SV_HYBRID(IADFIT(IFIT)))=.TRUE.
                  ENDIF
               ENDIF
 29         CONTINUE
         ENDIF
      ENDIF


C Set up expected locations of hits in layers

      NTRY=0
      CALL VZERO(ILLRLR,NLYRTF)

      IF(LLYR.LE.MLYR)THEN
         DO 39 ILYR = LLYR,MLYR

C require layer authorization

            IF (LUHTTF(ILYR).AND.
     2           LUSE_LAYER(ILYR,SAV_LVL_TYP,SAV_FIT_TYP))THEN

C this part for things with cylindrical geometry                  
C it is now common code for silicon

               NTRY=NTRY+1
               CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     2              RSQTF(ILYR),+1,
     3              XTRY(NTRY),YTRY(NTRY),PHITRY(NTRY),IER)
               IF(IER.EQ.0)THEN

                  STRY(NTRY)  = ARCFUN(KAPCF,ALPCF,BETCF,
     2                 XTRY(NTRY),YTRY(NTRY),XICF)
                  ZTRY(NTRY)  = Y0LF + TANDLF*STRY(NTRY)
                  IPLTRY(NTRY)=ILYR
                  WFTRY(NTRY)=-1
                  
C adjust TRY position for Z coordinate

                  COSTRY=XTRY(NTRY)/RTF(ILYR)
                  SINTRY=YTRY(NTRY)/RTF(ILYR)
                  XTRY(NTRY)=XTRY(NTRY)-ZTRY(NTRY)*TANSTF(ILYR)*SINTRY
                  YTRY(NTRY)=YTRY(NTRY)+ZTRY(NTRY)*TANSTF(ILYR)*COSTRY
                  PHITRY(NTRY)=PHITRY(NTRY)
     2                 +ZTRY(NTRY)*TANSTF(ILYR)/RTF(ILYR)

               ELSE
                  NTRY=NTRY-1
               ENDIF

#if defined(CLEO_ROADHITSDIAG)
                  print 8881,ilyr,ntry,tanstf(ilyr),rtf(ilyr),
     2                 phitry(ntry),ier
 8881             format(' ROADHITS: ilyr=',I3,' ntry=',I3,
     2                 ' tanstf=',f9.6,' rtf=',f6.3,
     3                 ' phitry=',F7.4,' IER=',I3) 
#endif

            ENDIF
 39      CONTINUE

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C find the addresses of hits that satisfy the expected locations

C zero the scalers for the TFHITS road hit structure

         NLYRHT=0
         NLYRLR=0
         NHIT=0

         NLRDRA=0
         NLRDRU=0
         NLRDRV=0

C zero the scalers for the TFHITS hit wire structure

         NLYRWR=0
         NHTWIR=0

C loop over the accepted layers

         DO 69 ITRY = 1,NTRY
            ILYR = IPLTRY(ITRY)
            EFFILR(ILYR) = 1.
            
C Find the hits in physical layer ilyr

            IF (NHIT .LT. NHITDM) THEN

C this part for silicon

               IF(
     1              (IDVCTF(ILYR).EQ.ISVX1).OR.
     2              (IDVCTF(ILYR).EQ.ISVZ1))THEN

                  CALL FINDHS(ILYR+0,PHITRY(ITRY),
     2                 XTRY(ITRY),YTRY(ITRY),ZTRY(ITRY),Y0LF,
     3                 HTWADR_RET,NHWMAX,NHTWIR_RET,EFF_RET)
                  EFFILR(ILYR) = EFF_RET
                  
C this part for things with cylindrical geometry                  

               ELSE
                  KAPRAD=KAPCF*RTF(ILYR)
                  CALL FINDHW(ILYR+0,PHITRY(ITRY),ZTRY(ITRY),KAPRAD,
     2                 HTWADR_RET,NHWMAX,NHTWIR_RET)

               ENDIF
               
               IF(NHTWIR_RET.GT.0) THEN
                  IF (LAMB_D_LAYER(ILYR)) THEN
                     MAMB   = 2
                     IF (TANSTF(ILYR) .GE. 0.0) THEN
                        LHW  = 1
                        MHW  = NHTWIR_RET
                        IHWD = 1
                     ELSE
                        LHW  = NHTWIR_RET
                        MHW  =  1
                        IHWD = -1
                     ENDIF
                  ELSE
                     MAMB = 1
                     LHW  = 1
                     MHW  = NHTWIR_RET
                     IHWD = 1
                  ENDIF
                  
                  NLYRHT = NLYRHT + 1
                  IPLRLR(NLYRHT) = ILYR
                  ILLRLR(ILYR)   = NLYRHT
                  LHITLR(NLYRHT) = NHIT + 1
                  SLYRLR(NLYRHT) = STRY(ITRY)

C fill in the TFHITS wire hit structure for the layer
C this will overwrite the result from SFHITS for a reCALL of FAST_TRAK

                  IF(LUSE_LAYER(ILYR,USE_PAT_REC,USE_CF))THEN
                     NLYRWR=NLYRWR+1
                     FILL_HTWADR=.TRUE.

                     IF(LSTEREOLAYER(ILYR))THEN
                        IF(TANSTF(ILYR).GT.0.)THEN
                           NLRDRU=NLRDRU+1
                        ELSE
                           NLRDRV=NLRDRV+1
                        ENDIF
                     ELSE
                        NLRDRA=NLRDRA+1
                     ENDIF
                     
                  ELSE
                     FILL_HTWADR=.FALSE.
                  ENDIF

C loop thru wires in road

                  DO 59 IHW = LHW,MHW,IHWD
                     IAMB =  2*MAMB-3

C loop thru possible ambiguities

                     DO 49 IA = 1,MAMB
                        NHIT = MIN0(NHIT+1,NHITDM)
                        IAMB = - IAMB
                        IADRHT(NHIT) = ISIGN(HTWADR_RET(IHW),IAMB)
                        IPLHIT(NHIT)=ILYR
                        OTHRHT(NHIT)=0

#if defined(CLEO_ROADHITSDIAG)
               print 8882,ilyr,nhit,iadrht(nhit),iplhit(nhit)
 8882          format(' ROADHITS: ilyr=',I3,10x,' nhit=',I3,
     2              ' iadrht=',I6,' iplhit=',I6) 
#endif

C end of loop over ambiguity
 49                  CONTINUE

C fill in the pointer to the other hit from the same wire
                     IF(
     1                    (MAMB.EQ.2).AND.
     2                    (IADRHT(NHIT).EQ.-IADRHT(NHIT-1))
     3                    )THEN
                        OTHRHT(NHIT)=NHIT-1
                        OTHRHT(NHIT-1)=NHIT
                     ENDIF

C fill in the TFHITS wire hit structure for the hit
C this will overwrite the result from SFHITS for a reCALL of FAST_TRAK

                     IF(FILL_HTWADR)THEN
                        NHTWIR=NHTWIR+1
                        HTWADR(NHTWIR)=HTWADR_RET(IHW)
                        HWSTAT(NHTWIR)=0
                     ENDIF

C end of loop over wire in road
 59               CONTINUE
                  
                  MHITLR(NLYRHT) = NHIT


C end of condition that hits were found in the layer
               ENDIF
            ENDIF
 69      CONTINUE
         
         CALL ROADHITS_COORD
      ENDIF

      CALL VFILL(LUSETF, KLYRTF, .FALSE.)

      CALL ENDCLK(KTIMES+ITHITS)
      RETURN
      END





