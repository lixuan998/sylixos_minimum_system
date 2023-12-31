/* Optimized strcmp implementation for PowerPC32.
   Copyright (C) 2003-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdep.h>

/* See strlen.s for comments on how the end-of-string testing works.  */

/* int [r3] strncmp (const char *s1 [r3], const char *s2 [r4], size_t size [r5])  */

EALIGN (strncmp, 4, 0)

#define rTMP2	r0
#define rRTN	r3
#define rSTR1	r3	/* first string arg */
#define rSTR2	r4	/* second string arg */
#define rN	r5	/* max string length */
#define rWORD1	r6	/* current word in s1 */
#define rWORD2	r7	/* current word in s2 */
#define rFEFE	r8	/* constant 0xfefefeff (-0x01010101) */
#define r7F7F	r9	/* constant 0x7f7f7f7f */
#define rNEG	r10	/* ~(word in s1 | 0x7f7f7f7f) */
#define rBITDIF	r11	/* bits that differ in s1 & s2 words */
#define rTMP	r12

	dcbt	0,rSTR1
	or	rTMP, rSTR2, rSTR1
	lis	r7F7F, 0x7f7f
	dcbt	0,rSTR2
	clrlwi.	rTMP, rTMP, 30
	cmplwi	cr1, rN, 0
	lis	rFEFE, -0x101
	bne	L(unaligned)
/* We are word aligned so set up for two loops.  first a word
   loop, then fall into the byte loop if any residual.  */
	srwi.	rTMP, rN, 2
	clrlwi	rN, rN, 30
	addi	rFEFE, rFEFE, -0x101
	addi	r7F7F, r7F7F, 0x7f7f
	cmplwi	cr1, rN, 0
	beq	L(unaligned)

	mtctr	rTMP	/* Power4 wants mtctr 1st in dispatch group.  */
	lwz	rWORD1, 0(rSTR1)
	lwz	rWORD2, 0(rSTR2)
	b	L(g1)

L(g0):
	lwzu	rWORD1, 4(rSTR1)
	bne-	cr1, L(different)
	lwzu	rWORD2, 4(rSTR2)
L(g1):	add	rTMP, rFEFE, rWORD1
	nor	rNEG, r7F7F, rWORD1
	bdz	L(tail)
	and.	rTMP, rTMP, rNEG
	cmpw	cr1, rWORD1, rWORD2
	beq+	L(g0)

/* OK. We've hit the end of the string. We need to be careful that
   we don't compare two strings as different because of gunk beyond
   the end of the strings...  */

#ifdef __LITTLE_ENDIAN__
L(endstring):
	slwi	rTMP, rTMP, 1
	addi    rTMP2, rTMP, -1
	andc    rTMP2, rTMP2, rTMP
	and	rWORD2, rWORD2, rTMP2		/* Mask off gunk.  */
	and	rWORD1, rWORD1, rTMP2
	rlwinm	rTMP2, rWORD2, 8, 0xffffffff	/* Byte reverse word.  */
	rlwinm	rTMP, rWORD1, 8, 0xffffffff
	rlwimi	rTMP2, rWORD2, 24, 0, 7
	rlwimi	rTMP, rWORD1, 24, 0, 7
	rlwimi	rTMP2, rWORD2, 24, 16, 23
	rlwimi	rTMP, rWORD1, 24, 16, 23
	xor.	rBITDIF, rTMP, rTMP2
	sub	rRTN, rTMP, rTMP2
	bgelr+
	ori	rRTN, rTMP2, 1
	blr

L(different):
	lwz	rWORD1, -4(rSTR1)
	rlwinm	rTMP2, rWORD2, 8, 0xffffffff	/* Byte reverse word.  */
	rlwinm	rTMP, rWORD1, 8, 0xffffffff
	rlwimi	rTMP2, rWORD2, 24, 0, 7
	rlwimi	rTMP, rWORD1, 24, 0, 7
	rlwimi	rTMP2, rWORD2, 24, 16, 23
	rlwimi	rTMP, rWORD1, 24, 16, 23
	xor.	rBITDIF, rTMP, rTMP2
	sub	rRTN, rTMP, rTMP2
	bgelr+
	ori	rRTN, rTMP2, 1
	blr

#else
L(endstring):
	and	rTMP, r7F7F, rWORD1
	beq	cr1, L(equal)
	add	rTMP, rTMP, r7F7F
	xor.	rBITDIF, rWORD1, rWORD2
	andc	rNEG, rNEG, rTMP
	blt-	L(highbit)
	cntlzw	rBITDIF, rBITDIF
	cntlzw	rNEG, rNEG
	addi	rNEG, rNEG, 7
	cmpw	cr1, rNEG, rBITDIF
	sub	rRTN, rWORD1, rWORD2
	bgelr+	cr1
L(equal):
	li	rRTN, 0
	blr

L(different):
	lwz	rWORD1, -4(rSTR1)
	xor.	rBITDIF, rWORD1, rWORD2
	sub	rRTN, rWORD1, rWORD2
	bgelr+
L(highbit):
	ori	rRTN, rWORD2, 1
	blr
#endif

/* Oh well.  In this case, we just do a byte-by-byte comparison.  */
	.align 4
L(tail):
	and.	rTMP, rTMP, rNEG
	cmpw	cr1, rWORD1, rWORD2
	bne-	L(endstring)
	addi	rSTR1, rSTR1, 4
	bne-	cr1, L(different)
	addi	rSTR2, rSTR2, 4
	cmplwi	cr1, rN, 0
L(unaligned):
	mtctr   rN	/* Power4 wants mtctr 1st in dispatch group */
	bgt	cr1, L(uz)
L(ux):
	li	rRTN, 0
	blr
	.align 4
L(uz):
	lbz	rWORD1, 0(rSTR1)
	lbz	rWORD2, 0(rSTR2)
	nop
	b	L(u1)
L(u0):
	lbzu	rWORD2, 1(rSTR2)
L(u1):
	bdz	L(u3)
	cmpwi	cr1, rWORD1, 0
	cmpw	rWORD1, rWORD2
	beq-	cr1, L(u3)
	lbzu	rWORD1, 1(rSTR1)
	bne-	L(u2)
	lbzu	rWORD2, 1(rSTR2)
	bdz	L(u3)
	cmpwi	cr1, rWORD1, 0
	cmpw	rWORD1, rWORD2
	bne-	L(u3)
	lbzu	rWORD1, 1(rSTR1)
	bne+	cr1, L(u0)

L(u2):	lbzu	rWORD1, -1(rSTR1)
L(u3):	sub	rRTN, rWORD1, rWORD2
	blr
END (strncmp)
libc_hidden_builtin_def (strncmp)
