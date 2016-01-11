/*
 * This software is being provided to you, the LICENSEE, by the 
 * Massachusetts Institute of Technology (M.I.T.) under the following 
 * license.  By obtaining, using and/or copying this software, you agree 
 * that you have read, understood, and will comply with these terms and 
 * conditions:  
 * 
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose and without fee or royalty is hereby 
 * granted, provided that you agree to comply with the following copyright 
 * notice and statements, including the disclaimer, and that the same 
 * appear on ALL copies of the software and documentation, including 
 * modifications that you make for internal use or for distribution:
 * 
 * Copyright 1995 by the Massachusetts Institute of Technology.  All rights 
 * reserved.  
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS", AND M.I.T. MAKES NO REPRESENTATIONS 
 * OR WARRANTIES, EXPRESS OR IMPLIED.  By way of example, but not 
 * limitation, M.I.T. MAKES NO REPRESENTATIONS OR WARRANTIES OF 
 * MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE USE OF 
 * THE LICENSED SOFTWARE OR DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY 
 * PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.   
 * 
 * The name of the Massachusetts Institute of Technology or M.I.T. may NOT
 * be used in advertising or publicity pertaining to distribution of the
 * software.  Title to copyright in this software and any associated
 * documentation shall at all times remain with M.I.T., and USER agrees to
 * preserve same.
 */
 
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* This file declares the sequences that will trigger the release of the
printer or the locator controller or the termination of the debug mode */

#ifndef vtprnctl_h
#define vtprnctl_h
                 
extern unsigned char PRNCTRL_TERM_VT52[];
#define PRNCTRL_TERM_VT52_COUNT 2

extern unsigned char PRNCTRL_TERM_VTANSI[];
#define PRNCTRL_TERM_VTANSI_COUNT 4

extern unsigned char LOCATORCTRL_TERM[];
#define LOCATORCTRL_TERM_COUNT 4

extern unsigned char DEBUG_TERM[];
#define DEBUG_TERM_COUNT 4;

#endif
