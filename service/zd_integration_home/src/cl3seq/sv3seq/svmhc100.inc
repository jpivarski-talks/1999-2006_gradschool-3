*
* $Id: svmhc100.inc,v 1.1 1998/01/07 20:49:30 pg Exp $
*
* $Log: svmhc100.inc,v $
* Revision 1.1  1998/01/07 20:49:30  pg
*   Added files needed for SVX hit storage. Rearranged and modified
* svgeompa.inc and svlaseq.inc to make hit-related things work.
*
* Revision 1.3  1994/12/21 17:20:33  zfiles
* Declare WNOMSV
*
* Revision 1.2  1994/12/20  18:19:55  zfiles
* Added WNOMSV.
*
* Revision 1.1.1.1  1994/10/04  20:59:00  zfiles
* first version of seq in CVS
*
*
*
* svmhc100.inc
*
C
C
C
      INTEGER MXSVMH
      PARAMETER ( MXSVMH = 3000 )
C NSVMH  - Number of Monte Carlo hits in the SVD detector
C ITKMSV    - track number from SVMC bank
C IHTMSV    - hit number from SVMC bank
C ISTMSV    - sense strip number from SVMC bank
C QDPMSV    - charge deposited on sense strip from SVMC bank
C FLDMSV    - folding # for Z strip (0-2)
C WNOMSV    - WAFER NUMBER (0-95)
C             units of the charge is not fixed yet.
      INTEGER NSVMH
      INTEGER ITKMSV, IHTMSV, ISTMSV, FLDMSV, WNOMSV
      REAL QDPMSV

      COMMON /SVMH00/ NSVMH, ISTMSV ( MXSVMH ), QDPMSV ( MXSVMH ),
     *                       ITKMSV ( MXSVMH ), IHTMSV ( MXSVMH ),
     *                       FLDMSV ( MXSVMH ), WNOMSV ( MXSVMH )
      SAVE /SVMH00/


