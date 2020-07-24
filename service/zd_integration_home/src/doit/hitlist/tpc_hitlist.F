*
* $Id: tpc_hitlist.F,v 1.2 2003/05/12 16:08:27 dpp Exp $
*
* $Log: tpc_hitlist.F,v $
* Revision 1.2  2003/05/12 16:08:27  dpp
*      -> use CLEO_DOITTPC to dummy this routine in CLEO production
*
* Revision 1.1  2003/03/05 17:50:35  dpp
*      -> NEW ROUTINE
*      -> main routine to convert LCD hit list to cdgeom
*      -> changed all variable names in cdscrtcd to have common root
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TPC_HITLIST
#if defined(CLEO_DOITTPC)
C.......................................................................
C.
C. TPC_HITLIST - transfer hit list from LCD into cdgeom
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 22-Oct-2002
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE


#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfindpar.inc"
C which includes 
C               #include "cl3seq/cdgm3/cdgeompa.inc"
C               #include "doit/duseq/tfgeompa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"
#include "doit/duseq/runev.inc"

#include "/home/axp/dpp/lcd_simulation/cornell/hep/lcd/io/fortran/lcdevt.inc"

      INTEGER HLIMIT,CLIMIT,LLIMIT

      INTEGER I,J
      INTEGER IGO,KILL
      INTEGER TRK_OLD

      REAL    XTMP,YTMP,RADIUSHIT,PHIHIT
      REAL    Z_HIT
      REAL    ATN2PI
      REAL    RADPREV,ZPREV,RADDIF
      REAL    Z_WIDTH

      INTEGER ILCD
      INTEGER IADR
      INTEGER NEXT_CELL
      REAL    PH_FRAC
      REAL    PH_HIT


c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      KILL=0

      current_event=current_event+1

      print 1000,current_event,NTRKHITS
 1000 FORMAT(' TPC_HITLIST: event:'  ,I8,'       found',I9,
     2     ' crossings')

      HLIMIT=EvntHitMax
      EvntHit_Num=0

      print 1001,HLIMIT
 1001 FORMAT(' TPC_HITLIST:   limit of number of hits=',I9,
     2     '      zero number of hits: EvntHit_Num')

      CLIMIT=KWS1CD
      CALL VZERO(EvntHit_MapDet,KWS1CD)

      print 1002,CLIMIT
 1002 FORMAT(' TPC_HITLIST:  limit of number of cells=',I9,
     2     '         zero the hit map: EvntHit_MapDet')

      LLIMIT=KLYRCD
      CALL VZERO(EvntHit_1stLyr,KLYRCD)

      print 1003,LLIMIT
 1003 FORMAT(' TPC_HITLIST: limit of number of layers=',I9,
     2     ' zero the 1st hit pointer: EvntHit_1stLyr')

      CALL VZERO(EvntHit_1stDEV,MXDVCD)


C process the input hit list

      trk_old=0
      IF(NTRKHITS.GT.1)THEN
        EvntHit_1stDEV(ITPC1)=1
        do 219 i=1,NTRKHITS

          IF(trkhitmcpart(i).ne.trk_old)THEN

            print 1012,trkhitmcpart(i)
 1012       format(
     1           ' <cr> for hits on track',I7,
     2           ' "n" to jump,',
     3           ' or "k" to stop asking')
            
#if defined(CLEO_SFDIAG)
            IF(kill.eq.0)THEN
              call dsf_rfv_input(igo)
              if(igo.eq.'n')go to 220
              if(igo.eq.'N')go to 220
              if(igo.eq.'k')kill=1
              if(igo.eq.'K')kill=1
            ENDIF
#endif
            trk_old=trkhitmcpart(i)
            RADPREV=0.
            ZPREV=0.
          ENDIF

          XTMP=trkhit(1,i)/100.
          YTMP=trkhit(2,i)/100.
          RADIUSHIT=SQRT(XTMP**2+YTMP**2)
          PHIHIT=ATN2PI(YTMP,XTMP)
          Z_HIT=trkhit(3,i)/100.



          print 1011,i,EvntHit_Num,
     1         trkhitlayer(i),trkhitmcpart(i),
     2         (trkhit(j,i),j=1,3),
     3         RADIUSHIT,PHIHIT,Z_HIT

 1011     format(' TPC_HITLIST:',I7,I8,
     1         ' lyr=',I4,' mcpart=',I6,
     2         ' x,y,z=',3F9.4,
     3         ' rad,phi,z=',F9.6,F7.3,F10.6)

          




            ILCD=trkhitlayer(i)+1

            IF(
     1           (abs(RADIUSHIT-RCD(ILCD)).lt.0.005).and.
     2           (trkhitmcpart(i).GT.0)
     3           )THEN
              
              
C calculate the z difference only for valid hits
              RADDIF=RADIUSHIT-RADPREV
              Z_WIDTH=Z_HIT-ZPREV
              IF(RADPREV.EQ.0)THEN
                Z_WIDTH=Z_HIT*.01/RADIUSHIT
              ENDIF
              RADPREV=RADIUSHIT
              ZPREV=Z_HIT
              
C get the pad hits              
              CALL TPC_PAD_RESPONSE(0,
     1             ILCD,PHIHIT,PHIHIT,
     2             IADR,PH_FRAC,NEXT_CELL)
              
 111          CONTINUE
              
              IF(NEXT_CELL.GT.0)THEN
                
                CALL TPC_PAD_RESPONSE(1,
     1               ILCD,PHIHIT,PHIHIT,
     2               IADR,PH_FRAC,NEXT_CELL)

                PH_HIT=PH_FRAC*999.
                
                CALL TPC_ADD_HIT(ILCD,IADR,Z_HIT,Z_WIDTH,PH_HIT)

                GO TO 111

C end of condition that there are more cells 
              ENDIF

C end of condition that hit matches the layer specified
            ENDIF
          
C end of loop over hits          
 219      continue

C jump point
 220      CONTINUE

          CALL TPC_RANDOM_NOISE

          CALL TPC_TIME_RESPONSE


C end of condition on number of hits
      ENDIF

C end of compliation flag CLEO_DOITTPC
#endif
      
      RETURN
      END





