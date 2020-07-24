/*
 *
 * $Id: unix_vms.h,v 1.1 1999/05/28 03:11:11 lyon Exp $
 *
 * $Log: unix_vms.h,v $
 * Revision 1.1  1999/05/28 03:11:11  lyon
 * Needed XTS routines from cleoks
 *
 * Revision 1.1  1996/09/11 09:27:56  clib
 * Add msubs to cleoks.
 *
 *
 */
#include <string.h>
extern char strargs[];
extern char *nxtstr;

#ifdef VMS
#include <descrip.h>
typedef struct dsc$descriptor_s VMSstring, *VMSstrptr;
#define init_VMStoC nxtstr = strargs
#define VMStoC(A) \
   strncpy(++nxtstr, (A)->dsc$a_pointer, (A)->dsc$w_length); \
   *(nxtstr += (A)->dsc$w_length) = '\0'
#define adr_CtoVMS(A) (A)->dsc$a_pointer
#define len_CtoVMS(A) (A)->dsc$w_length
#else
#define init_F77toC nxtstr = strargs
#define F77toC(A, B) strncpy(++nxtstr, (A), (B)); *(nxtstr += (B)) = '\0'
#endif

