*
* $Id: sfnext.F,v 1.11 2003/03/05 18:30:41 dpp Exp $
*
* $Log: sfnext.F,v $
* Revision 1.11  2003/03/05 18:30:41  dpp
*      -> diagnostics
*      -> F77
*      -> use intermediate chain storage for multiple z slices
*      -> remove (null) cuts on number of hits in individual devices in SFNEXT
*
* Revision 1.10  2001/11/20 00:04:26  dpp
*      -> remove silly option of return variable for older version of CLEO2
*      -> note: previously, killed the subchains if LCFIND is .TRUE.
*      -> add argument to comumicate number of chains found
*      -> use status bit encapsulation
*
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
      SUBROUTINE SFNEXT(SFNEXT_CONTROL,LMERGED)
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
C SFNEXT_CONTROL..output number of chains found,
C                 input number of chains to flag used
      INTEGER SFNEXT_CONTROL
C LMERGED.........=.TRUE. if output chain is a merged chain
      LOGICAL LMERGED
 
C TFHITCD (AND TFINDPAR) NEEDED FOR PHI0CS etc. AND NHIT; ORIGINATED IN VDFIND
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/statusbits.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"


      CHARACTER*6 CRNAME
      PARAMETER (CRNAME='SFNEXT')

      INTEGER PICK,TOTLEN_PICK,STRTLYR_PICK,NSINGLE_SAV
      INTEGER ICHN_LOOP,ICHN_TEST
      INTEGER J,K
      INTEGER STRTADR,STRTADR_TEMP
      INTEGER STRTLYR,STRTLYR_TEMP
      INTEGER STRTWIR,STRTWIR_TEMP
      INTEGER TOTLEN,CDLAY
      INTEGER IADR,IADR2,ListLoc
      INTEGER LYR1,LYR2
      INTEGER NSINGLE
      INTEGER ABORT
      INTEGER BETTER
      INTEGER IERR,IGO


      LOGICAL USE_INTERMEDIATE_LIST
      LOGICAL USE_SUPER_CHAINS
      LOGICAL FORGETIT
      LOGICAL PREVIOUS
      LOGICAL STILL_OK
      LOGICAL CHAIN_LOOP

      LOGICAL KILL_PREV_CHAIN/.TRUE./

      REAL F1
      REAL PI,TWOPI
      PARAMETER (PI=3.14159265)
      PARAMETER (TWOPI=2.*PI)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C-----------------------------------------------------------------------
C executable code starts here
C-----------------------------------------------------------------------

      USE_INTERMEDIATE_LIST=(SFZSLNWINDO.GT.1)
C                                                .AND.(.FALSE.)
      USE_SUPER_CHAINS=(CNTLMG.EQ.2)


#if defined(CLEO_SFDIAG)
      PRINT 1001,SFChanList_Num,NCHAIN,NCHANM
1001  FORMAT(' SFNEXT: SFChanList_Num/NCHAIN/NCHANM=',I5,'/'I3,'/',I3)
#endif

C-----------------------------------------------------------------------
C initialization, test if there are valid chains available
C-----------------------------------------------------------------------

      IERR=0
      
#if defined(CLEO_SFDIAG)
      ABORT=0
      CALL DSF_ABORT(ABORT)
      IF(ABORT.EQ.1)GO TO 199
#endif

      IF(USE_INTERMEDIATE_LIST)THEN
        STILL_OK=(SFChanList_Num.GT.0)
      ELSEIF(USE_SUPER_CHAINS)THEN
        STILL_OK=(NCHAIN.GT.0).AND.(NSUPCH.GT.0)
      ELSE
        STILL_OK=(NCHAIN.GT.0)
      ENDIF
          
C-----------------------------------------------------------------------
C kill the previous best chain
C kill all subchains if a good track was found
C this operates on SF variables only
C-----------------------------------------------------------------------

      IF(STILL_OK)THEN
        IF((NCHANM.GT.0).and.(KILL_PREV_CHAIN))THEN
          
          IF(USE_INTERMEDIATE_LIST)THEN
            
            ICHN_LOOP=0
 12         ICHN_LOOP=ICHN_LOOP+1
            IF(ICHN_LOOP.LE.NCHANM)THEN
              SFChanList_ChnUse(JCHAIN(ICHN_LOOP))=0
              GO TO 12
            ENDIF
            ICHN_LOOP=0
 13         ICHN_LOOP=ICHN_LOOP+1
            IF(ICHN_LOOP.LE.SFNEXT_CONTROL)THEN
              J=0
 14           J=J+1
              IF(J.LE.MKILCH)THEN
                K=SFChanList_KilChn(J,JCHAIN(ICHN_LOOP))
                IF(K.NE.0)THEN
                  SFChanList_ChnUse(K)=0
                  GO TO 14
                ENDIF
              ENDIF
              GO TO 13
            ENDIF
            
          ELSEIF(USE_SUPER_CHAINS)THEN
            
          ELSE
            
            ICHN_LOOP=0
 15         ICHN_LOOP=ICHN_LOOP+1
            IF(ICHN_LOOP.LE.NCHANM)THEN
              ICHANU(JCHAIN(ICHN_LOOP))=0
              GO TO 15
            ENDIF
            ICHN_LOOP=0
 16         ICHN_LOOP=ICHN_LOOP+1
            IF(ICHN_LOOP.LE.SFNEXT_CONTROL)THEN
              J=0
 17           J=J+1
              IF(J.LE.MKILCH)THEN
                K=KILCHN(J,JCHAIN(ICHN_LOOP))
                IF(K.NE.0)THEN
                  ICHANU(K)=0
                  GO TO 17
                ENDIF
              ENDIF
              GO TO 16
            ENDIF
            
          ENDIF
        ENDIF
      ENDIF
      IF(STILL_OK)THEN

C-----------------------------------------------------------------------
C find the new best chain; operates in SF varaibles only
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
        CALL DSF_PREP
#endif
        
        PICK=0
        NCHANM=0
        TOTLEN_PICK=0
        STRTLYR_PICK=MXLSFU
        NSINGLE_SAV=0
        
C-----------------------------------------------------------------------
C loop over the chains or superchains
C-----------------------------------------------------------------------
        
        ICHN_TEST=0
 21     ICHN_TEST=ICHN_TEST+1

C-----------------------------------------------------------------------
C check for the end of loop 
C-----------------------------------------------------------------------

        IF(USE_INTERMEDIATE_LIST)THEN
          CHAIN_LOOP=(ICHN_TEST.LE.SFChanList_Num)
        ELSEIF(USE_SUPER_CHAINS)THEN
          CHAIN_LOOP=(ICHN_TEST.LE.NSUPCH)
        ELSE
          CHAIN_LOOP=(ICHN_TEST.LE.NCHAIN) 
        ENDIF

        IF(CHAIN_LOOP)THEN

          BETTER=0
          FORGETIT=.FALSE.
          PREVIOUS=.FALSE.
          
            
C-----------------------------------------------------------------------
C chains from intermediate storage
C-----------------------------------------------------------------------

          IF(USE_INTERMEDIATE_LIST)THEN
            
C check for previously used chain
C require SFChanList_ChnUse.GT.1 for usable chains
            
            IF(SFChanList_ChnUse(ICHN_TEST).GT.1)THEN
              
C now check if any subpart of this chain has been previously killed
C and find the minimum radius layer and total length
              
              J=0
 31           J=J+1
              IF((J.LE.MKILCH).AND.(.NOT.FORGETIT))THEN
                K=SFChanList_KilChn(J,ICHN_TEST)
                IF(K.GT.0)THEN
                  IF(SFChanList_ChnUse(K).GT.0)THEN
                    
                  ELSE
#if defined(CLEO_SFDIAG)
                    PRINT 2035,ICHN_TEST,K,SFChanList_ChnUse(K)
 2035               FORMAT(' SFNEXT: DISREGARD CHAIN->',I5,
     1                   ' ..SFChanList_ChnUse(',I5,')=',I3,
     2                   ' SUBCHAIN')
#endif
                    FORGETIT=.TRUE.
                  ENDIF
                  GO TO 31
                ENDIF
              ENDIF
              
              IF((.NOT.FORGETIT))THEN
                ListLoc=SFChanList_1stLoc(ICHN_TEST)
                STRTADR=SFChanList_SFid(ListLoc)
                STRTLYR=SFLSFA(STRTADR)
                STRTWIR=SFWSFA(STRTADR)
                TOTLEN=SFChanList_Length(ICHN_TEST)
              ENDIF
              
            ELSE
              PREVIOUS=.TRUE.
              
#if defined(CLEO_SFDIAG)
              PRINT 2031,ICHN_TEST,SFChanList_ChnUse(ICHN_TEST)
 2031         FORMAT(' SFNEXT: DISREGARD CHAIN->',I5,
     1             ' ..SFChanList_ChnUse()=',I3,
     2             ' PREVIOUS')
#endif
            ENDIF

C-----------------------------------------------------------------------
C super chains, ie lists of simple chains that could be merged
C-----------------------------------------------------------------------
          
          ELSEIF(USE_SUPER_CHAINS)THEN
            
C check for previously used chain
C require SUPCUS.eq.1 for usable, previously unprocessed chains
            
            IF(SUPCUS(ICHN_TEST).EQ.1)THEN
              
C now check if any subpart of this chain has been previously killed
C and find the minimum radius layer
              
              STRTLYR=NLAYSF
              TOTLEN=0
              J=0
 41           J=J+1
              IF((J.LE.SUPCLN(ICHN_TEST)).AND.(.NOT.FORGETIT))THEN
                K=SUPCEL(SUPCAD(ICHN_TEST)-1+J)
                IF(K.GT.0)THEN
                  IF(ICHANU(K).GT.0)THEN
                    STRTADR_TEMP=LINCHN(K)
                    STRTLYR_TEMP=SFLSFA(STRTADR_TEMP)
                    STRTWIR_TEMP=SFWSFA(STRTADR_TEMP)
                    IF(STRTLYR_TEMP.LT.STRTLYR)THEN
                      STRTADR=STRTADR_TEMP
                      STRTLYR=STRTLYR_TEMP
                      STRTWIR=STRTWIR_TEMP
                    ENDIF
                    TOTLEN=TOTLEN+LENCHN(K)
                    
                  ELSE
#if defined(CLEO_SFDIAG)
                    PRINT 2045,ICHN_TEST,K,ICHANU(K)
 2045               FORMAT(' SFNEXT: DISREGARD SUPER CHAIN->',I2,
     1                   ' ..ICHANU(',I2,')=',I3,' SUBCHAIN')
#endif
                    
                    FORGETIT=.TRUE.
                  ENDIF
                  GO TO 41
                ENDIF
              ENDIF
              
            ELSE   
              PREVIOUS=.TRUE.
              
#if defined(CLEO_SFDIAG)
              PRINT 2041,ICHN_TEST,SUPCUS(ICHN_TEST)
 2041         FORMAT(' SFNEXT: DISREGARD SUPER CHAIN->',I5,
     1             ' ..SUPCUS()=',I3,' PREVIOUS')
#endif
            ENDIF
            
C-----------------------------------------------------------------------
C simple chains and fully merged chains
C-----------------------------------------------------------------------
            
          ELSE
            
C check for previously used chain
C require ICHANU.GT.1 for usable chains
            
            IF(ICHANU(ICHN_TEST).GT.1)THEN
              
C now check if any subpart of this chain has been previously killed
C and find the minimum radius layer and total length
              
              J=0
 51           J=J+1
              IF((J.LE.MKILCH).AND.(.NOT.FORGETIT))THEN
                K=KILCHN(J,ICHN_TEST)
                IF(K.GT.0)THEN
                  IF(ICHANU(K).GT.0)THEN
                    
                  ELSE
#if defined(CLEO_SFDIAG)
                    PRINT 2055,ICHN_TEST,K,ICHANU(K)
 2055               FORMAT(' SFNEXT: DISREGARD CHAIN->',I3,
     1                   ' ..ICHANU(',I2,')=',I3,' SUBCHAIN')
#endif
                    FORGETIT=.TRUE.
                  ENDIF
                  GO TO 51
                ENDIF
              ENDIF
              
              IF((.NOT.FORGETIT))THEN
                STRTADR=LINCHN(ICHN_TEST)
                STRTLYR=SFLSFA(STRTADR)
                STRTWIR=SFWSFA(STRTADR)
                TOTLEN=LENCHN(ICHN_TEST)
              ENDIF
              
            ELSE
              PREVIOUS=.TRUE.
              
#if defined(CLEO_SFDIAG)
              PRINT 2051,ICHN_TEST,ICHANU(ICHN_TEST)
 2051         FORMAT(' SFNEXT: DISREGARD CHAIN->',I5,
     1             ' ..ICHANU()=',I3,' PREVIOUS')
#endif
            ENDIF
            
C-----------------------------------------------------------------------
C end of condition if (USE_INTERMEDIATE_LIST) elseif (USE_SUPER_CHAINS) else
C-----------------------------------------------------------------------
            
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
                  PRINT 2007,ICHN_TEST,STRTLYR
 2007             FORMAT(' SFNEXT: DISREGARD CHAIN->',I5,
     2                 ' ..LOWER LAYER=',I3)
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
     1               BETTER=1
                IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).GT. 2))
     1               BETTER=1
                IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).LT.-2))
     1               BETTER=-1
                IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).LT.-3))
     1               BETTER=1
                IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).GT. 3))
     1               BETTER=-1
                IF((BETTER.EQ.0).AND.((TOTLEN-TOTLEN_PICK).GT.0))
     1               BETTER=1
                IF((BETTER.EQ.0).AND.((STRTLYR-STRTLYR_PICK).LT.0))
     1               BETTER=1
              ENDIF
              
                            
C-----------------------------------------------------------------------
C chain selection for DOIT, DPSF and applications except beam wall
C-----------------------------------------------------------------------

            ELSE
              
C-----------------------------------------------------------------------
C calculate the number of single hits
C add togetther all subparts for super chains
C-----------------------------------------------------------------------
              
              IF(.NOT.FORGETIT)THEN
                NSINGLE=0

                IF(USE_INTERMEDIATE_LIST)THEN

                  ListLoc=SFChanList_1stLoc(ICHN_TEST)
 61               CONTINUE
                  IADR=SFChanList_SFid(ListLoc)

                  IF(IADR.GT.0)THEN
                    LYR1=SFLSFA(IADR)
                    IF(ListLoc.LT.MHITSFLIST)THEN
                      IADR2=SFChanList_SFid(ListLoc+1)
                      LYR2=SFLSFA(IADR2)
                      IF(LYR2.NE.LYR1)THEN
                        NSINGLE=NSINGLE+1
                      ELSE
                        ListLoc=ListLoc+1
                      ENDIF
                    ELSE
                      NSINGLE=NSINGLE+1
                    ENDIF

                    IF(ListLoc.LT.MHITSFLIST)THEN
                      ListLoc=ListLoc+1
                      GO TO 61
                    ENDIF
                  ENDIF

                ELSEIF(USE_SUPER_CHAINS)THEN

                  J=0
 65               J=J+1
                  IF(J.LE.SUPCLN(ICHN_TEST))THEN
                    K=SUPCEL(SUPCAD(ICHN_TEST)-1+J)
                    IADR=LINCHN(K)
 66                 CONTINUE
                    IF(IADR.GT.0)THEN
                      IF(INUCHN(IADR).GT.0)THEN
                        IF(DOUBPT(IADR).EQ.0)NSINGLE=NSINGLE+1
                      ENDIF
                      IF(IADR.NE.MINCHN(K))THEN
                        IADR=ISTFOR(IADR)
                        GO TO 66
                      ENDIF
                    ENDIF
                    GO TO 65
                  ENDIF
                  
                ELSE

                  IADR=LINCHN(ICHN_TEST)
 67               CONTINUE
                  IF(IADR.GT.0)THEN
                    IF(INUCHN(IADR).GT.0)THEN
                      IF(DOUBPT(IADR).EQ.0)NSINGLE=NSINGLE+1
                    ENDIF
                    IF(IADR.NE.MINCHN(ICHN_TEST))THEN
                      IADR=ISTFOR(IADR)
                      GO TO 67
                    ENDIF
                  ENDIF

                ENDIF
              ENDIF
              
C-----------------------------------------------------------------------
C order of preference
C-----------------------------------------------------------------------
              
C for CNTLSF.EQ.2 (CLEO3) prefer chains that are long in the DR
C (count only single hits in the DR)
              
              IF(.NOT.FORGETIT)THEN
                IF(CNTLSF.EQ.2)THEN
                  IF((BETTER.EQ.0).AND.((NSINGLE-NSINGLE_SAV).GT.0))
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
              
C end of condition (or not) (CNTLSF.EQ.5)
            ENDIF
            
C-----------------------------------------------------------------------
C resume common code for chain selection
C-----------------------------------------------------------------------
            
C-----------------------------------------------------------------------
C save this as the current best chain
C-----------------------------------------------------------------------
            IF(.NOT.FORGETIT)THEN
              IF(BETTER.EQ.1)THEN
                
#if defined(CLEO_SFDIAG)
                PRINT 2008,
     1               ICHN_TEST,   STRTLYR,     TOTLEN,
     2               PICK,STRTLYR_PICK,TOTLEN_PICK
 2008           FORMAT(' SFNEXT:  new best CHAIN->',I5,
     1               ' START=',I3,'  LEN=',I3,
     2               '   replaces->',I5,' START=',I3,'  LEN=',I3)
#endif
                
                PICK=ICHN_TEST
                STRTLYR_PICK=STRTLYR
                CDLAY=CDLASF(STRTLYR_PICK)
                IF(CDLAY.LT.0)THEN
                  F1=SFSVF0(-CDLAY)+TWOPI/SFSVCL(-CDLAY)*STRTWIR
                ELSE
                  F1=PHIFCD(CDLAY)+CELLCD(CDLAY)*STRTWIR
                ENDIF
                TOTLEN_PICK=TOTLEN
                NSINGLE_SAV=NSINGLE
                
                IF(USE_SUPER_CHAINS)THEN
                  J=0
 83               J=J+1
                  IF(J.LE.SUPCLN(ICHN_TEST))THEN
                    JCHAIN(J)=SUPCEL(SUPCAD(ICHN_TEST)-1+J)
                    GO TO 83
                  ENDIF
                  JCHAIN(J)=0
                  NCHANM=SUPCLN(ICHN_TEST)
                  
                ELSE
                  JCHAIN(1)=ICHN_TEST
                  JCHAIN(2)=0
                  NCHANM=1
                ENDIF
                
              ELSE
                
#if defined(CLEO_SFDIAG)
                PRINT 2013,
     1               ICHN_TEST,   STRTLYR,     TOTLEN
 2013           FORMAT(' SFNEXT:notselectedCHAIN->',I5,
     1               ' START=',I3,'  LEN=',I3)
#endif
                
              ENDIF
              
            ENDIF
            
C-----------------------------------------------------------------------
C mark this chain so it won't be considered again
C-----------------------------------------------------------------------
            IF(FORGETIT)THEN

              IF(USE_INTERMEDIATE_LIST)THEN
                SFChanList_ChnUse(ICHN_TEST)=1
              ELSEIF(USE_SUPER_CHAINS)THEN
                SUPCUS(ICHN_TEST)=0
              ELSE
                ICHANU(ICHN_TEST)=1
              ENDIF
            ENDIF
            
C end loop over chain or super chain
          ENDIF
          GO TO 21
        ENDIF
       
C-----------------------------------------------------------------------
C best chain is now in  JCHAIN()
C-----------------------------------------------------------------------
       
#if defined(CLEO_SFDIAG)
        IF(USE_INTERMEDIATE_LIST)THEN
          PRINT 2014,PICK,PICK,SFChanList_ChnUse(PICK),
     2         STRTLYR_PICK,TOTLEN_PICK
 2014     FORMAT(' SFNEXT: FINALPICK intermediate CHAIN->',I5,
     1         ' .._ChnUse(',I5,')=',I3,' START=',I3,' LEN=',I3)
        ELSEIF(USE_SUPER_CHAINS)THEN
          PRINT 2010,PICK
          PRINT 2011,(JCHAIN(J),ICHANU(JCHAIN(J)),J=1,NCHANM)
          PRINT 2012,STRTLYR_PICK,TOTLEN_PICK
 2010     FORMAT(' SFNEXT: FINALPICK SUPER CHAIN->',I2)
 2011     FORMAT(34X,'..ICHANU(',I2,')=',I3)
 2012     FORMAT(34X,'................ START=',I2,' LEN=',I3)
          
        ELSE
          PRINT 2009,PICK,PICK,ICHANU(PICK),
     2         STRTLYR_PICK,TOTLEN_PICK
 2009     FORMAT(' SFNEXT: FINALPICK CHAIN->',I5,
     1         ' ..ICHANU(',I2,')=',I3,' START=',I3,' LEN=',I3)
        ENDIF
#endif
        
C-----------------------------------------------------------------------
C send information to output routine
C-----------------------------------------------------------------------
        
        IF(NCHANM.NE.0)THEN
          IF(USE_SUPER_CHAINS)SUPCUS(PICK)=2
          
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
          TANDLS   = 0.
#endif
#if defined(CLEO_SFZFIT)
          IF(USE_INTERMEDIATE_LIST)THEN
            Y0LS     = 0.
            SFZSLCNTR= SFChanList_Iwindo(PICK)*SFZSLGRANU
            TANDLS   = SFZSLCNTR
          ELSE
            Y0LS     = ZR0CHN(JCHAIN(1))/(1.*MULST)
            TANDLS   = ZDRCHN(JCHAIN(1))/(1.*MULST)
          ENDIF
#endif
          
C NOTE: THIS IS ALL WRONG, IT SHOULD BE TESTING ON , and loading, CNTLMG

C load the merging control
          CALL STATUSBITS(kSTfillcs,kstSFmergetype,CNTLSF)
          CALL STATUSBITS(kSTfillcs,kstSFchainnum,PICK)


          IF(CNTLSF.EQ.2)THEN
C super chains
            IF(NCHANM.NE.1)
     2           CALL STATUSBITS(kSTfillcs,kstSFmergeind,1)
            LMERGED=.FALSE.
          ELSE
C merged chains
            IF(KILCHN(2,PICK).NE.0)THEN
              CALL STATUSBITS(kSTfillcs,kstSFmergeind,1)
              LMERGED=.TRUE.
            ELSE
              LMERGED=.FALSE.
            ENDIF
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
C         CALL DSF_CHVIEW1(JCHAIN(1))
          CALL DSF_CHVIEW(JCHAIN)
          
          CALL DSF_ABORT(ABORT)
          IF(ABORT.EQ.1)GO TO 199
          
#if defined(CLEO_COSMIC)
          CALL SFCOSF(PICK,IERR)
#endif
          
#endif
          
          
        ELSE
C-----------------------------------------------------------------------
C branch point when no good chain is found
C-----------------------------------------------------------------------
          
#if defined(CLEO_SFDIAG)
C delete the last chosen chain
          CALL DSF_CHVIEW1(0)
#endif
          
C end of condition (or not) (NCHANM.NE.0)
        ENDIF
        
#if defined(CLEO_SFDIAG)
        CALL DSF_LEAV(CRNAME)
#endif

C end of condition (STILL_OK)        
      ENDIF
 199  CONTINUE
      
C-----------------------------------------------------------------------
C abort signal
C-----------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
      IF(ABORT.EQ.1)THEN
        IERR=0
        NCHANM=0
      ENDIF
#endif
      
C-----------------------------------------------------------------------
C exit
C-----------------------------------------------------------------------
      SFNEXT_CONTROL=NCHANM
      RETURN
      END







