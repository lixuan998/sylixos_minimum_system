/*
 * Copyright (c) 2013 ARM Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <private/bionic_asm.h>

#ifdef __ARMEB__
#define S2LOMEM lsl
#define S2LOMEMEQ lsleq
#define S2HIMEM lsr
#define MSB 0x000000ff
#define LSB 0xff000000
#define BYTE0_OFFSET 24
#define BYTE1_OFFSET 16
#define BYTE2_OFFSET 8
#define BYTE3_OFFSET 0
#else /* not  __ARMEB__ */
#define S2LOMEM lsr
#define S2LOMEMEQ lsreq
#define S2HIMEM lsl
#define BYTE0_OFFSET 0
#define BYTE1_OFFSET 8
#define BYTE2_OFFSET 16
#define BYTE3_OFFSET 24
#define MSB 0xff000000
#define LSB 0x000000ff
#endif /* not  __ARMEB__ */

.syntax         unified

#if defined (__thumb__)
        .thumb
        .thumb_func
#endif

ENTRY(strcmp)
      /* Use LDRD whenever possible.  */

/* The main thing to look out for when comparing large blocks is that
   the loads do not cross a page boundary when loading past the index
   of the byte with the first difference or the first string-terminator.

   For example, if the strings are identical and the string-terminator
   is at index k, byte by byte comparison will not load beyond address
   s1+k and s2+k; word by word comparison may load up to 3 bytes beyond
   k; double word - up to 7 bytes.  If the load of these bytes crosses
   a page boundary, it might cause a memory fault (if the page is not mapped)
   that would not have happened in byte by byte comparison.

   If an address is (double) word aligned, then a load of a (double) word
   from that address will not cross a page boundary.
   Therefore, the algorithm below considers word and double-word alignment
   of strings separately.  */

/* High-level description of the algorithm.

   * The fast path: if both strings are double-word aligned,
     use LDRD to load two words from each string in every loop iteration.
   * If the strings have the same offset from a word boundary,
     use LDRB to load and compare byte by byte until
     the first string is aligned to a word boundary (at most 3 bytes).
     This is optimized for quick return on short unaligned strings.
   * If the strings have the same offset from a double-word boundary,
     use LDRD to load two words from each string in every loop iteration, as in the fast path.
   * If the strings do not have the same offset from a double-word boundary,
     load a word from the second string before the loop to initialize the queue.
     Use LDRD to load two words from every string in every loop iteration.
     Inside the loop, load the second word from the second string only after comparing
     the first word, using the queued value, to guarantee safety across page boundaries.
   * If the strings do not have the same offset from a word boundary,
     use LDR and a shift queue. Order of loads and comparisons matters,
     similarly to the previous case.

   * Use UADD8 and SEL to compare words, and use REV and CLZ to compute the return value.
   * The only difference between ARM and Thumb modes is the use of CBZ instruction.
   * The only difference between big and little endian is the use of REV in little endian
     to compute the return value, instead of MOV.
*/

        .macro m_cbz reg label
#ifdef __thumb2__
        cbz     \reg, \label
#else   /* not defined __thumb2__ */
        cmp     \reg, #0
        beq     \label
#endif /* not defined __thumb2__ */
        .endm /* m_cbz */

        .macro m_cbnz reg label
#ifdef __thumb2__
        cbnz    \reg, \label
#else   /* not defined __thumb2__ */
        cmp     \reg, #0
        bne     \label
#endif /* not defined __thumb2__ */
        .endm /* m_cbnz */

        .macro  init
        /* Macro to save temporary registers and prepare magic values.  */
        subs    sp, sp, #16
        .cfi_def_cfa_offset 16
        strd    r4, r5, [sp, #8]
        .cfi_rel_offset r4, 0
        .cfi_rel_offset r5, 4
        strd    r6, r7, [sp]
        .cfi_rel_offset r6, 8
        .cfi_rel_offset r7, 12
        mvn     r6, #0  /* all F */
        mov     r7, #0  /* all 0 */
        .endm   /* init */

        .macro  magic_compare_and_branch w1 w2 label
        /* Macro to compare registers w1 and w2 and conditionally branch to label.  */
        cmp     \w1, \w2        /* Are w1 and w2 the same?  */
        magic_find_zero_bytes \w1
        it      eq
        cmpeq   ip, #0          /* Is there a zero byte in w1?  */
        bne     \label
        .endm /* magic_compare_and_branch */

        .macro  magic_find_zero_bytes w1
        /* Macro to find all-zero bytes in w1, result is in ip.  */
        uadd8   ip, \w1, r6
        sel     ip, r7, r6
        .endm /* magic_find_zero_bytes */

        .macro  setup_return w1 w2
#ifdef __ARMEB__
        mov     r1, \w1
        mov     r2, \w2
#else /* not  __ARMEB__ */
        rev     r1, \w1
        rev     r2, \w2
#endif /* not  __ARMEB__ */
        .endm /* setup_return */

        pld [r0, #0]
        pld [r1, #0]

        /* Are both strings double-word aligned?  */
        orr     ip, r0, r1
        tst     ip, #7
        bne     .L_do_align

        /* Fast path.  */
        init

.L_doubleword_aligned:

        /* Get here when the strings to compare are double-word aligned.  */
        /* Compare two words in every iteration.  */
        .p2align        2
2:
        pld [r0, #16]
        pld [r1, #16]

        /* Load the next double-word from each string.  */
        ldrd    r2, r3, [r0], #8
        ldrd    r4, r5, [r1], #8

        magic_compare_and_branch w1=r2, w2=r4, label=.L_return_24
        magic_compare_and_branch w1=r3, w2=r5, label=.L_return_35
        b       2b

.L_do_align:
        /* Is the first string word-aligned?  */
        ands    ip, r0, #3
        beq     .L_word_aligned_r0

        /* Fast compare byte by byte until the first string is word-aligned.  */
        /* The offset of r0 from a word boundary is in ip. Thus, the number of bytes
        to read until the next word boundary is 4-ip.  */
        bic     r0, r0, #3
        ldr     r2, [r0], #4
        lsls    ip, ip, #31
        beq     .L_byte2
        bcs     .L_byte3

.L_byte1:
        ldrb    ip, [r1], #1
        uxtb    r3, r2, ror #BYTE1_OFFSET
        subs    ip, r3, ip
        bne     .L_fast_return
        m_cbz   reg=r3, label=.L_fast_return

.L_byte2:
        ldrb    ip, [r1], #1
        uxtb    r3, r2, ror #BYTE2_OFFSET
        subs    ip, r3, ip
        bne     .L_fast_return
        m_cbz   reg=r3, label=.L_fast_return

.L_byte3:
        ldrb    ip, [r1], #1
        uxtb    r3, r2, ror #BYTE3_OFFSET
        subs    ip, r3, ip
        bne     .L_fast_return
        m_cbnz  reg=r3, label=.L_word_aligned_r0

.L_fast_return:
        mov     r0, ip
        bx      lr

.L_word_aligned_r0:
        init
        /* The first string is word-aligned.  */
        /* Is the second string word-aligned?  */
        ands    ip, r1, #3
        bne     .L_strcmp_unaligned

.L_word_aligned:
        /* The strings are word-aligned. */
        /* Is the first string double-word aligned?  */
        tst     r0, #4
        beq     .L_doubleword_aligned_r0

        /* If r0 is not double-word aligned yet, align it by loading
        and comparing the next word from each string.  */
        ldr     r2, [r0], #4
        ldr     r4, [r1], #4
        magic_compare_and_branch w1=r2 w2=r4 label=.L_return_24

.L_doubleword_aligned_r0:
        /* Get here when r0 is double-word aligned.  */
        /* Is r1 doubleword_aligned?  */
        tst     r1, #4
        beq     .L_doubleword_aligned

        /* Get here when the strings to compare are word-aligned,
        r0 is double-word aligned, but r1 is not double-word aligned.  */

        /* Initialize the queue.  */
        ldr     r5, [r1], #4

        /* Compare two words in every iteration.  */
        .p2align        2
3:
        pld [r0, #16]
        pld [r1, #16]

        /* Load the next double-word from each string and compare.  */
        ldrd    r2, r3, [r0], #8
        magic_compare_and_branch w1=r2 w2=r5 label=.L_return_25
        ldrd    r4, r5, [r1], #8
        magic_compare_and_branch w1=r3 w2=r4 label=.L_return_34
        b       3b

        .macro miscmp_word offsetlo offsethi
        /* Macro to compare misaligned strings.  */
        /* r0, r1 are word-aligned, and at least one of the strings
        is not double-word aligned.  */
        /* Compare one word in every loop iteration.  */
        /* OFFSETLO is the original bit-offset of r1 from a word-boundary,
        OFFSETHI is 32 - OFFSETLO (i.e., offset from the next word).  */

        /* Initialize the shift queue.  */
        ldr     r5, [r1], #4

        /* Compare one word from each string in every loop iteration.  */
        .p2align        2
7:
        ldr     r3, [r0], #4
        S2LOMEM r5, r5, #\offsetlo
        magic_find_zero_bytes w1=r3
        cmp     r7, ip, S2HIMEM #\offsetlo
        and     r2, r3, r6, S2LOMEM #\offsetlo
        it      eq
        cmpeq   r2, r5
        bne     .L_return_25
        ldr     r5, [r1], #4
        cmp     ip, #0
        eor r3, r2, r3
        S2HIMEM r2, r5, #\offsethi
        it      eq
        cmpeq   r3, r2
        bne     .L_return_32
        b       7b
        .endm /* miscmp_word */

.L_return_32:
        setup_return w1=r3, w2=r2
        b       .L_do_return
.L_return_34:
        setup_return w1=r3, w2=r4
        b       .L_do_return
.L_return_25:
        setup_return w1=r2, w2=r5
        b       .L_do_return
.L_return_35:
        setup_return w1=r3, w2=r5
        b       .L_do_return
.L_return_24:
        setup_return w1=r2, w2=r4

.L_do_return:

#ifdef __ARMEB__
        mov     r0, ip
#else /* not  __ARMEB__ */
        rev     r0, ip
#endif /* not  __ARMEB__ */

        /* Restore temporaries early, before computing the return value.  */
        ldrd    r6, r7, [sp]
        ldrd    r4, r5, [sp, #8]
        adds    sp, sp, #16
        .cfi_def_cfa_offset 0
        .cfi_restore r4
        .cfi_restore r5
        .cfi_restore r6
        .cfi_restore r7

        /* There is a zero or a different byte between r1 and r2.  */
        /* r0 contains a mask of all-zero bytes in r1.  */
        /* Using r0 and not ip here because cbz requires low register.  */
        m_cbz   reg=r0, label=.L_compute_return_value
        clz     r0, r0
        /* r0 contains the number of bits on the left of the first all-zero byte in r1.  */
        rsb     r0, r0, #24
        /* Here, r0 contains the number of bits on the right of the first all-zero byte in r1.  */
        lsr     r1, r1, r0
        lsr     r2, r2, r0

.L_compute_return_value:
        movs    r0, #1
        cmp     r1, r2
        /* The return value is computed as follows.
        If r1>r2 then (C==1 and Z==0) and LS doesn't hold and r0 is #1 at return.
        If r1<r2 then (C==0 and Z==0) and we execute SBC with carry_in=0,
        which means r0:=r0-r0-1 and r0 is #-1 at return.
        If r1=r2 then (C==1 and Z==1) and we execute SBC with carry_in=1,
        which means r0:=r0-r0 and r0 is #0 at return.
        (C==0 and Z==1) cannot happen because the carry bit is "not borrow".  */
        it      ls
        sbcls   r0, r0, r0
        bx      lr

    /* The code from the previous version of strcmp.S handles all of the
     * cases where the first string and seconds string cannot both be
     * aligned to a word boundary faster than the new algorithm. See
     * bionic/libc/arch-arm/cortex-a15/bionic/strcmp.S for the unedited
     * version of the code.
     */
.L_strcmp_unaligned:
	wp1 .req r0
	wp2 .req r1
	b1  .req r2
	w1  .req r4
	w2  .req r5
	t1  .req ip
	@ r3 is scratch

2:
	mov	b1, #1
	orr	b1, b1, b1, lsl #8
	orr	b1, b1, b1, lsl #16

	and	t1, wp2, #3
	bic	wp2, wp2, #3
	ldr	w1, [wp1], #4
	ldr	w2, [wp2], #4
	cmp	t1, #2
	beq	2f
	bhi	3f

	/* Critical inner Loop: Block with 3 bytes initial overlap */
	.p2align	2
1:
	bic	t1, w1, #MSB
	cmp	t1, w2, S2LOMEM #8
	sub	r3, w1, b1
	bic	r3, r3, w1
	bne	4f
	ands	r3, r3, b1, lsl #7
	it	eq
	ldreq	w2, [wp2], #4
	bne	5f
	eor	t1, t1, w1
	cmp	t1, w2, S2HIMEM #24
	bne	6f
	ldr	w1, [wp1], #4
	b	1b
4:
	S2LOMEM	w2, w2, #8
	b	8f

5:
#ifdef __ARMEB__
	/* The syndrome value may contain false ones if the string ends
	 * with the bytes 0x01 0x00
	 */
	tst	w1, #0xff000000
	itt	ne
	tstne	w1, #0x00ff0000
	tstne	w1, #0x0000ff00
	beq	7f
#else
	bics	r3, r3, #0xff000000
	bne	7f
#endif
	ldrb	w2, [wp2]
	S2LOMEM	t1, w1, #24
#ifdef __ARMEB__
	lsl	w2, w2, #24
#endif
	b	8f

6:
	S2LOMEM	t1, w1, #24
	and	w2, w2, #LSB
	b	8f

	/* Critical inner Loop: Block with 2 bytes initial overlap */
	.p2align	2
2:
	S2HIMEM	t1, w1, #16
	sub	r3, w1, b1
	S2LOMEM	t1, t1, #16
	bic	r3, r3, w1
	cmp	t1, w2, S2LOMEM #16
	bne	4f
	ands	r3, r3, b1, lsl #7
	it	eq
	ldreq	w2, [wp2], #4
	bne	5f
	eor	t1, t1, w1
	cmp	t1, w2, S2HIMEM #16
	bne	6f
	ldr	w1, [wp1], #4
	b	2b

5:
#ifdef __ARMEB__
	/* The syndrome value may contain false ones if the string ends
	 * with the bytes 0x01 0x00
	 */
	tst	w1, #0xff000000
	it	ne
	tstne	w1, #0x00ff0000
	beq	7f
#else
	lsls	r3, r3, #16
	bne	7f
#endif
	ldrh	w2, [wp2]
	S2LOMEM	t1, w1, #16
#ifdef __ARMEB__
	lsl	w2, w2, #16
#endif
	b	8f

6:
	S2HIMEM	w2, w2, #16
	S2LOMEM	t1, w1, #16
4:
	S2LOMEM	w2, w2, #16
	b	8f

	/* Critical inner Loop: Block with 1 byte initial overlap */
	.p2align	2
3:
	and	t1, w1, #LSB
	cmp	t1, w2, S2LOMEM #24
	sub	r3, w1, b1
	bic	r3, r3, w1
	bne	4f
	ands	r3, r3, b1, lsl #7
	it	eq
	ldreq	w2, [wp2], #4
	bne	5f
	eor	t1, t1, w1
	cmp	t1, w2, S2HIMEM #8
	bne	6f
	ldr	w1, [wp1], #4
	b	3b
4:
	S2LOMEM	w2, w2, #24
	b	8f
5:
	/* The syndrome value may contain false ones if the string ends
	 * with the bytes 0x01 0x00
	 */
	tst	w1, #LSB
	beq	7f
	ldr	w2, [wp2], #4
6:
	S2LOMEM	t1, w1, #8
	bic	w2, w2, #MSB
	b	8f
7:
	mov	r0, #0

    /* Restore registers and stack. */
    ldrd    r6, r7, [sp]
    ldrd    r4, r5, [sp, #8]
    adds    sp, sp, #16
    .cfi_def_cfa_offset 0
    .cfi_restore r4
    .cfi_restore r5
    .cfi_restore r6
    .cfi_restore r7

	bx	lr

8:
	and	r2, t1, #LSB
	and	r0, w2, #LSB
	cmp	r0, #1
	it	cs
	cmpcs	r0, r2
	itt	eq
	S2LOMEMEQ	t1, t1, #8
	S2LOMEMEQ	w2, w2, #8
	beq	8b
	sub	r0, r2, r0

    /* Restore registers and stack. */
    ldrd    r6, r7, [sp]
    ldrd    r4, r5, [sp, #8]
    adds    sp, sp, #16

	bx	lr
END(strcmp)
