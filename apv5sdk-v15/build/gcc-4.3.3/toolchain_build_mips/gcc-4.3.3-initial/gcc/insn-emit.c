/* Generated automatically by the program `genemit'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "function.h"
#include "expr.h"
#include "optabs.h"
#include "real.h"
#include "dfp.h"
#include "flags.h"
#include "output.h"
#include "insn-config.h"
#include "hard-reg-set.h"
#include "recog.h"
#include "resource.h"
#include "reload.h"
#include "toplev.h"
#include "regs.h"
#include "tm-constrs.h"
#include "ggc.h"
#include "basic-block.h"
#include "integrate.h"

#define FAIL return (end_sequence (), _val)
#define DONE return (_val = get_insns (), end_sequence (), _val)

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:734 */
rtx
gen_trap (void)
{
  return gen_rtx_TRAP_IF (VOIDmode,
	const1_rtx,
	const0_rtx);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:779 */
rtx
gen_addsf3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:779 */
rtx
gen_adddf3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:779 */
rtx
gen_addv2sf3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (V2SFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:997 */
rtx
gen_subsf3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:997 */
rtx
gen_subdf3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:997 */
rtx
gen_subv2sf3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (V2SFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1006 */
rtx
gen_subsi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1006 */
rtx
gen_subdi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DImode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1063 */
rtx
gen_mulv2sf3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (V2SFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1146 */
rtx
gen_mulsi3_mult3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1189 */
rtx
gen_mulsi3_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1189 */
rtx
gen_muldi3_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1199 */
rtx
gen_mulsi3_r4000 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1199 */
rtx
gen_muldi3_r4000 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1609 */
rtx
gen_mulsidi3_32bit_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1609 */
rtx
gen_umulsidi3_32bit_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1618 */
rtx
gen_mulsidi3_32bit_r4000 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1618 */
rtx
gen_umulsidi3_32bit_r4000 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1703 */
rtx
gen_msubsidi4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DImode,
	operand3,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1703 */
rtx
gen_umsubsidi4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DImode,
	operand3,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1743 */
rtx
gen_smulsi3_highpart_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)]))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1743 */
rtx
gen_umulsi3_highpart_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)]))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1756 */
rtx
gen_smulsi3_highpart_mulhi_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)]))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1756 */
rtx
gen_umulsi3_highpart_mulhi_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)]))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1794 */
rtx
gen_smuldi3_highpart (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (DImode,
	gen_rtx_LSHIFTRT (TImode,
	gen_rtx_MULT (TImode,
	gen_rtx_SIGN_EXTEND (TImode,
	operand1),
	gen_rtx_SIGN_EXTEND (TImode,
	operand2)),
	const_int_rtx[MAX_SAVED_CONST_INT + (64)]))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1794 */
rtx
gen_umuldi3_highpart (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (DImode,
	gen_rtx_LSHIFTRT (TImode,
	gen_rtx_MULT (TImode,
	gen_rtx_ZERO_EXTEND (TImode,
	operand1),
	gen_rtx_ZERO_EXTEND (TImode,
	operand2)),
	const_int_rtx[MAX_SAVED_CONST_INT + (64)]))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1812 */
rtx
gen_madsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	gen_rtx_MULT (SImode,
	operand1,
	operand2),
	copy_rtx (operand0))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1823 */
rtx
gen_maddsidi4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)),
	operand3));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1823 */
rtx
gen_umaddsidi4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)),
	operand3));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1992 */
rtx
gen_divmodsi4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (SImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_MOD (SImode,
	copy_rtx (operand1),
	copy_rtx (operand2)))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1992 */
rtx
gen_divmoddi4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (DImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_MOD (DImode,
	copy_rtx (operand1),
	copy_rtx (operand2)))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2004 */
rtx
gen_udivmodsi4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UDIV (SImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_UMOD (SImode,
	copy_rtx (operand1),
	copy_rtx (operand2)))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2004 */
rtx
gen_udivmoddi4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UDIV (DImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_UMOD (DImode,
	copy_rtx (operand1),
	copy_rtx (operand2)))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2026 */
rtx
gen_sqrtsf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2026 */
rtx
gen_sqrtdf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (DFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2026 */
rtx
gen_sqrtv2sf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (V2SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2093 */
rtx
gen_abssf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2093 */
rtx
gen_absdf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (DFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2093 */
rtx
gen_absv2sf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (V2SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2109 */
rtx
gen_clzsi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CLZ (SImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2109 */
rtx
gen_clzdi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CLZ (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2124 */
rtx
gen_negsi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (SImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2137 */
rtx
gen_negdi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2149 */
rtx
gen_negsf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2149 */
rtx
gen_negdf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (DFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2149 */
rtx
gen_negv2sf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (V2SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2157 */
rtx
gen_one_cmplsi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (SImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2157 */
rtx
gen_one_cmpldi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2295 */
rtx
gen_truncdfsf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_TRUNCATE (SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2317 */
rtx
gen_truncdisi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2328 */
rtx
gen_truncdihi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (HImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2339 */
rtx
gen_truncdiqi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (QImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2450 */
rtx
gen_zero_extendsidi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2584 */
rtx
gen_extendsidi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2693 */
rtx
gen_extendsfdf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_EXTEND (DFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2721 */
rtx
gen_fix_truncdfsi2_insn (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2731 */
rtx
gen_fix_truncdfsi2_macro (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DFmode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2759 */
rtx
gen_fix_truncsfsi2_insn (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2769 */
rtx
gen_fix_truncsfsi2_macro (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SFmode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2786 */
rtx
gen_fix_truncdfdi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2797 */
rtx
gen_fix_truncsfdi2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2808 */
rtx
gen_floatsidf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (DFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2819 */
rtx
gen_floatdidf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (DFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2830 */
rtx
gen_floatsisf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2841 */
rtx
gen_floatdisf2 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (SFmode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3072 */
rtx
gen_extzvsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTRACT (SImode,
	operand1,
	operand2,
	operand3));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3072 */
rtx
gen_extzvdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTRACT (DImode,
	operand1,
	operand2,
	operand3));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3110 */
rtx
gen_insvsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_ZERO_EXTRACT (SImode,
	operand0,
	operand1,
	operand2),
	operand3);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3110 */
rtx
gen_insvdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_ZERO_EXTRACT (DImode,
	operand0,
	operand1,
	operand2),
	operand3);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3133 */
rtx
gen_mov_lwl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	18));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3133 */
rtx
gen_mov_ldl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	18));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3143 */
rtx
gen_mov_lwr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	19));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3143 */
rtx
gen_mov_ldr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	19));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3154 */
rtx
gen_mov_swl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (BLKmode,
	gen_rtvec (2,
		operand1,
		operand2),
	20));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3154 */
rtx
gen_mov_sdl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (BLKmode,
	gen_rtvec (2,
		operand1,
		operand2),
	20));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3164 */
rtx
gen_mov_swr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (BLKmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand0),
	21));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3164 */
rtx
gen_mov_sdr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (BLKmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand0),
	21));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3346 */
rtx
gen_load_gotsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	24));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3346 */
rtx
gen_load_gotdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	24));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3666 */
rtx
gen_movcc (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	operand1);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4103 */
rtx
gen_movv2sf_hardfloat_64bit (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	operand1);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4116 */
rtx
gen_movv2sf_hardfloat_32bit (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	operand1);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4204 */
rtx
gen_load_lowdf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DFmode,
	gen_rtvec (1,
		operand1),
	0));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4204 */
rtx
gen_load_lowdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (1,
		operand1),
	0));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4204 */
rtx
gen_load_lowv2sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (1,
		operand1),
	0));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4204 */
rtx
gen_load_lowtf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (TFmode,
	gen_rtvec (1,
		operand1),
	0));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4218 */
rtx
gen_load_highdf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4218 */
rtx
gen_load_highdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4218 */
rtx
gen_load_highv2sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4218 */
rtx
gen_load_hightf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (TFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4233 */
rtx
gen_store_worddf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4233 */
rtx
gen_store_worddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4233 */
rtx
gen_store_wordv2sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4233 */
rtx
gen_store_wordtf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4248 */
rtx
gen_mthc1df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	32));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4248 */
rtx
gen_mthc1di (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	32));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4248 */
rtx
gen_mthc1v2sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	32));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4248 */
rtx
gen_mthc1tf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (TFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	32));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4259 */
rtx
gen_mfhc1df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	31));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4259 */
rtx
gen_mfhc1di (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	31));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4259 */
rtx
gen_mfhc1v2sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	31));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4259 */
rtx
gen_mfhc1tf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (1,
		operand1),
	31));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4276 */
rtx
gen_loadgp_newabi_si (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	22));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4276 */
rtx
gen_loadgp_newabi_di (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	22));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4295 */
rtx
gen_loadgp_absolute_si (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		operand1),
	22));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4295 */
rtx
gen_loadgp_absolute_di (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		operand1),
	22));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4311 */
rtx
gen_loadgp_blockage (void)
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		gen_rtx_REG (SImode,
	28)),
	4);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4321 */
rtx
gen_loadgp_rtp_si (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	22));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4321 */
rtx
gen_loadgp_rtp_di (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	22));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4343 */
rtx
gen_cprestore (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (2,
		operand0,
		gen_rtx_USE (VOIDmode,
	gen_rtx_REG (SImode,
	28))),
	5);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4380 */
rtx
gen_sync (void)
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	36);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4385 */
rtx
gen_synci (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	35);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4391 */
rtx
gen_rdhwr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand1),
	34));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4398 */
rtx
gen_clear_hazard (void)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	33),
		gen_hard_reg_clobber (SImode, 31)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4413 */
rtx
gen_memory_barrier (void)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_MEM (BLKmode,
	gen_rtx_SCRATCH (VOIDmode)),
	gen_rtx_UNSPEC (BLKmode,
	gen_rtvec (1,
		const0_rtx),
	41));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4419 */
rtx
gen_sync_compare_and_swapsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (2,
		operand2,
		operand3),
	37))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4419 */
rtx
gen_sync_compare_and_swapdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (2,
		operand2,
		operand3),
	37))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4435 */
rtx
gen_sync_addsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_PLUS (SImode,
	operand0,
	operand1)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4435 */
rtx
gen_sync_adddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_PLUS (DImode,
	operand0,
	operand1)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4450 */
rtx
gen_sync_subsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_MINUS (SImode,
	operand0,
	operand1)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4450 */
rtx
gen_sync_subdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_MINUS (DImode,
	operand0,
	operand1)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4462 */
rtx
gen_sync_old_addsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_PLUS (SImode,
	copy_rtx (operand1),
	operand2)),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4462 */
rtx
gen_sync_old_adddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_PLUS (DImode,
	copy_rtx (operand1),
	operand2)),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4479 */
rtx
gen_sync_old_subsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_MINUS (SImode,
	copy_rtx (operand1),
	operand2)),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4479 */
rtx
gen_sync_old_subdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_MINUS (DImode,
	copy_rtx (operand1),
	operand2)),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4493 */
rtx
gen_sync_new_addsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_PLUS (SImode,
	copy_rtx (operand1),
	copy_rtx (operand2))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4493 */
rtx
gen_sync_new_adddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_PLUS (DImode,
	copy_rtx (operand1),
	copy_rtx (operand2))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4510 */
rtx
gen_sync_new_subsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_MINUS (SImode,
	copy_rtx (operand1),
	copy_rtx (operand2))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4510 */
rtx
gen_sync_new_subdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_MINUS (DImode,
	copy_rtx (operand1),
	copy_rtx (operand2))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
rtx
gen_sync_iorsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_IOR (SImode,
	operand1,
	operand0)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
rtx
gen_sync_xorsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_XOR (SImode,
	operand1,
	operand0)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
rtx
gen_sync_andsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_AND (SImode,
	operand1,
	operand0)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
rtx
gen_sync_iordi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_IOR (DImode,
	operand1,
	operand0)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
rtx
gen_sync_xordi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_XOR (DImode,
	operand1,
	operand0)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
rtx
gen_sync_anddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_AND (DImode,
	operand1,
	operand0)),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
rtx
gen_sync_old_iorsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_IOR (SImode,
	operand2,
	copy_rtx (operand1))),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
rtx
gen_sync_old_xorsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_XOR (SImode,
	operand2,
	copy_rtx (operand1))),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
rtx
gen_sync_old_andsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_AND (SImode,
	operand2,
	copy_rtx (operand1))),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
rtx
gen_sync_old_iordi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_IOR (DImode,
	operand2,
	copy_rtx (operand1))),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
rtx
gen_sync_old_xordi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_XOR (DImode,
	operand2,
	copy_rtx (operand1))),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
rtx
gen_sync_old_anddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_AND (DImode,
	operand2,
	copy_rtx (operand1))),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
rtx
gen_sync_new_iorsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_IOR (SImode,
	operand2,
	copy_rtx (operand1))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
rtx
gen_sync_new_xorsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_XOR (SImode,
	operand2,
	copy_rtx (operand1))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
rtx
gen_sync_new_andsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		gen_rtx_AND (SImode,
	operand2,
	copy_rtx (operand1))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
rtx
gen_sync_new_iordi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_IOR (DImode,
	operand2,
	copy_rtx (operand1))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
rtx
gen_sync_new_xordi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_XOR (DImode,
	operand2,
	copy_rtx (operand1))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
rtx
gen_sync_new_anddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		gen_rtx_AND (DImode,
	operand2,
	copy_rtx (operand1))),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4573 */
rtx
gen_sync_nandsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		operand1),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4573 */
rtx
gen_sync_nanddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		operand1),
	38));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4586 */
rtx
gen_sync_old_nandsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		operand2),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4586 */
rtx
gen_sync_old_nanddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		operand2),
	38))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4601 */
rtx
gen_sync_new_nandsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		operand2),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4601 */
rtx
gen_sync_new_nanddi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		operand2),
	39))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4616 */
rtx
gen_sync_lock_test_and_setsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		operand2),
	40))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4616 */
rtx
gen_sync_lock_test_and_setdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_UNSPEC_VOLATILE (DImode,
	gen_rtvec (1,
		operand2),
	40))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4834 */
rtx
gen_rotrsi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ROTATERT (SImode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4834 */
rtx
gen_rotrdi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ROTATERT (DImode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_sunordered_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNORDERED (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_suneq_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNEQ (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_sunlt_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNLT (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_sunle_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNLE (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_seq_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_EQ (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_slt_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LT (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_sle_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LE (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_sunordered_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNORDERED (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_suneq_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNEQ (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_sunlt_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNLT (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_sunle_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNLE (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_seq_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_EQ (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_slt_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LT (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
rtx
gen_sle_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LE (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
rtx
gen_sge_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GE (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
rtx
gen_sgt_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GT (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
rtx
gen_sunge_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNGE (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
rtx
gen_sungt_sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNGT (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
rtx
gen_sge_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GE (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
rtx
gen_sgt_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GT (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
rtx
gen_sunge_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNGE (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
rtx
gen_sungt_df (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNGT (CCmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5345 */
rtx
gen_jump (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_LABEL_REF (VOIDmode,
	operand0));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5400 */
rtx
gen_indirect_jumpsi (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5400 */
rtx
gen_indirect_jumpdi (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5438 */
rtx
gen_tablejumpsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0),
		gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand1))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5438 */
rtx
gen_tablejumpdi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0),
		gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand1))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5516 */
rtx
gen_blockage (void)
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	4);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5543 */
rtx
gen_return (void)
{
  return gen_rtx_RETURN (VOIDmode);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5552 */
rtx
gen_return_internal (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_RETURN (VOIDmode),
		gen_rtx_USE (VOIDmode,
	operand0)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5577 */
rtx
gen_eh_set_lr_si (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_UNSPEC (VOIDmode,
	gen_rtvec (1,
		operand0),
	7),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5583 */
rtx
gen_eh_set_lr_di (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_UNSPEC (VOIDmode,
	gen_rtvec (1,
		operand0),
	7),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5623 */
rtx
gen_restore_gp (void)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (SImode,
	28),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		const0_rtx),
	6));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5687 */
rtx
gen_load_callsi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (SImode,
	79)),
	23));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5687 */
rtx
gen_load_calldi (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (SImode,
	79)),
	23));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5698 */
rtx
gen_set_got_version (void)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (SImode,
	79),
	gen_rtx_UNSPEC_VOLATILE (SImode,
	gen_rtvec (1,
		const0_rtx),
	42));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5706 */
rtx
gen_update_got_version (void)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (SImode,
	79),
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		gen_rtx_REG (SImode,
	79)),
	43));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5738 */
rtx
gen_sibcall_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand0),
	operand1);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5757 */
rtx
gen_sibcall_value_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand1),
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5765 */
rtx
gen_sibcall_value_multiple_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand1),
	operand2)),
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	copy_rtx (operand1)),
	copy_rtx (operand2)))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5824 */
rtx
gen_call_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand0),
	operand1),
		gen_hard_reg_clobber (SImode, 31)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5845 */
rtx
gen_call_internal_direct (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand0),
	operand1),
		const1_rtx,
		gen_hard_reg_clobber (SImode, 31)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5853 */
rtx
gen_call_split (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand0),
	operand1),
		gen_hard_reg_clobber (SImode, 31),
		gen_hard_reg_clobber (SImode, 28)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5875 */
rtx
gen_call_value_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand1),
	operand2)),
		gen_hard_reg_clobber (SImode, 31)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5894 */
rtx
gen_call_value_split (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand1),
	operand2)),
		gen_hard_reg_clobber (SImode, 31),
		gen_hard_reg_clobber (SImode, 28)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5905 */
rtx
gen_call_value_internal_direct (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand1),
	operand2)),
		const1_rtx,
		gen_hard_reg_clobber (SImode, 31)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5915 */
rtx
gen_call_value_multiple_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand1),
	operand2)),
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	copy_rtx (operand1)),
	copy_rtx (operand2))),
		gen_hard_reg_clobber (SImode, 31)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5937 */
rtx
gen_call_value_multiple_split (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (4,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	operand1),
	operand2)),
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_CALL (VOIDmode,
	gen_rtx_MEM (SImode,
	copy_rtx (operand1)),
	copy_rtx (operand2))),
		gen_hard_reg_clobber (SImode, 31),
		gen_hard_reg_clobber (SImode, 28)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5982 */
rtx
gen_prefetch (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PREFETCH (VOIDmode,
	operand0,
	operand1,
	operand2);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6005 */
rtx
gen_nop (void)
{
  return const0_rtx;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6013 */
rtx
gen_hazard_nop (void)
{
  return const1_rtx;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6090 */
rtx
gen_consttable_int (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (2,
		operand0,
		operand1),
	8);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6102 */
rtx
gen_consttable_float (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	9);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6118 */
rtx
gen_align (rtx operand0 ATTRIBUTE_UNUSED)
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	14);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:35 */
rtx
gen_mips_cond_move_tf_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	200));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:69 */
rtx
gen_mips_pul_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_VEC_MERGE (V2SFmode,
	operand1,
	operand2,
	const_int_rtx[MAX_SAVED_CONST_INT + (2)]));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:81 */
rtx
gen_mips_puu_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_VEC_MERGE (V2SFmode,
	operand1,
	gen_rtx_VEC_SELECT (V2SFmode,
	operand2,
	gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		const1_rtx,
		const0_rtx))),
	const_int_rtx[MAX_SAVED_CONST_INT + (2)]));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:95 */
rtx
gen_mips_pll_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_VEC_MERGE (V2SFmode,
	gen_rtx_VEC_SELECT (V2SFmode,
	operand1,
	gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		const1_rtx,
		const0_rtx))),
	operand2,
	const_int_rtx[MAX_SAVED_CONST_INT + (2)]));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:109 */
rtx
gen_mips_plu_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_VEC_MERGE (V2SFmode,
	gen_rtx_VEC_SELECT (V2SFmode,
	operand1,
	gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		const1_rtx,
		const0_rtx))),
	gen_rtx_VEC_SELECT (V2SFmode,
	operand2,
	gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		const1_rtx,
		const0_rtx))),
	const_int_rtx[MAX_SAVED_CONST_INT + (2)]));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:136 */
rtx
gen_vec_initv2sf_internal (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_VEC_CONCAT (V2SFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:155 */
rtx
gen_vec_extractv2sf (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_VEC_SELECT (SFmode,
	operand1,
	gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (1,
		operand2))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:225 */
rtx
gen_mips_alnv_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	202));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:237 */
rtx
gen_mips_addr_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	204));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:248 */
rtx
gen_mips_cvt_pw_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (1,
		operand1),
	205));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:258 */
rtx
gen_mips_cvt_ps_pw (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (1,
		operand1),
	206));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:268 */
rtx
gen_mips_mulr_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	207));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:307 */
rtx
gen_mips_cabs_cond_s (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	203));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:307 */
rtx
gen_mips_cabs_cond_d (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	203));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:323 */
rtx
gen_mips_c_cond_4s (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED,
	rtx operand4 ATTRIBUTE_UNUSED,
	rtx operand5 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV4mode,
	gen_rtvec (5,
		operand1,
		operand2,
		operand3,
		operand4,
		operand5),
	201));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:352 */
rtx
gen_mips_cabs_cond_4s (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED,
	rtx operand4 ATTRIBUTE_UNUSED,
	rtx operand5 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV4mode,
	gen_rtvec (5,
		operand1,
		operand2,
		operand3,
		operand4,
		operand5),
	203));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:386 */
rtx
gen_mips_c_cond_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	201));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:397 */
rtx
gen_mips_cabs_cond_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	203));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
rtx
gen_sunordered_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_UNORDERED (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
rtx
gen_suneq_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_UNEQ (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
rtx
gen_sunlt_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_UNLT (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
rtx
gen_sunle_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_UNLE (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
rtx
gen_seq_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_EQ (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
rtx
gen_slt_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_LT (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
rtx
gen_sle_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_LE (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:424 */
rtx
gen_sge_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_GE (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:424 */
rtx
gen_sgt_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_GT (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:424 */
rtx
gen_sunge_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_UNGE (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:424 */
rtx
gen_sungt_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		gen_rtx_UNGT (VOIDmode,
	operand1,
	operand2)),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:440 */
rtx
gen_bc1any4t (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_NE (VOIDmode,
	operand0,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand1),
	pc_rtx));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:452 */
rtx
gen_bc1any4f (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_NE (VOIDmode,
	operand0,
	constm1_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand1),
	pc_rtx));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:464 */
rtx
gen_bc1any2t (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_NE (VOIDmode,
	operand0,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand1),
	pc_rtx));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:476 */
rtx
gen_bc1any2f (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_NE (VOIDmode,
	operand0,
	constm1_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand1),
	pc_rtx));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:544 */
rtx
gen_mips_rsqrt1_s (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SFmode,
	gen_rtvec (1,
		operand1),
	209));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:544 */
rtx
gen_mips_rsqrt1_d (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DFmode,
	gen_rtvec (1,
		operand1),
	209));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:544 */
rtx
gen_mips_rsqrt1_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (1,
		operand1),
	209));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:553 */
rtx
gen_mips_rsqrt2_s (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	210));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:553 */
rtx
gen_mips_rsqrt2_d (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	210));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:553 */
rtx
gen_mips_rsqrt2_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	210));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:563 */
rtx
gen_mips_recip1_s (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SFmode,
	gen_rtvec (1,
		operand1),
	211));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:563 */
rtx
gen_mips_recip1_d (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DFmode,
	gen_rtvec (1,
		operand1),
	211));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:563 */
rtx
gen_mips_recip1_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (1,
		operand1),
	211));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:572 */
rtx
gen_mips_recip2_s (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	212));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:572 */
rtx
gen_mips_recip2_d (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	212));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:572 */
rtx
gen_mips_recip2_ps (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (2,
		operand1,
		operand2),
	212));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1 */
rtx
gen_addv2hi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (V2HImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	300))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1 */
rtx
gen_addv4qi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (V4QImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	300))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:50 */
rtx
gen_mips_addq_s_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	301)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:50 */
rtx
gen_mips_addq_s_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	301)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:50 */
rtx
gen_mips_addu_s_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	301)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:64 */
rtx
gen_subv2hi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (V2HImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	302))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:64 */
rtx
gen_subv4qi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (V4QImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	302))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:76 */
rtx
gen_mips_subq_s_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	303)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:76 */
rtx
gen_mips_subq_s_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	303)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:76 */
rtx
gen_mips_subu_s_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	303)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:90 */
rtx
gen_mips_addsc (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	304)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	184),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	304))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:104 */
rtx
gen_mips_addwc (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	184)),
	305)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	305))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:119 */
rtx
gen_mips_modsub (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	306));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:130 */
rtx
gen_mips_raddu_w_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	307));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:140 */
rtx
gen_mips_absq_s_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	308)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (1,
		operand1),
	308))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:140 */
rtx
gen_mips_absq_s_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	308)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (1,
		operand1),
	308))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:153 */
rtx
gen_mips_precrq_qb_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	309));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:163 */
rtx
gen_mips_precrq_ph_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	310));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:173 */
rtx
gen_mips_precrq_rs_ph_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	311)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	311))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:188 */
rtx
gen_mips_precrqu_s_qb_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	312)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	312))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:203 */
rtx
gen_mips_preceq_w_phl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	313));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:212 */
rtx
gen_mips_preceq_w_phr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	314));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:222 */
rtx
gen_mips_precequ_ph_qbl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	315));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:231 */
rtx
gen_mips_precequ_ph_qbr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	316));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:240 */
rtx
gen_mips_precequ_ph_qbla (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	317));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:249 */
rtx
gen_mips_precequ_ph_qbra (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	318));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:259 */
rtx
gen_mips_preceu_ph_qbl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	319));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:268 */
rtx
gen_mips_preceu_ph_qbr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	320));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:277 */
rtx
gen_mips_preceu_ph_qbla (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	321));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:286 */
rtx
gen_mips_preceu_ph_qbra (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	322));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:297 */
rtx
gen_mips_shll_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	323)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	323))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:297 */
rtx
gen_mips_shll_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	323)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	323))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:319 */
rtx
gen_mips_shll_s_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	324)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	324))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:319 */
rtx
gen_mips_shll_s_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	324)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	324))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:342 */
rtx
gen_mips_shrl_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	325));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:361 */
rtx
gen_mips_shra_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	326));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:379 */
rtx
gen_mips_shra_r_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	327));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:379 */
rtx
gen_mips_shra_r_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	327));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:400 */
rtx
gen_mips_muleu_s_ph_qbl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	328)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	328)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:414 */
rtx
gen_mips_muleu_s_ph_qbr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	329)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	329)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:429 */
rtx
gen_mips_mulq_rs_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	330)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	330)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:444 */
rtx
gen_mips_muleq_s_w_phl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	331)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	331)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:458 */
rtx
gen_mips_muleq_s_w_phr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	332)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	332)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:473 */
rtx
gen_mips_dpau_h_qbl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	333));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:484 */
rtx
gen_mips_dpau_h_qbr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	334));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:496 */
rtx
gen_mips_dpsu_h_qbl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	335));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:507 */
rtx
gen_mips_dpsu_h_qbr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	336));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:519 */
rtx
gen_mips_dpaq_s_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	337)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	337))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:535 */
rtx
gen_mips_dpsq_s_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	338)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	338))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:551 */
rtx
gen_mips_mulsaq_s_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	339)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	339))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:567 */
rtx
gen_mips_dpaq_sa_l_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	340)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	340))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:583 */
rtx
gen_mips_dpsq_sa_l_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	341)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	341))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:599 */
rtx
gen_mips_maq_s_w_phl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	342)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	342))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:614 */
rtx
gen_mips_maq_s_w_phr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	343)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	343))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:630 */
rtx
gen_mips_maq_sa_w_phl (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	344)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	344))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:645 */
rtx
gen_mips_maq_sa_w_phr (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	345)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	345))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:662 */
rtx
gen_mips_bitrev (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	346));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:672 */
rtx
gen_mips_insv (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (4,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	183),
		gen_rtx_REG (CCDSPmode,
	182)),
	347));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:685 */
rtx
gen_mips_repl_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (1,
		operand1),
	348));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:702 */
rtx
gen_mips_repl_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (1,
		operand1),
	349));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:715 */
rtx
gen_mips_cmp_eq_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand0,
		operand1,
		gen_rtx_REG (CCDSPmode,
	186)),
	350));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:715 */
rtx
gen_mips_cmpu_eq_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand0,
		operand1,
		gen_rtx_REG (CCDSPmode,
	186)),
	350));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:726 */
rtx
gen_mips_cmp_lt_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand0,
		operand1,
		gen_rtx_REG (CCDSPmode,
	186)),
	351));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:726 */
rtx
gen_mips_cmpu_lt_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand0,
		operand1,
		gen_rtx_REG (CCDSPmode,
	186)),
	351));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:737 */
rtx
gen_mips_cmp_le_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand0,
		operand1,
		gen_rtx_REG (CCDSPmode,
	186)),
	352));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:737 */
rtx
gen_mips_cmpu_le_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand0,
		operand1,
		gen_rtx_REG (CCDSPmode,
	186)),
	352));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:748 */
rtx
gen_mips_cmpgu_eq_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	353));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:758 */
rtx
gen_mips_cmpgu_lt_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	354));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:768 */
rtx
gen_mips_cmpgu_le_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	355));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:779 */
rtx
gen_mips_pick_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	186)),
	356));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:779 */
rtx
gen_mips_pick_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	186)),
	356));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:791 */
rtx
gen_mips_packrl_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	357));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:803 */
rtx
gen_mips_extr_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	358)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	358))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:824 */
rtx
gen_mips_extr_r_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	359)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	359))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:845 */
rtx
gen_mips_extr_rs_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	360)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	360))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:867 */
rtx
gen_mips_extr_s_h (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	361)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	361))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:889 */
rtx
gen_mips_extp (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	182)),
	362)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	187),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	362))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:911 */
rtx
gen_mips_extpdp (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	182)),
	363)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	182),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	182)),
	363)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	187),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	363))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:937 */
rtx
gen_mips_shilo (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	364));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:956 */
rtx
gen_mips_mthlip (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	182)),
	365)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	182),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	182)),
	365))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:972 */
rtx
gen_mips_wrdsp (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (6,
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	182),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand0,
		operand1),
	366)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	183),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand0,
		operand1),
	366)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	184),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand0,
		operand1),
	366)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand0,
		operand1),
	366)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand0,
		operand1),
	366)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	187),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand0,
		operand1),
	366))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:994 */
rtx
gen_mips_rddsp (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (7,
		operand1,
		gen_rtx_REG (CCDSPmode,
	182),
		gen_rtx_REG (CCDSPmode,
	183),
		gen_rtx_REG (CCDSPmode,
	184),
		gen_rtx_REG (CCDSPmode,
	185),
		gen_rtx_REG (CCDSPmode,
	186),
		gen_rtx_REG (CCDSPmode,
	187)),
	367));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1011 */
rtx
gen_mips_lbux (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (SImode,
	gen_rtx_MEM (QImode,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1023 */
rtx
gen_mips_lhx (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (SImode,
	gen_rtx_MEM (HImode,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1035 */
rtx
gen_mips_lwx (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1047 */
rtx
gen_mips_bposge (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GE (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	182),
	operand0),
	gen_rtx_LABEL_REF (VOIDmode,
	operand1),
	pc_rtx));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:3 */
rtx
gen_mips_absq_s_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (1,
		operand1),
	400)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (1,
		operand1),
	400))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:15 */
rtx
gen_mips_addu_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (V2HImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	401))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:27 */
rtx
gen_mips_addu_s_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	402)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	402))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:40 */
rtx
gen_mips_adduh_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	403));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:50 */
rtx
gen_mips_adduh_r_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	404));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:60 */
rtx
gen_mips_append (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	405));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:75 */
rtx
gen_mips_balign (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	406));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:90 */
rtx
gen_mips_cmpgdu_eq_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	407)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	186)),
	407))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:105 */
rtx
gen_mips_cmpgdu_lt_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	408)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	186)),
	408))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:120 */
rtx
gen_mips_cmpgdu_le_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	409)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	186),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		gen_rtx_REG (CCDSPmode,
	186)),
	409))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:135 */
rtx
gen_mips_dpa_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	410));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:146 */
rtx
gen_mips_dps_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	411));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:173 */
rtx
gen_mulv2hi3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (V2HImode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	416)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:186 */
rtx
gen_mips_mul_s_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	417)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	417)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:200 */
rtx
gen_mips_mulq_rs_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	418)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	418)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:214 */
rtx
gen_mips_mulq_s_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	419)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	419)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:228 */
rtx
gen_mips_mulq_s_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	420)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	420)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:242 */
rtx
gen_mips_mulsa_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	421));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:253 */
rtx
gen_mips_mult (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:263 */
rtx
gen_mips_multu (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:273 */
rtx
gen_mips_precr_qb_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	424));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:283 */
rtx
gen_mips_precr_sra_ph_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	425));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:298 */
rtx
gen_mips_precr_sra_r_ph_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	426));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:313 */
rtx
gen_mips_prepend (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	427));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:328 */
rtx
gen_mips_shra_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	428));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:347 */
rtx
gen_mips_shra_r_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	429));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:365 */
rtx
gen_mips_shrl_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	430));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:383 */
rtx
gen_mips_subu_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	431)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	431))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:396 */
rtx
gen_mips_subu_s_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	432)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	432))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:409 */
rtx
gen_mips_subuh_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	433));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:419 */
rtx
gen_mips_subuh_r_qb (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V4QImode,
	gen_rtvec (2,
		operand1,
		operand2),
	434));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:429 */
rtx
gen_mips_addqh_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	435));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:439 */
rtx
gen_mips_addqh_r_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	436));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:449 */
rtx
gen_mips_addqh_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	437));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:459 */
rtx
gen_mips_addqh_r_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	438));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:469 */
rtx
gen_mips_subqh_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	439));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:479 */
rtx
gen_mips_subqh_r_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2HImode,
	gen_rtvec (2,
		operand1,
		operand2),
	440));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:489 */
rtx
gen_mips_subqh_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	441));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:499 */
rtx
gen_mips_subqh_r_w (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	442));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:509 */
rtx
gen_mips_dpax_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	443));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:520 */
rtx
gen_mips_dpsx_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	444));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:531 */
rtx
gen_mips_dpaqx_s_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	445)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	445))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:546 */
rtx
gen_mips_dpaqx_sa_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	446)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	446))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:561 */
rtx
gen_mips_dpsqx_s_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	447)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	447))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:576 */
rtx
gen_mips_dpsqx_sa_w_ph (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	448)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	448))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addqq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (QQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (HQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addsq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_adddq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_adduqq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (UQQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_adduhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (UHQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addusq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (USQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addudq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (UDQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (HAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addsa3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addda3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_adduha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (UHAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_addusa3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (USAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
rtx
gen_adduda3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (UDAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
rtx
gen_usadduqq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_PLUS (UQQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
rtx
gen_usadduhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_PLUS (UHQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
rtx
gen_usadduha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_PLUS (UHAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
rtx
gen_usaddv4uqq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_PLUS (V4UQQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
rtx
gen_usaddv2uhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_PLUS (V2UHQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
rtx
gen_usaddv2uha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_PLUS (V2UHAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
rtx
gen_ssaddhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_PLUS (HQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
rtx
gen_ssaddsq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_PLUS (SQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
rtx
gen_ssaddha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_PLUS (HAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
rtx
gen_ssaddsa3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_PLUS (SAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
rtx
gen_ssaddv2hq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_PLUS (V2HQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
rtx
gen_ssaddv2ha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_PLUS (V2HAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	301))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subqq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (QQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (HQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subsq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subdq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subuqq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (UQQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subuhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (UHQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subusq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (USQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subudq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (UDQmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (HAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subsa3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subda3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subuha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (UHAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subusa3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (USAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
rtx
gen_subuda3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (UDAmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
rtx
gen_ussubuqq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_MINUS (UQQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
rtx
gen_ussubuhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_MINUS (UHQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
rtx
gen_ussubuha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_MINUS (UHAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
rtx
gen_ussubv4uqq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_MINUS (V4UQQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
rtx
gen_ussubv2uhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_MINUS (V2UHQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
rtx
gen_ussubv2uha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_US_MINUS (V2UHAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
rtx
gen_sssubhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MINUS (HQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
rtx
gen_sssubsq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MINUS (SQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
rtx
gen_sssubha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MINUS (HAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
rtx
gen_sssubsa3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MINUS (SAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
rtx
gen_sssubv2hq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MINUS (V2HQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
rtx
gen_sssubv2ha3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MINUS (V2HAmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	303))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:93 */
rtx
gen_ssmulv2hq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MULT (V2HQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	330)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:93 */
rtx
gen_ssmulhq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MULT (HQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	330)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:93 */
rtx
gen_ssmulsq3 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MULT (SQmode,
	operand1,
	operand2)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (2,
		operand1,
		operand2),
	330)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:106 */
rtx
gen_ssmaddsqdq4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_PLUS (DQmode,
	gen_rtx_SS_MULT (DQmode,
	gen_rtx_SAT_FRACT (DQmode,
	operand1),
	gen_rtx_SAT_FRACT (DQmode,
	operand2)),
	operand3)),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	340))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:123 */
rtx
gen_ssmsubsqdq4 (rtx operand0 ATTRIBUTE_UNUSED,
	rtx operand1 ATTRIBUTE_UNUSED,
	rtx operand2 ATTRIBUTE_UNUSED,
	rtx operand3 ATTRIBUTE_UNUSED)
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SS_MINUS (DQmode,
	operand3,
	gen_rtx_SS_MULT (DQmode,
	gen_rtx_SAT_FRACT (DQmode,
	operand1),
	gen_rtx_SAT_FRACT (DQmode,
	operand2)))),
		gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCDSPmode,
	185),
	gen_rtx_UNSPEC (CCDSPmode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand3),
	341))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:747 */
rtx
gen_conditional_trap (rtx operand0,
	rtx operand1)
{
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
#line 752 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_MODE_CLASS (GET_MODE (cmp_operands[0])) == MODE_INT
      && operands[1] == const0_rtx)
    {
      mips_expand_conditional_trap (GET_CODE (operands[0]));
      DONE;
    }
  FAIL;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_TRAP_IF (VOIDmode,
	gen_rtx_fmt_ee (GET_CODE (operand0), VOIDmode,
		operand2,
		operand3),
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:788 */
rtx
gen_addsi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:788 */
rtx
gen_adddi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:839 */
extern rtx gen_split_701 (rtx, rtx *);
rtx
gen_split_701 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 853 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  HOST_WIDE_INT val = INTVAL (operands[1]);

  if (val >= 0)
    {
      operands[1] = GEN_INT (0x7f);
      operands[2] = GEN_INT (val - 0x7f);
    }
  else
    {
      operands[1] = GEN_INT (- 0x80);
      operands[2] = GEN_INT (val + 0x80);
    }
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:868 */
extern rtx gen_split_702 (rtx, rtx *);
rtx
gen_split_702 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 885 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  HOST_WIDE_INT val = INTVAL (operands[2]);

  if (val >= 0)
    {
      operands[2] = GEN_INT (0x7);
      operands[3] = GEN_INT (val - 0x7);
    }
  else
    {
      operands[2] = GEN_INT (- 0x8);
      operands[3] = GEN_INT (val + 0x8);
    }
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:900 */
extern rtx gen_split_703 (rtx, rtx *);
rtx
gen_split_703 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 914 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  HOST_WIDE_INT val = INTVAL (operands[1]);

  if (val >= 0)
    {
      operands[1] = GEN_INT (0xf);
      operands[2] = GEN_INT (val - 0xf);
    }
  else
    {
      operands[1] = GEN_INT (- 0x10);
      operands[2] = GEN_INT (val + 0x10);
    }
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	copy_rtx (operand0),
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (DImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:929 */
extern rtx gen_split_704 (rtx, rtx *);
rtx
gen_split_704 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 946 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  HOST_WIDE_INT val = INTVAL (operands[2]);

  if (val >= 0)
    {
      operands[2] = GEN_INT (0x7);
      operands[3] = GEN_INT (val - 0x7);
    }
  else
    {
      operands[2] = GEN_INT (- 0x8);
      operands[3] = GEN_INT (val + 0x8);
    }
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (DImode,
	copy_rtx (operand0),
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:975 */
extern rtx gen_split_705 (rtx, rtx *);
rtx
gen_split_705 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 984 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[3] = gen_lowpart (SImode, operands[0]); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1033 */
rtx
gen_mulsf3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1033 */
rtx
gen_muldf3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DFmode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1118 */
rtx
gen_mulsi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1123 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (ISA_HAS_MUL3)
    emit_insn (gen_mulsi3_mult3 (operands[0], operands[1], operands[2]));
  else if (TARGET_FIX_R4000)
    emit_insn (gen_mulsi3_r4000 (operands[0], operands[1], operands[2]));
  else
    emit_insn (gen_mulsi3_internal (operands[0], operands[1], operands[2]));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1133 */
rtx
gen_muldi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1138 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_R4000)
    emit_insn (gen_muldi3_r4000 (operands[0], operands[1], operands[2]));
  else
    emit_insn (gen_muldi3_internal (operands[0], operands[1], operands[2]));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1172 */
extern rtx gen_peephole2_710 (rtx, rtx *);
rtx
gen_peephole2_710 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  start_sequence ();
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_MULT (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	operand3),
		gen_rtx_CLOBBER (VOIDmode,
	operand0))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1220 */
extern rtx gen_peephole2_711 (rtx, rtx *);
rtx
gen_peephole2_711 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  start_sequence ();
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	const0_rtx));
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (SImode,
	gen_rtx_MULT (SImode,
	operand1,
	operand2),
	copy_rtx (operand0))),
		gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_PLUS (SImode,
	gen_rtx_MULT (SImode,
	copy_rtx (operand1),
	copy_rtx (operand2)),
	copy_rtx (operand0))),
		gen_rtx_CLOBBER (VOIDmode,
	operand3))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1278 */
extern rtx gen_split_712 (rtx, rtx *);
rtx
gen_split_712 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();
#line 1294 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand6,
	gen_rtx_MULT (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	operand4),
		gen_rtx_CLOBBER (VOIDmode,
	operand5))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	copy_rtx (operand6),
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1297 */
extern rtx gen_split_713 (rtx, rtx *);
rtx
gen_split_713 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();
#line 1315 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (4,
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_PLUS (SImode,
	gen_rtx_MULT (SImode,
	operand1,
	operand2),
	copy_rtx (operand3))),
		gen_rtx_CLOBBER (VOIDmode,
	operand4),
		gen_rtx_CLOBBER (VOIDmode,
	operand5),
		gen_rtx_CLOBBER (VOIDmode,
	operand6))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		copy_rtx (operand5),
		copy_rtx (operand4)),
	26)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1359 */
extern rtx gen_split_714 (rtx, rtx *);
rtx
gen_split_714 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();
#line 1379 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand6,
	gen_rtx_NEG (SImode,
	operand3)));
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	gen_rtx_MULT (SImode,
	operand2,
	copy_rtx (operand6)),
	operand1)),
		gen_rtx_CLOBBER (VOIDmode,
	operand4),
		gen_rtx_CLOBBER (VOIDmode,
	operand5))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1422 */
extern rtx gen_peephole2_715 (rtx, rtx *);
rtx
gen_peephole2_715 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  start_sequence ();
#line 1436 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_SET (VOIDmode,
	operand3,
	copy_rtx (operand1)),
		gen_rtx_CLOBBER (VOIDmode,
	operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1453 */
extern rtx gen_peephole2_716 (rtx, rtx *);
rtx
gen_peephole2_716 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  if ((operands[0] = peep2_find_free_register (0, 1, "d", SImode, &_regs_allocated)) == NULL_RTX)
    return NULL;
  start_sequence ();
#line 1473 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[6] = XEXP (operands[4], GET_CODE (operands[4]) == PLUS ? 0 : 1);
  operands[7] = gen_rtx_fmt_ee (GET_CODE (operands[4]), SImode,
				operands[2], operands[0]);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand6),
		gen_rtx_CLOBBER (VOIDmode,
	operand5),
		gen_rtx_CLOBBER (VOIDmode,
	operand1))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand7));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1489 */
extern rtx gen_peephole2_717 (rtx, rtx *);
rtx
gen_peephole2_717 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  if ((operands[0] = peep2_find_free_register (0, 2, "d", SImode, &_regs_allocated)) == NULL_RTX)
    return NULL;
  start_sequence ();
#line 1509 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[6] = XEXP (operands[4], GET_CODE (operands[4]) == PLUS ? 0 : 1);
  operands[7] = gen_rtx_fmt_ee (GET_CODE (operands[4]), SImode,
				operands[2], operands[0]);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand6),
		gen_rtx_CLOBBER (VOIDmode,
	operand4),
		gen_rtx_CLOBBER (VOIDmode,
	operand1))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand5,
	operand7));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1533 */
extern rtx gen_split_718 (rtx, rtx *);
rtx
gen_split_718 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();
#line 1549 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand6,
	gen_rtx_MULT (SImode,
	operand2,
	operand3)),
		gen_rtx_CLOBBER (VOIDmode,
	operand4),
		gen_rtx_CLOBBER (VOIDmode,
	operand5))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	operand1,
	copy_rtx (operand6))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1552 */
extern rtx gen_split_719 (rtx, rtx *);
rtx
gen_split_719 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx _val = 0;
  start_sequence ();
#line 1570 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (4,
		gen_rtx_SET (VOIDmode,
	operand1,
	gen_rtx_MINUS (SImode,
	copy_rtx (operand1),
	gen_rtx_MULT (SImode,
	operand2,
	operand3))),
		gen_rtx_CLOBBER (VOIDmode,
	operand4),
		gen_rtx_CLOBBER (VOIDmode,
	operand5),
		gen_rtx_CLOBBER (VOIDmode,
	operand6))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		copy_rtx (operand5),
		copy_rtx (operand4)),
	26)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1587 */
rtx
gen_mulsidi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1596 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (!TARGET_64BIT)
    {
      if (!TARGET_FIX_R4000)
	emit_insn (gen_mulsidi3_32bit_internal (operands[0], operands[1],
						   operands[2]));
      else
	emit_insn (gen_mulsidi3_32bit_r4000 (operands[0], operands[1],
					        operands[2]));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (4,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode)))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1587 */
rtx
gen_umulsidi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1596 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (!TARGET_64BIT)
    {
      if (!TARGET_FIX_R4000)
	emit_insn (gen_umulsidi3_32bit_internal (operands[0], operands[1],
						   operands[2]));
      else
	emit_insn (gen_umulsidi3_32bit_r4000 (operands[0], operands[1],
					        operands[2]));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (4,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode)))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1629 */
extern rtx gen_split_722 (rtx, rtx *);
rtx
gen_split_722 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
#line 1671 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_SIGN_EXTEND (DImode,
	gen_rtx_MULT (SImode,
	operand1,
	operand2))),
		gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_ASHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	copy_rtx (operand1)),
	gen_rtx_SIGN_EXTEND (DImode,
	copy_rtx (operand2))),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand5,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		copy_rtx (operand3),
		copy_rtx (operand4)),
	26)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		copy_rtx (operand4),
		copy_rtx (operand3)),
	26)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand5),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand5),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand5),
	gen_rtx_LSHIFTRT (DImode,
	copy_rtx (operand5),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand0),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_IOR (DImode,
	copy_rtx (operand0),
	copy_rtx (operand5))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1629 */
extern rtx gen_split_723 (rtx, rtx *);
rtx
gen_split_723 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
#line 1671 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_SIGN_EXTEND (DImode,
	gen_rtx_MULT (SImode,
	operand1,
	operand2))),
		gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_ASHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	copy_rtx (operand1)),
	gen_rtx_ZERO_EXTEND (DImode,
	copy_rtx (operand2))),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand5,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		copy_rtx (operand3),
		copy_rtx (operand4)),
	26)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		copy_rtx (operand4),
		copy_rtx (operand3)),
	26)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand5),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand5),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand5),
	gen_rtx_LSHIFTRT (DImode,
	copy_rtx (operand5),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand0),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_IOR (DImode,
	copy_rtx (operand0),
	copy_rtx (operand5))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1724 */
rtx
gen_smulsi3_highpart (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1732 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (ISA_HAS_MULHI)
    emit_insn (gen_smulsi3_highpart_mulhi_internal (operands[0],
						       operands[1],
						       operands[2]));
  else
    emit_insn (gen_smulsi3_highpart_internal (operands[0], operands[1],
					         operands[2]));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)]))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1724 */
rtx
gen_umulsi3_highpart (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1732 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (ISA_HAS_MULHI)
    emit_insn (gen_umulsi3_highpart_mulhi_internal (operands[0],
						       operands[1],
						       operands[2]));
  else
    emit_insn (gen_umulsi3_highpart_internal (operands[0], operands[1],
					         operands[2]));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)]))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1931 */
rtx
gen_divsf3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1936 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (const_1_operand (operands[1], SFmode))
    if (!(ISA_HAS_FP4 && flag_unsafe_math_optimizations))
      operands[1] = force_reg (SFmode, operands[1]);
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (SFmode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1931 */
rtx
gen_divdf3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1936 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (const_1_operand (operands[1], DFmode))
    if (!(ISA_HAS_FP4 && TARGET_FLOAT64 && flag_unsafe_math_optimizations))
      operands[1] = force_reg (DFmode, operands[1]);
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (DFmode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1931 */
rtx
gen_divv2sf3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 1936 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (const_1_operand (operands[1], V2SFmode))
    if (!(TARGET_SB1 && flag_unsafe_math_optimizations))
      operands[1] = force_reg (V2SFmode, operands[1]);
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (V2SFmode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2181 */
rtx
gen_andsi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 2186 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16)
    operands[2] = force_reg (SImode, operands[2]);
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2181 */
rtx
gen_anddi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 2186 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16)
    operands[2] = force_reg (DImode, operands[2]);
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (DImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2211 */
rtx
gen_iorsi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 2216 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16)
    operands[2] = force_reg (SImode, operands[2]);
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2211 */
rtx
gen_iordi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 2216 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16)
    operands[2] = force_reg (DImode, operands[2]);
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (DImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2241 */
rtx
gen_xorsi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_XOR (SImode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2241 */
rtx
gen_xordi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_XOR (DImode,
	operand1,
	operand2));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2376 */
extern rtx gen_split_735 (rtx, rtx *);
rtx
gen_split_735 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 2389 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = gen_lowpart (DImode, operands[0]);
  operands[3] = GEN_INT (BITS_PER_WORD - GET_MODE_BITSIZE (QImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (DImode,
	operand1,
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (DImode,
	copy_rtx (operand2),
	copy_rtx (operand3))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2376 */
extern rtx gen_split_736 (rtx, rtx *);
rtx
gen_split_736 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 2389 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = gen_lowpart (DImode, operands[0]);
  operands[3] = GEN_INT (BITS_PER_WORD - GET_MODE_BITSIZE (HImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (DImode,
	operand1,
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (DImode,
	copy_rtx (operand2),
	copy_rtx (operand3))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2394 */
extern rtx gen_split_737 (rtx, rtx *);
rtx
gen_split_737 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 2407 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = gen_lowpart (DImode, operands[0]);
  operands[3] = GEN_INT (BITS_PER_WORD - GET_MODE_BITSIZE (QImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (DImode,
	operand1,
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_ASHIFTRT (DImode,
	copy_rtx (operand2),
	copy_rtx (operand3)))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2394 */
extern rtx gen_split_738 (rtx, rtx *);
rtx
gen_split_738 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 2407 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = gen_lowpart (DImode, operands[0]);
  operands[3] = GEN_INT (BITS_PER_WORD - GET_MODE_BITSIZE (HImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (DImode,
	operand1,
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_ASHIFTRT (DImode,
	copy_rtx (operand2),
	copy_rtx (operand3)))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2450 */
extern rtx gen_split_739 (rtx, rtx *);
rtx
gen_split_739 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
#line 2462 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[1] = gen_lowpart (DImode, operands[1]); }
  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (DImode,
	operand1,
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_LSHIFTRT (DImode,
	copy_rtx (operand0),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2470 */
extern rtx gen_split_740 (rtx, rtx *);
rtx
gen_split_740 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx _val = 0;
  start_sequence ();
#line 2487 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (DImode,
	operand1,
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_LSHIFTRT (DImode,
	copy_rtx (operand0),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2492 */
rtx
gen_zero_extendqisi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2496 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16 && !GENERATE_MIPS16E
      && !memory_operand (operands[1], QImode))
    {
      emit_insn (gen_andsi3 (operands[0],
				     gen_lowpart (SImode, operands[1]),
				     force_reg (SImode,
						GEN_INT (0x00ff))));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (SImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2492 */
rtx
gen_zero_extendqidi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2496 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16 && !GENERATE_MIPS16E
      && !memory_operand (operands[1], QImode))
    {
      emit_insn (gen_anddi3 (operands[0],
				     gen_lowpart (DImode, operands[1]),
				     force_reg (DImode,
						GEN_INT (0x00ff))));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2492 */
rtx
gen_zero_extendhisi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2496 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16 && !GENERATE_MIPS16E
      && !memory_operand (operands[1], HImode))
    {
      emit_insn (gen_andsi3 (operands[0],
				     gen_lowpart (SImode, operands[1]),
				     force_reg (SImode,
						GEN_INT (0xffff))));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (SImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2492 */
rtx
gen_zero_extendhidi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2496 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16 && !GENERATE_MIPS16E
      && !memory_operand (operands[1], HImode))
    {
      emit_insn (gen_anddi3 (operands[0],
				     gen_lowpart (DImode, operands[1]),
				     force_reg (DImode,
						GEN_INT (0xffff))));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2535 */
rtx
gen_zero_extendqihi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2539 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16 && !memory_operand (operands[1], QImode))
    {
      emit_insn (gen_zero_extendqisi2 (gen_lowpart (SImode, operands[0]),
				       operands[1]));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (HImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2584 */
extern rtx gen_split_746 (rtx, rtx *);
rtx
gen_split_746 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 2593 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  emit_note (NOTE_INSN_DELETED);
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2600 */
rtx
gen_extendqisi2 (rtx operand0,
	rtx operand1)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (SImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2600 */
rtx
gen_extendqidi2 (rtx operand0,
	rtx operand1)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2600 */
rtx
gen_extendhisi2 (rtx operand0,
	rtx operand1)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (SImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2600 */
rtx
gen_extendhidi2 (rtx operand0,
	rtx operand1)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
extern rtx gen_split_751 (rtx, rtx *);
rtx
gen_split_751 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 2626 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = GEN_INT (GET_MODE_BITSIZE (SImode)
			 - GET_MODE_BITSIZE (QImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFTRT (SImode,
	copy_rtx (operand0),
	copy_rtx (operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
extern rtx gen_split_752 (rtx, rtx *);
rtx
gen_split_752 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 2626 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = gen_lowpart (DImode, operands[1]);
  operands[2] = GEN_INT (GET_MODE_BITSIZE (DImode)
			 - GET_MODE_BITSIZE (QImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (DImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFTRT (DImode,
	copy_rtx (operand0),
	copy_rtx (operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
extern rtx gen_split_753 (rtx, rtx *);
rtx
gen_split_753 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 2626 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = GEN_INT (GET_MODE_BITSIZE (SImode)
			 - GET_MODE_BITSIZE (HImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFTRT (SImode,
	copy_rtx (operand0),
	copy_rtx (operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
extern rtx gen_split_754 (rtx, rtx *);
rtx
gen_split_754 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 2626 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = gen_lowpart (DImode, operands[1]);
  operands[2] = GEN_INT (GET_MODE_BITSIZE (DImode)
			 - GET_MODE_BITSIZE (HImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (DImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFTRT (DImode,
	copy_rtx (operand0),
	copy_rtx (operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2646 */
rtx
gen_extendqihi2 (rtx operand0,
	rtx operand1)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (HImode,
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2661 */
extern rtx gen_split_756 (rtx, rtx *);
rtx
gen_split_756 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 2672 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = GEN_INT (GET_MODE_BITSIZE (SImode)
			 - GET_MODE_BITSIZE (QImode));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFTRT (SImode,
	copy_rtx (operand0),
	copy_rtx (operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2709 */
rtx
gen_fix_truncdfsi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2713 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (!ISA_HAS_TRUNC_W)
    {
      emit_insn (gen_fix_truncdfsi2_macro (operands[0], operands[1]));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2747 */
rtx
gen_fix_truncsfsi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2751 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (!ISA_HAS_TRUNC_W)
    {
      emit_insn (gen_fix_truncsfsi2_macro (operands[0], operands[1]));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2852 */
rtx
gen_fixuns_truncdfsi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2856 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  rtx reg1 = gen_reg_rtx (DFmode);
  rtx reg2 = gen_reg_rtx (DFmode);
  rtx reg3 = gen_reg_rtx (SImode);
  rtx label1 = gen_label_rtx ();
  rtx label2 = gen_label_rtx ();
  REAL_VALUE_TYPE offset;

  real_2expN (&offset, 31, DFmode);

  if (reg1)			/* Turn off complaints about unreached code.  */
    {
      mips_emit_move (reg1, CONST_DOUBLE_FROM_REAL_VALUE (offset, DFmode));
      do_pending_stack_adjust ();

      emit_insn (gen_cmpdf (operands[1], reg1));
      emit_jump_insn (gen_bge (label1));

      emit_insn (gen_fix_truncdfsi2 (operands[0], operands[1]));
      emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
				   gen_rtx_LABEL_REF (VOIDmode, label2)));
      emit_barrier ();

      emit_label (label1);
      mips_emit_move (reg2, gen_rtx_MINUS (DFmode, operands[1], reg1));
      mips_emit_move (reg3, GEN_INT (trunc_int_for_mode
				     (BITMASK_HIGH, SImode)));

      emit_insn (gen_fix_truncdfsi2 (operands[0], reg2));
      emit_insn (gen_iorsi3 (operands[0], operands[0], reg3));

      emit_label (label2);

      /* Allow REG_NOTES to be set on last insn (labels don't have enough
	 fields, and can't be used for REG_NOTES anyway).  */
      emit_insn (gen_rtx_USE (VOIDmode, stack_pointer_rtx));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSIGNED_FIX (SImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2897 */
rtx
gen_fixuns_truncdfdi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2901 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  rtx reg1 = gen_reg_rtx (DFmode);
  rtx reg2 = gen_reg_rtx (DFmode);
  rtx reg3 = gen_reg_rtx (DImode);
  rtx label1 = gen_label_rtx ();
  rtx label2 = gen_label_rtx ();
  REAL_VALUE_TYPE offset;

  real_2expN (&offset, 63, DFmode);

  mips_emit_move (reg1, CONST_DOUBLE_FROM_REAL_VALUE (offset, DFmode));
  do_pending_stack_adjust ();

  emit_insn (gen_cmpdf (operands[1], reg1));
  emit_jump_insn (gen_bge (label1));

  emit_insn (gen_fix_truncdfdi2 (operands[0], operands[1]));
  emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
			       gen_rtx_LABEL_REF (VOIDmode, label2)));
  emit_barrier ();

  emit_label (label1);
  mips_emit_move (reg2, gen_rtx_MINUS (DFmode, operands[1], reg1));
  mips_emit_move (reg3, GEN_INT (BITMASK_HIGH));
  emit_insn (gen_ashldi3 (reg3, reg3, GEN_INT (32)));

  emit_insn (gen_fix_truncdfdi2 (operands[0], reg2));
  emit_insn (gen_iordi3 (operands[0], operands[0], reg3));

  emit_label (label2);

  /* Allow REG_NOTES to be set on last insn (labels don't have enough
     fields, and can't be used for REG_NOTES anyway).  */
  emit_insn (gen_rtx_USE (VOIDmode, stack_pointer_rtx));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSIGNED_FIX (DImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2939 */
rtx
gen_fixuns_truncsfsi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2943 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  rtx reg1 = gen_reg_rtx (SFmode);
  rtx reg2 = gen_reg_rtx (SFmode);
  rtx reg3 = gen_reg_rtx (SImode);
  rtx label1 = gen_label_rtx ();
  rtx label2 = gen_label_rtx ();
  REAL_VALUE_TYPE offset;

  real_2expN (&offset, 31, SFmode);

  mips_emit_move (reg1, CONST_DOUBLE_FROM_REAL_VALUE (offset, SFmode));
  do_pending_stack_adjust ();

  emit_insn (gen_cmpsf (operands[1], reg1));
  emit_jump_insn (gen_bge (label1));

  emit_insn (gen_fix_truncsfsi2 (operands[0], operands[1]));
  emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
			       gen_rtx_LABEL_REF (VOIDmode, label2)));
  emit_barrier ();

  emit_label (label1);
  mips_emit_move (reg2, gen_rtx_MINUS (SFmode, operands[1], reg1));
  mips_emit_move (reg3, GEN_INT (trunc_int_for_mode
				 (BITMASK_HIGH, SImode)));

  emit_insn (gen_fix_truncsfsi2 (operands[0], reg2));
  emit_insn (gen_iorsi3 (operands[0], operands[0], reg3));

  emit_label (label2);

  /* Allow REG_NOTES to be set on last insn (labels don't have enough
     fields, and can't be used for REG_NOTES anyway).  */
  emit_insn (gen_rtx_USE (VOIDmode, stack_pointer_rtx));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSIGNED_FIX (SImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2981 */
rtx
gen_fixuns_truncsfdi2 (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 2985 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  rtx reg1 = gen_reg_rtx (SFmode);
  rtx reg2 = gen_reg_rtx (SFmode);
  rtx reg3 = gen_reg_rtx (DImode);
  rtx label1 = gen_label_rtx ();
  rtx label2 = gen_label_rtx ();
  REAL_VALUE_TYPE offset;

  real_2expN (&offset, 63, SFmode);

  mips_emit_move (reg1, CONST_DOUBLE_FROM_REAL_VALUE (offset, SFmode));
  do_pending_stack_adjust ();

  emit_insn (gen_cmpsf (operands[1], reg1));
  emit_jump_insn (gen_bge (label1));

  emit_insn (gen_fix_truncsfdi2 (operands[0], operands[1]));
  emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
			       gen_rtx_LABEL_REF (VOIDmode, label2)));
  emit_barrier ();

  emit_label (label1);
  mips_emit_move (reg2, gen_rtx_MINUS (SFmode, operands[1], reg1));
  mips_emit_move (reg3, GEN_INT (BITMASK_HIGH));
  emit_insn (gen_ashldi3 (reg3, reg3, GEN_INT (32)));

  emit_insn (gen_fix_truncsfdi2 (operands[0], reg2));
  emit_insn (gen_iordi3 (operands[0], operands[0], reg3));

  emit_label (label2);

  /* Allow REG_NOTES to be set on last insn (labels don't have enough
     fields, and can't be used for REG_NOTES anyway).  */
  emit_insn (gen_rtx_USE (VOIDmode, stack_pointer_rtx));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSIGNED_FIX (DImode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3031 */
rtx
gen_extv (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 3037 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_expand_ext_as_unaligned_load (operands[0], operands[1],
					 INTVAL (operands[2]),
					 INTVAL (operands[3])))
    DONE;
  else
    FAIL;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTRACT (VOIDmode,
	operand1,
	operand2,
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3046 */
rtx
gen_extzv (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 3052 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_expand_ext_as_unaligned_load (operands[0], operands[1],
					 INTVAL (operands[2]),
					 INTVAL (operands[3])))
    DONE;
  else if (mips_use_ins_ext_p (operands[1], INTVAL (operands[2]),
			       INTVAL (operands[3])))
    {
      if (GET_MODE (operands[0]) == DImode)
        emit_insn (gen_extzvdi (operands[0], operands[1], operands[2],
				operands[3]));
      else
        emit_insn (gen_extzvsi (operands[0], operands[1], operands[2],
				operands[3]));
      DONE;
    }
  else
    FAIL;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTRACT (VOIDmode,
	operand1,
	operand2,
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3084 */
rtx
gen_insv (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 3090 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_expand_ins_as_unaligned_store (operands[0], operands[3],
					  INTVAL (operands[1]),
					  INTVAL (operands[2])))
    DONE;
  else if (mips_use_ins_ext_p (operands[0], INTVAL (operands[1]),
			       INTVAL (operands[2])))
    {
      if (GET_MODE (operands[0]) == DImode)
        emit_insn (gen_insvdi (operands[0], operands[1], operands[2],
			       operands[3]));
      else
        emit_insn (gen_insvsi (operands[0], operands[1], operands[2],
			       operands[3]));
      DONE;
   }
   else
     FAIL;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	gen_rtx_ZERO_EXTRACT (VOIDmode,
	operand0,
	operand1,
	operand2),
	operand3));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3190 */
extern rtx gen_split_766 (rtx, rtx *);
rtx
gen_split_766 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3201 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = mips_unspec_address (operands[1], SYMBOL_64_HIGH);
  operands[3] = mips_unspec_address (operands[1], SYMBOL_64_MID);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_HIGH (DImode,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_LO_SUM (DImode,
	copy_rtx (operand0),
	copy_rtx (operand2))));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand0),
	const_int_rtx[MAX_SAVED_CONST_INT + (16)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_LO_SUM (DImode,
	copy_rtx (operand0),
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand0),
	const_int_rtx[MAX_SAVED_CONST_INT + (16)])));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3215 */
extern rtx gen_peephole2_767 (rtx, rtx *);
rtx
gen_peephole2_767 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  HARD_REG_SET _regs_allocated;
  CLEAR_HARD_REG_SET (_regs_allocated);
  if ((operands[0] = peep2_find_free_register (1, 1, "d", DImode, &_regs_allocated)) == NULL_RTX)
    return NULL;
  start_sequence ();
#line 3225 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[3] = mips_unspec_address (operands[2], SYMBOL_64_HIGH);
  operands[4] = mips_unspec_address (operands[2], SYMBOL_64_LOW);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand1,
	gen_rtx_HIGH (DImode,
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_HIGH (DImode,
	operand4)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_LO_SUM (DImode,
	copy_rtx (operand1),
	copy_rtx (operand3))));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand1),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand1),
	gen_rtx_PLUS (DImode,
	copy_rtx (operand1),
	copy_rtx (operand0))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3243 */
extern rtx gen_split_768 (rtx, rtx *);
rtx
gen_split_768 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();
#line 3256 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[3] = mips_unspec_address (operands[1], SYMBOL_64_HIGH);
  operands[4] = mips_unspec_address (operands[1], SYMBOL_64_LOW);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_HIGH (DImode,
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_HIGH (DImode,
	operand4)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_LO_SUM (DImode,
	copy_rtx (operand0),
	copy_rtx (operand3))));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand2),
	gen_rtx_LO_SUM (DImode,
	copy_rtx (operand2),
	copy_rtx (operand4))));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand0),
	const_int_rtx[MAX_SAVED_CONST_INT + (32)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (DImode,
	copy_rtx (operand0),
	copy_rtx (operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3268 */
extern rtx gen_split_769 (rtx, rtx *);
rtx
gen_split_769 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 3274 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = mips_unspec_address (operands[1], SYMBOL_32_HIGH);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand2));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFT (SImode,
	copy_rtx (operand0),
	const_int_rtx[MAX_SAVED_CONST_INT + (16)])));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3280 */
extern rtx gen_split_770 (rtx, rtx *);
rtx
gen_split_770 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3288 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = mips_unspec_address (operands[1], SYMBOL_GOTOFF_DISP);
  operands[3] = pic_offset_table_rtx;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_HIGH (SImode,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3280 */
extern rtx gen_split_771 (rtx, rtx *);
rtx
gen_split_771 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3288 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = mips_unspec_address (operands[1], SYMBOL_GOTOFF_DISP);
  operands[3] = pic_offset_table_rtx;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_HIGH (DImode,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (DImode,
	copy_rtx (operand0),
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3295 */
extern rtx gen_split_772 (rtx, rtx *);
rtx
gen_split_772 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3304 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[3] = mips_unspec_address (operands[2], SYMBOL_GOTOFF_DISP); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand3),
	24)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3295 */
extern rtx gen_split_773 (rtx, rtx *);
rtx
gen_split_773 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3304 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[3] = mips_unspec_address (operands[2], SYMBOL_GOTOFF_DISP); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand1,
		operand3),
	24)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3310 */
extern rtx gen_split_774 (rtx, rtx *);
rtx
gen_split_774 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3318 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = pic_offset_table_rtx;
  operands[3] = mips_unspec_address (operands[1], SYMBOL_GOTOFF_DISP);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand2,
		operand3),
	24)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3310 */
extern rtx gen_split_775 (rtx, rtx *);
rtx
gen_split_775 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3318 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = pic_offset_table_rtx;
  operands[3] = mips_unspec_address (operands[1], SYMBOL_GOTOFF_DISP);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand2,
		operand3),
	24)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3327 */
extern rtx gen_split_776 (rtx, rtx *);
rtx
gen_split_776 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3335 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = pic_offset_table_rtx;
  operands[3] = mips_unspec_address (operands[1], SYMBOL_GOTOFF_PAGE);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand2,
		operand3),
	24)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3327 */
extern rtx gen_split_777 (rtx, rtx *);
rtx
gen_split_777 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3335 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = pic_offset_table_rtx;
  operands[3] = mips_unspec_address (operands[1], SYMBOL_GOTOFF_PAGE);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand2,
		operand3),
	24)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3382 */
extern rtx gen_split_778 (rtx, rtx *);
rtx
gen_split_778 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 3388 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_move_integer (operands[2], operands[0], INTVAL (operands[1]));
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3382 */
extern rtx gen_split_779 (rtx, rtx *);
rtx
gen_split_779 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 3388 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_move_integer (operands[2], operands[0], INTVAL (operands[1]));
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3394 */
extern rtx gen_split_780 (rtx, rtx *);
rtx
gen_split_780 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3400 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_split_symbol (operands[2], operands[1],
		     MAX_MACHINE_MODE, &operands[3]);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand3));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3394 */
extern rtx gen_split_781 (rtx, rtx *);
rtx
gen_split_781 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 3400 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_split_symbol (operands[2], operands[1],
		     MAX_MACHINE_MODE, &operands[3]);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand3));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3411 */
rtx
gen_movdi (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 3415 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_legitimize_move (DImode, operands[0], operands[1]))
    DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3505 */
extern rtx gen_split_783 (rtx, rtx *);
rtx
gen_split_783 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 3523 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  HOST_WIDE_INT val = INTVAL (operands[1]);

  if (val < 0)
    operands[2] = const0_rtx;
  else if (val >= 32 * 8)
    {
      int off = val & 7;

      operands[1] = GEN_INT (0x8 + off);
      operands[2] = GEN_INT (val - off - 0x8);
    }
  else
    {
      int off = val & 7;

      operands[1] = GEN_INT (off);
      operands[2] = GEN_INT (val - off);
    }
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	copy_rtx (operand0),
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_MEM (DImode,
	gen_rtx_PLUS (DImode,
	copy_rtx (operand0),
	operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3550 */
rtx
gen_movsi (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 3554 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_legitimize_move (SImode, operands[0], operands[1]))
    DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3601 */
extern rtx gen_split_785 (rtx, rtx *);
rtx
gen_split_785 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 3618 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  HOST_WIDE_INT val = INTVAL (operands[1]);

  if (val < 0)
    operands[2] = const0_rtx;
  else if (val >= 32 * 4)
    {
      int off = val & 3;

      operands[1] = GEN_INT (0x7c + off);
      operands[2] = GEN_INT (val - off - 0x7c);
    }
  else
    {
      int off = val & 3;

      operands[1] = GEN_INT (off);
      operands[2] = GEN_INT (val - off);
    }
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3643 */
extern rtx gen_split_786 (rtx, rtx *);
rtx
gen_split_786 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 3654 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  int val = INTVAL (operands[1]);

  operands[1] = GEN_INT (0xff);
  operands[2] = GEN_INT (val - 0xff);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3689 */
rtx
gen_reload_incc (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 3694 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_fcc_reload (operands[0], operands[1], operands[2]);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	operand2));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3699 */
rtx
gen_reload_outcc (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 3704 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_fcc_reload (operands[0], operands[1], operands[2]);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	operand2));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3762 */
rtx
gen_movhi (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 3766 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_legitimize_move (HImode, operands[0], operands[1]))
    DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3821 */
extern rtx gen_split_790 (rtx, rtx *);
rtx
gen_split_790 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 3838 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  HOST_WIDE_INT val = INTVAL (operands[1]);

  if (val < 0)
    operands[2] = const0_rtx;
  else if (val >= 32 * 2)
    {
      int off = val & 1;

      operands[1] = GEN_INT (0x7e + off);
      operands[2] = GEN_INT (val - off - 0x7e);
    }
  else
    {
      int off = val & 1;

      operands[1] = GEN_INT (off);
      operands[2] = GEN_INT (val - off);
    }
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_MEM (HImode,
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3866 */
rtx
gen_movqi (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 3870 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_legitimize_move (QImode, operands[0], operands[1]))
    DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3913 */
extern rtx gen_split_792 (rtx, rtx *);
rtx
gen_split_792 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 3927 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  HOST_WIDE_INT val = INTVAL (operands[1]);

  if (val < 0)
    operands[2] = const0_rtx;
  else
    {
      operands[1] = GEN_INT (0x7f);
      operands[2] = GEN_INT (val - 0x7f);
    }
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_MEM (QImode,
	gen_rtx_PLUS (SImode,
	copy_rtx (operand0),
	operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3941 */
rtx
gen_movsf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 3945 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_legitimize_move (SFmode, operands[0], operands[1]))
    DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3986 */
rtx
gen_movdf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 3990 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_legitimize_move (DFmode, operands[0], operands[1]))
    DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4042 */
rtx
gen_movtf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4046 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_legitimize_move (TFmode, operands[0], operands[1]))
    DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4052 */
extern rtx gen_split_796 (rtx, rtx *);
rtx
gen_split_796 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 4059 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_split_doubleword_move (operands[0], operands[1]);
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4066 */
extern rtx gen_split_797 (rtx, rtx *);
rtx
gen_split_797 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 4072 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_split_doubleword_move (operands[0], operands[1]);
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4066 */
extern rtx gen_split_798 (rtx, rtx *);
rtx
gen_split_798 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 4072 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_split_doubleword_move (operands[0], operands[1]);
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4066 */
extern rtx gen_split_799 (rtx, rtx *);
rtx
gen_split_799 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 4072 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_split_doubleword_move (operands[0], operands[1]);
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4079 */
extern rtx gen_split_800 (rtx, rtx *);
rtx
gen_split_800 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();
#line 4087 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = gen_lowpart (SImode, operands[0]);
  operands[3] = GEN_INT (-INTVAL (operands[1]));
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand3));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand2),
	gen_rtx_NEG (SImode,
	copy_rtx (operand2))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4094 */
rtx
gen_movv2sf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4098 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_legitimize_move (V2SFmode, operands[0], operands[1]))
    DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4140 */
rtx
gen_mfhilo_si (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	26));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4140 */
rtx
gen_mfhilo_di (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		operand1,
		operand2),
	26));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4175 */
rtx
gen_move_doubleword_fprdf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4179 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (FP_REG_RTX_P (operands[0]))
    {
      rtx low = mips_subword (operands[1], 0);
      rtx high = mips_subword (operands[1], 1);
      emit_insn (gen_load_lowdf (operands[0], low));
      if (ISA_HAS_MXHC1)
 	emit_insn (gen_mthc1df (operands[0], high, operands[0]));
      else
	emit_insn (gen_load_highdf (operands[0], high, operands[0]));
    }
  else
    {
      rtx low = mips_subword (operands[0], 0);
      rtx high = mips_subword (operands[0], 1);
      emit_insn (gen_store_worddf (low, operands[1], const0_rtx));
      if (ISA_HAS_MXHC1)
	emit_insn (gen_mfhc1df (high, operands[1]));
      else
	emit_insn (gen_store_worddf (high, operands[1], const1_rtx));
    }
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4175 */
rtx
gen_move_doubleword_fprdi (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4179 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (FP_REG_RTX_P (operands[0]))
    {
      rtx low = mips_subword (operands[1], 0);
      rtx high = mips_subword (operands[1], 1);
      emit_insn (gen_load_lowdi (operands[0], low));
      if (ISA_HAS_MXHC1)
 	emit_insn (gen_mthc1di (operands[0], high, operands[0]));
      else
	emit_insn (gen_load_highdi (operands[0], high, operands[0]));
    }
  else
    {
      rtx low = mips_subword (operands[0], 0);
      rtx high = mips_subword (operands[0], 1);
      emit_insn (gen_store_worddi (low, operands[1], const0_rtx));
      if (ISA_HAS_MXHC1)
	emit_insn (gen_mfhc1di (high, operands[1]));
      else
	emit_insn (gen_store_worddi (high, operands[1], const1_rtx));
    }
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4175 */
rtx
gen_move_doubleword_fprv2sf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4179 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (FP_REG_RTX_P (operands[0]))
    {
      rtx low = mips_subword (operands[1], 0);
      rtx high = mips_subword (operands[1], 1);
      emit_insn (gen_load_lowv2sf (operands[0], low));
      if (ISA_HAS_MXHC1)
 	emit_insn (gen_mthc1v2sf (operands[0], high, operands[0]));
      else
	emit_insn (gen_load_highv2sf (operands[0], high, operands[0]));
    }
  else
    {
      rtx low = mips_subword (operands[0], 0);
      rtx high = mips_subword (operands[0], 1);
      emit_insn (gen_store_wordv2sf (low, operands[1], const0_rtx));
      if (ISA_HAS_MXHC1)
	emit_insn (gen_mfhc1v2sf (high, operands[1]));
      else
	emit_insn (gen_store_wordv2sf (high, operands[1], const1_rtx));
    }
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4175 */
rtx
gen_move_doubleword_fprtf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4179 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (FP_REG_RTX_P (operands[0]))
    {
      rtx low = mips_subword (operands[1], 0);
      rtx high = mips_subword (operands[1], 1);
      emit_insn (gen_load_lowtf (operands[0], low));
      if (ISA_HAS_MXHC1)
 	emit_insn (gen_mthc1tf (operands[0], high, operands[0]));
      else
	emit_insn (gen_load_hightf (operands[0], high, operands[0]));
    }
  else
    {
      rtx low = mips_subword (operands[0], 0);
      rtx high = mips_subword (operands[0], 1);
      emit_insn (gen_store_wordtf (low, operands[1], const0_rtx));
      if (ISA_HAS_MXHC1)
	emit_insn (gen_mfhc1tf (high, operands[1]));
      else
	emit_insn (gen_store_wordtf (high, operands[1], const1_rtx));
    }
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4269 */
rtx
gen_load_const_gp_si (rtx operand0)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CONST (SImode,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		const0_rtx),
	25)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4269 */
rtx
gen_load_const_gp_di (rtx operand0)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CONST (DImode,
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (1,
		const0_rtx),
	25)));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4276 */
extern rtx gen_split_810 (rtx, rtx *);
rtx
gen_split_810 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
#line 4287 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[3] = gen_rtx_HIGH (Pmode, operands[1]);
  operands[4] = gen_rtx_PLUS (Pmode, operands[0], operands[2]);
  operands[5] = gen_rtx_LO_SUM (Pmode, operands[0], operands[1]);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand3));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	operand4));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	operand5));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4276 */
extern rtx gen_split_811 (rtx, rtx *);
rtx
gen_split_811 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
#line 4287 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[3] = gen_rtx_HIGH (Pmode, operands[1]);
  operands[4] = gen_rtx_PLUS (Pmode, operands[0], operands[2]);
  operands[5] = gen_rtx_LO_SUM (Pmode, operands[0], operands[1]);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand3));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	operand4));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	operand5));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4295 */
extern rtx gen_split_812 (rtx, rtx *);
rtx
gen_split_812 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 4302 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_emit_move (operands[0], operands[1]);
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4295 */
extern rtx gen_split_813 (rtx, rtx *);
rtx
gen_split_813 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 4302 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_emit_move (operands[0], operands[1]);
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4321 */
extern rtx gen_split_814 (rtx, rtx *);
rtx
gen_split_814 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();
#line 4334 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[3] = mips_unspec_address (operands[1], SYMBOL_ABSOLUTE);
  operands[4] = mips_unspec_address (operands[2], SYMBOL_HALF);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_HIGH (SImode,
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		copy_rtx (operand0),
		copy_rtx (operand3)),
	24)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		copy_rtx (operand0),
		operand4),
	24)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4321 */
extern rtx gen_split_815 (rtx, rtx *);
rtx
gen_split_815 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();
#line 4334 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[3] = mips_unspec_address (operands[1], SYMBOL_ABSOLUTE);
  operands[4] = mips_unspec_address (operands[2], SYMBOL_HALF);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_HIGH (DImode,
	operand3)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		copy_rtx (operand0),
		copy_rtx (operand3)),
	24)));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_UNSPEC (DImode,
	gen_rtvec (2,
		copy_rtx (operand0),
		operand4),
	24)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4359 */
rtx
gen_clear_cache (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4363 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

{
  if (ISA_HAS_SYNCI)
    {
      mips_expand_synci_loop (operands[0], operands[1]);
      emit_insn (gen_sync ());
      emit_insn (gen_clear_hazard ());
    }
  else if (mips_cache_flush_func && mips_cache_flush_func[0])
    {
      rtx len = gen_reg_rtx (Pmode);
      emit_insn (gen_sub3_insn (len, operands[1], operands[0]));
      MIPS_ICACHE_SYNC (operands[0], len);
    }
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit (operand0);
  emit (operand1);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4637 */
rtx
gen_movmemsi (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 4643 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (mips_expand_block_move (operands[0], operands[1], operands[2]))
    DONE;
  else
    FAIL;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_SET (VOIDmode,
	operand0,
	operand1),
		gen_rtx_USE (VOIDmode,
	operand2),
		gen_rtx_USE (VOIDmode,
	operand3))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
rtx
gen_ashlsi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 4662 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* On the mips16, a shift of more than 8 is a four byte instruction,
     so, for a shift between 8 and 16, it is just as fast to do two
     shifts of 8 or less.  If there is a lot of shifting going on, we
     may win in CSE.  Otherwise combine will put the shifts back
     together again.  This can be called by mips_function_arg, so we must
     be careful not to allocate a new register if we've reached the
     reload pass.  */
  if (TARGET_MIPS16
      && optimize
      && GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) > 8
      && INTVAL (operands[2]) <= 16
      && !reload_in_progress
      && !reload_completed)
    {
      rtx temp = gen_reg_rtx (SImode);

      emit_insn (gen_ashlsi3 (temp, operands[1], GEN_INT (8)));
      emit_insn (gen_ashlsi3 (operands[0], temp,
				     GEN_INT (INTVAL (operands[2]) - 8)));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
rtx
gen_ashrsi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 4662 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* On the mips16, a shift of more than 8 is a four byte instruction,
     so, for a shift between 8 and 16, it is just as fast to do two
     shifts of 8 or less.  If there is a lot of shifting going on, we
     may win in CSE.  Otherwise combine will put the shifts back
     together again.  This can be called by mips_function_arg, so we must
     be careful not to allocate a new register if we've reached the
     reload pass.  */
  if (TARGET_MIPS16
      && optimize
      && GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) > 8
      && INTVAL (operands[2]) <= 16
      && !reload_in_progress
      && !reload_completed)
    {
      rtx temp = gen_reg_rtx (SImode);

      emit_insn (gen_ashrsi3 (temp, operands[1], GEN_INT (8)));
      emit_insn (gen_ashrsi3 (operands[0], temp,
				     GEN_INT (INTVAL (operands[2]) - 8)));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
rtx
gen_lshrsi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 4662 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* On the mips16, a shift of more than 8 is a four byte instruction,
     so, for a shift between 8 and 16, it is just as fast to do two
     shifts of 8 or less.  If there is a lot of shifting going on, we
     may win in CSE.  Otherwise combine will put the shifts back
     together again.  This can be called by mips_function_arg, so we must
     be careful not to allocate a new register if we've reached the
     reload pass.  */
  if (TARGET_MIPS16
      && optimize
      && GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) > 8
      && INTVAL (operands[2]) <= 16
      && !reload_in_progress
      && !reload_completed)
    {
      rtx temp = gen_reg_rtx (SImode);

      emit_insn (gen_lshrsi3 (temp, operands[1], GEN_INT (8)));
      emit_insn (gen_lshrsi3 (operands[0], temp,
				     GEN_INT (INTVAL (operands[2]) - 8)));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
rtx
gen_ashldi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 4662 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* On the mips16, a shift of more than 8 is a four byte instruction,
     so, for a shift between 8 and 16, it is just as fast to do two
     shifts of 8 or less.  If there is a lot of shifting going on, we
     may win in CSE.  Otherwise combine will put the shifts back
     together again.  This can be called by mips_function_arg, so we must
     be careful not to allocate a new register if we've reached the
     reload pass.  */
  if (TARGET_MIPS16
      && optimize
      && GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) > 8
      && INTVAL (operands[2]) <= 16
      && !reload_in_progress
      && !reload_completed)
    {
      rtx temp = gen_reg_rtx (DImode);

      emit_insn (gen_ashldi3 (temp, operands[1], GEN_INT (8)));
      emit_insn (gen_ashldi3 (operands[0], temp,
				     GEN_INT (INTVAL (operands[2]) - 8)));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (DImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
rtx
gen_ashrdi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 4662 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* On the mips16, a shift of more than 8 is a four byte instruction,
     so, for a shift between 8 and 16, it is just as fast to do two
     shifts of 8 or less.  If there is a lot of shifting going on, we
     may win in CSE.  Otherwise combine will put the shifts back
     together again.  This can be called by mips_function_arg, so we must
     be careful not to allocate a new register if we've reached the
     reload pass.  */
  if (TARGET_MIPS16
      && optimize
      && GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) > 8
      && INTVAL (operands[2]) <= 16
      && !reload_in_progress
      && !reload_completed)
    {
      rtx temp = gen_reg_rtx (DImode);

      emit_insn (gen_ashrdi3 (temp, operands[1], GEN_INT (8)));
      emit_insn (gen_ashrdi3 (operands[0], temp,
				     GEN_INT (INTVAL (operands[2]) - 8)));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (DImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
rtx
gen_lshrdi3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 4662 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* On the mips16, a shift of more than 8 is a four byte instruction,
     so, for a shift between 8 and 16, it is just as fast to do two
     shifts of 8 or less.  If there is a lot of shifting going on, we
     may win in CSE.  Otherwise combine will put the shifts back
     together again.  This can be called by mips_function_arg, so we must
     be careful not to allocate a new register if we've reached the
     reload pass.  */
  if (TARGET_MIPS16
      && optimize
      && GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) > 8
      && INTVAL (operands[2]) <= 16
      && !reload_in_progress
      && !reload_completed)
    {
      rtx temp = gen_reg_rtx (DImode);

      emit_insn (gen_lshrdi3 (temp, operands[1], GEN_INT (8)));
      emit_insn (gen_lshrdi3 (operands[0], temp,
				     GEN_INT (INTVAL (operands[2]) - 8)));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (DImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
extern rtx gen_split_824 (rtx, rtx *);
rtx
gen_split_824 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 4809 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[2] = GEN_INT (INTVAL (operands[2]) - 8); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (SImode,
	operand1,
	const_int_rtx[MAX_SAVED_CONST_INT + (8)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFT (SImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
extern rtx gen_split_825 (rtx, rtx *);
rtx
gen_split_825 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 4809 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[2] = GEN_INT (INTVAL (operands[2]) - 8); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand1,
	const_int_rtx[MAX_SAVED_CONST_INT + (8)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFTRT (SImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
extern rtx gen_split_826 (rtx, rtx *);
rtx
gen_split_826 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 4809 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[2] = GEN_INT (INTVAL (operands[2]) - 8); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand1,
	const_int_rtx[MAX_SAVED_CONST_INT + (8)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_LSHIFTRT (SImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
extern rtx gen_split_827 (rtx, rtx *);
rtx
gen_split_827 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 4809 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[2] = GEN_INT (INTVAL (operands[2]) - 8); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (DImode,
	operand1,
	const_int_rtx[MAX_SAVED_CONST_INT + (8)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFT (DImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
extern rtx gen_split_828 (rtx, rtx *);
rtx
gen_split_828 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 4809 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[2] = GEN_INT (INTVAL (operands[2]) - 8); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (DImode,
	operand1,
	const_int_rtx[MAX_SAVED_CONST_INT + (8)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_ASHIFTRT (DImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
extern rtx gen_split_829 (rtx, rtx *);
rtx
gen_split_829 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 4809 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[2] = GEN_INT (INTVAL (operands[2]) - 8); }
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (DImode,
	operand1,
	const_int_rtx[MAX_SAVED_CONST_INT + (8)])));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_LSHIFTRT (DImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4820 */
extern rtx gen_split_830 (rtx, rtx *);
rtx
gen_split_830 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
#line 4829 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	copy_rtx (operand0),
	gen_rtx_LSHIFTRT (SImode,
	copy_rtx (operand0),
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4871 */
rtx
gen_cmpsi (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4876 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  cmp_operands[0] = operands[0];
  cmp_operands[1] = operands[1];
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	cc0_rtx,
	gen_rtx_COMPARE (CCmode,
	operand0,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4871 */
rtx
gen_cmpdi (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4876 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  cmp_operands[0] = operands[0];
  cmp_operands[1] = operands[1];
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	cc0_rtx,
	gen_rtx_COMPARE (CCmode,
	operand0,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4882 */
rtx
gen_cmpsf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4887 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  cmp_operands[0] = operands[0];
  cmp_operands[1] = operands[1];
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	cc0_rtx,
	gen_rtx_COMPARE (CCmode,
	operand0,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4882 */
rtx
gen_cmpdf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 4887 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  cmp_operands[0] = operands[0];
  cmp_operands[1] = operands[1];
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	cc0_rtx,
	gen_rtx_COMPARE (CCmode,
	operand0,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bunordered (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, UNORDERED);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNORDERED (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bordered (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, ORDERED);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_ORDERED (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bunlt (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, UNLT);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNLT (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bunge (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, UNGE);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNGE (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_buneq (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, UNEQ);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNEQ (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bltgt (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, LTGT);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LTGT (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bunle (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, UNLE);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNLE (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bungt (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, UNGT);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_UNGT (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_beq (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, EQ);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_EQ (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bne (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, NE);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_NE (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bgt (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, GT);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GT (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bge (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, GE);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GE (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_blt (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, LT);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LT (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_ble (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, LE);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LE (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bgtu (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, GTU);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GTU (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bgeu (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, GEU);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GEU (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bltu (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, LTU);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LTU (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
rtx
gen_bleu (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5038 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_branch (operands, LEU);
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LEU (CCmode,
	cc0_rtx,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5044 */
rtx
gen_condjump (rtx operand0,
	rtx operand1)
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	operand0,
	gen_rtx_LABEL_REF (VOIDmode,
	operand1),
	pc_rtx));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5057 */
rtx
gen_seq (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5062 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (EQ, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_EQ (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5085 */
rtx
gen_sne (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5090 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (NE, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NE (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5101 */
rtx
gen_sgt (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5106 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (GT, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GT (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5126 */
rtx
gen_sge (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5131 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (GE, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GE (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5142 */
rtx
gen_slt (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5147 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (LT, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LT (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5172 */
rtx
gen_sle (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5177 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (LE, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LE (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5206 */
rtx
gen_sgtu (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5211 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (GTU, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GTU (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5231 */
rtx
gen_sgeu (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5236 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (GEU, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GEU (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5247 */
rtx
gen_sltu (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5252 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (LTU, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LTU (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5277 */
rtx
gen_sleu (rtx operand0)
{
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
#line 5282 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ if (mips_expand_scc (LEU, operands[0])) DONE; else FAIL; }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LEU (SImode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5388 */
rtx
gen_indirect_jump (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5391 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = force_reg (Pmode, operands[0]);
  if (Pmode == SImode)
    emit_jump_insn (gen_indirect_jumpsi (operands[0]));
  else
    emit_jump_insn (gen_indirect_jumpdi (operands[0]));
  DONE;
}
    operand0 = operands[0];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5407 */
rtx
gen_tablejump (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 5412 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16_SHORT_JUMP_TABLES)
    operands[0] = expand_binop (Pmode, add_optab,
				convert_to_mode (Pmode, operands[0], false),
				gen_rtx_LABEL_REF (Pmode, operands[1]),
				0, 0, OPTAB_WIDEN);
  else if (TARGET_GPWORD)
    operands[0] = expand_binop (Pmode, add_optab, operands[0],
				pic_offset_table_rtx, 0, 0, OPTAB_WIDEN);
  else if (TARGET_RTP_PIC)
    {
      /* When generating RTP PIC, we use case table entries that are relative
	 to the start of the function.  Add the function's address to the
	 value we loaded.  */
      rtx start = get_hard_reg_initial_val (Pmode, PIC_FUNCTION_ADDR_REGNUM);
      operands[0] = expand_binop (ptr_mode, add_optab, operands[0],
				  start, 0, 0, OPTAB_WIDEN);
    }

  if (Pmode == SImode)
    emit_jump_insn (gen_tablejumpsi (operands[0], operands[1]));
  else
    emit_jump_insn (gen_tablejumpdi (operands[0], operands[1]));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0));
  emit_insn (gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand1)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5452 */
rtx
gen_builtin_setjmp_setup (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5455 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  rtx addr;

  addr = plus_constant (operands[0], GET_MODE_SIZE (Pmode) * 3);
  mips_emit_move (gen_rtx_MEM (Pmode, addr), pic_offset_table_rtx);
  DONE;
}
    operand0 = operands[0];
  }
  emit_insn (gen_rtx_USE (VOIDmode,
	operand0));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5467 */
rtx
gen_builtin_longjmp (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5470 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* The elements of the buffer are, in order:  */
  int W = GET_MODE_SIZE (Pmode);
  rtx fp = gen_rtx_MEM (Pmode, operands[0]);
  rtx lab = gen_rtx_MEM (Pmode, plus_constant (operands[0], 1*W));
  rtx stack = gen_rtx_MEM (Pmode, plus_constant (operands[0], 2*W));
  rtx gpv = gen_rtx_MEM (Pmode, plus_constant (operands[0], 3*W));
  rtx pv = gen_rtx_REG (Pmode, PIC_FUNCTION_ADDR_REGNUM);
  /* Use gen_raw_REG to avoid being given pic_offset_table_rtx.
     The target is bound to be using $28 as the global pointer
     but the current function might not be.  */
  rtx gp = gen_raw_REG (Pmode, GLOBAL_POINTER_REGNUM);

  /* This bit is similar to expand_builtin_longjmp except that it
     restores $gp as well.  */
  mips_emit_move (hard_frame_pointer_rtx, fp);
  mips_emit_move (pv, lab);
  emit_stack_restore (SAVE_NONLOCAL, stack, NULL_RTX);
  mips_emit_move (gp, gpv);
  emit_insn (gen_rtx_USE (VOIDmode, hard_frame_pointer_rtx));
  emit_insn (gen_rtx_USE (VOIDmode, stack_pointer_rtx));
  emit_insn (gen_rtx_USE (VOIDmode, gp));
  emit_indirect_jump (pv);
  DONE;
}
    operand0 = operands[0];
  }
  emit_insn (gen_rtx_USE (VOIDmode,
	operand0));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5504 */
rtx
gen_prologue (void)
{
  rtx _val = 0;
  start_sequence ();
  {
#line 5507 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_prologue ();
  DONE;
}
  }
  emit_insn (const1_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5524 */
rtx
gen_epilogue (void)
{
  rtx _val = 0;
  start_sequence ();
  {
#line 5527 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_epilogue (false);
  DONE;
}
  }
  emit_insn (const_int_rtx[MAX_SAVED_CONST_INT + (2)]);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5532 */
rtx
gen_sibcall_epilogue (void)
{
  rtx _val = 0;
  start_sequence ();
  {
#line 5535 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_epilogue (true);
  DONE;
}
  }
  emit_insn (const_int_rtx[MAX_SAVED_CONST_INT + (2)]);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5561 */
rtx
gen_eh_return (rtx operand0)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[1];
    operands[0] = operand0;
#line 5564 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_MODE (operands[0]) != word_mode)
    operands[0] = convert_to_mode (word_mode, operands[0], 0);
  if (TARGET_64BIT)
    emit_insn (gen_eh_set_lr_di (operands[0]));
  else
    emit_insn (gen_eh_set_lr_si (operands[0]));
  DONE;
}
    operand0 = operands[0];
  }
  emit_insn (gen_rtx_USE (VOIDmode,
	operand0));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5589 */
extern rtx gen_split_872 (rtx, rtx *);
rtx
gen_split_872 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 5594 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_set_return_address (operands[0], operands[1]);
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5599 */
rtx
gen_exception_receiver (void)
{
  rtx _val = 0;
  start_sequence ();
  {
#line 5602 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* See the comment above load_call<mode> for details.  */
  emit_insn (gen_set_got_version ());

  /* If we have a call-clobbered $gp, restore it from its save slot.  */
  if (HAVE_restore_gp)
    emit_insn (gen_restore_gp ());
  DONE;
}
  }
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5612 */
rtx
gen_nonlocal_goto_receiver (void)
{
  rtx _val = 0;
  start_sequence ();
  {
#line 5615 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  /* See the comment above load_call<mode> for details.  */
  emit_insn (gen_set_got_version ());
  DONE;
}
  }
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5623 */
extern rtx gen_split_875 (rtx, rtx *);
rtx
gen_split_875 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 5630 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_restore_gp ();
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5727 */
rtx
gen_sibcall (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 5733 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_call (0, XEXP (operands[0], 0), operands[1], operands[2], true);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	operand0,
	operand1),
		gen_rtx_USE (VOIDmode,
	operand2),
		gen_rtx_USE (VOIDmode,
	operand3))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5745 */
rtx
gen_sibcall_value (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 5751 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_call (operands[0], XEXP (operands[1], 0),
		    operands[2], operands[3], true);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	operand1,
	operand2)),
		gen_rtx_USE (VOIDmode,
	operand3))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5776 */
rtx
gen_call (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 5782 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_call (0, XEXP (operands[0], 0), operands[1], operands[2], false);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	operand0,
	operand1),
		gen_rtx_USE (VOIDmode,
	operand2),
		gen_rtx_USE (VOIDmode,
	operand3))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5824 */
extern rtx gen_split_879 (rtx, rtx *);
rtx
gen_split_879 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 5832 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  emit_call_insn (gen_call_split (operands[0], operands[1]));
  if (!find_reg_note (operands[2], REG_NORETURN, 0))
    mips_restore_gp ();
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5862 */
rtx
gen_call_value (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[4];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 5868 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_call (operands[0], XEXP (operands[1], 0),
		    operands[2], operands[3], false);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	operand1,
	operand2)),
		gen_rtx_USE (VOIDmode,
	operand3))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5875 */
extern rtx gen_split_881 (rtx, rtx *);
rtx
gen_split_881 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 5884 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  emit_call_insn (gen_call_value_split (operands[0], operands[1],
					operands[2]));
  if (!find_reg_note (operands[3], REG_NORETURN, 0))
    mips_restore_gp ();
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5915 */
extern rtx gen_split_882 (rtx, rtx *);
rtx
gen_split_882 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands ATTRIBUTE_UNUSED)
{
  rtx _val = 0;
  start_sequence ();
#line 5927 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  emit_call_insn (gen_call_value_multiple_split (operands[0], operands[1],
						 operands[2], operands[3]));
  if (!find_reg_note (operands[4], REG_NORETURN, 0))
    mips_restore_gp ();
  DONE;
}
  emit_insn (const0_rtx);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5952 */
rtx
gen_untyped_call (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 5958 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  int i;

  emit_call_insn (GEN_CALL (operands[0], const0_rtx, NULL, const0_rtx));

  for (i = 0; i < XVECLEN (operands[2], 0); i++)
    {
      rtx set = XVECEXP (operands[2], 0, i);
      mips_emit_move (SET_DEST (set), SET_SRC (set));
    }

  emit_insn (gen_blockage ());
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	operand0,
	const0_rtx),
		operand1,
		operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6058 */
rtx
gen_movsicc (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[6];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 6065 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_move (operands);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
    operand5 = operands[5];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SImode,
	operand5,
	operand2,
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6058 */
rtx
gen_movdicc (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[6];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 6065 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_move (operands);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
    operand5 = operands[5];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (DImode,
	operand5,
	operand2,
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6070 */
rtx
gen_movsfcc (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[6];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 6077 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_move (operands);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
    operand5 = operands[5];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SFmode,
	operand5,
	operand2,
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6070 */
rtx
gen_movdfcc (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[6];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 6077 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  mips_expand_conditional_move (operands);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
    operand5 = operands[5];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (DFmode,
	operand5,
	operand2,
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6124 */
extern rtx gen_split_888 (rtx, rtx *);
rtx
gen_split_888 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx _val = 0;
  start_sequence ();
#line 6128 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ operands[0] = mips_rewrite_small_data (operands[0]); }
  operand0 = operands[0];
  emit (operand0);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:48 */
rtx
gen_movv2sfcc (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[6];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
#line 55 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  /* We can only support MOVN.PS and MOVZ.PS.
     NOTE: MOVT.PS and MOVF.PS have different semantics from MOVN.PS and 
	   MOVZ.PS.  MOVT.PS and MOVF.PS depend on two CC values and move 
	   each item independently.  */

  if (GET_MODE_CLASS (GET_MODE (cmp_operands[0])) != MODE_INT)
    FAIL;

  mips_expand_conditional_move (operands);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
    operand5 = operands[5];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (V2SFmode,
	operand5,
	operand2,
	operand3)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:125 */
rtx
gen_vec_initv2sf (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 129 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  rtx op0 = force_reg (SFmode, XVECEXP (operands[1], 0, 0));
  rtx op1 = force_reg (SFmode, XVECEXP (operands[1], 0, 1));
  emit_insn (gen_vec_initv2sf_internal (operands[0], op0, op1));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit (operand0);
  emit (operand1);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:173 */
rtx
gen_vec_setv2sf (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 178 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  rtx temp;

  /* We don't have an insert instruction, so we duplicate the float, and
     then use a PUL instruction.  */
  temp = gen_reg_rtx (V2SFmode);
  emit_insn (gen_mips_cvt_ps_s (temp, operands[1], operands[1]));
  if (INTVAL (operands[2]) == !BYTES_BIG_ENDIAN)
    emit_insn (gen_mips_pul_ps (operands[0], temp, operands[0]));
  else
    emit_insn (gen_mips_pul_ps (operands[0], operands[0], temp));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (operand0);
  emit (operand1);
  emit (operand2);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:193 */
rtx
gen_mips_cvt_ps_s (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 198 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  if (BYTES_BIG_ENDIAN)
    emit_insn (gen_vec_initv2sf_internal (operands[0], operands[1],
	       operands[2]));
  else
    emit_insn (gen_vec_initv2sf_internal (operands[0], operands[2],
	       operands[1]));
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit (operand0);
  emit (operand1);
  emit (operand2);
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:209 */
rtx
gen_mips_cvt_s_pl (rtx operand0,
	rtx operand1)
{
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
#line 214 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{ operands[2] = GEN_INT (BYTES_BIG_ENDIAN); }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_VEC_SELECT (SFmode,
	operand1,
	gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (1,
		operand2)))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:217 */
rtx
gen_mips_cvt_s_pu (rtx operand0,
	rtx operand1)
{
  rtx operand2;
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
#line 222 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{ operands[2] = GEN_INT (!BYTES_BIG_ENDIAN); }
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_VEC_SELECT (SFmode,
	operand1,
	gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (1,
		operand2)))));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:279 */
rtx
gen_mips_abs_ps (rtx operand0,
	rtx operand1)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[2];
    operands[0] = operand0;
    operands[1] = operand1;
#line 284 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  /* If we can ignore NaNs, this operation is equivalent to the
     rtl ABS code.  */
  if (!HONOR_NANS (V2SFmode))
    {
      emit_insn (gen_absv2sf2 (operands[0], operands[1]));
      DONE;
    }
}
    operand0 = operands[0];
    operand1 = operands[1];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (V2SFmode,
	gen_rtvec (1,
		operand1),
	208)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:323 */
extern rtx gen_split_896 (rtx, rtx *);
rtx
gen_split_896 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx _val = 0;
  start_sequence ();
#line 344 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  operands[6] = simplify_gen_subreg (CCV2mode, operands[0], CCV4mode, 0);
  operands[7] = simplify_gen_subreg (CCV2mode, operands[0], CCV4mode, 8);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand6,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand5),
	201)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand7,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (3,
		operand3,
		operand4,
		copy_rtx (operand5)),
	201)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:352 */
extern rtx gen_split_897 (rtx, rtx *);
rtx
gen_split_897 (rtx curr_insn ATTRIBUTE_UNUSED, rtx *operands)
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx _val = 0;
  start_sequence ();
#line 373 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  operands[6] = simplify_gen_subreg (CCV2mode, operands[0], CCV4mode, 0);
  operands[7] = simplify_gen_subreg (CCV2mode, operands[0], CCV4mode, 8);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand6,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (3,
		operand1,
		operand2,
		operand5),
	203)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand7,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (3,
		operand3,
		operand4,
		copy_rtx (operand5)),
	203)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:409 */
rtx
gen_scc_ps (rtx operand0,
	rtx operand1)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (CCV2mode,
	gen_rtvec (1,
		operand1),
	214));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:489 */
rtx
gen_single_cc (rtx operand0,
	rtx operand1)
{
  return gen_rtx_NE (VOIDmode,
	gen_rtx_UNSPEC (CCmode,
	gen_rtvec (2,
		operand0,
		operand1),
	213),
	const0_rtx);
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:582 */
rtx
gen_vcondv2sf (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3,
	rtx operand4,
	rtx operand5)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[6];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
    operands[3] = operand3;
    operands[4] = operand4;
    operands[5] = operand5;
#line 591 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  mips_expand_vcondv2sf (operands[0], operands[1], operands[2],
			 GET_CODE (operands[3]), operands[4], operands[5]);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
    operand3 = operands[3];
    operand4 = operands[4];
    operand5 = operands[5];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (V2SFmode,
	gen_rtx_fmt_ee (GET_CODE (operand3), VOIDmode,
		operand4,
		operand5),
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:597 */
rtx
gen_sminv2sf3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 602 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  mips_expand_vcondv2sf (operands[0], operands[1], operands[2],
			 LE, operands[1], operands[2]);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SMIN (V2SFmode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:608 */
rtx
gen_smaxv2sf3 (rtx operand0,
	rtx operand1,
	rtx operand2)
{
  rtx _val = 0;
  start_sequence ();
  {
    rtx operands[3];
    operands[0] = operand0;
    operands[1] = operand1;
    operands[2] = operand2;
#line 613 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  mips_expand_vcondv2sf (operands[0], operands[1], operands[2],
			 LE, operands[2], operands[1]);
  DONE;
}
    operand0 = operands[0];
    operand1 = operands[1];
    operand2 = operands[2];
  }
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SMAX (V2SFmode,
	operand1,
	operand2)));
  _val = get_insns ();
  end_sequence ();
  return _val;
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:157 */
rtx
gen_mips_madd (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand2),
	gen_rtx_SIGN_EXTEND (DImode,
	operand3)),
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:157 */
rtx
gen_mips_maddu (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand2),
	gen_rtx_ZERO_EXTEND (DImode,
	operand3)),
	operand1));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:165 */
rtx
gen_mips_msub (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DImode,
	operand1,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand2),
	gen_rtx_SIGN_EXTEND (DImode,
	operand3))));
}

/* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:165 */
rtx
gen_mips_msubu (rtx operand0,
	rtx operand1,
	rtx operand2,
	rtx operand3)
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DImode,
	operand1,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand2),
	gen_rtx_ZERO_EXTEND (DImode,
	operand3))));
}



void
add_clobbers (rtx pattern ATTRIBUTE_UNUSED, int insn_code_number)
{
  switch (insn_code_number)
    {
    case 438:
      XVECEXP (pattern, 0, 2) = gen_hard_reg_clobber (SImode, 31);
      XVECEXP (pattern, 0, 3) = gen_hard_reg_clobber (SImode, 28);
      break;

    case 435:
    case 433:
      XVECEXP (pattern, 0, 1) = gen_hard_reg_clobber (SImode, 31);
      XVECEXP (pattern, 0, 2) = gen_hard_reg_clobber (SImode, 28);
      break;

    case 437:
    case 436:
    case 432:
      XVECEXP (pattern, 0, 2) = gen_hard_reg_clobber (SImode, 31);
      break;

    case 434:
    case 431:
    case 284:
      XVECEXP (pattern, 0, 1) = gen_hard_reg_clobber (SImode, 31);
      break;

    case 177:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SFmode));
      break;

    case 175:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DFmode));
      break;

    case 41:
    case 40:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode));
      XVECEXP (pattern, 0, 2) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode));
      XVECEXP (pattern, 0, 3) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode));
      break;

    case 33:
    case 32:
      XVECEXP (pattern, 0, 2) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      break;

    case 34:
    case 31:
    case 28:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      XVECEXP (pattern, 0, 2) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      XVECEXP (pattern, 0, 3) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      break;

    case 27:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode));
      XVECEXP (pattern, 0, 2) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode));
      break;

    case 422:
    case 197:
    case 55:
    case 54:
    case 39:
    case 38:
    case 25:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (DImode));
      break;

    case 421:
    case 56:
    case 49:
    case 48:
    case 24:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      break;

    case 53:
    case 52:
    case 51:
    case 50:
    case 35:
    case 30:
    case 29:
    case 26:
    case 23:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      XVECEXP (pattern, 0, 2) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      break;

    default:
      gcc_unreachable ();
    }
}


int
added_clobbers_hard_reg_p (int insn_code_number)
{
  switch (insn_code_number)
    {
    case 177:
    case 175:
    case 41:
    case 40:
    case 33:
    case 32:
    case 34:
    case 31:
    case 28:
    case 27:
    case 422:
    case 197:
    case 55:
    case 54:
    case 39:
    case 38:
    case 25:
    case 421:
    case 56:
    case 49:
    case 48:
    case 24:
    case 53:
    case 52:
    case 51:
    case 50:
    case 35:
    case 30:
    case 29:
    case 26:
    case 23:
      return 0;

    case 438:
    case 435:
    case 433:
    case 437:
    case 436:
    case 432:
    case 434:
    case 431:
    case 284:
      return 1;

    default:
      gcc_unreachable ();
    }
}
