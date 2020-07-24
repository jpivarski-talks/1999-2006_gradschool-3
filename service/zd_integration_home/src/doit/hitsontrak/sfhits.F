*
* $Id: sfhits.F,v 1.5 2003/03/05 17:58:36 dpp Exp $
*
* $Log: sfhits.F,v $
* Revision 1.5  2003/03/05 17:58:36  dpp
*      -> new assignments for negative INUCHN
*      -> do not test non-clustered hits for noise in layer
*      -> do test clustered, but not peak, hits for noise in layer
*         but do not count them as noise if the cluster number matches
*      -> changed all variable names in cdscrtcd to have common root
*      -> use sf_data_access instead of direct access of cdscrtcd
*         this allows diresct access to the selected hit on multi-hit pads
*      -> change the name of the hit level arrays again, to RoadWirLst_
*      -> remove lyrsf_prev from sfhitt argument list;
*         zero with initialization call
*      -> implement the intermediate chain buffer for z slices
*      -> change sfhitt output code to logical
*
* Revision 1.4  2002/09/05 18:58:36  dpp
*      -> comments
*      -> remove local ICHAIN
*      -> change names of wire-level arrays to avoid confusion
*      -> argument to skip the hits not defined by SF
*      -> diagnostic formating
*      -> allow wider hit string based on calculated curvature of chain
*         change and expansion in the use of status
*
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
      SUBROUTINE SFHITS(SFHITS_CONTROL)
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


* RoadWirLst_stat - wire level status,
*                         0=isolated, 
*                         1=part of isolated doublet,
*                         2=not isolated only because it is near top of loop
*                         3=not isolated part of original chain
*                         4=noise found in road 

C----------
C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom, and cdscrtcd (if needed)
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"
C----------


#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "seq/clutil/mesrep.inc"

C== Argument

C SFHITS_CONTROL..SFHITS_ONLY_SF: select the wires from SF only
C                 SFHITS_FULL:    do the fill in and check oter hits in layer
                     INTEGER SFHITS_CONTROL
C== Local variables

C USE_SFChanList.....use the SFChanList_ intermediate chain list for 
C                    multiple z slices 
                     LOGICAL USE_SFChanList
C ON_LAST_HIT........indicates the current hit is the last hit of the chain
                     LOGICAL ON_LAST_HIT 
C ListLoc_LAST.......the last location in the SFChanList_SFid buffer
                     INTEGER ListLoc_LAST 
C ListLoc_CURR.......the current location in the SFChanList_SFid buffer
                     INTEGER ListLoc_CURR 
C ListLoc_NEXT.......the next location in the SFChanList_SFid buffer
                     INTEGER ListLoc_NEXT 
C ListLoc_DOUB.......location in SFChanList_SFid buffer, testing for double hit
                     INTEGER ListLoc_DOUB
C SFADR_LAST.........last TRUE encoutered SF address
                     INTEGER SFADR_LAST
C SFADR_CURR.........current found or projected SF address
                     INTEGER SFADR_CURR
C SFADR_NEXT.........next TRUE SF address, from the link list
                     INTEGER SFADR_NEXT
C SFADR_DOUB.........SF address of a double hit
                     INTEGER SFADR_DOUB
C SFADR_TEMP.........SF address of hit within test of hits in TF layer      
                     INTEGER SFADR_TEMP
C FILLING_IN.........=.TRUE. if filling in hits between hits in the link list 
                     LOGICAL FILLING_IN
C CD_cell_id.........cell ID in CD geometry
                     INTEGER CD_cell_id
C IHIT_TEMP..........hit address returned from SF_DATA_ACCESS
                     INTEGER IHIT_TEMP
C LYRSF_LAST.........SF layer of last TRUE SF address
                     INTEGER LYRSF_LAST
C LYRSF_CURR.........SF layer of current SF address
                     INTEGER LYRSF_CURR
C LYRSF_TEST.........SF layer tested for hits in "filling in" mode
                     INTEGER LYRSF_TEST
C LYRSF_NEXT.........SF layer of next SF address
                     INTEGER LYRSF_NEXT
C LYRSF_DOUB.........SF layer of address of double hit
                     INTEGER LYRSF_DOUB
C LYRSF_TEMP.........
                     INTEGER LYRSF_TEMP
C LYRCD..............CD layer
                     INTEGER LYRCD
C LYRTF..............TF layer
                     INTEGER LYRTF
C NWIR...............number of wires in layer
                     INTEGER NWIR
C RAD_CHAIN_END......radius at the end of the chain
                     REAL RAD_CHAIN_END
C TAN_EXIT...........tangent of the exit angle at the end of the chain
                     REAL TAN_EXIT
C COS_EXIT...........cosine of the exit angle at the end of the chain
                     REAL COS_EXIT
C SIN_EXIT...........sine of the exit angle at the end of the chain
                     REAL SIN_EXIT
C ARGT...............temporary argument for series
                     REAL ARGT
C ARG1...............argument of series
                     REAL ARG1
C ARG2...............argument of series  squared
                     REAL ARG2
C ARG3...............argument of series  cubed
                     REAL ARG3
C SQRM...............some factor determined for best convergence of the series
                     REAL SQRM
C CURV_USE...........curvature to use foe entrance angle
                     REAL CURV_USE
C WIRE_LAST..........wire number of last TRUE SF address
                     INTEGER WIRE_LAST
C WIRE_CURR..........wire number of current SF address
                     INTEGER WIRE_CURR
C WIRE_NEXT..........wire number of next SF address
                     INTEGER WIRE_NEXT
C WIRE_DOUB..........wire number of a double hit
                     INTEGER WIRE_DOUB
C WIRDIF.............wire difference, for checking double hit
                     INTEGER WIRDIF
C NHIT_PREV..........RoadWirLst_Nhit from the end of the previous layer
                     INTEGER NHIT_PREV
C NHIT_INDX..........index for loop over hits accumulates in a layer
                     INTEGER NHIT_INDX
C DIRSER.............direction of search in window
                     INTEGER DIRSER
C NSER...............number of wires to search
                     INTEGER NSER
C WIRSTR.............starting point for search in window, each direction
                     INTEGER WIRSTR(-1:1)
C WIRTST_NO_MOD......wire to be tested, before adjusting to {0:NWIR-1}
                     INTEGER WIRTST_NO_MOD
C WIRTST.............wire to be tested, after adjusting to {0:NWIR-1}
                     INTEGER WIRTST

                     INTEGER ISER
                     INTEGER JSER
                     INTEGER I
                     INTEGER COUNT
                     INTEGER IOS

C GOOD_SFHITT........hit Test code from sfhitt
                     LOGICAL GOOD_SFHITT
C PRIMRH.............address of the SF primary hit in the RoadWirLst_ array
                     INTEGER PRIMRH
C SNDRYH.............address of the SF secondary hit in the RoadWirLst_ array
                     INTEGER SNDRYH
C CLUS_PRIM..........address of the cluster primary hit in the EvntHit arrays
                     INTEGER CLUS_PRIM
C CLUS_PRIM_P........address of the cluster primary hit in the EvntHit arrays
C                    of the SF primary hit
                     INTEGER CLUS_PRIM_P
C CLUS_PRIM_S........address of the cluster primary hit in the EvntHit arrays
C                    of the SF secondary hit
                     INTEGER CLUS_PRIM_S
C GOOD_CELL..........continue with good cell in chain
                     LOGICAL GOOD_CELL
C STRUCTURE_OK.......found no faults in the data structure
                     LOGICAL STRUCTURE_OK
C GOOD_CELL_D........continue with good cell (double hit) in chain 
                     LOGICAL GOOD_CELL_D
C SF_TESTS_OK........passes certian SF tests
                     LOGICAL SF_TESTS_OK
C NWIRTX.............number of wires in current layer, multipled
                     INTEGER NWIRTX
C ZNOWX..............z of chain at layer, multipled
                     INTEGER ZNOWX
C WLF0...............wire number, last address, multiplied, relative to 0
                     INTEGER WLF0
C WCRLXZ.............wire number, currrent layer, radial out, 
C                    same Z as last layer
                     INTEGER WCRLXZ
C DIFTN..............difference in tandip of layers
                     INTEGER DIFTN
C WSHFZ..............wire shift due to Z
                     INTEGER WSHFZ
C WCRLX..............wire number, current layer, radial out, Z corrected
                     INTEGER WCRLX
C WNF0...............wire number, next address, multiplied, relative to 0
                     INTEGER WNF0
C WCRNXZ.............wire number, current layer, radial out, 
C                    same Z as last layer
                     INTEGER WCRNXZ
C WCRNX..............wire number, current layer, radial in, Z corrected
                     INTEGER WCRNX
C WIRE_DIF...........WCRNX-WCRLX
                     INTEGER WIRE_DIF
C RAD_DIF............radial difference between LYRSF_NEXT and LYRSF_LAST
                     INTEGER RAD_DIF
C WNPX...............wire predicted, multiplied
                     INTEGER WNPX
C ENTR_ANG...........entrance angle based on curvature, radius
                     REAL ENTR_ANG
C CROSS_WID_MAX......crossing width maximum, 
C                    based on entrance angle and cell height
                     INTEGER CROSS_WID_MAX
C WID_HIT_STR........width of  string of hits
                     INTEGER WID_HIT_STR
C WIRE_FND_MIN.......minimum wire number found
                     INTEGER WIRE_FND_MIN
C WIRE_FND_MAX.......maximum wire number found
                     INTEGER WIRE_FND_MAX
                     
#if defined(CLEO_SFHITDIAG)
                     INTEGER IGO,IKILL
                     INTEGER ITEMP
                     
C COMMNT_HIT.........comment regarding where hit came from
                     CHARACTER*8 COMMNT_HIT(NHITDM)
#endif
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12


C-----------------------------------------------------------------------
C  Executable code starts here
C-----------------------------------------------------------------------
#if defined(CLEO_SFHITDIAG)
      IKILL=0
#endif

      USE_SFChanList=(SFZSLNWINDO.GT.1)

      RoadWirLst_Nlyr=0
      RoadWirLst_Nhit=0

C initialize SFHITT variables
      CALL SFHITT(0,   0,0,0,0,  GOOD_SFHITT)
      
      
#if defined(CLEO_SFHITDIAG)
      ITEMP=NHITDM
      PRINT 5001,ITEMP,CURCS,PHI0CS,D0CS
 5001 FORMAT(' SFHITS:  entry: NHITDM=',I8,
     2     ' CURCS=',F9.6,' PHI0CS=',F9.6,' D0CS=',F9.6)
#endif
      
      
C-----------------------------------------------------------------------
C loop over the chains that were passed
C-----------------------------------------------------------------------

      IF(NCHANM.GT.0)THEN
        DO 79 I=1,NCHANM
          ICHAIN=JCHAIN(I)
          
#if defined(CLEO_SFHITDIAG)
          PRINT 5002,I,ICHAIN
 5002     FORMAT(' SFHITS: SEQ CHAIN#',I2,' CHAIN#',I3)
#endif
          
          
C-----------------------------------------------------------------------
C get curvature of this subchain from the chain information
C taken from sfbwal (beam wall)
C use this curvature for this chain even though multi chains might be passed
C (chains in the SFChanList do not have this LVLCHN information)
C-----------------------------------------------------------------------
          
          IF(.NOT.USE_SFChanList)THEN
            LYRCD=CDLASF(SFLSFA(MINCHN(ICHAIN)))
            IF(LYRCD.LT.0)THEN
              RAD_CHAIN_END=SFSVRD(-LYRCD)
            ELSE
              RAD_CHAIN_END=RCD(LYRCD)
            ENDIF
            
            TAN_EXIT=(1.*LVLCHN(2,ICHAIN)/MULST)*(TWOPI/CELSSF)*100.
            ARGT=TAN_EXIT**2
            IF    (ARGT.LT.0.5)THEN
              SQRM=1.
              ARG1=ARGT
            ELSEIF(ARGT.LT.2.0)THEN
              SQRM=ROOT2
              ARG1=(ARGT-1.)/2.
            ELSE
              SQRM=2.
              ARG1=(ARGT-3.)/4.
            ENDIF
            ARG2=ARG1*ARG1
            ARG3=ARG2*ARG1
            COS_EXIT=1./(SQRM*(1.+ARG1/2.-ARG2/8.+ARG3/16.))
            SIN_EXIT=COS_EXIT*TAN_EXIT
            CURV_USE=SIN_EXIT/RAD_CHAIN_END
            
#if defined(CLEO_SFHITDIAG)
            PRINT 5003,ICHAIN,RAD_CHAIN_END,TAN_EXIT,CURV_USE
 5003       FORMAT(
     1           ' SFHITS: chain num',I3,' recalculate curvature:',
     2           ' tan at (R=',F6.2,')=',F6.2,' curv=',F6.2)
#endif
          ENDIF    
          
C-----------------------------------------------------------------------
C loop over hits in the chain
C-----------------------------------------------------------------------
          
          IF(USE_SFChanList)THEN
            ListLoc_LAST=SFChanList_1stLoc(ICHAIN)
            SFADR_LAST=SFChanList_SFid(ListLoc_LAST)
          ELSE
            SFADR_LAST=LINCHN(ICHAIN)
          ENDIF
          
          LYRSF_LAST=SFLSFA(SFADR_LAST)
          WIRE_LAST= SFWSFA(SFADR_LAST)
          
          FILLING_IN=.FALSE.
          IF(USE_SFChanList)THEN
            ListLoc_CURR=ListLoc_LAST
          ENDIF
          SFADR_CURR=SFADR_LAST
          LYRSF_CURR=LYRSF_LAST
          WIRE_CURR= WIRE_LAST
          
          COUNT=0
 11       COUNT=COUNT+1


#if defined(CLEO_SFHITDIAG)
c          IF(IKILL.EQ.0)THEN
c            call dsf_rfv_input(igo)
c            if(igo.eq.'K')IKILL=1
c          ENDIF
#endif

          
C continue only if current address is non-zero
C and for positive SF layer (CD layer, not extension)
          IF((SFADR_CURR.GT.0).AND.(LYRSF_CURR.GT.0))THEN
            
C do not use east VD
            GOOD_CELL=(WIRE_CURR.LT.NWRSF(LYRSF_CURR))
            
C continue only for good cells
            IF(GOOD_CELL)THEN
              
C-----------------------------------------------------------------------
C get next ADDRESS,LAYER,WIRE in a chain 
C only if not in "filling in" mode and if not at end of chain
C if on the last hit, LYRSF_LAST is left as current LYRSF_CURR
C-----------------------------------------------------------------------
              
              IF(FILLING_IN)THEN
                STRUCTURE_OK=.TRUE.
              ELSE
                IF(USE_SFChanList)THEN
                  IF(ListLoc_CURR.LT.MHITSFLIST)THEN
                    ON_LAST_HIT=(SFChanList_SFid(ListLoc_CURR+1).EQ.0)
                  ELSE
                    ON_LAST_HIT=.TRUE.
                  ENDIF
                ELSE
                  ON_LAST_HIT=(SFADR_CURR.EQ.MINCHN(ICHAIN))
                ENDIF

                IF(ON_LAST_HIT)THEN
                  SFADR_NEXT=0
                  STRUCTURE_OK=.TRUE.
                ELSE
                  IF(USE_SFChanList)THEN
C                   ((ListLoc_CURR.LT.MHITSFLIST)already tested)
                    ListLoc_NEXT=ListLoc_CURR+1
                    SFADR_NEXT=SFChanList_SFid(ListLoc_NEXT)
                  ELSE
                    SFADR_NEXT=ISTFOR(SFADR_CURR)
                  ENDIF
                  IF(SFADR_NEXT.NE.0)THEN
                    LYRSF_NEXT=SFLSFA(SFADR_NEXT)
                    WIRE_NEXT= SFWSFA(SFADR_NEXT)
                    
C-----------------------------------------------------------------------
C internal test of data structure
C-----------------------------------------------------------------------
                    
                    IF(USE_SFChanList)THEN
                      STRUCTURE_OK=.TRUE.
                    ELSE
                      IF(SFADR_NEXT.GT.SFADR_CURR)THEN
                        STRUCTURE_OK=.TRUE.
                      ELSE
                        STRUCTURE_OK=.FALSE.
                        
C error messages if data structure is corrupted
                        WRITE(CHMESS,5021,IOSTAT=IOS)
     1                       NRUN,NEVENT,ICHAIN,
     2                       LINCHN(ICHAIN),MINCHN(ICHAIN),
     3                       LENCHN(ICHAIN),
     4                       SFADR_LAST,SFADR_CURR,SFADR_NEXT
 5021                   FORMAT(
     1                       ' EV=',I6,'/',I6,' CH',I3,
     2                       ' L=',I5,' M=',I5,
     3                       ' L=',I3,
     4                       ' AL=',I5,' A=',I5,' AN=',I5)
                        CALL MESLOG(' SFHITS',1,MSERRO)
                        
#if defined(CLEO_SFHITDIAG)
                        PRINT 5022
 5022                   FORMAT(
     1                       ' SFHITS: SSS TTT U  U  CC  K  K'/
     2                       ' SFHITS:S     T  U  U C  C K K '/
     3                       ' SFHITS: SS   T  U  U C    KK  '/
     4                       ' SFHITS:   S  T  U  U C  C K K '/
     5                       ' SFHITS:SSS   T   UU   CC  K  K')
                        READ 5023,IGO
 5023                   FORMAT(A1)
#endif
                        
C kill all hits collected so far, try the next chain
                        RoadWirLst_Nhit=0
C end of error handling
                      ENDIF
C end of condition (not) (USE_SFChanList)
                    ENDIF
C end of condition (SFADR_NEXT.NE.0)
                  ENDIF
C end of condition (not) (ON_LAST_HIT)
                ENDIF
C end of condition (not) (FILLING_IN)
              ENDIF
C end of condition (GOOD_CELL)
            ELSE

#if defined(CLEO_SFHITDIAG)
              PRINT 5009,GOOD_CELL,WIRE_CURR,
     3             LYRSF_CURR,NWRSF(LYRSF_CURR)
 5009         FORMAT(' SFHITS:end of initial "good cell" test',
     2             ' GOOD_CELL=',L1,' WIRE_CURR=',I4,
     3             ' NWRSF(',I4,')=',I5)
#endif
              
            ENDIF

C-----------------------------------------------------------------------
C continue only for good cells and no errors in the data structure
C-----------------------------------------------------------------------
            
            IF(GOOD_CELL.AND.STRUCTURE_OK)THEN
              
C-----------------------------------------------------------------------
C Note: as part of the effort to eliminate TF references from the standard
C  SF routines, there is no defined "INTFSF" ie address in TF map of SF layer
C  hence must convert to CD layer and then to TF layer
C-----------------------------------------------------------------------
              
              LYRCD=CDLASF(LYRSF_CURR)
              LYRTF=ILCDTF(LYRCD)
              NWIR=NWIRTF(LYRTF)
              NHIT_PREV=RoadWirLst_Nhit

              IF(USE_SFChanList)THEN
                ENTR_ANG=0.
                CROSS_WID_MAX=1
              ELSE
                ENTR_ANG=ABS(CURV_USE*RTF(LYRTF))
                IF(ENTR_ANG    .GT.0.95)THEN
                  CROSS_WID_MAX=5
                ELSEIF(ENTR_ANG.GT.0.89)THEN
                  CROSS_WID_MAX=4
                ELSEIF(ENTR_ANG.GT.0.71)THEN
                  CROSS_WID_MAX=3
                ELSEIF(ENTR_ANG.GT.0.4)THEN
                  CROSS_WID_MAX=2
                ELSE
                  CROSS_WID_MAX=1
                ENDIF
              ENDIF

#if defined(CLEO_SFHITDIAG)
              PRINT 5004,COUNT,SFADR_CURR,
     2             LYRTF,WIRE_CURR,NSER,
     3             ENTR_ANG,CROSS_WID_MAX,
     4             NWIR,NHIT_PREV,
     5             SFADR_NEXT,LYRSF_NEXT,WIRE_NEXT
 5004         FORMAT(' '/
     1             ' SFHITS:========== SEQ HIT',I7,'  SFADR',I7,
     2             ' TFLYR=',I3,' WIRE#',I4,' SEARCHWINDOW',I3,
     3             ' SentANG=',F6.2,' crossWID=',I1,
     4             '  NWIR=',I4,' NHIT_PREV=',I5/
     5             80X,' NEXT adr=',I7,'  lyr=',I4,' wire=',I5)
#endif
              
              
C-----------------------------------------------------------------------
C continue only if the hit buffers are not saturated
C-----------------------------------------------------------------------

              IF(RoadWirLst_Nhit.LT.NHITDM)THEN
                
C-----------------------------------------------------------------------
C MUST TEST PRIMARY AND DOUBLE
C test pimary hit
C-----------------------------------------------------------------------
                
                PRIMRH=0
                WIRSTR(-1)=WIRE_CURR
                WIRSTR( 1)=WIRE_CURR
                
C-----------------------------------------------------------------------
C test the hit quality
C SFHITT tests if there is a valid hit on the cell id
C        increments RoadWirLst_Nhit and RoadWirLst_Nlyr
C        and fills RoadWirLst_adrs
C
C SFHITT uses the cell id to test for a valid hit, so....
C        it can only be looking at the primary hit of the multiple hits
C        and it does not have access to the hit info for z slices
C-----------------------------------------------------------------------

                ISER=0
                CD_cell_id=INDXTF(LYRTF)+WIRE_CURR

                CALL SFHITT(CD_cell_id,LYRSF_CURR,LYRTF,
     2               ISER,WIRE_CURR,GOOD_SFHITT)

                IF(GOOD_SFHITT)THEN
                  PRIMRH=RoadWirLst_Nhit
                  RoadWirLst_stat(PRIMRH)=0

                  IF(.NOT.USE_SFChanList)THEN

                    sf_data_input_cd_adr=CD_cell_id
#define SF_DATA_MAPV_CD
#include "doit/sf_data_access/sf_data_access.inc"
                    IHIT_TEMP=sf_data_output_hit_adr
                    sf_data_input_hit_adr=IHIT_TEMP
#define SF_DATA_GET_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"
                    CLUS_PRIM_P=sf_data_output_primary

                  ENDIF
                    
#if defined(CLEO_SFHITDIAG)
                  COMMNT_HIT(PRIMRH)='primary '
#endif
                ENDIF
                
                WIRE_FND_MIN=WIRE_CURR
                WIRE_FND_MAX=WIRE_CURR

C-----------------------------------------------------------------------
C end of "buffer not saturated" condition
C continue only if the hit buffers are not saturated
C-----------------------------------------------------------------------

              ENDIF
              IF(RoadWirLst_Nhit.LT.NHITDM)THEN
                
C-----------------------------------------------------------------------
C test for double hits
C-----------------------------------------------------------------------
                
                SNDRYH=0
                IF(.NOT.FILLING_IN)THEN
                  IF(USE_SFChanList)THEN
                    ListLoc_DOUB=ListLoc_CURR+1
                    IF(ListLoc_DOUB.LE.MHITSFLIST)THEN
                      SFADR_DOUB=SFChanList_SFid(ListLoc_DOUB)
                      IF(SFADR_DOUB.NE.0)THEN
                        LYRSF_DOUB=SFLSFA(SFADR_DOUB)
                        IF(LYRSF_DOUB.NE.LYRSF_CURR)THEN
                          SFADR_DOUB=0
                        ELSE
                          ListLoc_NEXT=ListLoc_DOUB+1
                          IF(ListLoc_NEXT.LE.MHITSFLIST)THEN
                            SFADR_NEXT=SFChanList_SFid(ListLoc_NEXT)
                            IF(SFADR_NEXT.NE.0)THEN
                              LYRSF_NEXT=SFLSFA(SFADR_NEXT)
                              WIRE_NEXT= SFWSFA(SFADR_NEXT)
                            ENDIF
                          ELSE
                            SFADR_NEXT=0
                          ENDIF
                        ENDIF
                      ENDIF
                    ELSE
                      SFADR_DOUB=0
                    ENDIF
                  ELSE
                    SFADR_DOUB=DOUBPT(SFADR_CURR)
                  ENDIF

                  IF(SFADR_DOUB.NE.0)THEN
                    LYRSF_DOUB=SFLSFA(SFADR_DOUB)
                    WIRE_DOUB=SFWSFA(SFADR_DOUB)
                    
C verify that the double hit is adjacent
C and adjust the start of the search window
                    WIRDIF=WIRE_DOUB-WIRE_CURR
                    IF(WIRDIF.LT.-NWIR/2)WIRDIF=WIRDIF+NWIR
                    IF(WIRDIF.GE. NWIR/2)WIRDIF=WIRDIF-NWIR
                    IF    (WIRDIF.EQ.-1)THEN
                      WIRSTR(-1)=WIRE_DOUB
                      WIRE_FND_MIN=WIRE_CURR-1
                      GOOD_CELL_D=.TRUE.
                    ELSEIF(WIRDIF.EQ. 1)THEN
                      WIRSTR( 1)=WIRE_DOUB
                      WIRE_FND_MAX=WIRE_CURR+1
                      GOOD_CELL_D=.TRUE.
                    ELSE
                      GOOD_CELL_D=.FALSE.
                    ENDIF      
                    
C test the hit quality 
                    IF(GOOD_CELL_D)THEN
                      CD_cell_id=INDXTF(LYRTF)+WIRE_DOUB

                      CALL SFHITT(CD_cell_id,LYRSF_DOUB,LYRTF,
     2                     ISER,WIRE_DOUB,GOOD_SFHITT)

                      IF(GOOD_SFHITT)THEN
                        SNDRYH=RoadWirLst_Nhit
                        RoadWirLst_stat(SNDRYH)=0

                        IF(.NOT.USE_SFChanList)THEN
                          
                          sf_data_input_cd_adr=CD_cell_id
#define SF_DATA_MAPV_CD
#include "doit/sf_data_access/sf_data_access.inc"
                          IHIT_TEMP=sf_data_output_hit_adr
                          sf_data_input_hit_adr=IHIT_TEMP
#define SF_DATA_GET_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"
                          CLUS_PRIM_S=sf_data_output_primary
                          
                      ENDIF
                      
#if defined(CLEO_SFHITDIAG)
                        COMMNT_HIT(SNDRYH)='secndary'
#endif
                        IF(PRIMRH.NE.0)THEN
                          RoadWirLst_stat(PRIMRH)=1
                          RoadWirLst_stat(RoadWirLst_Nhit)=1
                        ENDIF
                      ENDIF
                    ENDIF

C end of "double hit" condition
                  ENDIF
C end of the "not filling in" condition
                ENDIF
                
C-----------------------------------------------------------------------
C end of "buffer not saturated" condition
C continue only if the hit buffers are not saturated
C-----------------------------------------------------------------------
                
              ENDIF
              IF(RoadWirLst_Nhit.LT.NHITDM)THEN
                
C-----------------------------------------------------------------------
C look for other hits in the layer
C use CROSS_WID_MAX to determine the width of the hit string allowed
C ONLY FOR SFHITS_CONTROL.EQ.SFHITS_FULL
C-----------------------------------------------------------------------

                IF(SFHITS_CONTROL.EQ.SFHITS_FULL)THEN
                  
                  NSER=NSRWTF(LYRTF)
                
C loop over directions of the search window
                  DO 49 DIRSER=-1,1,2
                    ISER  =0
                    
C loop over the search window
C relative to the hits already found as primary or double
                    DO 47 JSER = 1,NSER
                      IF(RoadWirLst_Nhit.LT.NHITDM)THEN
                        ISER    = ISER   + DIRSER
                        WIRTST_NO_MOD  = WIRSTR(DIRSER)+ISER
                        WIRTST=WIRTST_NO_MOD
                        IF(WIRTST.LT.   0)WIRTST=WIRTST+NWIR
                        IF(WIRTST.GE.NWIR)WIRTST=WIRTST-NWIR
                        
C test if hit is excluded from SF
                        SFADR_TEMP=INSFSF(LYRSF_CURR)+WIRTST

                        IF(USE_SFChanList)THEN
                          SF_TESTS_OK=.TRUE.
                        ELSE
                          IF(
     1                         (INUCHN(SFADR_TEMP).EQ.-2).OR.
     1                         (INUCHN(SFADR_TEMP).EQ.-3).OR.
     1                         (INUCHN(SFADR_TEMP).EQ.-4).OR.
     1                         (INUCHN(SFADR_TEMP).EQ.-5).OR.
     1                         (INUCHN(SFADR_TEMP).EQ.-9)
     6                         )THEN
                            SF_TESTS_OK=.FALSE.
                          ELSE
                            SF_TESTS_OK=.TRUE.
                          ENDIF
                        ENDIF
                        
C test the hit quality
                        IF(SF_TESTS_OK)THEN
                          CD_cell_id=INDXTF(LYRTF)+WIRTST

                          CALL SFHITT(CD_cell_id,LYRSF_CURR,LYRTF,
     2                         ISER,WIRTST,GOOD_SFHITT)
                          
                          IF(GOOD_SFHITT)THEN
                            RoadWirLst_stat(RoadWirLst_Nhit)=4
                            
                            IF(DIRSER.EQ.-1)THEN
                              WIRE_FND_MIN=WIRTST_NO_MOD
                            ELSEIF(DIRSER.EQ.1)THEN
                              WIRE_FND_MAX=WIRTST_NO_MOD
                            ENDIF

                            IF(.NOT.USE_SFChanList)THEN
                              
                              sf_data_input_cd_adr=CD_cell_id
#define SF_DATA_MAPV_CD
#include "doit/sf_data_access/sf_data_access.inc"
                              IHIT_TEMP=sf_data_output_hit_adr
                              sf_data_input_hit_adr=IHIT_TEMP
#define SF_DATA_GET_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"
                              CLUS_PRIM=sf_data_output_primary

                              IF(PRIMRH.NE.0)THEN
                                IF(.NOT.(
     1                               (INUCHN(SFADR_TEMP).EQ.-1).AND.
     2                               (CLUS_PRIM.EQ.CLUS_PRIM_P)))THEN 
                                  RoadWirLst_stat(PRIMRH)=3
                                ENDIF
                              ENDIF
                              IF(SNDRYH.NE.0)THEN
                                IF(.NOT.(
     1                               (INUCHN(SFADR_TEMP).EQ.-1).AND.
     2                               (CLUS_PRIM.EQ.CLUS_PRIM_S)))THEN 
                                  RoadWirLst_stat(SNDRYH)=3
                                ENDIF
                              ENDIF
                              
                            ENDIF
                                                        
#if defined(CLEO_SFHITDIAG)
                            COMMNT_HIT(RoadWirLst_Nhit)='window  '
#endif

C end of condition that good hit was found by SFHITT
                          ENDIF
C end of SF quality tests
                        ENDIF
C end of "buffer not saturated" condition (within the loop)
                      ENDIF
C end of loop over search window
 47                 CONTINUE
C end of loop over direction of search window
 49               CONTINUE
                ENDIF
                
C-----------------------------------------------------------------------
C end of "buffer not saturated" condition
C-----------------------------------------------------------------------

              ENDIF

C-----------------------------------------------------------------------
C loop over hits found on layer to adjust status for allowed width
C-----------------------------------------------------------------------

              WID_HIT_STR=WIRE_FND_MAX-WIRE_FND_MIN+1

#if defined(CLEO_SFHITDIAG)
              PRINT 5005,COUNT,SFADR_CURR,
     2             LYRTF,WIRE_CURR,WID_HIT_STR,
     3             CROSS_WID_MAX
 5005         FORMAT(
     1             ' SFHITS:========== SEQ HIT',I7,'  SFADR',I7,
     2             ' TFLYR=',I3,' WIRE#',I4,15X,
     3             ' string width',I3,' crossWID=',I1)
#endif

              IF(RoadWirLst_Nhit.GT.NHIT_PREV)THEN
                DO 55 NHIT_INDX=(NHIT_PREV+1),RoadWirLst_Nhit
                  IF(
     1                 (WID_HIT_STR.LE.CROSS_WID_MAX).AND.
     2                 (RoadWirLst_stat(NHIT_INDX).EQ.3))THEN
                    RoadWirLst_stat(NHIT_INDX)=2
                  ENDIF


C-----------------------------------------------------------------------
C print out the info
C-----------------------------------------------------------------------

#if defined(CLEO_SFHITDIAG)
                  PRINT 5006,
     2                 RoadWirLst_Nlyr,RoadWirLst_Nhit,
     3                 NHIT_INDX,RoadWirLst_adrs(NHIT_INDX),
     4                 RoadWirLst_stat(NHIT_INDX),
     5                 COMMNT_HIT(NHIT_INDX)
 5006             FORMAT(
     1                 ' SFHITS:layerswithwires=',I3,
     2                 ' totalwires=',I4,
     3                 ' RoadWirLst_adrs(',I3,')=',I5,
     4                 ' RoadWirLst_stat=',I5,
     5                 ' from ',A8)
#endif
                  
                  
 55             CONTINUE
              ENDIF

C-----------------------------------------------------------------------
C prepare to loop back to process next wire in chain
C wait, first see if there are missing layers in the sf chain
C-----------------------------------------------------------------------

              LYRSF_TEST=LYRSF_CURR+1

              IF(SFHITS_CONTROL.EQ.SFHITS_FULL)THEN
                FILLING_IN=((LYRSF_TEST).LT.LYRSF_NEXT)
              ELSE
                FILLING_IN=.FALSE.
              ENDIF
              
C-----------------------------------------------------------------------
C found missing layers
C fill in the missing hits
C-----------------------------------------------------------------------

              IF(FILLING_IN)THEN
                LYRSF_CURR=LYRSF_TEST
                NWIRTX=NWRSSF(LYRSF_CURR)*MULST
                IF(USE_SFChanList)THEN
                  ZNOWX=0.
                ELSE
                  ZNOWX=ZR0CHN(ICHAIN)
     2                 +ZDRCHN(ICHAIN)*RADSSF(LYRSF_CURR)/100/MULST
                ENDIF

C code adapted from SFMERG
                WLF0=WIRE_LAST*MULST+OFFSSF(LYRSF_LAST)
                WCRLXZ=WLF0*NWRSSF(LYRSF_CURR)/NWRSSF(LYRSF_LAST)
     2               -OFFSSF(LYRSF_CURR)
                
                DIFTN=TANSSF(LYRSF_CURR)-TANSSF(LYRSF_LAST)
                WSHFZ=ZNOWX*DIFTN/MULST
                WCRLX=WCRLXZ+WSHFZ
                
                IF(WCRLX.LT.0     )WCRLX=WCRLX+NWIRTX
                IF(WCRLX.GE.NWIRTX)WCRLX=WCRLX-NWIRTX
                
                WNF0=WIRE_NEXT*MULST+OFFSSF(LYRSF_NEXT)
                WCRNXZ=WNF0*NWRSSF(LYRSF_CURR)/NWRSSF(LYRSF_NEXT)
     2               -OFFSSF(LYRSF_CURR)
                
                DIFTN=TANSSF(LYRSF_CURR)-TANSSF(LYRSF_NEXT)
                WSHFZ=ZNOWX*DIFTN/MULST
                WCRNX=WCRNXZ+WSHFZ
                
                WIRE_DIF=WCRNX-WCRLX
                IF(WIRE_DIF.LT.-NWIRTX/2)WCRNX=WCRNX+NWIRTX
                IF(WIRE_DIF.GE. NWIRTX/2)WCRNX=WCRNX-NWIRTX
                
                RAD_DIF=RADSSF(LYRSF_NEXT)-RADSSF(LYRSF_LAST)
                
                IF    (WCRNX.LT.WCRLX)THEN
                  WNPX=( (RADSSF(LYRSF_NEXT)-RADSSF(LYRSF_CURR))
     1                 *(WCRLX-WCRNX) )/RAD_DIF  +WCRNX
                ELSEIF(WCRNX.GT.WCRLX)THEN
                  WNPX=( (RADSSF(LYRSF_CURR)-RADSSF(LYRSF_LAST))
     1                 *(WCRNX-WCRLX) )/RAD_DIF  +WCRLX
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
                PRINT 5007,
     2               LYRSF_LAST,LYRSF_CURR,LYRSF_NEXT,
     3               NWRSSF(LYRSF_CURR),ZNOWX,
     4               WCRLX,WCRNX,WNPX,WIRE_CURR
 5007           FORMAT(
     2               ' SFHITS: filling_in:',
     3               ' LL=',I2,' LC=',I2,' LN=',I2,
     4               ' nw=',I5,' Z=',I7,
     5               ' WradOUT=',I7,' WradIN=',I7,
     6               ' Wpred=',I7,' (=',I4,' )' )
#endif

C-----------------------------------------------------------------------
C no missing layers,
C go to the next element in the the link list 
C and set the current hit to that element
C ( 12-dec-2002 it appears to fill in only one layer )
C-----------------------------------------------------------------------

              ELSE
                IF(USE_SFChanList)THEN
                  ListLoc_LAST=ListLoc_NEXT
                  ListLoc_CURR=ListLoc_NEXT
                ENDIF

                SFADR_LAST=SFADR_NEXT
                SFADR_CURR=SFADR_NEXT
                LYRSF_LAST=LYRSF_NEXT
                LYRSF_CURR=LYRSF_NEXT
                WIRE_LAST= WIRE_NEXT
                WIRE_CURR= WIRE_NEXT

C-----------------------------------------------------------------------
C end of condition of missing layers
C-----------------------------------------------------------------------
              ENDIF
              GO TO 11

C-----------------------------------------------------------------------
C end of the "good cell" and "no error in data structure" condition
C-----------------------------------------------------------------------
            ELSE
#if defined(CLEO_SFHITDIAG)
              PRINT 5010,GOOD_CELL,STRUCTURE_OK
 5010         FORMAT(' SFHITS: failed "good cell" condition',
     2             ' GOOD_CELL=',L1,' STRUCTURE_OK=',L1)
#endif
            ENDIF
C-----------------------------------------------------------------------
C end of the "non-zero cell" condition, this is end of look over cells
C-----------------------------------------------------------------------

          ELSE
#if defined(CLEO_SFHITDIAG)
            PRINT 5008,SFADR_CURR,LYRSF_CURR
 5008       FORMAT(' SFHITS: failed "non-zero cell" condition',
     2           ' SFADR_CURR=',I8,'  LYRSF_CURR=',I8)
#endif
          ENDIF
C-----------------------------------------------------------------------
C end loop over input chains
C-----------------------------------------------------------------------
 79     CONTINUE
C-----------------------------------------------------------------------
C break point for no input chains
C-----------------------------------------------------------------------
      ENDIF
      RETURN
      END
