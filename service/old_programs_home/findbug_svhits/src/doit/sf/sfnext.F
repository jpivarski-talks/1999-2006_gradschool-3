*
* $Id: sfnext.F,v 1.9 2000/12/11 19:35:46 dpp Exp $
*
* $Log: sfnext.F,v $
* Revision 1.9  2000/12/11 19:35:46  dpp
*      -> diagnostics
*      -> moving towards F77
*      -> FIX ERROR: order dependence of chain preference
*
* Revision 1.8  2000/10/24 22:39:12  dpp
*      -> fix compile switches to allow SFDIAG without XTSUBS
*
* Revision 1.7  2000/10/02 20:51:10  dpp
*      -> new use of STAT words
*
* Revision 1.6  2000/08/18 19:08:59  dpp
*      -> zero the CS kinks; will copy to CF in fast_track_fit_wire_positn
*
* Revision 1.5  1999/05/14 21:32:42  dpp
* lower layer limit lost without diagsf
*
* Revision 1.4  1999/04/27 19:39:47  dpp
* remove reset of XTS_NEW_TRACK
*
* Revision 1.3  1999/01/29 14:48:40  dpp
* trivial
*
* Revision 1.2  1998/07/09 01:05:38  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/25 10/05/94  17.47.07  by  Anders Ryd
*CMZ :  6.00/14 01/03/94  16.50.28  by  Rob Kutschke
*CMZ :  6.00/07 31/08/93  14.04.50  by  Rob Kutschke
*CMZ :  6.00/01 05/05/93  16.50.42  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.29  by  Dan Peterson
*CMZ :  2.00/00 08/05/89  14.46.12  by  Dan Peterson
*-- Author :
#if !defined(CLEO_CLIBCOMP)
      SUBROUTINE SFNEXT
#endif
#if defined(CLEO_CLIBCOMP)
      SUBROUTINE SFNEXT(LCFIND)
#endif
C......................................................................
C.
C. SFNEXT - Select the next chain to send to DUFIND
C.
C. COMMON    : /STEPCn/ CURCS,PHI0CS,etc. in TFHITSCD
C. CALLS     :
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 27-Jul-88
C. LAST MOD  : 26-Sept-88
C.
C. Modification Log.
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C== Argument declarations

C LCFIND - .TRUE. if DUFIND was successful with previous chain
C        - not modified-
#if defined(CLEO_CLIBCOMP)
      LOGICAL LCFIND
#endif

C IERR  -   ERR IF NOT ZERO
      INTEGER IERR

C TFHITCD (AND TFINDPAR) NEEDED FOR PHI0CS etc. AND NHIT; ORIGINATED IN VDFIND
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cfctlcde.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"
#if defined(CLEO_XTSUBS)
C#include "doit/sfseq/xts_cntl.inc"
#endif

      CHARACTER*6 CRNAME
      PARAMETER (CRNAME='SFNEXT')
      INTEGER PICK,TOTLEN_PICK,STRTLYR_PICK,MAXDR
      INTEGER I,J,K
      INTEGER STRTLYR,STRTLYR_TEMP,W1,W1TEMP,TOTLEN,CDLAY
      INTEGER IADR,LYR,WIR,NHITPT,NHITVD,NHITDR,NHT1DR
      INTEGER ABORT
      INTEGER BETTER
      LOGICAL FORGETIT,PREVIOUS
      REAL F1
      REAL PI,TWOPI
      PARAMETER (PI=3.14159265)
      PARAMETER (TWOPI=2.*PI)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C-----------------------------------------------------------------------
C executable code starts here
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
      PRINT 1001,NCHAIN,NCHANM
1001  FORMAT(' SFNEXT: NCHAIN/NCHANM=',I3,'/',I3)
#endif

C-----------------------------------------------------------------------
C initialization, test if there are valid chains available
C-----------------------------------------------------------------------

      IERR=0

#if defined(CLEO_SFDIAG)
      CALL DSF_ABORT(ABORT)
      IF(ABORT.EQ.1)GO TO 195
#endif

      IF(NCHAIN.LE.0)GO TO 199
      IF(CNTLMG.EQ.2)THEN
         IF(NSUPCH.LE.0)GO TO 199
      ELSE

C-----------------------------------------------------------------------
C CoNTrol_MerGe=1; kill the previous best chain and all subchains
C this operates on SF variables only, according to the logical argument:LCFIND
C-----------------------------------------------------------------------

         IF(NCHANM.GT.0)THEN
            DO 5 I=1,NCHANM
 5          ICHANU(JCHAIN(I))=0
            IF(LCFIND)THEN
               DO 9 I=1,NCHANM
                  DO 7 J=1,MKILCH
                     K=KILCHN(J,JCHAIN(I))
                     IF(K.EQ.0)GO TO 9
 7                   ICHANU(K)=0
 9                CONTINUE
            ENDIF
         ENDIF
      ENDIF

C-----------------------------------------------------------------------
C find the new best chain; operates in SF varaibles only
C-----------------------------------------------------------------------

      LCFIND=.FALSE.

#if defined(CLEO_SFDIAG)
      CALL DSF_PREP
#endif

      PICK=0
      NCHANM=0
      TOTLEN_PICK=0
      STRTLYR_PICK=MXLSFU
      MAXDR=0

C-----------------------------------------------------------------------
C loop over the chains or superchains
C-----------------------------------------------------------------------

      I=0
21    I=I+1
      BETTER=0
      FORGETIT=.FALSE.
      PREVIOUS=.FALSE.

C-----------------------------------------------------------------------
C super chains, ie lists of simple chains that could be merged
C-----------------------------------------------------------------------

      IF(CNTLMG.EQ.2)THEN
C check for the end of loop and previously used chain
C require SUPCUS.eq.1 for usable, previously unprocessed chains
         IF(I.GT.NSUPCH)GO TO 97
         IF(SUPCUS(I).EQ.1)THEN

C now check if any subpart of this chain has been previously killed
C and find the minimum radius layer
C super chains, ie lists of simple chains that could be merged

            STRTLYR=NLAYSF
            TOTLEN=0
            J=0
 23         J=J+1
            IF((J.LE.SUPCLN(I)).AND.(.NOT.FORGETIT))THEN
               K=SUPCEL(SUPCAD(I)-1+J)
               IF(K.GT.0)THEN
                  IF(ICHANU(K).GT.0)THEN
                     STRTLYR_TEMP=SFLSFA(LINCHN(K))
                     W1TEMP=SFWSFA(LINCHN(K))
                     IF(STRTLYR_TEMP.LT.STRTLYR)THEN
                        STRTLYR=STRTLYR_TEMP
                        W1=W1TEMP
                     ENDIF
                     TOTLEN=TOTLEN+LENCHN(K)
                     
                  ELSE
#if defined(CLEO_SFDIAG)
                     PRINT 2006,I,K,ICHANU(K)
 2006                FORMAT(' SFNEXT: DISREGARD SUPER CHAIN->',I2,
     1                    ' ..ICHANU(',I2,')=',I3,' SUBCHAIN')
#endif

                     FORGETIT=.TRUE.
                  ENDIF
                  GO TO 23
               ENDIF
            ENDIF

         ELSE   
            PREVIOUS=.TRUE.

#if defined(CLEO_SFDIAG)
            PRINT 2002,I,I,SUPCUS(I)
 2002       FORMAT(' SFNEXT: DISREGARD SUPER CHAIN->',I2,
     1           ' ..SUPCUS(',I2,')=',I3,' PREVIOUS')
#endif
         ENDIF

C-----------------------------------------------------------------------
C simple chains and fully merged chains
C-----------------------------------------------------------------------

      ELSE
C check for the end of loop and previously used chain
C require ICHANU.GT.1 for usable chains
         IF(I.GT.NCHAIN)GO TO 97
         IF(ICHANU(I).GT.1)THEN

C now check if any subpart of this chain has been previously killed
C and find the minimum radius layer and total length
C simple chains and fully merged chains
            J=0
 33         J=J+1
            IF((J.LE.MKILCH).AND.(.NOT.FORGETIT))THEN
               K=KILCHN(J,I)
               IF(K.GT.0)THEN
                  IF(ICHANU(K).GT.0)THEN

                  ELSE
#if defined(CLEO_SFDIAG)
                     PRINT 2005,I,K,ICHANU(K)
 2005                FORMAT(' SFNEXT: DISREGARD CHAIN->',I2,
     1                    ' ..ICHANU(',I2,')=',I3,' SUBCHAIN')
#endif
                     FORGETIT=.TRUE.
                  ENDIF
                  GO TO 33
               ENDIF
            ENDIF

            IF((.NOT.FORGETIT))THEN

               STRTLYR=SFLSFA(LINCHN(I))
               W1=SFWSFA(LINCHN(I))
               TOTLEN=LENCHN(I)
            ENDIF

         ELSE
            PREVIOUS=.TRUE.

#if defined(CLEO_SFDIAG)
            PRINT 2001,I,I,ICHANU(I)
 2001       FORMAT(' SFNEXT: DISREGARD CHAIN->',I2,
     1           ' ..ICHANU(',I2,')=',I3,' PREVIOUS')
#endif
         ENDIF
      ENDIF


C-----------------------------------------------------------------------
C skip any previously used or removed chains
C-----------------------------------------------------------------------
      IF(.NOT.PREVIOUS)THEN

C-----------------------------------------------------------------------
C require minimum starting radius for most applications
C-----------------------------------------------------------------------

         IF(CNTLSF.NE.5)THEN
            IF(.NOT.FORGETIT)THEN
               IF(STRTLYR.GT.MXLSFU)THEN

#if defined(CLEO_SFDIAG)
                  PRINT 2007,I,STRTLYR
 2007             FORMAT(' SFNEXT: DISREGARD CHAIN->',I2,
     2                 ' ..LOWER LAYER=',I2)
#endif
                  
                  FORGETIT=.TRUE.
               ENDIF
            ENDIF
         ENDIF
      

C-----------------------------------------------------------------------
C chain selection beam wall
C-----------------------------------------------------------------------

         IF(CNTLSF.EQ.5)THEN
C require minimum number of hits
C start by grabbing the first chain
C prefer chains that are long
C prefer chains that start at low radius
C again prefer chains that are long
C again prefer chains that start at low radius
            IF(.NOT.FORGETIT)THEN
               IF(TOTLEN.LT.6)FORGETIT=.TRUE.
            ENDIF

            IF(.NOT.FORGETIT)THEN
               IF((BETTER.EQ.0).AND.(TOTLEN_PICK.EQ.0))
     1              BETTER=1
               IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).GT. 2))
     1              BETTER=1
               IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).LT.-2))
     1              BETTER=-1
               IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).LT.-3))
     1              BETTER=1
               IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).GT. 3))
     1              BETTER=-1
               IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).GT.0))
     1              BETTER=1
               IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).LT.0))
     1              BETTER=1
            ENDIF
         


C-----------------------------------------------------------------------
C chain selection for DUET, DPSF and applications except beam wall
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C require minimum number of hits in each detector element
C calculate the number of hits in the separate detector elements
C add togetther all subparts for super chains
C-----------------------------------------------------------------------

         ELSE
            IF(.NOT.FORGETIT)THEN
               NHITPT=0
               NHITVD=0
               NHITDR=0
               NHT1DR=0

C set chain number for simple and fully merged chains

               IF(CNTLMG.EQ.2)GO TO 60
               K=I
               GO TO 62

C loop over simple chains for super chains

 60            J=0
 61            J=J+1
               IF(J.GT.SUPCLN(I))GO TO 69
               K=SUPCEL(SUPCAD(I)-1+J)

C common loop over hits in each chain

 62            IADR=LINCHN(K)
 63            IF(IADR.EQ.0)GO TO 67
               IF(INUCHN(IADR).GT.0)THEN
                  LYR=SFLSFA(IADR)
                  WIR=SFWSFA(IADR)
                  IF(LYR.LE.MLSFPT)THEN
                     NHITPT=NHITPT+1
                  ELSEIF(LYR.LE.MLSFVD)THEN
                     NHITVD=NHITVD+1
                  ELSEIF(LYR.LE.MLSFDR)THEN
                     NHITDR=NHITDR+1
                     IF(DOUBPT(IADR).EQ.0)NHT1DR=NHT1DR+1
                  ENDIF
               ENDIF
               IF(IADR.EQ.MINCHN(K))GO TO 67
               IADR=ISTFOR(IADR)
               GO TO 63
 67            IF(CNTLMG.EQ.2)GO TO 61
 69            CONTINUE


C-----------------------------------------------------------------------
C require minimum number of hits in the sub-devices
C-----------------------------------------------------------------------

               IF(NHITPT.LT.MNULPT)FORGETIT=.TRUE.
               IF(NHITVD.LT.MNULVD)FORGETIT=.TRUE.
               IF(NHITDR.LT.MNULDR)FORGETIT=.TRUE.
            ENDIF

C-----------------------------------------------------------------------
C order of preference
C-----------------------------------------------------------------------

C for CNTLSF.EQ.2 (CLEO3) prefer chains that are long in the DR
C (count only single hits in the DR)

            IF(.NOT.FORGETIT)THEN
               IF(CNTLSF.EQ.2)THEN
                  IF((BETTER.EQ.0).AND.((NHT1DR-MAXDR).GT.0))
     1                 BETTER=1
               ELSE

C prefer chains that start at low radius
C prefer chains that are long
C again prefer chains that start at low radius
C again prefer chains that are long

                  IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).LT.-10))
     1                 BETTER=1
                  IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).GT. 10))
     1                 BETTER=-1
                  IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).GT. 10))
     1                 BETTER=1
                  IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).LT.-10))
     1                 BETTER=-1
                  IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).LT.0))
     1                 BETTER=1
                  IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).GT.0))
     1                 BETTER=1
               ENDIF
            ENDIF
         ENDIF

C-----------------------------------------------------------------------
C resume common code for chain selection
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C save this as the current best chain
C-----------------------------------------------------------------------
         IF((.NOT.FORGETIT).AND.(BETTER.EQ.1))THEN
         
#if defined(CLEO_SFDIAG)
            PRINT 2008,
     1           I,   STRTLYR,     TOTLEN,
     2           PICK,STRTLYR_PICK,TOTLEN_PICK
 2008       FORMAT(' SFNEXT:  new best CHAIN->',I2,
     1           ' START=',I2,'  LEN=',I3,
     2           '   replaces->',I2,' START=',I2,'  LEN=',I3)
#endif

            PICK=I
            STRTLYR_PICK=STRTLYR
            CDLAY=CDLASF(STRTLYR_PICK)
            IF(CDLAY.LT.0)THEN
               F1=SFSVF0(-CDLAY)+TWOPI/SFSVCL(-CDLAY)*W1
            ELSE
               F1=PHIFCD(CDLAY)+CELLCD(CDLAY)*W1
            ENDIF
            TOTLEN_PICK=TOTLEN
            MAXDR=NHT1DR

            IF(CNTLMG.EQ.2)THEN
               J=0
 83            J=J+1
               IF(J.LE.SUPCLN(I))THEN
                  JCHAIN(J)=SUPCEL(SUPCAD(I)-1+J)
                  GO TO 83
               ENDIF
               JCHAIN(J)=0
               NCHANM=SUPCLN(I)

            ELSE
               JCHAIN(1)=I
               JCHAIN(2)=0
               NCHANM=1
            ENDIF
            
         ENDIF
      
C-----------------------------------------------------------------------
C mark this chain so it won't be considered again
C-----------------------------------------------------------------------
         IF(FORGETIT)THEN
            IF(CNTLMG.EQ.2)THEN
               SUPCUS(I)=0
            ELSE
               ICHANU(I)=1
            ENDIF
         ENDIF
      
C end loop over chain or super chain
      ENDIF
      GO TO 21
97    CONTINUE

C-----------------------------------------------------------------------
C best chain is now in  JCHAIN()
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
      IF(CNTLMG.EQ.2)THEN
         PRINT 2010,PICK
         PRINT 2011,(JCHAIN(J),ICHANU(JCHAIN(J)),J=1,NCHANM)
         PRINT 2012,STRTLYR_PICK,TOTLEN_PICK
 2010    FORMAT(' SFNEXT: FINALPICK SUPER CHAIN->',I2)
 2011    FORMAT(34X,'..ICHANU(',I2,')=',I3)
 2012    FORMAT(34X,'................ START=',I2,' LEN=',I3)

      ELSE
         PRINT 2009,PICK,PICK,ICHANU(PICK),
     2        STRTLYR_PICK,TOTLEN_PICK
 2009    FORMAT(' SFNEXT: FINALPICK CHAIN->',I2,
     1        ' ..ICHANU(',I2,')=',I3,' START=',I2,' LEN=',I3)
      ENDIF
#endif

C send information to output routine

      IF(NCHANM.EQ.0)GO TO 179
      IF(CNTLMG.EQ.2)SUPCUS(PICK)=2
      LCFIND=.TRUE.


C-----------------------------------------------------------------------
C output variables for DUET, DPSF and most applications 
C-----------------------------------------------------------------------


#if defined(CLEO_SFDUET)
C zero the seed parameters, including D0CF
C zero the seed parameters: STATCS(1:n)
C this also zeros the kinks
      CALL VZERO( KAPCS , NPARTF )
      PHI0CS=F1
      IF(PHI0CS.LT.0    )PHI0CS=PHI0CS+TWOPI
      IF(PHI0CS.GE.TWOPI)PHI0CS=PHI0CS-TWOPI
      CURCS    = .00001
      CALL CDKABG(CURCS,PHI0CS,D0CS,KAPCS,ALPCS,BETCS,GAMCS,XICS)

      PHI6CS=0.
      RKN6CS=9.
      SKN6CS=0.
      KNK6CS=0
      FTK6CS=0
      PHI7CS=0.
      RKN7CS=9.
      SKN7CS=0.
      KNK7CS=0
      FTK7CS=0


#if !defined(CLEO_SFZFIT)
      Y0LS     = 0.
      TANDLS   = 0.
#endif
#if defined(CLEO_SFZFIT)
      Y0LS     = ZR0CHN(JCHAIN(1))/(1.*MULST)
      TANDLS   = ZDRCHN(JCHAIN(1))/(1.*MULST)
#endif

C load the merging control
      CALL SBYT(CNTLSF,STATCS(1),7,2)
      CALL SBYT(PICK,STATCS(1),10,8)
      IF(CNTLSF.EQ.2)THEN
C super chains
         IF(NCHANM.NE.1)CALL SBYT(1,STATCS(1),9,1)
      ELSE
C merged chains
         IF(KILCHN(2,PICK).NE.0)CALL SBYT(1,STATCS(1),9,1)
      ENDIF
#endif



C-----------------------------------------------------------------------
C output variables for SFBWAL
C-----------------------------------------------------------------------

      IF(CNTLSF.EQ.5)THEN
      ENDIF
C-----------------------------------------------------------------------
C resume common code for output
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
C display the chosen chain
C      CALL DSF_CHVIEW1(JCHAIN(1))
      CALL DSF_CHVIEW(JCHAIN)

      CALL DSF_ABORT(ABORT)
      IF(ABORT.EQ.1)GO TO 195

#if defined(CLEO_COSMIC)
      CALL SFCOSF(PICK,IERR)
#endif

#endif

      GO TO 191

C branch point when no good chain is found

179   CONTINUE

#if defined(CLEO_SFDIAG)
C delete the last chosen chain
      CALL DSF_CHVIEW1(0)
#endif

C normal end

191   CONTINUE

#if defined(CLEO_SFDIAG)
      CALL DSF_LEAV(CRNAME)
      GO TO 199

C abort signal
195   IERR=0
      NCHANM=0
#endif


199   CONTINUE
      RETURN
      END







