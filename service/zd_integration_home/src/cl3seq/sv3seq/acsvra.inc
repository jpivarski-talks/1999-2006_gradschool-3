*
* $Id: acsvra.inc,v 1.1.1.1 1995/11/01 21:18:35 lkg Exp $
*
* $Log: acsvra.inc,v $
* Revision 1.1.1.1  1995/11/01 21:18:35  lkg
* Import renamed CLEO3 include repository: cl3seq
*
* Revision 1.1.1.1  1994/10/04  20:59:05  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/48 14/02/94  14.44.12  by  Dan Payne
*-- Author :    Dan Payne   18/01/94
C.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
C. ACSVRA - Get ptr-ptr to channel id and ADC values for RAW SVX data
C.
C. USAGE: a) Include sequence ACSVRASQ in the declarations part of the
C.           routine.
C.        b) Requires standard parameter CRNAME containing name of
C.           current subroutine.
C.        c) Include sequence //SEQ/MESREP in routine.
C.        d) Values must be assigned to the two variables:
C.                RWSVRA,
C.                NWSVRA,
C.           before ACSVRA is used.
C.
C. Inputs    : RWSVRA     ! Read-write mode [ 'RO' | 'WO' ]
C.             NWSVRA     ! For 'WO', max # words needed for each array
C.                          hanging off this bank
C.
C. Outputs   : NWSVRA     ! For 'WO' and 'RO', # words allocated
C.             IPSVRA(2)  ! ptrs-ptrs to arrays in the bank
C.             IDSVRA(2)  ! User id's assigned to the arrays in this bank
C.
C.             IPSVRI     ! ptr-ptr to channel id's
C.             IPSVRQ     ! ptr-ptr to ADC values
C.
C. Author : Dan Payne (18/01/94)
C.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

C Get ptrs-ptrs for SVRA raw hit bank

      IPSVRA(1) = 0
      IPSVRA(2) = 0
      IDSVRA(1) = 0
      IDSVRA(2) = 0

      CALL ZRGUGP('SVRA', RWSVRA, MTSVRA,
     +            NWSVRA, IPSVRA, IDSVRA, CRNAME)

C Check that successful

      IF (IPSVRA(1) .LE. 0 .OR. IPSVRA(2) .LE. 0) THEN
        CHMESS = 'Problem getting ptr-ptrs to SVRA arrays'
        CALL MESLOG(CRNAME, 1, MSERRO)
      ENDIF

      IF (IDSVRA(1) .LE. 0 .OR. IDSVRA(2) .LE. 0) THEN
        CHMESS = 'Problem getting user IDs for SVRA arrays'
        CALL MESLOG(CRNAME, 2, MSERRO)
      ENDIF

      IPSVRI = IPSVRA(1)
      IPSVRQ = IPSVRA(2)



