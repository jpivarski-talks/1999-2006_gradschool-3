*
* $Id: svghc100.inc,v 1.1 1998/01/07 20:49:29 pg Exp $
*
* $Log: svghc100.inc,v $
* Revision 1.1  1998/01/07 20:49:29  pg
*   Added files needed for SVX hit storage. Rearranged and modified
* svgeompa.inc and svlaseq.inc to make hit-related things work.
*
* Revision 1.1.1.1  1994/10/04 20:59:00  zfiles
* first version of seq in CVS
*
*
*
* svghc100.inc
*
C
C
C
      INTEGER MXSVGH
      PARAMETER ( MXSVGH = 3000 )
C NSVGH  - Number of Monte Carlo hits in the SVD detector
*
* XIWGSV  - x co-ord of point of entrance world coordinates
* YIWGSV  - y co-ord of point of entrance world coordinates
* ZIWGSV  - z co-ord of point of entrance world coordinates
* MOMGSV  - momentum at point of entrance
* XILGSV  - x co-ord of point of entrance local coordinates
* YILGSV  - y co-ord of point of entrance local coordinates
* ZILGSV  - z co-ord of point of entrance local coordinates
* XOLGSV  - x co-ord of point of exit local coordinates
* YOLGSV  - y co-ord of point of exit local coordinates
* ZOLGSV  - z co-ord of point of exit local coordinates
* WNOGSV  - wafer number
* PATGSV  - length of the path in the silicon
* QDPGSV  - energy deposited in the silicon
* TLNGSV  - Track length at the entrance
C           units of the charge is not fixed yet.
*
      INTEGER NSVGH
      REAL XIWGSV, YIWGSV, ZIWGSV
      REAL XILGSV, YILGSV, ZILGSV
      REAL XOLGSV, YOLGSV, ZOLGSV
      REAL MOMGSV, PATGSV, QDPGSV
      INTEGER WNOGSV, ITRGSV
      REAL TLNGSV

      COMMON /SVGH00/ NSVGH,
     * XIWGSV( MXSVGH ), YIWGSV( MXSVGH ), ZIWGSV( MXSVGH ),
     * XILGSV( MXSVGH ), YILGSV( MXSVGH ), ZILGSV( MXSVGH ),
     * XOLGSV( MXSVGH ), YOLGSV( MXSVGH ), ZOLGSV( MXSVGH ),
     * MOMGSV( MXSVGH ), PATGSV( MXSVGH ), QDPGSV( MXSVGH ),
     * WNOGSV( MXSVGH ), ITRGSV( MXSVGH ), TLNGSV( MXSVGH )
      SAVE /SVGH00/


