*
* $Id: findhs.F,v 1.15 2001/11/20 00:07:24 dpp Exp $
*
* $Log: findhs.F,v $
* Revision 1.15  2001/11/20 00:07:24  dpp
*      -> add new control parameter include file
*
* Revision 1.14  2001/04/19 17:15:26  dpp
*      -> expand diagnostic printout
*
* Revision 1.13  2001/03/30 00:01:50  dpp
*      -> split off determination of whether input Z matches hybrid
*      -> split off determination of whether input phi matches hybrid
*      -> calculate layer efficiency: the best efficiency of hybrids
*         that might be consistant with the extrapolated track
*
* Revision 1.12  2001/01/31 00:17:10  dpp
* get rid of infinite loop; do not retest low edge after high edge
*
* Revision 1.11  2000/11/15 17:00:47  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.10  2000/10/24 22:52:41  dpp
*      -> require z match for phi hits
*
* Revision 1.9  2000/08/18 19:13:45  dpp
*     -> change compile switch
*
* Revision 1.8  2000/05/26 16:55:21  dpp
*      -> allow correlation hit in other view to place hit in road
*      -> require hit available
*
* Revision 1.7  2000/05/17 19:48:01  dpp
*     -> bug fixes
*
* Revision 1.6  2000/05/16 19:03:39  dpp
* compatible with merged SV hit list
*
* Revision 1.5  2000/04/26 15:09:40  dpp
*      -> search for data in svscrtch.F
*      -> demand phi match for z hits
*      -> (must also put a flag in doit_init_sigma.F to show that data is in
*          this different location)
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      SUBROUTINE FINDHS(INP_TF_LYR,INP_PHI,INP_X,INP_Y,INP_Z,INP_Z0,
     2     IADR,NHWMAX,NHW,EFF_LAYER)
C.......................................................................
C.
C. FINDHS - Search for hit wires near INP_PHI, INP_Z in silicon
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : ROADHITS
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 06-apr-2000 adapted from FINDHW
C. LAST MOD  : 
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
*--
* INP_TF_LYR.. Layer number
* INP_PHI..... Expected PHI value
* INP_X....... Expected X value
* INP_Y....... Expected Y value
* INP_Z....... Expected Z value
* INP_Z0...... Z0 of fit, added to window in Z for phi hybrids
*
* IADR........ Addresses of hit wires found
* NHWMAX...... Maximum # of hit wires calling program will accept
* NHW......... # of hit wires found
* EFF_LAYER... efficiency of layer, best of bybrids close to the track
*--
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/svgeometry.inc"
#include "doit/duseq/svscratch.inc"

C ARGUMEMNTS
      INTEGER INP_TF_LYR
      REAL INP_PHI,INP_X,INP_Y,INP_Z,INP_Z0
      INTEGER IADR(*),NHWMAX,NHW
      REAL EFF_LAYER

C LOCAL VARIABLES
      INTEGER NSERF,IWIRF0,NWIRF,NWIRZ
      INTEGER ISERF,IWIRF,IWIRZ,IWADR

      INTEGER LF_LYR_CF
      REAL SIGEXT
      INTEGER ERR_TFEXER

      INTEGER SV_HIT
      INTEGER HIT_LIMIT
      REAL PHI_ROAD

      REAL PHI_SMALL,PHI_LARGE,PHI_TESTI,PHI_ADDED,PHI_TEMPO,PHI_DIF_S
      REAL Z_SMALL,Z_LARGE,Z_TEMPO

      REAL MISS,MISSZ
      INTEGER I_HYBRID
      INTEGER N_HYB_OVERLAP,J_EFF
      INTEGER INP_SV_LAYER
      LOGICAL MATCH_TYPE,KEEP_HIT,IN_WINDOW
      LOGICAL FIRST/.TRUE./

      REAL PHI_TOLER
      PARAMETER (PHI_TOLER=.05)
      REAL Z_TOLER_MIN,Z_TOLER
      PARAMETER (Z_TOLER_MIN=.010)

      CHARACTER*9 PRINT_LABEL

C DOIT FUNCTION
      REAL Z2PI,PMPI

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------

      IF(FIRST)THEN
         FIRST=.FALSE.

C print out the hybrid info
#if defined(CLEO_SVDFINDHSDIAG)
         DO 10 I_HYBRID=1,NUM_SV_HYBRIDS
            PRINT 1001,I_HYBRID,
     1           SVBOUNDARY_X  (I_HYBRID,1),SVBOUNDARY_X  (I_HYBRID,2),
     1           SVBOUNDARY_Y  (I_HYBRID,1),SVBOUNDARY_Y  (I_HYBRID,2),
     1           SVBOUNDARY_R  (I_HYBRID,1),SVBOUNDARY_R  (I_HYBRID,2),
     1           SVBOUNDARY_PHI(I_HYBRID,1),SVBOUNDARY_PHI(I_HYBRID,2),
     1           SVBOUNDARY_Z  (I_HYBRID,1),SVBOUNDARY_Z  (I_HYBRID,2),
     1           SV_HYBRID_LAYER(I_HYBRID),
     1           SV_HYBRID_EFF (I_HYBRID,1),SV_HYBRID_EFF (I_HYBRID,2)
 10      CONTINUE
 1001    FORMAT(' FINDHS: hybrid ',I3,
     1        ' [X]',2F6.3,' [Y]',2F6.3,
     1        ' [R]',2F6.3,' [f]',2F6.3,' [Z]',2F6.3,' [L]',I3,
     1        ' [eff]',2F5.2)

         DO 12 SV_HIT=1,SV_NHITS
            PRINT 1005,
     1           SV_HIT,
     2           SV_LAYER(SV_HIT),SV_AVAIL(SV_HIT),
     3           SV_HYBRID_IDENTIFY(SV_LAYER(SV_HIT)),
     4           SV_HYBRID_USE(SV_HYBRID(SV_HIT)),
     4                 SV_X(SV_HIT),SV_Y(SV_HIT),
     5                 SV_Z(SV_HIT),SV_PHI(SV_HIT)
 12      CONTINUE
 1005    FORMAT(' FINDHS:',I5,
     1        '  L:',I3,
     2        ' avail:',I2,' ident=',L1,' USE=',L1,
     4        ', X,Y,Z,f',4F8.4)
#endif
      ENDIF

C zero the returned hit counter
      NHW=0

C find the SV layer for the input TF layer
C      INP_SV_LAYER=IXSFTF(INP_TF_LYR)
      IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
         INP_SV_LAYER=IL_TF_SV_F(INP_TF_LYR)
         LF_LYR_CF=IL_SV_Z_TF(INP_SV_LAYER)
      ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
         INP_SV_LAYER=IL_TF_SV_Z(INP_TF_LYR)
      ENDIF

C get the limit of the loop over hits
      HIT_LIMIT=SV_NHITS


C create the phi window 
      IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
         PHI_ROAD=
     1        SEARCH_LAYER(INP_TF_LYR,SAV_HIT_TYP,SAV_FIT_TYP)
     2        *SIGMA_LAYER(INP_TF_LYR,SAV_HIT_TYP,SAV_FIT_TYP)
     3        /RTF(INP_TF_LYR)
      ENDIF


C calculate the Z extrapolation error
      IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
         CALL TFEXER(LF_LYR_CF,SIGEXT,ERR_TFEXER)
         IF(ERR_TFEXER.NE.0)SIGEXT=0.
         Z_TOLER=2.*SIGEXT
         IF(Z_TOLER.LT.Z_TOLER_MIN)Z_TOLER=Z_TOLER_MIN
      ENDIF

C calculate the efficiency for the layer
C as the maximum efficiency of possible hybrids within the search window

      N_HYB_OVERLAP=0
      EFF_LAYER=0.
      DO 109 I_HYBRID=1,NUM_SV_HYBRIDS
         IN_WINDOW=(SV_HYBRID_LAYER(I_HYBRID).EQ.INP_SV_LAYER)
C CF: test if the input Z is within the hybrid
         IF(IN_WINDOW)THEN
            CALL FINDHS_Z_WINDOW(I_HYBRID,INP_Z,INP_Z0,Z_TOLER,
     2           IN_WINDOW,MISSZ)
         ENDIF
         IF(IN_WINDOW)THEN
C LF: test if input phi is within hybrid
            CALL FINDHS_PHI_WINDOW(I_HYBRID,INP_PHI,PHI_TOLER,
     2           IN_WINDOW,MISS)
         ENDIF
         IF(IN_WINDOW)THEN
            N_HYB_OVERLAP=N_HYB_OVERLAP+1
            IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
               J_EFF=1
            ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
               J_EFF=2
            ENDIF
            IF(SV_HYBRID_EFF(I_HYBRID,J_EFF).GT.EFF_LAYER)
     2           EFF_LAYER=SV_HYBRID_EFF(I_HYBRID,J_EFF)
         ENDIF

 109     CONTINUE

C print out layer info
#if defined(CLEO_SVDFINDHSDIAG)
      IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
         PRINT 1003,
     1        INP_TF_LYR,INP_SV_LAYER,
     2        SEARCH_LAYER(INP_TF_LYR,SAV_HIT_TYP,SAV_FIT_TYP),
     3        SIGMA_LAYER(INP_TF_LYR,SAV_HIT_TYP,SAV_FIT_TYP),
     4        RTF(INP_TF_LYR),
     5        PHI_ROAD,INP_Z,INP_Z0,SIGEXT,Z_TOLER,SV_NHITS,
     6        EFF_LAYER,N_HYB_OVERLAP
 1003    FORMAT(' FINDHStflyr',I3,' SVlyr',I2,
     2        ' Nsig',I4,' sig',F8.6,' rad',F5.3,
     3        ' ROAD',F9.6,
     4        ' inpZ=',F6.3,' inpZ0=',F6.3,
     5        ' SIGEXT=',F7.4,' Ztol=',F7.4,
     6        ' svhits=',I4,
     7        ' eff=',F4.2,' from',I2,' hybrd')  
      ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
         PRINT 1004,
     1        INP_TF_LYR,INP_SV_LAYER,
     2        PHI_TOLER,SV_NHITS,
     3        EFF_LAYER,N_HYB_OVERLAP
 1004    FORMAT(' FINDHS:tflyr',I3,' SVlyr',I2,
     2        ' phi_toler=',F6.3,' svhits=',I4,
     3        ' eff=',F4.2,' from',I2,' hybrds')
      ENDIF
#endif
      

C loop over the hits
      IF(HIT_LIMIT.GE.1)THEN
         DO 250 SV_HIT=1,HIT_LIMIT
            MATCH_TYPE=.FALSE.
            KEEP_HIT=.FALSE.


C require layer match
            IF(SV_LAYER(SV_HIT).EQ.INP_SV_LAYER)THEN

C CF: require R-PHI hit
               IF(
     1              (SAV_FIT_TYP.EQ.FITTYP_CF).AND.
     2              (SV_TYPE(SV_HIT).EQ.0) )THEN
                  MATCH_TYPE=.TRUE.

C CF: test the phi difference
                  MISS=PMPI(SV_PHI(SV_HIT)-INP_PHI)
                  KEEP_HIT=(ABS(MISS).LE.PHI_ROAD)

C CF: test if the input Z is within the hybrid
                  IF(KEEP_HIT)THEN
                     I_HYBRID=SV_HYBRID(SV_HIT)
                     CALL FINDHS_Z_WINDOW(I_HYBRID,INP_Z,INP_Z0,Z_TOLER,
     2                    IN_WINDOW,MISSZ)
                     KEEP_HIT=IN_WINDOW
                  ELSE
                     MISSZ=0.
                  ENDIF

C LF: require S-Z hit
               ELSEIF(
     1                 (SAV_FIT_TYP.EQ.FITTYP_LF).AND.
     2                 (SV_TYPE(SV_HIT).EQ.1) )THEN
                  MATCH_TYPE=.TRUE.

C LF: test if input phi is within hybrid
                  I_HYBRID=SV_HYBRID(SV_HIT)
                  CALL FINDHS_PHI_WINDOW(I_HYBRID,INP_PHI,PHI_TOLER,
     2                 IN_WINDOW,MISS)
                  KEEP_HIT=IN_WINDOW

C end of CF/LF separation
               ENDIF

#if defined(CLEO_SVDFINDHSDIAG)
               IF(MATCH_TYPE)THEN
                  PRINT_LABEL='matchlayr'
                  IF(KEEP_HIT)THEN
                     PRINT_LABEL='MATCHROAD'
                  ENDIF
               ELSE
                  PRINT_LABEL='???what??'
               ENDIF
#endif
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
                  
C require hit available for use
               IF(KEEP_HIT)THEN
               	  IF(SV_AVAIL(SV_HIT).LE.0)THEN
                     KEEP_HIT=.FALSE.
#if defined(CLEO_SVDFINDHSDIAG)
                     PRINT_LABEL='NOT AVAIL'
#endif
                  ENDIF
               ENDIF


C maybe require hit in other view
               IF(KEEP_HIT)THEN
                  IF(SV_HYBRID_USE_VALID)THEN
                     IF(SV_HYBRID_IDENTIFY(SV_LAYER(SV_HIT)))THEN
                        IF(.NOT.SV_HYBRID_USE(SV_HYBRID(SV_HIT)))THEN
                           KEEP_HIT=.FALSE.
#if defined(CLEO_SVDFINDHSDIAG)
                           PRINT_LABEL='NO COINCD'
#endif
                        ENDIF
                     ENDIF
                  ENDIF
               ENDIF


C save the hit
               IF(KEEP_HIT)THEN
                  IF(NHW.LT.NHWMAX)THEN
                     NHW=NHW+1
                     IADR(NHW)=SV_HIT
                  ELSE
                     KEEP_HIT=.FALSE.
#if defined(CLEO_SVDFINDHSDIAG)
                     PRINT_LABEL=' NO ROOM '
#endif
                  ENDIF
               ENDIF


#if defined(CLEO_SVDFINDHSDIAG)
               IF(MATCH_TYPE)THEN
                  IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                     PRINT 1006,PRINT_LABEL,
     1                    SV_LAYER(SV_HIT),
     2                    SV_HIT,
     3                    SV_HYBRID(SV_HIT),
     4                    SV_X(SV_HIT),SV_Y(SV_HIT),
     5                    SV_Z(SV_HIT),SV_PHI(SV_HIT),
     6                    INP_PHI,MISS,
     7                    INP_Z,
     8                    SVBOUNDARY_Z((SV_HYBRID(SV_HIT)),1),
     9                    SVBOUNDARY_Z((SV_HYBRID(SV_HIT)),2),
     x                    MISSZ

                  ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
                     PRINT 1007,PRINT_LABEL,
     1                    SV_LAYER(SV_HIT),
     2                    SV_HIT,
     3                    SV_HYBRID(SV_HIT),
     4                    SV_X(SV_HIT),SV_Y(SV_HIT),
     5                    SV_Z(SV_HIT),SV_PHI(SV_HIT),
     6                    INP_PHI,
     7                    SVBOUNDARY_PHI((SV_HYBRID(SV_HIT)),1),
     8                    SVBOUNDARY_PHI((SV_HYBRID(SV_HIT)),2),
     9                    MISS
                  ENDIF
               ENDIF

 1006          FORMAT(' FINDHS:CF:', A9,
     1              ', L',I2,
     2              ', hit',I4,
     3              ', hyb',I4,
     4              ', XYZf',4F8.4,
     6              ', inp phi',F6.3,
     9              ', MISS=',F6.3,
     6              ', inp Z',F6.3,
     7              ', EDGE@',2F6.3,
     9              ', MISS=',F6.3)
 1007          FORMAT(' FINDHS:LF:', A9,
     1              ', L',I2,
     2              ', hit',I4,
     3              ', hyb',I4,
     4              ', XYZf',4F8.4,
     6              ', inp phi',F6.3,
     7              ', EDGE@',2F6.3,
     9              ', MISS=',F6.3)
#endif
               
C end of the layer match requirement
            ENDIF

C end of loop over hits
 250      CONTINUE

C end of non-zero number of hits requirement
      ENDIF

      RETURN
      END

