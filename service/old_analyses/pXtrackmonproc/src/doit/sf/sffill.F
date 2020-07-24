*
* $Id: sffill.F,v 1.5 2001/08/21 14:09:10 dpp Exp $
*
* $Log: sffill.F,v $
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


C...... LOCAL VARIABLE DOCUMENTAION
C IRTRNn...RETURN FLAG FOR INTERNAL PROCEDURES
C ICHAIN...COUNTER
C ADLESF...ADDRESS OF LOW END HIT,                   SF INDEXING
C ILYRLE...LAYER OF LOW END HIT
C IWRLE....WIRE OF LOW END HIT
C ADHESF...ADDRESS OF HIGH END HIT,                  SF INDEXING
C ADHECD...ADDRESS OF HIGH END HIT,                  CD INDEXING
C ADDOCD...ADDRESS OF HIGH END DOUBLE HIT,           CD INDEXING
C ILYRHE...LAYER OF HIGH END HIT
C IWRHE....WIRE OF HIGH END HIT
C IADRL....ADDRESS OF LAST ENCOUNTERED HIT IN CHAIN
C IADRN....ADDRESS OF NEXT HIT IN CHAIN
C IADRT....ADDRESS OF TEST HIT,                      SF INDEXING
C ADRHCD...HIT ADDRESS OF THE TEST HIT,              CD INDEXING
C ADRHAJ...HIT ADDRESS OF HIT ADJACENT TO TEST HIT   CD INDEXING
C IADRA....ADDRESS OF ADJACENT HIT
C IADRLD...ADDRESS OF LAST HIT IN STRING OF ADJACENT HITS
C IADRF....ADDRESS OF PRIMARY FOUND HIT, SAVED DURING SEARCH FOR ADJACENT HITS
C NDOUBL...NUMBER OF ADJACENT HITS FOUND
C IPM......INDICATION THAT PLUS/MINUS DIRECTION IS STILL ACTIVE FOR ADJACENT
C JPM......NUMBER OF HITS FOUND ADJACENT IN PLUS/MINUS DIRECTION
C KDIR.....DIRECTION BEING TESTED IN ADJACENT SEARCH
C ILYRT....TEST LAYER
C ITRY.....COUNTER OF TESTED HITS IN A LAYER
C IADD.....NUMBER OF WIRES TO ADD TO GO TO NEXT TRY
C LADR.....BEGINNING MAP ADDRESS OF A LAYER
C IWRLF0...WIRE NUMBER  -LAST-FOUND    -MULTIPLIED AND RELATIVE TO 0
C IWRNF0...WIRE NUMBER  -NEXT-FOUND    -MULTIPLIED AND RELATIVE TO 0
C IW1NRX...WIRE NUMBER 1-NOW -RADIALOUT-MULTIPLIED
C IW2NRX...WIRE NUMBER 2-NOW -RADIALOUT-MULTIPLIED
C IWRNPX...WIRE NUMBER  -NOW -PREDICTED-MULTIPLIED
C IWRNPU...WIRE NUMBER  -NOW -PREDICTED-USER
C IWRNTU...WIRE NUMBER  -NOW -TEST     -USER
C IWRNFU...WIRE NUMBER  -NOW -FOUND    -USER
C NWIRTX...NUMBER OF WIRES IN LAYER, MULTIPLIED
C IDIF.....DIFFERENCE IN WIRE NUMBER
C IDRAD....RADIUS DIFFERENCE
C IRELX....TRUCATED LOSS
C J........COUNTER
C IGOT.....FLAG
C ICHNIN...INPUT LAYER NUMBER; IF=0, DO ALL
C HOWFIL...1:FILL IN CHAIN, ONLY FOR FOUND HITS, PUT IN STANDARD CHAIN, INUCHN,
C          2:FILL IN EVEN MISSING HITS, AND VDEAST, PUT IN INUEFF
C EXTRP1...NUMBER OF LAYERS TO EXTRAPOLATE INWARD
C TIMHIT...TDC TIME OF HIT
C TIMH0....T0 FOR HITS
C FIRSTH...FLAG OF FIRST HIT IN CHAIN
      INTEGER IRTRN1,IRTRN2
      INTEGER ICHAIN,FIRSTH
      INTEGER ADLESF,ILYRLE,IWRLE,LSFTMP
      INTEGER ADHESF,ILYRHE,IWRHE,ADHECD,ADDOCD
      INTEGER IADRL,IADRN,IADRA,IADRLD,IADRF
      INTEGER NDOUBL,IPM(2),JPM(2),KDIR
      INTEGER IWRLF0,IWRNF0,IW1NRX,IW2NRX
      INTEGER IWRNPX,IWRNPU,IWRNTU,IWRNFU
      INTEGER NWIRTX,IDIF,IDRAD,IRELX,J,IGOT
      INTEGER IADRT,ILYRT,ITRY,IADD,LADR
      INTEGER ADRHCD,ADRHAJ(-5:5)
      INTEGER ICHNIN,HOWFIL,EXTRP1
      INTEGER TIMHIT(2)
      INTEGER TIMH0/1600/
C----------------------Executable code starts here---------------------
      IF(HOWFIL.EQ.1)GO TO 111
      IF(HOWFIL.EQ.2)GO TO 111
      GO TO 397
111   CONTINUE
      IF(NCHAIN.LT.1)GO TO 397

      sf_data_input_flag=1
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

C start loop over all chains, or obtain chain number from input

      IF(ICHNIN.EQ.0)THEN
          ICHAIN=1
      ELSE
          ICHAIN=ICHNIN
      ENDIF

C locate first hit
C and check for double hits for the first hit

 121  ADHESF=LINCHN(ICHAIN)
      FIRSTH=1

C      PRINT 1006,ICHAIN,HOWFIL,ADHESF
1006  FORMAT(' SFFILL: STARTING CHAIN #',I7,' HOWFIL=',I2,
     1            ' FIRST CELL-ID=',I7)

C>>>execute procedure to test double hits
C   return is based on value of FIRSTH
      GO TO 801

C transfer the last hit to the "LOW END" position and get the next hit

131   FIRSTH=0
      ADLESF=ADHESF
      ILYRLE=ILYRHE
      IWRLE=IWRHE
      ADHESF=ISTFOR(ADHESF)

C double hit test is performed on "HE";
C do not execute if there are no more hits
      IF(ADHESF.EQ.0)GO TO 159
C>>>execute procedure to test double hits
C   return is based on value of FIRSTH
      GO TO 801
159   CONTINUE

C LOAD "LAST" AND "NEXT" HIT COUNTERS FOR FILLING
      IADRL=ADLESF
      IADRN=ADHESF

C CAN NOW TEST EAST VD, FILL INUEFF
C WILL COME HERE FOR EVERY HIT, STORED IN "LE"
C ON THE LAST HIT, "HE" WILL BE SET TO =0 AND "LE" WILL
C STORE THE LAST HIT
C THIS IS NOT NEEDED EXECPT IN SFDIAG

C>>execute procedure to test the EAST VD
C  return is based on IRTRN2
#if defined(CLEO_SFDIAG)
      ILYRT=ILYRLE
      IRTRN2=1
      GO TO 851
169   CONTINUE
#endif

C THIS TIME, REALLY LEAVE WHEN DONE
      IF(ADHESF.EQ.0)GO TO 395
C CHECK FOR GAP
      IF(ILYRHE.EQ.ILYRLE+1)GO TO 131

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C HERE, THERE IS A GAP BETWEEN ILYRLE AND ILYRHE
C THIS CODE ADAPTED FROM SFMERG
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      IWRLF0=IWRLE*MULST+OFFSF(ILYRLE)
      IWRNF0=IWRHE*MULST+OFFSF(ILYRHE)
      ILYRT=ILYRLE
341   ILYRT=ILYRT+1
      IF(ILYRT.EQ.ILYRHE)GO TO 131
      IW1NRX=IWRLF0*NWRSF(ILYRT)/NWRSF(ILYRLE)-OFFSF(ILYRT)
      NWIRTX=NWRSF(ILYRT)*MULST
      IF(IW1NRX.LT.0)IW1NRX=IW1NRX+NWIRTX
      IF(IW1NRX.GE.NWIRTX)IW1NRX=IW1NRX-NWIRTX
      IW2NRX=IWRNF0*NWRSF(ILYRT)/NWRSF(ILYRHE)-OFFSF(ILYRT)
      IDIF=IW2NRX-IW1NRX
      IF(IDIF.LT.-NWIRTX/2)IW2NRX=IW2NRX+NWIRTX
      IF(IDIF.GE. NWIRTX/2)IW2NRX=IW2NRX-NWIRTX
      IDRAD=RADSF(ILYRHE)-RADSF(ILYRLE)

      IF    (IW2NRX.LT.IW1NRX)THEN
          IWRNPX=( (RADSF(ILYRHE)-RADSF(ILYRT))
     1            *(IW1NRX-IW2NRX) )/IDRAD  +IW2NRX
      ELSEIF(IW2NRX.GT.IW1NRX)THEN
          IWRNPX=( (RADSF(ILYRT)-RADSF(ILYRLE))
     1            *(IW2NRX-IW1NRX) )/IDRAD  +IW1NRX
      ELSE
          IWRNPX=IW1NRX
      ENDIF

      IWRNPU=(IWRNPX+MULSTH)/MULST
      IF(IWRNPU.LT.0)IWRNPU=IWRNPU+NWRSF(ILYRT)
      IF(IWRNPU.GE.NWRSF(ILYRT))IWRNPU=IWRNPU-NWRSF(ILYRT)

#if defined(CLEO_SFDIAG)
C      PRINT 1003,ILYRT,ILYRLE,ILYRHE,IW1NRX,IW2NRX,IWRNPX,IWRNPU
1003  FORMAT(
     1' SFFILL: WORKIN ON LAYER ',I4,'  (BETWEEN',I4,' AND',I4,' )'/
     2'                                IW1NRX=',I6/
     3'                                IW2NRX=',I6/
     4'                                IWRNPX=',I6/
     5'                                IWRNPU=',I4)
#endif

C>>execute procedure to search for hit
C  return is based on IRTRN1
      IRTRN1=1
      GO TO 501
359   CONTINUE

C loop back for next intermediate layer      
      GO TO 341


C end loop over chains
395   CONTINUE
      IF(ICHNIN.NE.0)GO TO 397
      IF(ICHAIN.EQ.NCHAIN)GO TO 397
      ICHAIN=ICHAIN+1
      GO TO 121
397   CONTINUE

      sf_data_input_flag=0
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

      RETURN


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C 
C PROCEDURE TO SEARCH FOR THE PREDICTED HIT 
C 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C THIS CODE ADAPTED FROM SFCHAN

501   CONTINUE
      IRELX=IBITS(IWRNPX,0,10)
      IADD=-1
      IF(IRELX.GT.MULSTH)IADD=1
      LADR=INSFSF(ILYRT)

C first try
      IWRNTU=IWRNPU
      ITRY=1
      IGOT=-1

511   IADRT=LADR+IWRNTU

#if defined(CLEO_SFDIAG)
      DO 513 J=-5,5

      sf_data_input_sf_adr=IADRT+J
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHAJ(J)=sf_data_output_hit_adr

513   CONTINUE

C      PRINT 1004,IWRNTU,IADRT,
C     3          (ADRHAJ(J)      ,J=-5,5),
C     1          (INUCHN(IADRT+J),J=-5,5),
C     2          (INUEFF(IADRT+J),J=-5,5)
1004  FORMAT(' SFFILL: IWRNTU=',I6,' IADRT=',I6/
     3       '         "MAP":  ',11I5/
     1       '        INUCHN:  ',11I5/
     2       '        INUEFF:  ',11I5)
#endif

      sf_data_input_sf_adr=IADRT
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHCD=sf_data_output_hit_adr

      IF(ADRHCD.LE.0)GO TO 535

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C FOUND IT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C demand not previously used, DO NOT delete this requirement
      IF(INUCHN(IADRT).GT. 0)GO TO 599

C demand dood hit unused by anything (NOT ACTIVE)
C      IF(INUCHN(IADRT).NE.0)GO TO 599
C demand time cut, (NOT ACTIVE; high time ok)
C      IF(INUCHN(IADRT).EQ.-1)GO TO 599
C demand good pulse height (not pedistal)
C in CLEO3, PH is required to be between PULS_HT_L and PULS_HT_H
C  (usually set as 1. to 999999999.)
C but allow low pulse height for HOWFIL=2
      IF((HOWFIL.EQ.1).AND.(INUCHN(IADRT).EQ.-2))GO TO 599
C demand not used in DUET, or not adjacent to that used in DUET
      IF(INUCHN(IADRT).EQ.-3)GO TO 599
      IF(INUCHN(IADRT).EQ.-4)GO TO 599

C demand not assigned to another chain in SFFILL
#if defined(CLEO_SFDIAG)
      IF(INUEFF(IADRT).NE.0)GO TO 599
#endif

      IGOT=+1
      GO TO 551


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C DIDNT FIND IT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C try another hit
535   IF(ITRY.EQ.3)GO TO 539
      ITRY=ITRY+1
      IF(ITRY.EQ.2)IWRNTU=IWRNPU+IADD
      IF(ITRY.EQ.3)IWRNTU=IWRNPU-IADD
      IF(IWRNTU.LT.0           )IWRNTU=IWRNTU+NWRSF(ILYRT)
      IF(IWRNTU.GE.NWRSF(ILYRT))IWRNTU=IWRNTU-NWRSF(ILYRT)
      GO TO 511

C found no hit within window
539   CONTINUE

#if !defined(CLEO_SFDIAG)
C skip this layer
      GO TO 599
#endif

#if defined(CLEO_SFDIAG)
C put hit in INUCHN for HOWFIL=2
      IF(HOWFIL.NE.2)GO TO 599
C use first predicted hit to fill dummy hit
      IWRNTU=IWRNPU
      IADRT=LADR+IWRNPU
C do not allow two chains to claim the same hit
      IF(INUEFF(IADRT).NE.0)GO TO 599
#endif


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C IT IS AVAILABLE, EITHER FOUND OR NOT FOUND
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C adjust the link list
551   ISTFOR(IADRL)=IADRT
      ISTBAK(IADRN)=IADRT
      ISTFOR(IADRT)=IADRN
      ISTBAK(IADRT)=IADRL
C    ##DO NOT YET SET IADRL=IADRT

C for HOWFIL=1, increment chain length and save in INUCHN 
      IF(HOWFIL.EQ.1)THEN
            LENCHN(ICHAIN)=LENCHN(ICHAIN)+1
            INUCHN(IADRT)=ICHAIN
      ELSE

#if defined(CLEO_SFDIAG)
C do not increment chain length for hits stored in INUEFF but not INUCHN 
            INUEFF(IADRT)=IGOT*ICHAIN
#endif
C have created a hit at IADRT; save the hit in the list of hits
            IF((IGOT.EQ.-1).AND.(NLSTSF.LT.MLSTSF))THEN
                NLSTSF=NLSTSF+1
                LSTCEL(NLSTSF)=IADRT
            ENDIF
      ENDIF

555   CONTINUE

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C CHECK FOR DOUBLE HITS
C THIS IS DIFFERENT THAN IN SFCHAN: ALLOWS STRINGS
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C hold starting point
      IADRF=IADRT
      IADRLD=IADRT
      IWRNFU=IWRNTU
      IF(IGOT.NE.1)GO TO 589

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
561   IF(IPM(KDIR).EQ.1)GO TO 563
      IF(IPM(3-KDIR).EQ.0)GO TO 589
      KDIR=3-KDIR

C limit width in each direction
563   IF(JPM(KDIR).GT.3)GO TO 575
      JPM(KDIR)=JPM(KDIR)+1
      IWRNTU=IWRNFU -(-1)**KDIR *JPM(KDIR)
      IF(IWRNTU.LT.0           )IWRNTU=IWRNTU+NWRSF(ILYRT)
      IF(IWRNTU.GE.NWRSF(ILYRT))IWRNTU=IWRNTU-NWRSF(ILYRT)
      IADRT=LADR+IWRNTU

      sf_data_input_sf_adr=IADRT
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHCD=sf_data_output_hit_adr

      IF(ADRHCD.LE.0)GO TO 571

C only save one double hit
      IF(NDOUBL.GE.1)GO TO 567
      IF(HOWFIL.EQ.2)GO TO 565

C when HOWFIL=1, must test the possible double hit 
C follow test made in SFCHAN
C BAD HIT; do not save but continue search of string to maximum width
      IF(INUCHN(IADRT).LT.0)GO TO 567
C USED HIT;do not save but continue search of string to maximum width
      IF(INUCHN(IADRT).GT.0)GO TO 567
C not strictly adjacent, do not save
      IF(JPM(KDIR).NE.1)GO TO 567
      INUCHN(IADRT)=ICHAIN

C resume common part
565   NDOUBL=NDOUBL+1
      DOUBPT(IADRLD)=IADRT
      IADRLD=IADRT

C WHEN HOWFIL=2, LABEL DOUBLE HIT BUT NOT INUEFF OR INUCHN
C WHEN HOWFIL=2, DO NOT EXTEND SEARCH FOR EXTRA LONG CHAINS
C WHEN HOWFIL=2, DO NOT CARE IF USED, JUST STOP HERE
      IF(HOWFIL.EQ.2)GO TO 589

C found double hit, flip direction and continue
567   KDIR=3-KDIR
      GO TO 561

C no hit found
571   IPM(KDIR)=0
      GO TO 567

C too wire, kill everything
575   IADRLD=IADRF
577   IADRT=DOUBPT(IADRLD)
      IF(IADRT.LE.0)GO TO 579
      DOUBPT(IADRLD)=0
      IF(HOWFIL.EQ.1)INUCHN(IADRT)=0
      IADRLD=IADRT
      GO TO 577
579   ISTFOR(IADRL)=IADRN
      ISTBAK(IADRN)=IADRL
      IF(HOWFIL.EQ.2)GO TO 581
      INUCHN(IADRF)=0
      LENCHN(ICHAIN)=LENCHN(ICHAIN)-1
      GO TO 595
581   CONTINUE

#if defined(CLEO_SFDIAG)
      IF(HOWFIL.EQ.2)INUEFF(IADRF)=0
#endif
      GO TO 595

C end; allowing any double hit to remain
589   CONTINUE
      IF(DOUBPT(IADRF).EQ.0)SNGCHN(ICHAIN)=SNGCHN(ICHAIN)+1
      IADRL=IADRF
595   CONTINUE

C>>execute procedure to test the EAST VD
C  return is based on IRTRN2
#if defined(CLEO_SFDIAG)
      IRTRN2=2
      GO TO 851
597   CONTINUE
#endif

599   CONTINUE
      IF(IRTRN1.EQ.1)GO TO 359
      GO TO 359

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C 
C PROCEDURE TO CHECK FOR POOR CHOICE OUT OF A DOUBLE HIT 
C 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
801   CONTINUE

C this would be very dangerous if, in an earlier call, it had chosen
C to assign the same adjacent hit to two chains; do not flip for
C HOWFIL=1 AND do not assign the same adjacent hit in HOWFIL=1

      IF(HOWFIL.EQ.2)THEN

C this is done for the "HIGH END" before using for interpolation
C special routing here for the first hit
C this stuff is done for every hit; before filling in between hits

         IF(DOUBPT(ADHESF).GT.0)THEN

C select the low time for double hits
            LSFTMP=SFLSFA(ADHESF)
            ADHECD=ADHESF-INSFSF(LSFTMP)+INCDSF(LSFTMP)

            LSFTMP=SFLSFA(DOUBPT(ADHESF))
            ADDOCD=DOUBPT(ADHESF)-INSFSF(LSFTMP)+INCDSF(LSFTMP)

      sf_data_input_hit_adr=ADHECD
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      TIMHIT(1)=sf_data_output_tdc_raw

      sf_data_input_hit_adr=ADDOCD
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      TIMHIT(2)=sf_data_output_tdc_raw

            IF(TIMHIT(1).GT.TIMHIT(2))THEN
               IADRL=ISTBAK(ADHESF)
               IADRN=ISTFOR(ADHESF)
               IADRA=DOUBPT(ADHESF)
               IF(IADRL.NE.0)ISTFOR(IADRL)=IADRA
               ISTBAK(IADRA)=IADRL
               ISTFOR(IADRA)=IADRN
               IF(IADRN.NE.0)ISTBAK(IADRN)=IADRA
               INUCHN(IADRA)=INUCHN(ADHESF)
               DOUBPT(IADRA)=ADHESF
C INUCHN(ADHESF) IS OK AS IS
               DOUBPT(ADHESF)=0
               ISTBAK(ADHESF)=0
               ISTFOR(ADHESF)=0
               ADHESF=IADRA
               IF(FIRSTH.EQ.1)LINCHN(ICHAIN)=IADRA
               IF(IADRN.EQ.0) MINCHN(ICHAIN)=IADRA
C end of requirement: TIMHIT(1).GT.TIMHIT(2)
            ENDIF

C end of requirement: existence of double hit
         ENDIF

#if defined(CLEO_SFDIAG)
C start filling the efficiency array
C do not fill the double hit
         INUEFF(ADHESF)=INUCHN(ADHESF)
#endif

C end of requirement: HOWFIL=2
      ENDIF

C load the layer and wire number
      ILYRHE=SFLSFA(ADHESF)
      IWRHE=SFWSFA(ADHESF)

      IF(FIRSTH.EQ.1)GO TO 131
      GO TO 159
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C 
C PROCEDURE TO TEST EAST VD
C 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

C WILL COME HERE FOR EVERY HIT, STORED IN "LE"
C ON THE LAST HIT, "HE" WILL BE SET TO =0 AND "LE" WILL
C STORE THE LAST HIT
C THIS IS NOT NEEDED EXECPT IN SFDIAG, FILL INUEFF
#if defined(CLEO_SFDIAG)
851   IF(HOWFIL.EQ.1)GO TO 859
      IF(ILYRT.LE.MLSFPT)GO TO 859
      IF(ILYRT.GT.MLSFVD)GO TO 859
      IADRT=IADRL+NWRSF(ILYRT)
      IF(INUEFF(IADRT).NE.0)GO TO 859
      IGOT=-1

      sf_data_input_sf_adr=IADRT
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHCD=sf_data_output_hit_adr

      IF(ADRHCD.GT.0)IGOT=+1
      ISTFOR(IADRL)=IADRT
      ISTFOR(IADRT)=IADRN
      IF(IADRN.NE.0)THEN
          ISTBAK(IADRN)=IADRT
      ELSE
          MINCHN(ICHAIN)=IADRT
      ENDIF
      ISTBAK(IADRT)=IADRL
      INUEFF(IADRT)=IGOT*ICHAIN

C have created a hit at IADRT; save the hit in the list of hits
          IF((IGOT.EQ.-1).AND.(NLSTSF.LT.MLSTSF))THEN
              NLSTSF=NLSTSF+1
              LSTCEL(NLSTSF)=IADRT
          ENDIF

      IADRL=IADRT

C      PRINT 1005,ILYRLE,IADRT,ISTBAK(IADRT),ISTFOR(IADRT)
1005  FORMAT(' SFFILL: DOING VDEAST ON LAYER',I7,
     1      ' ADDED CELL-ID',I6,' BETWEEN',I7,' AND',I7)

859   CONTINUE

      IF(IRTRN2.EQ.1)GO TO 169
      GO TO 597
#endif
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      END






