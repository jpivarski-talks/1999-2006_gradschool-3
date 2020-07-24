*
* $Id: sffill.F,v 1.7 2003/03/05 18:27:25 dpp Exp $
*
* $Log: sffill.F,v $
* Revision 1.7  2003/03/05 18:27:25  dpp
*      -> new assignments for negative INUCHN
*      -> remove special (faster?) reset for sfbwal
*      -> re-organize calculation to avoid MULST*NWIR**2, which can overflow
*         ... but restored to original for "no affect on compress" version
*      -> split off sffill_check_double.F and sffill_fill_vd_alt.F
*      -> extensive F77
*
* Revision 1.6  2002/09/05 18:48:35  dpp
*      -> remove local ICHAIN
*
* Revision 1.5  2001/08/21 14:09:10  dpp
*      -> fix: fill INUEFF for all hits on chain
*      -> comment out  print statements
*
* Revision 1.4  1999/04/27 19:38:16  dpp
* update number of single hits
*
* Revision 1.3  1998/07/09 01:44:22  lyon
* Retrofitted for sf_data_access.inc
*
* Revision 1.2  1998/05/08 14:14:09  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.7  1996/04/16 19:59:20  dpp
* bug fix, certain lines are diagnostic only
*
* Revision 1.6  1996/04/15 17:10:42  dpp
* reorganize to prepare for extrapolation
*
* Revision 1.5  1996/03/21 19:28:09  zfiles
* provide direct access to data, without SFDATA buffer, for beam wall use
*
* Revision 1.4  1996/03/20 17:15:56  zfiles
* various changes for speed: veroing in SFIND, removed SFCDAD
*
* Revision 1.3  1996/02/14 21:04:19  zfiles
* remove call to SFLWF; use SFLSFA etc
*
* Revision 1.2  1995/11/27 17:01:11  nk
* remove layer number references
*
* Revision 1.1.1.1  1994/10/08  01:00:43  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/18 22/03/94  14.37.50  by  Dan Peterson
*-- Author :
      SUBROUTINE SFFILL(ICHNIN,HOWFIL,EXTRP1)
C......................................................................
C.
C. SFFILL  -  fill in the skipped hits in a chain and flag missing hits
C.
C. COMMON    : /STEPCn/
C. CALLS     : CHVM1  CHVM2
C. CALLED    : SFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 20-Jul-88
C. LAST MOD  : 06-Oct-88
C.
C. Modification Log
C
C MUST NOT BE RUN AFTER SFMERG WHICH CHANGES ISTFOR/ISTBAK BEYOND BOUNDRIES
C
C************* WILL IGNORE HOW TRACK WAS GENERATED, AND USE SF GEOMETRY
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

C...... arguments

C ICHNIN...INPUT LAYER NUMBER; IF=0, DO ALL
      INTEGER ICHNIN
C HOWFIL...1:FILL IN CHAIN, ONLY FOR FOUND HITS, PUT IN STANDARD CHAIN, INUCHN,
C          2:FILL IN EVEN MISSING HITS, AND VDEAST, PUT IN INUEFF
      INTEGER HOWFIL
C EXTRP1...NUMBER OF LAYERS TO EXTRAPOLATE INWARD
      INTEGER EXTRP1

C...... local variable documentaion

C ADLESF...ADDRESS OF LOW END HIT,                   SF INDEXING
      INTEGER ADLESF
C ILYRLE...LAYER OF LOW END HIT
      INTEGER ILYRLE
C IWRLE....WIRE OF LOW END HIT
      INTEGER IWRLE
C ADHESF...ADDRESS OF HIGH END HIT,                  SF INDEXING
      INTEGER ADHESF
C ILYRHE...LAYER OF HIGH END HIT
      INTEGER ILYRHE
C IWRHE....WIRE OF HIGH END HIT
      INTEGER IWRHE
C IADRL....ADDRESS OF LAST ENCOUNTERED HIT IN CHAIN
      INTEGER IADRL
C IADRN....ADDRESS OF NEXT HIT IN CHAIN
      INTEGER IADRN
C IADRT....ADDRESS OF TEST HIT,                      SF INDEXING
      INTEGER IADRT
C ADRHCD...HIT ADDRESS OF THE TEST HIT,              CD INDEXING
      INTEGER ADRHCD
C ADRHAJ...HIT ADDRESS OF HIT ADJACENT TO TEST HIT   CD INDEXING
      INTEGER ADRHAJ(-5:5)
C IADRLD...ADDRESS OF LAST HIT IN STRING OF ADJACENT HITS
      INTEGER IADRLD
C IADRF....ADDRESS OF PRIMARY FOUND HIT, SAVED DURING SEARCH FOR ADJACENT HITS
      INTEGER IADRF
C NDOUBL...NUMBER OF ADJACENT HITS FOUND
      INTEGER NDOUBL
C IPM......INDICATION THAT PLUS/MINUS DIRECTION IS STILL ACTIVE FOR ADJACENT
      INTEGER IPM(2)
C JPM......NUMBER OF HITS FOUND ADJACENT IN PLUS/MINUS DIRECTION
      INTEGER JPM(2)
C KDIR.....DIRECTION BEING TESTED IN ADJACENT SEARCH
      INTEGER KDIR
C ILYRT....TEST LAYER
      INTEGER ILYRT
C ITRY.....COUNTER OF TESTED HITS IN A LAYER
      INTEGER ITRY
C IADD.....NUMBER OF WIRES TO ADD TO GO TO NEXT TRY
      INTEGER IADD
C LADR.....BEGINNING MAP ADDRESS OF A LAYER
      INTEGER LADR
C IWRLF0...WIRE NUMBER  -LAST-FOUND    -MULTIPLIED AND RELATIVE TO 0
      INTEGER IWRLF0
C IWRNF0...WIRE NUMBER  -NEXT-FOUND    -MULTIPLIED AND RELATIVE TO 0
      INTEGER IWRNF0
C IW1NRX...WIRE NUMBER 1-NOW -RADIALOUT-MULTIPLIED
      INTEGER IW1NRX
      integer iw1nrx_old
C IW2NRX...WIRE NUMBER 2-NOW -RADIALOUT-MULTIPLIED
      INTEGER IW2NRX
      integer iw2nrx_old
C IWRNPX...WIRE NUMBER  -NOW -PREDICTED-MULTIPLIED
      INTEGER IWRNPX
C IWRNPU...WIRE NUMBER  -NOW -PREDICTED-USER
      INTEGER IWRNPU
C IWRNTU...WIRE NUMBER  -NOW -TEST     -USER
      INTEGER IWRNTU
C IWRNFU...WIRE NUMBER  -NOW -FOUND    -USER
      INTEGER IWRNFU
C NWIRTX...NUMBER OF WIRES IN LAYER, MULTIPLIED
      INTEGER NWIRTX
C IDIF.....DIFFERENCE IN WIRE NUMBER
      INTEGER IDIF
C IDRAD....RADIUS DIFFERENCE
      INTEGER IDRAD
C IRELX....TRUCATED LOSS
      INTEGER IRELX
C J........COUNTER
      INTEGER J
C IGOT.............FLAG
      INTEGER IGOT
C FIRSTH...........LOGICAL FLAG OF FIRST HIT IN CHAIN
      LOGICAL FIRSTH
C OPEN_HIT.........FLAG THAT HIT IS USABLE
      INTEGER OPEN_HIT
C HIT_AND_UNUSED...LOGICAL, hit is found and is unused by previous tracks
      LOGICAL HIT_AND_UNUSED
C ADD_THIS_HIT.....LOGICAL, add hit to the link list
      LOGICAL ADD_THIS_HIT
C DOUBLE_UNUSED....LOGICAL, double hit found and is unused by previous tracks
      LOGICAL DOUBLE_UNUSED
C ADD_DOUBLE.......LOGICAL, add double hit to the hit list 
      LOGICAL ADD_DOUBLE
C EXTEND_SEARCH....LOGICAL, extend the search for extra long chains
      LOGICAL EXTEND_SEARCH
C KILLED_DOUBLES...LOGICAL, killed an extended chain of double hits
      LOGICAL KILLED_DOUBLES

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C-----------------------Executable code starts here---------------------
      IF(
     1     (HOWFIL.EQ.1).OR.
     2     (HOWFIL.EQ.2))THEN
        
        IF(NCHAIN.GE.1)THEN
          
          sf_data_input_flag=1
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"
          
C-----------------------------------------------------------------------
C start loop over all chains, or obtain chain number from input
C-----------------------------------------------------------------------
          
          IF(ICHNIN.EQ.0)THEN
            ICHAIN=1
          ELSE
            ICHAIN=ICHNIN
          ENDIF
          
 121      CONTINUE
          IF(ICHAIN.LE.NCHAIN)THEN
            
C-----------------------------------------------------------------------
C locate first hit
C and check for double hits for the first hit
C-----------------------------------------------------------------------
            
            ADHESF=LINCHN(ICHAIN)
            FIRSTH=.TRUE.
            
#if defined(CLEO_SFDIAG)
            PRINT 1006,ICHAIN,HOWFIL,ADHESF
 1006       FORMAT(' SFFILL: STARTING CHAIN #',I7,' HOWFIL=',I2,
     1            ' FIRST CELL-ID=',I7)
#endif
            
C execute procedure to test double hits and fill high end layer and wire 

            CALL SFFILL_CHECK_DOUBLE(HOWFIL,FIRSTH,ADHESF,ILYRHE,IWRHE)

C-----------------------------------------------------------------------
C transfer the last hit to the "LOW END" position and get the next hit
C-----------------------------------------------------------------------

 131        CONTINUE
            FIRSTH=.FALSE.
            ADLESF=ADHESF
            ILYRLE=ILYRHE
            IWRLE=IWRHE
            ADHESF=ISTFOR(ADHESF)
            
C-----------------------------------------------------------------------
C execute procedure to test double hits and fill high end layer and wire 
C double hit test is performed on "HE";
C do not execute if there are no more hits
C-----------------------------------------------------------------------
            
            IF(ADHESF.GT.0)THEN
              CALL SFFILL_CHECK_DOUBLE
     2             (HOWFIL,FIRSTH,ADHESF,ILYRHE,IWRHE)
            ENDIF
            
C-----------------------------------------------------------------------
C load "LAST" and "NEXT" hit counters for filling
C-----------------------------------------------------------------------

            IADRL=ADLESF
            IADRN=ADHESF
            
C-----------------------------------------------------------------------
C can now test east vd, fill INUEFF
C will come here for every hit, stored in "LE"
C on the last hit, "HE" will be set to =0; "LE" will store the last hit
C this is not needed execpt in SFDIAG
C-----------------------------------------------------------------------
            
#if defined(CLEO_SFDIAG)
            ILYRT=ILYRLE
            CALL SFFILL_FILL_VD_ALT(HOWFIL,ILYRT,IADRL,IADRN)
#endif
            
C-----------------------------------------------------------------------
C continue procedure only if there is a "high end" hit
C and if there is a gap in layers between "low end" and "high end"
C-----------------------------------------------------------------------

            IF(ADHESF.GT.0)THEN
              IF(ILYRLE+1.LT.ILYRHE)THEN
      
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C HERE, THERE IS A GAP BETWEEN ILYRLE AND ILYRHE
C THIS CODE ADAPTED FROM SFMERG
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

                IWRLF0=IWRLE*MULST+OFFSF(ILYRLE)
                IWRNF0=IWRHE*MULST+OFFSF(ILYRHE)
                ILYRT=ILYRLE

C-----------------------------------------------------------------------
C loop over intermetdiate layers
C-----------------------------------------------------------------------

 341            ILYRT=ILYRT+1
                IF(ILYRT.LT.ILYRHE)THEN
                  
                  IW1NRX=IWRLF0
     2                 +IWRLF0*(NWRSF(ILYRT)-NWRSF(ILYRLE))
     3                 /NWRSF(ILYRLE)
     4                 -OFFSF(ILYRT)

                  iw1nrx_old=iwrlf0*nwrsf(ilyrt)/nwrsf(ilyrle)
     2                 -offsf(ilyrt)
                  iw1nrx=iw1nrx_old

                  NWIRTX=NWRSF(ILYRT)*MULST
                  IF(IW1NRX.LT.0)IW1NRX=IW1NRX+NWIRTX
                  IF(IW1NRX.GE.NWIRTX)IW1NRX=IW1NRX-NWIRTX

                  IW2NRX=IWRNF0
     2                 +IWRNF0*(NWRSF(ILYRT)-NWRSF(ILYRHE))
     3                 /NWRSF(ILYRHE)
     4                 -OFFSF(ILYRT)

                  iw2nrx_old=iwrnf0*nwrsf(ilyrt)/nwrsf(ilyrhe)
     2                 -offsf(ilyrt)
                  iw2nrx=iw2nrx_old

                  IDIF=IW2NRX-IW1NRX
                  IF(IDIF.LT.-NWIRTX/2)IW2NRX=IW2NRX+NWIRTX
                  IF(IDIF.GE. NWIRTX/2)IW2NRX=IW2NRX-NWIRTX

                  IDRAD=RADSF(ILYRHE)-RADSF(ILYRLE)
                  
                  IF    (IW2NRX.LT.IW1NRX)THEN
                    IWRNPX=( (RADSF(ILYRHE)-RADSF(ILYRT))
     1                   *(IW1NRX-IW2NRX) )/IDRAD  +IW2NRX
                  ELSEIF(IW2NRX.GT.IW1NRX)THEN
                    IWRNPX=( (RADSF(ILYRT)-RADSF(ILYRLE))
     1                   *(IW2NRX-IW1NRX) )/IDRAD  +IW1NRX
                  ELSE
                    IWRNPX=IW1NRX
                  ENDIF
                  
                  IWRNPU=(IWRNPX+MULSTH)/MULST
                  IF(IWRNPU.LT.0)IWRNPU=IWRNPU+NWRSF(ILYRT)
                  IF(IWRNPU.GE.NWRSF(ILYRT))IWRNPU=IWRNPU-NWRSF(ILYRT)
                  
#if defined(CLEO_SFDIAG)
                  PRINT 1003,
     1                 ILYRT,ILYRLE,ILYRHE,
     2                 IW1NRX,IW2NRX,IWRNPX,IWRNPU
 1003             FORMAT(
     1                 ' SFFILL: WORKIN ON LAYER ',I4,
     2                 '  (BETWEEN',I4,' AND',I4,' )'/
     2                 '                                IW1NRX=',I6/
     3                 '                                IW2NRX=',I6/
     4                 '                                IWRNPX=',I6/
     5                 '                                IWRNPU=',I4)
#endif

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C 
C PROCEDURE TO SEARCH FOR THE PREDICTED HIT 
C THIS CODE ADAPTED FROM SFCHAN
C 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

                  IRELX=IBITS(IWRNPX,0,10)
                  IADD=-1
                  IF(IRELX.GT.MULSTH)IADD=1
                  LADR=INSFSF(ILYRT)
                  
C first try
                  IWRNTU=IWRNPU
                  ITRY=1
                  IGOT=-1
                  
 511              IADRT=LADR+IWRNTU
                  
#if defined(CLEO_SFDIAG)
                  DO 513 J=-5,5
                    
                    sf_data_input_sf_adr=IADRT+J
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
                    ADRHAJ(J)=sf_data_output_hit_adr
                    
 513              CONTINUE
                  
                  PRINT 1004,IWRNTU,IADRT,
     3                 (ADRHAJ(J)      ,J=-5,5),
     1                 (INUCHN(IADRT+J),J=-5,5),
     2                 (INUEFF(IADRT+J),J=-5,5)
 1004             FORMAT(' SFFILL: IWRNTU=',I6,' IADRT=',I6/
     3                 '         "MAP":  ',11I5/
     1                 '        INUCHN:  ',11I5/
     2                 '        INUEFF:  ',11I5)
#endif
                  
                  
C-----------------------------------------------------------------------
C TEST IF HIT IS THERE AND UNUSED BY PREVIOUS TRACK
C-----------------------------------------------------------------------
                  HIT_AND_UNUSED=.TRUE.
                  ADD_THIS_HIT=.TRUE.

                  sf_data_input_sf_adr=IADRT
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
                  ADRHCD=sf_data_output_hit_adr
                  
                  IF(ADRHCD.LE.0)HIT_AND_UNUSED=.FALSE.
                  
                  IF(HIT_AND_UNUSED)THEN

                    sf_data_input_hit_adr=ADRHCD
#define SF_DATA_OPEN_HIT
#include "doit/sf_data_access/sf_data_access.inc"
                    OPEN_HIT=sf_data_output_open_hit
                    
                    
#if defined(CLEO_SFDIAG)
                    PRINT 1007,OPEN_HIT
 1007               FORMAT(' SFFILL: OPEN_HIT=',I7,
     1                   ' will treat as a "did not find"',
     2                   ' for negative')  
#endif
                    
                    IF(OPEN_HIT.LT.0)HIT_AND_UNUSED=.FALSE.
                  ENDIF
                  
C-----------------------------------------------------------------------
C FOUND IT
C-----------------------------------------------------------------------
                  IF(HIT_AND_UNUSED)THEN
                  
C demand not previously used by SF (this pass)
C DO NOT delete this requirement
                    IF(INUCHN(IADRT).GT. 0)ADD_THIS_HIT=.FALSE.
                  
C demand good hit unused by anything (NOT ACTIVE)
C                   IF(INUCHN(IADRT).NE. 0)ADD_THIS_HIT=.FALSE.
                  
C demand not a (clustered hit, but not primary hit)
                    IF(INUCHN(IADRT).EQ.-1)ADD_THIS_HIT=.FALSE.
                  
C demand time cut, (NOT ACTIVE; high time ok)
C                   IF(INUCHN(IADRT).EQ.-2)ADD_THIS_HIT=.FALSE.
C                   IF(INUCHN(IADRT).EQ.-3)ADD_THIS_HIT=.FALSE.
C                   IF(INUCHN(IADRT).EQ.-4)ADD_THIS_HIT=.FALSE.
C                   IF(INUCHN(IADRT).EQ.-5)ADD_THIS_HIT=.FALSE.
                  
C demand good pulse height (not pedistal)
C in CLEO3, PH is required to be between PULS_HT_L and PULS_HT_H
C  (usually set as 1. to 999999999.)
C but allow low pulse height for HOWFIL=2
                    IF(
     1                   (HOWFIL.EQ.1).AND.
     2                   (INUCHN(IADRT).EQ.-6))ADD_THIS_HIT=.FALSE.
C demand not used in DUET, or not adjacent to that used in DUET
                    IF(INUCHN(IADRT).EQ.-7)ADD_THIS_HIT=.FALSE.
                    IF(INUCHN(IADRT).EQ.-8)ADD_THIS_HIT=.FALSE.
C demand not a (not-clustered (low charge) hit)
                    IF(INUCHN(IADRT).EQ.-9)ADD_THIS_HIT=.FALSE.
                  
                  
C demand not assigned to another chain in SFFILL
#if defined(CLEO_SFDIAG)
                    IF(INUEFF(IADRT).NE.0)ADD_THIS_HIT=.FALSE.
#endif
                  
                    IGOT=+1
                  
C-----------------------------------------------------------------------
C DIDNT FIND IT (HIT_AND_UNUSED.eq.false.)
C-----------------------------------------------------------------------
                  ELSE  

C try another hit within a window
                    IF(ITRY.LT.3)THEN
                      ITRY=ITRY+1
                      IF(ITRY.EQ.2)IWRNTU=IWRNPU+IADD
                      IF(ITRY.EQ.3)IWRNTU=IWRNPU-IADD
                      IF(IWRNTU.LT.0           )
     1                     IWRNTU=IWRNTU+NWRSF(ILYRT)
                      IF(IWRNTU.GE.NWRSF(ILYRT))
     1                     IWRNTU=IWRNTU-NWRSF(ILYRT)
                      GO TO 511
                      
C found no hit within window
                    ELSE
                      
#if defined(CLEO_SFDIAG)
C put hit in INUCHN for HOWFIL=2
                      IF(HOWFIL.NE.2)ADD_THIS_HIT=.FALSE.
C use first predicted hit to fill dummy hit
                      IWRNTU=IWRNPU
                      IADRT=LADR+IWRNPU
C do not allow two chains to claim the same hit
                      IF(INUEFF(IADRT).NE.0)ADD_THIS_HIT=.FALSE.
#else
C skip this layer when not on SFDIAG
                      ADD_THIS_HIT=.FALSE.
#endif
                    ENDIF
                    
                  ENDIF

C-----------------------------------------------------------------------
C IT IS AVAILABLE, EITHER FOUND OR NOT FOUND
C-----------------------------------------------------------------------

                  IF(ADD_THIS_HIT)THEN

C adjust the link list
                    ISTFOR(IADRL)=IADRT
                    ISTBAK(IADRN)=IADRT
                    ISTFOR(IADRT)=IADRN
                    ISTBAK(IADRT)=IADRL
C    ##DO NOT YET SET IADRL=IADRT

#if defined(CLEO_SFDIAG)
                    PRINT 1011,IADRT,IADRL,IADRN
 1011               FORMAT(' SFFILL: add hit IADRT=',I8,
     2                   '  IADRL,IADRN=',2I8)
#endif

                    
C for HOWFIL=1, increment chain length and save in INUCHN 
                    IF(HOWFIL.EQ.1)THEN
                      LENCHN(ICHAIN)=LENCHN(ICHAIN)+1
                      INUCHN(IADRT)=ICHAIN
                    ELSE

C (HOWFIL.EQ.2)
C do not increment chain length for hits stored in INUEFF but not INUCHN 
#if defined(CLEO_SFDIAG)
                      INUEFF(IADRT)=IGOT*ICHAIN
#endif
                    ENDIF
                    
C-----------------------------------------------------------------------
C CHECK FOR DOUBLE HITS
C THIS IS DIFFERENT THAN IN SFCHAN: ALLOWS STRINGS
C-----------------------------------------------------------------------

C hold starting point

                    IADRF=IADRT
                    IADRLD=IADRT
                    IWRNFU=IWRNTU
                    IF(IGOT.EQ.1)THEN

C allow extended search, looking for wide string
                      EXTEND_SEARCH=.TRUE.
                      KILLED_DOUBLES=.FALSE.
C both directions allowed at start
                      IPM(1)=1
                      IPM(2)=1
C nothing found in each direction at start
                      JPM(1)=0
                      JPM(2)=0
                      NDOUBL=0
                      
C choose the starting direction based on prediction
                      KDIR=1
                      IF(IADD.EQ.-1)KDIR=2

                      
C start in this direction if allowed
 561                  CONTINUE
                      IF(IPM(KDIR).EQ.1)THEN
                        
C limit width in each direction
                        IF(JPM(KDIR).LE.3)THEN
                          JPM(KDIR)=JPM(KDIR)+1
                          IWRNTU=IWRNFU -(-1)**KDIR *JPM(KDIR)
                          IF(IWRNTU.LT.0           )
     1                         IWRNTU=IWRNTU+NWRSF(ILYRT)
                          IF(IWRNTU.GE.NWRSF(ILYRT))
     1                         IWRNTU=IWRNTU-NWRSF(ILYRT)
                          IADRT=LADR+IWRNTU
                          
                          
C-----------------------------------------------------------------------
C TEST IF HIT IS THERE AND UNUSED BY PREVIOUS TRACK
C-----------------------------------------------------------------------
                          DOUBLE_UNUSED=.TRUE.
                          
                          sf_data_input_sf_adr=IADRT
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
                          ADRHCD=sf_data_output_hit_adr
                          
                          IF(ADRHCD.LE.0)DOUBLE_UNUSED=.FALSE.
                  
                          IF(DOUBLE_UNUSED)THEN

                            sf_data_input_hit_adr=ADRHCD
#define SF_DATA_OPEN_HIT
#include "doit/sf_data_access/sf_data_access.inc"
                            OPEN_HIT=sf_data_output_open_hit
                            
                    
#if defined(CLEO_SFDIAG)
                            PRINT 1012,KDIR,JPM(KDIR),IADRT,OPEN_HIT
 1012                       FORMAT(' SFFILL: DOUBLE: JPM(',I2,')=',I2,
     2                           ' IADRT=',I7,' OPEN_HIT=',I7)
#endif
                    
                            IF(OPEN_HIT.LT.0)DOUBLE_UNUSED=.FALSE.
                          ENDIF

                          IF(DOUBLE_UNUSED)THEN
                          
C only save one double hit
                            ADD_DOUBLE=.TRUE.
                            IF(NDOUBL.GE.1)ADD_DOUBLE=.FALSE.
                            
                            IF(HOWFIL.EQ.1)THEN
                              IF(ADD_DOUBLE)THEN
C when HOWFIL=1, must test the possible double hit 
C follow test made in SFCHAN
C BAD HIT; do not save but continue search of string to maximum width
                                IF(INUCHN(IADRT).LT.0)
     1                               ADD_DOUBLE=.FALSE.
C USED HIT;do not save but continue search of string to maximum width
                                IF(INUCHN(IADRT).GT.0)
     1                               ADD_DOUBLE=.FALSE.
C not strictly adjacent, do not save
                                IF(JPM(KDIR).NE.1)
     1                               ADD_DOUBLE=.FALSE.
                              ENDIF
                              IF(ADD_DOUBLE)THEN
                                INUCHN(IADRT)=ICHAIN
                              ENDIF
                            ENDIF
                            
C resume common part
                            IF(ADD_DOUBLE)THEN
                              NDOUBL=NDOUBL+1
                              DOUBPT(IADRLD)=IADRT
                              IADRLD=IADRT
                              
#if defined(CLEO_SFDIAG)
                            PRINT 1013,KDIR,JPM(KDIR),IADRT
 1013                       FORMAT(' SFFILL: DOUBLE: JPM(',I2,')=',I2,
     2                           ' IADRT=',I7,' add double hit')
#endif

C WHEN HOWFIL=2, LABEL DOUBLE HIT BUT NOT INUEFF OR INUCHN
C WHEN HOWFIL=2, DO NOT EXTEND SEARCH FOR EXTRA LONG CHAINS
C WHEN HOWFIL=2, DO NOT CARE IF USED, JUST STOP HERE
                              IF(HOWFIL.EQ.2)EXTEND_SEARCH=.FALSE.
                              
C end of condition: (ADD_DOUBLE) 
                            ENDIF
                            
C found a hit, flip direction and continue
                            IF(EXTEND_SEARCH)THEN
                              KDIR=3-KDIR
                              GO TO 561
                            ENDIF
                            
C else of condition (ADRHCD.GT.0)
C no hit found, kill this direction, flip direction and continue
                          ELSE
                            IPM(KDIR)=0
                            KDIR=3-KDIR
                            GO TO 561
                            
C end of condition (ADRHCD.GT.0)
                          ENDIF
                          
C else of condition (JPM(KDIR).LE.3)
C too many wires, kill everything
                        ELSE
                          IADRLD=IADRF

#if defined(CLEO_SFDIAG)
                            PRINT 1014,KDIR,JPM(KDIR),IADRF
 1014                       FORMAT(' SFFILL: DOUBLE: JPM(',I2,')=',I2,
     2                           ' IADRF=',I7,' kill everything')
#endif

 577                      IADRT=DOUBPT(IADRLD)
                          IF(IADRT.GT.0)THEN
                            DOUBPT(IADRLD)=0
                            IF(HOWFIL.EQ.1)INUCHN(IADRT)=0
                            IADRLD=IADRT
                            GO TO 577
                          ENDIF
                          ISTFOR(IADRL)=IADRN
                          ISTBAK(IADRN)=IADRL
                          IF(HOWFIL.EQ.1)THEN
                            INUCHN(IADRF)=0
                            LENCHN(ICHAIN)=LENCHN(ICHAIN)-1
                          ELSE
C (HOWFIL.EQ.2)
#if defined(CLEO_SFDIAG)
                            INUEFF(IADRF)=0
#endif
                          ENDIF
                          KILLED_DOUBLES=.TRUE.
C end of condition (JPM(KDIR).LE.3)
                        ENDIF
                        
C else of condition: (IPM(KDIR).EQ.1)
C test the other direction
                      ELSE
                        IF(IPM(3-KDIR).EQ.1)THEN
                          KDIR=3-KDIR
                          GO TO 561
                        ENDIF
C end of condition: (IPM(KDIR).EQ.1)
                      ENDIF
                      
c end of condition (IGOT.EQ.1)
                    ENDIF

C end; allowing any double hit to remain
                    IF(.NOT.KILLED_DOUBLES)THEN
                      IF(DOUBPT(IADRF).EQ.0)
     1                     SNGCHN(ICHAIN)=SNGCHN(ICHAIN)+1
                      IADRL=IADRF
                    ENDIF

C-----------------------------------------------------------------------
C execute procedure to test the EAST VD
C-----------------------------------------------------------------------
                    
#if defined(CLEO_SFDIAG)
                    CALL SFFILL_FILL_VD_ALT(HOWFIL,ILYRT,IADRL,IADRN)
#endif
                    
C end of condition: (ADD_THIS_HIT)
                  ENDIF
                  
C loop back for next intermediate layer      
                  GO TO 341
C end of condition:(ILYRT.LT.ILYRHE); still in gap
                ENDIF
C and end of condition:(ILYRLE+1.LT.ILYRHE); there is a gap to test
              ENDIF
C advance to next hit in link list; continue looking for gaps
              GO TO 131
C end of condition: ADHESF, this is the end of processing a chain
            ENDIF
C loop back for next chain
            IF(ICHNIN.EQ.0)THEN
              ICHAIN=ICHAIN+1
              GO TO 121
            ENDIF
C end of condition: ICHAIN
          ENDIF
C end of condition: NCHAIN
        ENDIF
C end of condition: HOWFIL
      ENDIF
      
      sf_data_input_flag=0
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

      RETURN
      END
