*
* $Id: sfhits.F,v 1.3 2000/01/24 19:58:29 dpp Exp $
*
* $Log: sfhits.F,v $
* Revision 1.3  2000/01/24 19:58:29  dpp
*      -> exclude hits with INUCHN=-1
*
* Revision 1.2  2000/01/06 22:08:50  dpp
* F77 modifications
* fill in layers missing from SF link list
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
* Revision 1.4  1996/01/25 15:52:07  zfiles
* calls SFHITT
* arguments include hit status
* *real-change-in-output* allow isolated double hits
* *real-change-in-output* do not mark hits in wings as usable in the
*   case where the SF hit is not a doit hit
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE SFHITS
C.......................................................................
C.
C. SFHITS - find the hits in a road about the SFIND track
C.
C. COMMON    : ?
C. CALLS     : SFHITT, MESLOG
C. CALLED    : Only by SFFTRK
C. AUTHOR    : by D. Peterson
C.
C. VERSION   : 2.00
C. CREATED   : ?
C. LAST MOD  : 28-10-97
C.
C. Modification Log.
C. 28-JULY-88 : Changed to run from pointers from SFIND
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE


* HWSTAT - hit status, 0=isolated, 1=isolated doublet, 2=not isolated 

#include "doit/duseq/tfindpar.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfhitscd.inc"
#include "seq/clutil/mesrep.inc"

C== Local variables


C SFADR_LAST....last TRUE encoutered SF address
C SFADR_CURR....current found or projected SF address
C SFADR_NEXT....next TRUE SF address, from the link list
C SFADR_DOUB....SF address of a double hit
C SFADR_TEMP....SF address of hit within test of hits in TF layer      
C FILLING_IN....=.TRUE. if filling in hits between hits in the link list 
      INTEGER SFADR_LAST,SFADR_CURR,SFADR_NEXT,SFADR_DOUB,SFADR_TEMP
      LOGICAL FILLING_IN

C TFADR.........TF  cell address
      INTEGER TFADR

C LYRSF_LAST....SF layer of last TRUE SF address
C LYRSF_CURR....SF layer of current SF address
C LYRSF_TEST....SF layer tested for hits in "filling in" mode
C LYRSF_NEXT....SF layer of next SF address
C LYRSF_DOUB....SF layer of address of double hit
      INTEGER LYRSF_LAST,LYRSF_CURR,LYRSF_TEST,LYRSF_NEXT,LYRSF_DOUB
      INTEGER LYRSF_TEMP
C LYRCD.........CD layer
C LYRTF.........TF layer
      INTEGER LYRCD,LYRTF

C ILYRL.........previous SF layer as returned by SFHITT
      INTEGER ILYRL

C WIRE_LAST.....wire number of last TRUE SF address
C WIRE_CURR.....wire number of current SF address
C WIRE_NEXT.....wire number of next SF address
C WIRE_DOUB.....wire number of a double hit
      INTEGER WIRE_LAST,WIRE_CURR,WIRE_NEXT,WIRE_DOUB

      INTEGER DIRSER,NSER,NWIR
      INTEGER WIRDIF
      INTEGER WIRSTR(-1:1),WIRTST,ISER,JSER
      INTEGER I,ICHAIN,COUNT
      INTEGER IOS
      INTEGER GOOD,PRIMRH,SNDRYH,OTHERH,LYRHTS

C GOOD_CELL.....continue with good cell in chain
C STRUCTURE_OK..found no faults in the data structure
C GOOD_CELL_D...continue with good cell (double hit) in chain 
C SF_TESTS_OK...passes certian SF tests
      LOGICAL GOOD_CELL,STRUCTURE_OK,GOOD_CELL_D,SF_TESTS_OK

C NWIRTX........number of wires in current layer, multipled
C ZNOWX.........z of chain at layer, multipled
C WLF0..........wire number, last address, multiplied, relative to 0
C WCRLXZ........wire number, currrent layer, radial out, same Z as last layer
C DIFTN.........difference in tandip of layers
C WSHFZ.........wire shift due to Z
C WCRLX.........wire number, current layer, radial out, Z corrected
C WNF0..........wire number, next address, multiplied, relative to 0
C WCRNXZ........wire number, current layer, radial out, same Z as last layer
C WCRNX.........wire number, current layer, radial in, Z corrected
C WIRE_DIF......WCRNX-WCRLX
C RAD_DIF.......radial difference between LYRSF_NEXT and LYRSF_LAST
C WNPX..........wire predicted, multiplied
      INTEGER NWIRTX,ZNOWX,DIFTN,WSHFZ
      INTEGER WLF0,WCRLXZ,WCRLX
      INTEGER WNF0,WCRNXZ,WCRNX
      INTEGER WIRE_DIF,RAD_DIF,WNPX

#if defined(CLEO_SFHITDIAG)
      INTEGER IGO,ITEMP
#endif

C---------------------------------------------------------------------
C  Executable code starts here
C---------------------------------------------------------------------


      NLYRWR = 0
      NHTWIR = 0
      ILYRL  = 0

#if defined(CLEO_SFHITDIAG)
      ITEMP=NHITDM
      PRINT 5001,ITEMP
5001  FORMAT(' SFHITS:  entry: NHITDM=',I3)
#endif


      IF(NCHANM.GT.0)THEN
C loop over the chains that were passed
         DO 79 I=1,NCHANM
            ICHAIN=JCHAIN(I)

#if defined(CLEO_SFHITDIAG)
            PRINT 5002,I,ICHAIN
 5002       FORMAT(' SFHITS: SEQ CHAIN#',I2,' CHAIN#',I3)
#endif


C loop over hits in the chain
            SFADR_LAST=LINCHN(ICHAIN)
            LYRSF_LAST=SFLSFA(SFADR_LAST)
            WIRE_LAST= SFWSFA(SFADR_LAST)

            FILLING_IN=.FALSE.
            SFADR_CURR=SFADR_LAST
            LYRSF_CURR=LYRSF_LAST
            WIRE_CURR= WIRE_LAST

            COUNT=0
 11         COUNT=COUNT+1

C continue only if current address is non-zero
            IF((SFADR_CURR.GT.0).AND.(LYRSF_CURR.GT.0))THEN

C do not use east VD
               GOOD_CELL=(WIRE_CURR.LT.NWRSF(LYRSF_CURR))

C continue only for good cells
               IF(GOOD_CELL)THEN

C get next ADDRESS,LAYER,WIRE in a chain only if not in "filling in" mode
C and if not at end of chain
                  IF(FILLING_IN)THEN
                     STRUCTURE_OK=.TRUE.
                  ELSE
                     IF(SFADR_CURR.EQ.MINCHN(ICHAIN))THEN
                        SFADR_NEXT=0
                        STRUCTURE_OK=.TRUE.
                     ELSE
                        SFADR_NEXT=ISTFOR(SFADR_CURR)
                        IF(SFADR_NEXT.NE.0)THEN
                           LYRSF_NEXT=SFLSFA(SFADR_NEXT)
                           WIRE_NEXT= SFWSFA(SFADR_NEXT)
                  
C internal test of data structure
                           IF(SFADR_NEXT.GT.SFADR_CURR)THEN
                              STRUCTURE_OK=.TRUE.
                           ELSE
                              STRUCTURE_OK=.FALSE.

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C error messages if data structure is corrupted
                              WRITE(CHMESS,9901,IOSTAT=IOS)
     1                             NRUN,NEVENT,ICHAIN,
     2                             LINCHN(ICHAIN),MINCHN(ICHAIN),
     3                             LENCHN(ICHAIN),
     4                             SFADR_LAST,SFADR_CURR,SFADR_NEXT
 9901                         FORMAT(
     1                             ' EV=',I6,'/',I6,' CH',I3,
     2                             ' L=',I5,' M=',I5,
     3                             ' L=',I3,
     4                             ' AL=',I5,' A=',I5,' AN=',I5)
                              CALL MESLOG(' SFHITS',1,MSERRO)
#if defined(CLEO_SFHITDIAG)
                              PRINT 5011
                              PRINT 5012
                              PRINT 5013
                              PRINT 5014
                              PRINT 5015
 5011                         FORMAT(' SFHITS: SSS TTT U  U  CC  K  K')
 5012                         FORMAT(' SFHITS:S     T  U  U C  C K K ')
 5013                         FORMAT(' SFHITS: SS   T  U  U C    KK  ')
 5014                         FORMAT(' SFHITS:   S  T  U  U C  C K K ')
 5015                         FORMAT(' SFHITS:SSS   T   UU   CC  K  K')
                              READ 5016,IGO
 5016                         FORMAT(A1)
#endif

C kill all hits collected so far, try the next chain
                              NHTWIR=0
C end of error handling
                           ENDIF
C end of "found next hit with address=0 anyway" condition
                        ENDIF
C end "not end of chain recognized by MINCHN" condition
                     ENDIF
C end of "not filling in" condition
                  ENDIF
C end of the "good cell" condition
               ENDIF

C continue only for good cells and no errors in the data structure
               IF(GOOD_CELL.AND.STRUCTURE_OK)THEN

C Note: as part of the effort to eliminate TF references from the standard
C  SF routines, the is no defined "INTFSF" ie address in TF map of SF layer
C  hence must convert to CD layer and then to TF layer

                  LYRCD=CDLASF(LYRSF_CURR)
                  LYRTF=ILCDTF(LYRCD)
                  NWIR=NWIRTF(LYRTF)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
#if defined(CLEO_SFHITDIAG)
                  PRINT 5003,COUNT,SFADR_CURR,
     2                 LYRTF,WIRE_CURR,NSER,NWIR,NHTWIR
 5003             FORMAT(
     1                 ' SFHITS:********** SEQ HIT',I3,'  SFADR',I5,
     2                 ' TFLYR=',I3,' WIRE#',I3,' SEARCHWINDOW',I3,
     3                 '  NWIR=',I3,' NHTWIR=',I5)
#endif

C MUST TEST PRIMARY AND DOUBLE

C continue only if the hit buffers are not saturated
                  IF(NHTWIR.LT.NHITDM)THEN

C test pimary hit
                     LYRHTS=0
                     PRIMRH=0
                     WIRSTR(-1)=WIRE_CURR
                     WIRSTR( 1)=WIRE_CURR

C test the hit quality
                     TFADR = INDXTF(LYRTF) + WIRE_CURR
                     ISER=0
                     CALL SFHITT(TFADR,LYRSF_CURR,ILYRL,
     2                    LYRTF,ISER,WIRE_CURR,GOOD)
                     IF(GOOD.EQ.1)THEN
                        PRIMRH=NHTWIR
                        HWSTAT(PRIMRH)=0
                     ENDIF

C end of "buffer not saturated" condition
C continue only if the hit buffers are not saturated
                  ENDIF
                  IF(NHTWIR.LT.NHITDM)THEN
                     
C test for double hits
                     SNDRYH=0
                     IF(.NOT.FILLING_IN)THEN
                        SFADR_DOUB=DOUBPT(SFADR_CURR)
                        IF(SFADR_DOUB.NE.0)THEN
                           LYRSF_DOUB=SFLSFA(SFADR_DOUB)
                           WIRE_DOUB=SFWSFA(SFADR_DOUB)
                        
C adjust the start of the search window
                           WIRDIF=WIRE_DOUB-WIRE_CURR
                           IF(WIRDIF.LT.-NWIR/2)WIRDIF=WIRDIF+NWIR
                           IF(WIRDIF.GE. NWIR/2)WIRDIF=WIRDIF-NWIR
                           IF    (WIRDIF.EQ.-1)THEN
                              WIRSTR(-1)=WIRE_DOUB
                              GOOD_CELL_D=.TRUE.
                           ELSEIF(WIRDIF.EQ. 1)THEN
                              WIRSTR( 1)=WIRE_DOUB
                              GOOD_CELL_D=.TRUE.
                           ELSE
                              GOOD_CELL_D=.FALSE.
                           ENDIF      

C test the hit quality and verify that the double hit is adjacent
                           IF(GOOD_CELL_D)THEN
                              TFADR = INDXTF(LYRTF) + WIRE_DOUB
                              CALL SFHITT(TFADR,LYRSF_DOUB,ILYRL,
     2                             LYRTF,ISER,WIRE_DOUB,GOOD)
                              IF(GOOD.EQ.1)THEN
                                 SNDRYH=NHTWIR
                                 HWSTAT(NHTWIR)=0 
                                 IF(PRIMRH.NE.0)THEN
                                    HWSTAT(PRIMRH)=1
                                    HWSTAT(NHTWIR )=1
                                 ENDIF
                              ENDIF
                           ENDIF
C end of "double hit" condition
                        ENDIF
C end of the "not filling in" condition
                     ENDIF

C end of "buffer not saturated" condition
C continue only if the hit buffers are not saturated
                  ENDIF
                  IF(NHTWIR.LT.NHITDM)THEN

                     NSER=NSRWTF(LYRTF)

C loop over directions of the search window
                     DO 49 DIRSER=-1,1,2
                        ISER  =0

C loop over the search window
                        DO 47 JSER = 1,NSER
                           IF(NHTWIR.LT.NHITDM)THEN
                              ISER    = ISER   + DIRSER
                              WIRTST  = WIRSTR(DIRSER)+ISER
                              IF(WIRTST.LT.   0)WIRTST=WIRTST+NWIR
                              IF(WIRTST.GE.NWIR)WIRTST=WIRTST-NWIR

C test if hit is excluded from SF
                              SFADR_TEMP=INSFSF(LYRSF_CURR)+WIRTST
                              IF(INUCHN(SFADR_TEMP).EQ.-1)THEN
                                 SF_TESTS_OK=.FALSE.
                              ELSE
                                 SF_TESTS_OK=.TRUE.
                              ENDIF

C test the hit quality
                              IF(SF_TESTS_OK)THEN
                                 TFADR = INDXTF(LYRTF) + WIRTST
                                 CALL SFHITT(TFADR,LYRSF_CURR,ILYRL,
     2                                LYRTF,ISER,WIRTST,GOOD)
                                 IF(GOOD.EQ.1)THEN
                                    HWSTAT(NHTWIR)=2
                                    IF(PRIMRH.NE.0)THEN
                                       HWSTAT(PRIMRH)=2
                                       HWSTAT(NHTWIR )=2
                                    ENDIF
                                    IF(SNDRYH.NE.0)THEN
                                       HWSTAT(SNDRYH)=2
                                       HWSTAT(NHTWIR )=2
                                    ENDIF

#if defined(CLEO_SFHITDIAG)
                                    PRINT 5005,
     2                                   NLYRWR,NHTWIR,HTWADR(NHTWIR),
     3                                   NHTWIR,HWSTAT(NHTWIR)
#endif
                                 ENDIF

C end of SF quality tests
                              ENDIF

C end of "buffer not saturated" condition (within the loop)
                           ENDIF

C end of loop over search window
 47                     CONTINUE

C end of loop over direction of search window
 49                  CONTINUE

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C print out the info
#if defined(CLEO_SFHITDIAG)
                     IF(PRIMRH.NE.0)THEN
                        PRINT 5005,
     2                       NLYRWR,PRIMRH,HTWADR(PRIMRH),
     3                       PRIMRH,HWSTAT(PRIMRH)
                     ENDIF
                     IF(SNDRYH.NE.0)THEN
                        PRINT 5005,NLYRWR,SNDRYH,HTWADR(SNDRYH),
     2                       SNDRYH,HWSTAT(SNDRYH)
 5005                   FORMAT(
     1                       ' SFHITS:layerswithhits=',I3,
     2                       ' HTWADR(',I3,')=',I5,
     3                       ' HWSTAT(',I3,')=',I5 )
                     ENDIF
#endif

C end of "buffer not saturated" condition
                  ENDIF

C loop back to process next wire in chain
                  LYRSF_TEST=LYRSF_CURR+1
                  FILLING_IN=((LYRSF_TEST).LT.LYRSF_NEXT)
C                  FILLING_IN=.FALSE.

C fill in the missing hits
                  IF(FILLING_IN)THEN
                     LYRSF_CURR=LYRSF_TEST
                     NWIRTX=NWRSSF(LYRSF_CURR)*MULST
                     ZNOWX=ZR0CHN(ICHAIN)
     2                    +ZDRCHN(ICHAIN)*RADSSF(LYRSF_CURR)/100/MULST

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C code adapted from SFMERG
                     WLF0=WIRE_LAST*MULST+OFFSSF(LYRSF_LAST)
                     WCRLXZ=WLF0*NWRSSF(LYRSF_CURR)/NWRSSF(LYRSF_LAST)
     2                    -OFFSSF(LYRSF_CURR)

                     DIFTN=TANSSF(LYRSF_CURR)-TANSSF(LYRSF_LAST)
                     WSHFZ=ZNOWX*DIFTN/MULST
                     WCRLX=WCRLXZ+WSHFZ

                     IF(WCRLX.LT.0     )WCRLX=WCRLX+NWIRTX
                     IF(WCRLX.GE.NWIRTX)WCRLX=WCRLX-NWIRTX
                     
                     WNF0=WIRE_NEXT*MULST+OFFSSF(LYRSF_NEXT)
                     WCRNXZ=WNF0*NWRSSF(LYRSF_CURR)/NWRSSF(LYRSF_NEXT)
     2                    -OFFSSF(LYRSF_CURR)

                     DIFTN=TANSSF(LYRSF_CURR)-TANSSF(LYRSF_NEXT)
                     WSHFZ=ZNOWX*DIFTN/MULST
                     WCRNX=WCRNXZ+WSHFZ

                     WIRE_DIF=WCRNX-WCRLX
                     IF(WIRE_DIF.LT.-NWIRTX/2)WCRNX=WCRNX+NWIRTX
                     IF(WIRE_DIF.GE. NWIRTX/2)WCRNX=WCRNX-NWIRTX
                     
                     RAD_DIF=RADSSF(LYRSF_NEXT)-RADSSF(LYRSF_LAST)
                     
                     IF    (WCRNX.LT.WCRLX)THEN
                        WNPX=( (RADSSF(LYRSF_NEXT)-RADSSF(LYRSF_CURR))
     1                       *(WCRLX-WCRNX) )/RAD_DIF  +WCRNX
                     ELSEIF(WCRNX.GT.WCRLX)THEN
                        WNPX=( (RADSSF(LYRSF_CURR)-RADSSF(LYRSF_LAST))
     1                       *(WCRNX-WCRLX) )/RAD_DIF  +WCRLX
                     ELSE
                        WNPX=WCRLX
                     ENDIF
C end of code adapted from SFMERG

                     WNPX=WNPX+MULSTH
                     IF(WNPX.LT.     0)WNPX=WNPX+NWIRTX
                     IF(WNPX.GE.NWIRTX)WNPX=WNPX-NWIRTX

                     WIRE_CURR=WNPX/MULST
                     SFADR_CURR=INSFSF(LYRSF_CURR)+WIRE_CURR

#if defined(CLEO_SFHITDIAG)
                     PRINT 5006,
     2                    LYRSF_LAST,LYRSF_CURR,LYRSF_NEXT,
     3                    NWRSSF(LYRSF_CURR),ZNOWX,
     4                    WCRLX,WCRNX,WNPX,WIRE_CURR
 5006                FORMAT(
     2                    ' SFHITS: filling_in:',
     3                    ' LL=',I2,' LC=',I2,' LN=',I2,
     4                    ' nw=',I3,' Z=',I7,
     5                    ' WradOUT=',I7,' WradIN=',I7,
     6                    ' Wpred=',I7,' (=',I4,' )' )
#endif
C go to the next element in the the link list 
C and set the current hit to that element
                  ELSE
                     SFADR_LAST=SFADR_NEXT
                     SFADR_CURR=SFADR_NEXT
                     LYRSF_LAST=LYRSF_NEXT
                     LYRSF_CURR=LYRSF_NEXT
                     WIRE_LAST= WIRE_NEXT
                     WIRE_CURR= WIRE_NEXT
                  ENDIF
                  GO TO 11
C end of the "good cell" and "no error in data structure" condition
               ENDIF

C end of the "non-zero cell" condition, this is end of look over cells
            ENDIF

C end loop over input chains
 79      CONTINUE

C break point for no input chains
      ENDIF
      RETURN
      END







