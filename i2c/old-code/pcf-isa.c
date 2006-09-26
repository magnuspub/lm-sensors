/* ------------------------------------------------------------------------- */
/* pcf-isa.c i2c-hw access for PCF 8584 on ISA boards			     */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 1995-96 Simon G. Vogl

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.		     */
/* ------------------------------------------------------------------------- */
static char rcsid[] = "$Id: pcf-isa.c,v 1.1 1998/09/05 18:20:09 i2c Exp $";
/*
 * $Log: pcf-isa.c,v $
 * Revision 1.1  1998/09/05 18:20:09  i2c
 * Initial revision
 *
 * Revision 1.3  1998/01/20 10:01:29  i2c
 * *** empty log message ***
 *
 * Revision 1.2  1996/11/20 20:20:46  i2c
 * first version for ISA bus.
 *
 * Revision 1.1  1996/11/17 11:00:03  i2c
 * Initial revision
 *
 *
 */

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/errno.h>

#include "i2c.h"
#include "i2c-priv.h"

/* ----- global defines -----------------------------------------------	*/
#define DEB(x)		/* should be reasonable open, close &c. 	*/
#define DEB2(x) 	/* low level debugging - very slow 		*/
#define DEBE(x)	x	/* error messages 				*/

/* ----- local functions --------------------------------------------------- */

#if (PCFADAPS) > 1
#  define Local static
#else
#  define Local inline
#endif


Local	int  pcf_read	(int minor, int adr)
{
	if (adr)
		return inb(BASE + 1);		/* read status port	*/
	else
		return inb(BASE);		/* read data port	*/
}


Local	void pcf_write	(int minor, int adr,char data)
{
	if (adr)
		outb(data, BASE + 1);
	else
		outb(data, BASE);
}


Local	int  pcf_init	(int minor)
{
	if (check_region(i2c_table[minor].base, 2) < 0 ) {
		DEBE(printk("i2c_init: Port %#x already in use.\n",
			i2c_table[minor].base));
		return -ENODEV;
	} else {
		request_region(i2c_table[minor].base, 2, 
			"i2c (ISA bus)");
	}
	return 0;
}


Local	void pcf_exit	(int minor)
{
	release_region( i2c_table[minor].base , 2 );
}


/* ------------------------------------------------------------------------
 * Encapsulate the above functions in the correct operations structure.
 * This is only done when more than one hardware adapter is supported.
 */
#if (PCFADAPS) > 1
struct i2c_pcf_ops pcf_isa_ops = {
	pcf_read,
	pcf_write,
	pcf_init,
	pcf_exit
};
#endif