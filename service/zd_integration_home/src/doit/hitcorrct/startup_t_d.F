*
* $Id: startup_t_d.F,v 1.4 2003/03/05 17:47:26 dpp Exp $
*
* $Log: startup_t_d.F,v $
* Revision 1.4  2003/03/05 17:47:26  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.3  2002/06/05 03:04:08  atems
* minor fixes to compile under linux
*
* Revision 1.2  2000/03/01 23:02:14  dpp
*      -> new version can use time in common
*
* Revision 1.1  2000/01/24 19:59:10  dpp
*      -> time to distance conversion from startup data
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE STARTUP_T_D(MODE,TIME_0_IN)
C......................................................................
C.
C. STARTUP_T_D  :  calculate distance from TIME (in nano sec)
C.
C. COMMON    : 

C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 19-Jan-2000
C. LAST MOD  : 
C.
C. Modification Log.
C......................................................................
#if !defined(Linux)
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/xts_cntl.inc"
#include "seq/clutil/mesrep.inc"
#include "doit/duseq/runev.inc"

C...... ARGUMENTS DOCUMENTATION

C MODE..........=1, TIME_0_IN is input of TIME ZERO, value in TDC counts
C               =2, use time in ps from EvntHit_rawTIM() in CDSCRTCD
C               =3, ONLY AT FIRST CALL
C                   input values of TIME_LOW,THE_TIME,DIST_AT_TIME
C                   and do the rest of the initialization
      INTEGER MODE
C TIME_0_IN.....input of TIME ZERO, value in TDC counts
      INTEGER TIME_0_IN

C...... LOCAL VARIABLE DOCUMENTAION

C TIME_LOW_IN.....input value of TIME_LOW
      REAL TIME_LOW_IN
C THE_TIME_IN.....input value of THE_TIME(2) 
      REAL THE_TIME_IN
C DIST_AT_TIME_IN.input value of DIST_AT_TIME(2)
      REAL DIST_AT_TIME_IN

C HIST_SIZE.....size of the TDC histogram
      INTEGER HIST_SIZE
      PARAMETER (HIST_SIZE=100)
C IVALU.........TIME stored histogram value , used
      INTEGER IVALU(HIST_SIZE)
C IVALS.........TIME stored histogram running sum, up to the low edge
      INTEGER IVALS(HIST_SIZE)
C NORMAL_VAL....IVALU normalized
      REAL NORMAL_VAL(HIST_SIZE)
C NORMAL_INT....IVALS normalized to give the fraction between the
C               two values of "THE_TIME"
      REAL NORMAL_INT(HIST_SIZE)


c      DATA IVALU/
c     1     11,  19,  23,  19,  24,  60,  89,  84,  91, 185,
c     2    330, 403, 361, 439, 809, 851, 817, 885,1119,1311,     
c     3   1299,1312,1540,1604,1584,1574,1451,1453,1379,1354,   
c     4   1283,1317,1214,1156,1193,1207,1140,1051,1114,1006,
c     5    957, 825, 829, 746, 676, 619, 557, 466, 382, 390,
c     6    332, 262, 213, 229, 196, 192, 169, 188, 194, 190,
c     7    192, 181, 207, 194, 226, 210, 220, 226, 184, 179,
c     8    223, 180, 179, 188, 163, 150, 142, 138, 108, 127,
c     9    118,  84,  95,  85, 106,  79,  92,  77,  75,  65,
c     x     71,  49,  62,  72,  60,  71,  62,  75,  45,  49/
C set 1
c      DATA IVALU/
c     1      11,  10,   9,  16,  14,  34,  38,  35,  35,  46,
c     2     398,1376,1912,1830,1748,1550,1484,1404,1359,1248,
c     3    1166,1132,1111,1124,1127,1048,1040, 999, 980,1031,
c     4     998,1037,1068,1017, 940, 969, 814, 700, 562, 480,
c     5     328, 289, 281, 230, 234, 180, 169, 158, 155, 129,
c     6     130, 114, 133, 106, 121, 148, 161, 159, 164, 189,
c     7     212, 165, 172, 180, 198, 134, 127, 112, 156,  99,
c     8      82,  79, 131,  67,  36,  38,  75,  53,  33,  34,
c     9      40,  50,  24,  15,  35,  31,  34,  31,  17,  13,
c     x      19,  15,  10,  16,  20,  11,  14,  14,  13,  13/      
C set 2
      DATA IVALU/
     1     78,    89,    74,    96,    72,
     2     81,   100,    96,    92,    93,
     3     91,   104,   108,   103,   114,
     4     90,   111,    98,    75,   110,
     5    101,   896, 23330, 92893,141712,
     6 146576,136250,126465,119612,112956,
     7 107512,102912, 98729, 94412, 91508,
     8  88257, 85664, 82995, 80543, 77868,
     9  76488, 74644, 72617, 70954, 69244,
     x  68384, 66660, 65188, 65635, 64372,
     1  63718, 63044, 62816, 61740, 61642,
     2  61307, 61198, 61680, 61335, 61189,
     3  61996, 62438, 62931, 63808, 64318,
     4  64783, 65125, 66150, 65931, 65288,
     5  63108, 60815, 57317, 52970, 48232,
     6  43272, 37731, 32885, 28780, 24547,
     7  21492, 18701, 16520, 14824, 13261,
     8  12007, 11009, 10495,  9524,  8870,
     9   8579,  7986,  7575,  7061,  6756,
     x   6264,  6085,  5934,  5731,  5344/

C TIME_LOW......time for the low edge of the first bin in nanoseconds
C TIME_WID......width of a time bin in nanoseconds
      REAL TIME_LOW,TIME_WID

c      DATA TIME_LOW/-50./
c      DATA TIME_LOW/-95./
C set 1
c      DATA TIME_LOW/-65./
c      DATA TIME_WID/10./
C set 2
C after return from bunch finder and CDOffCal with GlobalT0=5588000
C data time corresponding to 50%rise is 20 ns,
C 50% rise on histogram is at 118 above the plot start,
C re-adjust plot to have start at -98ns so 50% rise is at 20 ns
C At the same time, saw 250 ns paired with 0 ns in bhabha 
      DATA TIME_LOW/-100./
      DATA TIME_WID/5./


C THE_TIME......the time that has a specified  distance
C DIST_AT_TIME..distance at a specific time
C BIN_AT_TIME...bin number the contains the specified time
C INTEGRAL_AT_T.integral of the time distribution at a specific time 
      REAL THE_TIME(2)
      REAL DIST_AT_TIME(2)
      INTEGER BIN_AT_TIME(2)
      REAL INTEGRAL_AT_T(2)


      DATA DIST_AT_TIME/    0.,   .006786 /
      DATA THE_TIME    /    0.,   301.    /
      
C INTEGRAL_DIFF.difference in the integrals for the two specified times
      REAL INTEGRAL_DIFF

C N_INT_AT_TIME.normalized integral at the input time
      REAL N_INT_AT_TIME

C BIN...........position in the hitsogram for a specified time
C               integer value is the low edge of the bin
C INT_BIN.......integer part of BIN
C PART_BIN......fractional part of BIN over INT_BIN
      REAL BIN,PART_BIN
      INTEGER INT_BIN



C TIME_DIST_MIN.minimum time for which distance is defined
C TIME_DIST_MAX.minimum time for which distance is defined
C DIST_STOR.....stored value of distance for each time
      INTEGER TIME_DIST_MIN,TIME_DIST_MAX
      PARAMETER (TIME_DIST_MIN=-200,TIME_DIST_MAX=1500)
      REAL DIST_STOR(TIME_DIST_MIN:TIME_DIST_MAX)

C J1.......loop index
C J2.......another loop index
C JTIME....another loop index
C JTIMED...JTIME divided by some granularity
C JTIMEM...JTIMED multiplied by the granularity 
      INTEGER J1,J2,JTIME,JTIMED,JTIMEM

C IHIT.....index loop over hit
      INTEGER IHIT
C TIME_NS..drift time in ns
      INTEGER TIME_NS


C.IOS......DESTINATION OF IOSTAT
      INTEGER IOS

C LFIRST...=.TRUE. IF THIS IS FIRST CALL
      LOGICAL LFIRST
      DATA LFIRST/.TRUE./

      integer lay,line,encoded,line_tot
      real print_time,print_dist

C----------------------Executable code starts here---------------------

      IF(LFIRST)THEN
         LFIRST=.FALSE.

C replace tuning values if this is MODE 3

         IF(MODE.EQ.3)THEN
            PRINT 2001
 2001       FORMAT(' enter "TIME_LOW"; low edge of the histogram (F)')
            ACCEPT 2011,TIME_LOW_IN
 2011       FORMAT(F)
            PRINT 2021,TIME_LOW_IN
 2021       FORMAT(' STARTUP_T_D: will use TIME_LOW=',F6.1)

            PRINT 2002
 2002       FORMAT(' enter "THE_TIME"; at histogram high edge (F)')
            ACCEPT 2012,THE_TIME_IN
 2012       FORMAT(F)
            PRINT 2022,THE_TIME_IN
 2022       FORMAT(' STARTUP_T_D: will use THE_TIME(2)=',F6.1)

            PRINT 2003
 2003       FORMAT(' enter "DIST_AT_TIME"; at histogram high edge(F)')
            ACCEPT 2013,DIST_AT_TIME_IN
 2013       FORMAT(F)
            PRINT 2023,DIST_AT_TIME_IN
 2023       FORMAT(' STARTUP_T_D: will use DIST_AT_TIME(2)=',F9.6)


            TIME_LOW=TIME_LOW_IN
            THE_TIME(2)=THE_TIME_IN
            DIST_AT_TIME(2)=DIST_AT_TIME_IN
         ENDIF

C build the TIME histogram sum

         IVALS(1)=0.
         DO 17 J1=2,HIST_SIZE
            IVALS(J1)=IVALS(J1-1)+IVALU(J1-1)
            PRINT 1001,J1,IVALU(J1),IVALS(J1)
 1001       FORMAT(' STARTUP_T_D: BIN,VAL,SUM=',I4,I16,I11)
 17      CONTINUE

C find the sum up to each of the specified times
C  eg, if THE_TIME=  0, TIME_LOW=0, and TIME_WID=10, BIN= 1  (the low edge)
C  eg, if THE_TIME=430, TIME_LOW=0, and TIME_WID=10, BIN=44

         PRINT 1002,TIME_LOW,TIME_WID
 1002    FORMAT(' STARTUP_T_D: TIME_LOW,TIME_WID=',2F6.2)

         DO 27 J2=1,2
            BIN=(THE_TIME(J2)-TIME_LOW)/TIME_WID +1
            BIN=BIN+1000.
            INT_BIN=BIN
            PART_BIN=BIN-INT_BIN
            INT_BIN=INT_BIN-1000
            BIN=BIN-1000.
            IF(INT_BIN.LT.1)THEN
               INT_BIN=1
               PART_BIN=0.
            ELSEIF(INT_BIN.GT.HIST_SIZE)THEN
               INT_BIN=HIST_SIZE
               PART_BIN=1.
            ENDIF
            BIN_AT_TIME(J2)=INT_BIN
            INTEGRAL_AT_T(J2)=IVALS(INT_BIN)+IVALU(INT_BIN)*PART_BIN

            PRINT 1003,
     1           J2,THE_TIME(J2),
     2           INT_BIN,PART_BIN,
     3           INTEGRAL_AT_T(J2)
 1003       FORMAT(
     1           ' STARTUP_T_D: ',I3,' TIME=',F6.2,
     2           ' bin',I3,'+',F4.2,
     2           ' INTEGRAL=',F13.1)

 27      CONTINUE

C find the integral corresponding to the specified time difference
C and fill in normalized array

         INTEGRAL_DIFF=INTEGRAL_AT_T(2)-INTEGRAL_AT_T(1)
         
         DO 37 J1=1,HIST_SIZE
            NORMAL_VAL(J1)=
     1           IVALU(J1)/INTEGRAL_DIFF
            NORMAL_INT(J1)=
     1           (IVALS(J1)-INTEGRAL_AT_T(1))/INTEGRAL_DIFF

            PRINT 1004,J1,NORMAL_VAL(J1),NORMAL_INT(J1)
 1004       FORMAT(' STARTUP_T_D: ',I3,
     2           ' normVAL=',F9.6,' normINT=',F9.6) 

 37      CONTINUE


C build the output plot

         DO 47 JTIME=TIME_DIST_MIN,TIME_DIST_MAX

            IF(JTIME.LT.THE_TIME(1))THEN
               INT_BIN=BIN_AT_TIME(1)
               PART_BIN=(JTIME-THE_TIME(1))/TIME_WID
               N_INT_AT_TIME=PART_BIN*NORMAL_VAL(INT_BIN)
               DIST_STOR(JTIME)=
     1              DIST_AT_TIME(1)+
     2              (DIST_AT_TIME(2)-DIST_AT_TIME(1))*N_INT_AT_TIME

            ELSEIF(JTIME.GT.THE_TIME(2))THEN
               INT_BIN=BIN_AT_TIME(2)
               PART_BIN=(JTIME-THE_TIME(2))/TIME_WID
               N_INT_AT_TIME=1.+PART_BIN*NORMAL_VAL(INT_BIN)
               DIST_STOR(JTIME)=
     1              DIST_AT_TIME(1)+
     2              (DIST_AT_TIME(2)-DIST_AT_TIME(1))*N_INT_AT_TIME

            ELSE
               BIN=(JTIME-TIME_LOW)/TIME_WID +1
               BIN=BIN+1000.
               INT_BIN=BIN
               PART_BIN=BIN-INT_BIN
               INT_BIN=INT_BIN-1000
               BIN=BIN-1000.
               N_INT_AT_TIME=
     1              NORMAL_INT(INT_BIN)+PART_BIN*NORMAL_VAL(INT_BIN)
               DIST_STOR(JTIME)=
     1           DIST_AT_TIME(1)+
     2           (DIST_AT_TIME(2)-DIST_AT_TIME(1))*N_INT_AT_TIME
            ENDIF

            IF((JTIME.GE.-10).AND.(JTIME.LE.500))THEN
               JTIMED=JTIME /10
               JTIMEM=JTIMED*10
               IF(JTIME.EQ.JTIMEM)THEN
                  PRINT 6607,JTIME,DIST_STOR(JTIME)
 6607             FORMAT(' STARTUP_T_D: ',I5,F10.6)
               ENDIF
            ENDIF

 47      CONTINUE


         go to 58
         open(unit=31,
     2        file='drift.txt',status='NEW')
         lay=201*47
         write(31,9991)lay
 9991    format(i4)
         
         line_tot=0
         
         do 57 lay=1,47
            do 56 line=1,201
               
               line_tot=line_tot+1
               
               print_time=1.*(line-1)*2
               
               j1=print_time
               IF(J1.LT.TIME_DIST_MIN)THEN
                  J1=TIME_DIST_MIN
               ELSEIF(J1.GT.TIME_DIST_MAX)THEN
                  J1=TIME_DIST_MAX
               ENDIF
               print_dist=DIST_STOR(J1)
               
               encoded=line+10000+lay*10000000
               
               write(31,9992)line_tot,print_time,
     2              print_dist,encoded
               print 9992,line_tot,print_time,
     2              print_dist,encoded

 9992          format(I7,E19.11,E19.11,I11)
               
 56         continue
 57      continue
         close(unit=31)
 58      continue
         





      ENDIF

C this is the regular code

      IF(
     1     (MODE.EQ.1).OR.
     2     (MODE.EQ.2))THEN

         IF(EvntHit_Num.GT.0)THEN
            DO 104 IHIT=1,EvntHit_Num
               
               IF(MODE.EQ.1)THEN
                  TIME_NS=(TIME_0_IN-EvntHit_rawTDC(IHIT))*.5
               ELSEIF(MODE.EQ.2)THEN
                  TIME_NS=EvntHit_rawTIM(IHIT)/1000
               ENDIF
               
               J1=TIME_NS
               IF(J1.LT.TIME_DIST_MIN)THEN
                  J1=TIME_DIST_MIN
               ELSEIF(J1.GT.TIME_DIST_MAX)THEN
                  J1=TIME_DIST_MAX
               ENDIF
                EvntHit_Drift(IHIT)=DIST_STOR(J1)
 104        CONTINUE
         ENDIF
      ENDIF

#endif(Linux)

      RETURN
      END








