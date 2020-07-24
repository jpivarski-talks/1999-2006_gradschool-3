*
* $Id: cdcalpa.inc,v 1.1 1999/11/29 21:44:18 lyon Exp $
*
* $Log: cdcalpa.inc,v $
* Revision 1.1  1999/11/29 21:44:18  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.6  1996/05/24 16:46:13  clib
* Fixed version comments.
*
* Revision 1.5  1996/05/24 16:45:25  clib
* Commons for invalid constants removed.
*
* Revision 1.4  1996/03/08 01:25:52  zfiles
* Pointer arrays reduced to integer*2, Msegcl2 parameter reduced to 3400 from
* 6000, to restrain ridiculous growth in executable size. (integer*2 is illegal
* in CLEO standards (nk))
*
* Revision 1.3  1996/02/09 23:44:45  zfiles
* Z corrections. Parameter sizing new drift function arrays extended
*
* Revision 1.2  1996/02/07 22:14:22  zfiles
* Added variables for recompress. From Ken Mclean
*
* Revision 1.1.1.1  1994/10/04  20:58:49  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/55 01/07/94  10.51.20  by  Karen Lingel, Physicist
*CMZ :  1.02/53 13/06/94  15.06.41  by  G. Ludwig & A. Bellerive
*     Added parameters for new signal propagation correction and VD East
*     hits.
*CMZ :  1.02/52 09/06/94  19.14.47  by  D. Fujino
*     From Don Fujino, change number of time "slopes".
*CMZ :  1.02/26 28/09/92  23.17.34  by  Martin Lohner
*CMZ :  1.02/25 21/09/92  11.21.11  by  Karen Lingel
*CMZ :  1.02/19 04/05/92  21.05.00  by  Martin Lohner
*CMZ :  1.01/63 04/09/90  14.18.50  by  G. Ludwig
*     Added maximum dimensions for CFANCL
*CMZ :          30/04/90  14.28.32  by  G. Ludwig
*CMZ :          17/12/89  18.26.22  by  G. Ludwig
*     Renamed xMAXCL parameters as MxSLCL (x=T,Q)
*CMZ :          18/10/89  17.57.22  by  G. Ludwig
*CMZ :  1.01/00 29/07/89  19.11.02  by  Greg Ludwig
*     Replaced MPRECL by MSLTCL for new preamp indexing.
*-- Author :
C..........................................................................
C.
C. CDCALPA - SEQuence for CLEO-II Central Detector Calibration PARAMETERS
C.
C. Author    : ??  (CLEO-1.5 version CLINC:CALSCD.PAR)
C.           : G. Ludwig, CLEO-II version.   25-Jun-89
C.
C..........................................................................
*
* MTSLCL - Maximum number of time "slopes"
* MQSLCL - Maximum number of charge "slopes"
      INTEGER     MTSLCL,     MQSLCL
      PARAMETER ( MTSLCL = 4, MQSLCL = 3 )
*
* MSEGCL - Max # time segments in drift-time relation
*
      INTEGER     MSEGCL        , MSEGCL2
      PARAMETER ( MSEGCL = 200  , MSEGCL2 = 3400 )
*
* MSGQCL - Max # segments for charge correction to time
* MDFQCL - Max # drift regions for charge correction to time
* MDVQCL - Max configuration number of a device with charge correction to time
*
*      INTEGER     MSGQCL,      MDFQCL,      MDVQCL
*      PARAMETER ( MSGQCL = 10, MDFQCL = 10, MDVQCL = 8 )
*
* MSGZCL - Max # segments for Z correction to drift (DR stereo wires)
* MDFZCL - Max # drift regions for Z correction to drift (DR stereo wires)
*
      INTEGER     MSGZCL,      MDFZCL
      PARAMETER ( MSGZCL = 20, MDFZCL = 20 )
*
* MSGACL - Max # segments for sine of entrance-angle (DR only)
* MDFACL - Max # drift regions for entrance-angle correction (DR only)
*
*      INTEGER     MSGACL,       MDFACL
*      PARAMETER ( MSGACL = 90, MDFACL = 6 )
*
* KEVDCL - Maximum negative layer number for VD east signal propagation
* MSIPCL - Max # of segments in Z for signal propagation correction
*
      INTEGER     KEVDCL,      MSIPCL
      PARAMETER ( KEVDCL = 10, MSIPCL = 14 )
*
* MXWTCL - Max # parameters for weight function
*
      INTEGER     MXWTCL
      PARAMETER ( MXWTCL = 5 )
*
* MCRTCL - Maximum crate address
* MSLTCL - Maximum slot number in any crate
*
      INTEGER     MCRTCL,       MSLTCL
      PARAMETER ( MCRTCL = 56,  MSLTCL = 23 )
C                          ^THIS IS A KLUDGE.  Since CRTDEV bombs when
C                           the crate number is out of range, this is
C                           the highest crate number CURRENTLY used in
C                           the CD, with no room for expansion.
C
#if defined(CLEO_VIANA)
C
C 14-Dec-87  MH   The following was added by Martin Hempstead and Vivek Sharma
C            VS   for use with VD anode charge gains and various corrections
C                 needed by VIANA.
*
* MPCDRF - Number of params for VD drift-time correction
* MPCDIP - Number of params for VD dip angle correction
* MVDCRT - Number of VD crates
*
      INTEGER     MPCDRF,     MPCDIP,     MVDCRT
      PARAMETER ( MPCDRF = 4, MPCDIP = 3, MVDCRT = 4 )
C
C                 Following are for the Pulse Height map
*
* MBINGN - Number of VD charge gain bins
* MBINCT - Number of VD dip angle bins
* MBINDF - Number of VD drift dist bins
* MBINPH - Number of VD pulse height bins (categories)
*
      INTEGER     MBINGN,     MBINCT,     MBINDF,     MBINPH
      PARAMETER ( MBINGN = 5, MBINCT = 8, MBINDF = 6, MBINPH = 3 )
#endif




