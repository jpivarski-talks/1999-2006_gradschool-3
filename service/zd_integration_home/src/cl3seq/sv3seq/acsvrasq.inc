*
* $Id: acsvrasq.inc,v 1.1.1.1 1995/11/01 21:18:36 lkg Exp $
*
* $Log: acsvrasq.inc,v $
* Revision 1.1.1.1  1995/11/01 21:18:36  lkg
* Import renamed CLEO3 include repository: cl3seq
*
* Revision 1.1.1.1  1994/10/04  20:59:05  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/48 14/02/94  14.44.12  by  Dan Payne
*-- Author :    Dan Payne   18/01/94
C.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
C. ACSVRASQ - This sequence is needed in conjunction with the
C.            sequence ACSVRA to access the SVX raw data.
C.
C. USAGE:     It should be placed in the declarations section of any
C.            routine which uses the sequnce ACSVRA.
C.            Values should be assigned to the variables:
C.                NWSVRA - number of words expected
C.                RWSVRA - R/W mode: 'RO', 'WO'
C.            before ACSVRA is used.
C.
C. Author: Dan Payne (18/01/94)
C.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
C
C # arrays hanging off the RAW data bank
      INTEGER    MTSVRA
      PARAMETER( MTSVRA=2)
C ptr-ptr to arrays (set to zero in sequence: ACSVRA)
      INTEGER IPSVRA(MTSVRA)
C id's assigned to the arrays (set to zero in sequnce: ACSVRA)
      INTEGER IDSVRA(MTSVRA)
C ptr-ptr to channel id's
      INTEGER IPSVRI
C ptr-ptr to ADC values
      INTEGER IPSVRQ
C
C # channels if 'RO' or words requested if 'WO'
      INTEGER NWSVRA
C Read-write mode, 'RO', 'WO'
      CHARACTER*2 RWSVRA

      SAVE IPSVRA, IDSVRA, IPSVRI, IPSVRQ, NWSVRA, RWSVRA
