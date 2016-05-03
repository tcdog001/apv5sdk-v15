/* Generated automatically by the program `genoutput'
   from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "flags.h"
#include "ggc.h"
#include "rtl.h"
#include "expr.h"
#include "insn-codes.h"
#include "tm_p.h"
#include "function.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"

#include "conditions.h"
#include "insn-attr.h"

#include "recog.h"

#include "toplev.h"
#include "output.h"
#include "target.h"
#include "tm-constrs.h"

static const char *
output_0 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 737 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (ISA_HAS_COND_TRAP)
    return "teq\t$0,$0";
  else if (TARGET_MIPS16)
    return "break 0";
  else
    return "break";
}
}

static const char * const output_6[] = {
  "addu\t%0,%1,%2",
  "addiu\t%0,%1,%2",
};

static const char * const output_7[] = {
  "daddu\t%0,%1,%2",
  "daddiu\t%0,%1,%2",
};

static const char * const output_8[] = {
  "addiu\t%0,%2",
  "addiu\t%0,%1,%2",
  "addiu\t%0,%2",
  "addiu\t%0,%1,%2",
  "addu\t%0,%1,%2",
};

static const char * const output_9[] = {
  "daddiu\t%0,%2",
  "daddiu\t%0,%1,%2",
  "daddiu\t%0,%2",
  "daddiu\t%0,%1,%2",
  "daddu\t%0,%1,%2",
};

static const char * const output_10[] = {
  "addu\t%0,%1,%2",
  "addiu\t%0,%1,%2",
};

static const char *
output_23 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1153 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 1)
    return "mult\t%1,%2";
  if (TARGET_MIPS3900)
    return "mult\t%0,%1,%2";
  return "mul\t%0,%1,%2";
}
}

static const char *
output_28 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1265 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  static const char *const madd[] = { "madd\t%1,%2", "madd\t%0,%1,%2" };
  if (which_alternative == 2)
    return "#";
  if (GENERATE_MADD_MSUB && which_alternative != 0)
    return "#";
  return madd[which_alternative];
}
}

static const char *
output_29 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1325 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 1)
    return "macc\t%0,%1,%2";
  else if (TARGET_MIPS5500)
    return "madd\t%1,%2";
  else
    /* The VR4130 assumes that there is a two-cycle latency between a macc
       that "writes" to $0 and an instruction that reads from it.  We avoid
       this by assigning to $1 instead.  */
    return "%[macc\t%@,%1,%2%]";
}
}

static const char *
output_30 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1347 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 1)
    return "msac\t%0,%2,%3";
  else if (TARGET_MIPS5500)
    return "msub\t%2,%3";
  else
    return "msac\t$0,%2,%3";
}
}

static const char * const output_34[] = {
  "msub\t%2,%3",
  "#",
  "#",
};

static const char * const output_35[] = {
  "muls\t$0,%1,%2",
  "muls\t%0,%1,%2",
};

static const char *
output_46 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1711 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (ISA_HAS_DSPR2)
    return "msub\t%q0,%1,%2";
  else if (TARGET_MIPS5500 || GENERATE_MADD_MSUB)
    return "msub\t%1,%2";
  else
    return "msac\t$0,%1,%2";
}
}

static const char *
output_47 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1711 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (ISA_HAS_DSPR2)
    return "msubu\t%q0,%1,%2";
  else if (TARGET_MIPS5500 || GENERATE_MADD_MSUB)
    return "msubu\t%1,%2";
  else
    return "msacu\t$0,%1,%2";
}
}

static const char * const output_50[] = {
  "mult\t%1,%2",
  "mulhi\t%0,%1,%2",
};

static const char * const output_51[] = {
  "multu\t%1,%2",
  "mulhiu\t%0,%1,%2",
};

static const char * const output_52[] = {
  "mulshi\t%.,%1,%2",
  "mulshi\t%0,%1,%2",
};

static const char * const output_53[] = {
  "mulshiu\t%.,%1,%2",
  "mulshiu\t%0,%1,%2",
};

static const char *
output_57 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1831 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MAD)
    return "mad\t%1,%2";
  else if (ISA_HAS_DSPR2)
    return "madd\t%q0,%1,%2";
  else if (GENERATE_MADD_MSUB || TARGET_MIPS5500)
    return "madd\t%1,%2";
  else
    /* See comment in *macc.  */
    return "%[macc\t%@,%1,%2%]";
}
}

static const char *
output_58 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1831 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MAD)
    return "madu\t%1,%2";
  else if (ISA_HAS_DSPR2)
    return "maddu\t%q0,%1,%2";
  else if (GENERATE_MADD_MSUB || TARGET_MIPS5500)
    return "maddu\t%1,%2";
  else
    /* See comment in *macc.  */
    return "%[maccu\t%@,%1,%2%]";
}
}

static const char *
output_77 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1959 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "div.s\t%0,%1,%2\n\tmov.s\t%0,%0";
  else
    return "div.s\t%0,%1,%2";
}
}

static const char *
output_78 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1959 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "div.d\t%0,%1,%2\n\tmov.d\t%0,%0";
  else
    return "div.d\t%0,%1,%2";
}
}

static const char *
output_79 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1959 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "div.ps\t%0,%1,%2\n\tmov.ps\t%0,%0";
  else
    return "div.ps\t%0,%1,%2";
}
}

static const char *
output_80 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1977 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "recip.s\t%0,%2\n\tmov.s\t%0,%0";
  else
    return "recip.s\t%0,%2";
}
}

static const char *
output_81 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1977 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "recip.d\t%0,%2\n\tmov.d\t%0,%0";
  else
    return "recip.d\t%0,%2";
}
}

static const char *
output_82 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 1977 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "recip.ps\t%0,%2\n\tmov.ps\t%0,%0";
  else
    return "recip.ps\t%0,%2";
}
}

static const char *
output_83 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2000 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_division ("div\t$0,%1,%2", operands); }
}

static const char *
output_84 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2000 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_division ("ddiv\t$0,%1,%2", operands); }
}

static const char *
output_85 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2012 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_division ("divu\t$0,%1,%2", operands); }
}

static const char *
output_86 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2012 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_division ("ddivu\t$0,%1,%2", operands); }
}

static const char *
output_87 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2030 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "sqrt.s\t%0,%1\n\tmov.s\t%0,%0";
  else
    return "sqrt.s\t%0,%1";
}
}

static const char *
output_88 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2030 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "sqrt.d\t%0,%1\n\tmov.d\t%0,%0";
  else
    return "sqrt.d\t%0,%1";
}
}

static const char *
output_89 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2030 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "sqrt.ps\t%0,%1\n\tmov.ps\t%0,%0";
  else
    return "sqrt.ps\t%0,%1";
}
}

static const char *
output_90 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2048 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "rsqrt.s\t%0,%2\n\tmov.s\t%0,%0";
  else
    return "rsqrt.s\t%0,%2";
}
}

static const char *
output_91 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2048 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "rsqrt.d\t%0,%2\n\tmov.d\t%0,%0";
  else
    return "rsqrt.d\t%0,%2";
}
}

static const char *
output_92 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2048 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "rsqrt.ps\t%0,%2\n\tmov.ps\t%0,%0";
  else
    return "rsqrt.ps\t%0,%2";
}
}

static const char *
output_93 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2066 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "rsqrt.s\t%0,%2\n\tmov.s\t%0,%0";
  else
    return "rsqrt.s\t%0,%2";
}
}

static const char *
output_94 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2066 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "rsqrt.d\t%0,%2\n\tmov.d\t%0,%0";
  else
    return "rsqrt.d\t%0,%2";
}
}

static const char *
output_95 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2066 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_FIX_SB1)
    return "rsqrt.ps\t%0,%2\n\tmov.ps\t%0,%0";
  else
    return "rsqrt.ps\t%0,%2";
}
}

static const char *
output_101 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2128 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16)
    return "neg\t%0,%1";
  else
    return "subu\t%0,%.,%1";
}
}

static const char *
output_106 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2161 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16)
    return "not\t%0,%1";
  else
    return "nor\t%0,%.,%1";
}
}

static const char *
output_107 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2161 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (TARGET_MIPS16)
    return "not\t%0,%1";
  else
    return "nor\t%0,%.,%1";
}
}

static const char * const output_108[] = {
  "and\t%0,%1,%2",
  "andi\t%0,%1,%x2",
};

static const char * const output_109[] = {
  "and\t%0,%1,%2",
  "andi\t%0,%1,%x2",
};

static const char * const output_112[] = {
  "or\t%0,%1,%2",
  "ori\t%0,%1,%x2",
};

static const char * const output_113[] = {
  "or\t%0,%1,%2",
  "ori\t%0,%1,%x2",
};

static const char * const output_116[] = {
  "xor\t%0,%1,%2",
  "xori\t%0,%1,%x2",
};

static const char * const output_117[] = {
  "xor\t%0,%1,%2",
  "xori\t%0,%1,%x2",
};

static const char * const output_118[] = {
  "xor\t%0,%2",
  "cmpi\t%1,%2",
  "cmp\t%1,%2",
};

static const char * const output_119[] = {
  "xor\t%0,%2",
  "cmpi\t%1,%2",
  "cmp\t%1,%2",
};

static const char * const output_123[] = {
  "sll\t%0,%1,0",
  "sw\t%1,%0",
};

static const char * const output_124[] = {
  "sll\t%0,%1,0",
  "sh\t%1,%0",
};

static const char * const output_125[] = {
  "sll\t%0,%1,0",
  "sb\t%1,%0",
};

static const char * const output_141[] = {
  "#",
  "lwu\t%0,%1",
};

static const char *
output_142 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2475 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return "#";

  operands[1] = gen_lowpart (SImode, operands[1]);
  return "lwu\t%0,%1";
}
}

static const char * const output_143[] = {
  "andi\t%0,%1,0x00ff",
  "lbu\t%0,%1",
};

static const char * const output_144[] = {
  "andi\t%0,%1,0x00ff",
  "lbu\t%0,%1",
};

static const char * const output_145[] = {
  "andi\t%0,%1,0xffff",
  "lhu\t%0,%1",
};

static const char * const output_146[] = {
  "andi\t%0,%1,0xffff",
  "lhu\t%0,%1",
};

static const char * const output_155[] = {
  "andi\t%0,%1,0x00ff",
  "lbu\t%0,%1",
};

static const char * const output_157[] = {
  "#",
  "lw\t%0,%1",
};

static const char * const output_158[] = {
  "seb\t%0",
  "lb\t%0,%1",
};

static const char * const output_159[] = {
  "seb\t%0",
  "lb\t%0,%1",
};

static const char * const output_160[] = {
  "seh\t%0",
  "lh\t%0,%1",
};

static const char * const output_161[] = {
  "seh\t%0",
  "lh\t%0,%1",
};

static const char * const output_162[] = {
  "#",
  "lb\t%0,%1",
};

static const char * const output_163[] = {
  "#",
  "lb\t%0,%1",
};

static const char * const output_164[] = {
  "#",
  "lh\t%0,%1",
};

static const char * const output_165[] = {
  "#",
  "lh\t%0,%1",
};

static const char * const output_166[] = {
  "seb\t%0,%1",
  "lb\t%0,%1",
};

static const char * const output_167[] = {
  "seb\t%0,%1",
  "lb\t%0,%1",
};

static const char * const output_168[] = {
  "seh\t%0,%1",
  "lh\t%0,%1",
};

static const char * const output_169[] = {
  "seh\t%0,%1",
  "lh\t%0,%1",
};

static const char * const output_170[] = {
  "seb\t%0",
  "lb\t%0,%1",
};

static const char * const output_171[] = {
  "#",
  "lb\t%0,%1",
};

static const char * const output_172[] = {
  "seb\t%0,%1",
  "lb\t%0,%1",
};

static const char *
output_175 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2736 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (set_nomacro)
    return ".set\tmacro\n\ttrunc.w.d %0,%1,%2\n\t.set\tnomacro";
  else
    return "trunc.w.d %0,%1,%2";
}
}

static const char *
output_177 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 2774 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (set_nomacro)
    return ".set\tmacro\n\ttrunc.w.s %0,%1,%2\n\t.set\tnomacro";
  else
    return "trunc.w.s %0,%1,%2";
}
}

static const char *
output_214 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3438 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_215 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3449 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_216 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3460 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_217 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3471 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_218 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3482 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_219 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3568 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_220 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3579 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_221 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3670 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char * const output_235[] = {
  "move\t%0,%1",
  "li\t%0,%1",
  "lhu\t%0,%1",
  "sh\t%z1,%0",
  "mt%0\t%1",
};

static const char * const output_236[] = {
  "move\t%0,%1",
  "move\t%0,%1",
  "move\t%0,%1",
  "li\t%0,%1",
  "#",
  "lhu\t%0,%1",
  "sh\t%1,%0",
};

static const char * const output_237[] = {
  "move\t%0,%1",
  "li\t%0,%1",
  "lbu\t%0,%1",
  "sb\t%z1,%0",
  "mt%0\t%1",
};

static const char * const output_238[] = {
  "move\t%0,%1",
  "move\t%0,%1",
  "move\t%0,%1",
  "li\t%0,%1",
  "#",
  "lbu\t%0,%1",
  "sb\t%1,%0",
};

static const char *
output_239 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3956 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_240 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3967 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_241 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 3978 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_242 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4001 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_243 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4013 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_244 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4024 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_245 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4035 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_247 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4111 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char *
output_248 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4124 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_move (operands[0], operands[1]); }
}

static const char * const output_251[] = {
  "macchi\t%0,%.,%.",
  "macc\t%0,%.,%.",
};

static const char * const output_252[] = {
  "dmacchi\t%0,%.,%.",
  "dmacc\t%0,%.,%.",
};

static const char *
output_253 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4209 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = mips_subword (operands[0], 0);
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_254 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4209 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = mips_subword (operands[0], 0);
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_255 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4209 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = mips_subword (operands[0], 0);
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_256 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4209 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = mips_subword (operands[0], 0);
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_257 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4224 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = mips_subword (operands[0], 1);
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_258 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4224 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = mips_subword (operands[0], 1);
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_259 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4224 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = mips_subword (operands[0], 1);
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_260 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4224 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[0] = mips_subword (operands[0], 1);
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_261 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4239 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = mips_subword (operands[1], INTVAL (operands[2]));
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_262 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4239 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = mips_subword (operands[1], INTVAL (operands[2]));
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_263 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4239 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = mips_subword (operands[1], INTVAL (operands[2]));
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_264 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4239 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = mips_subword (operands[1], INTVAL (operands[2]));
  return mips_output_move (operands[0], operands[1]);
}
}

static const char *
output_280 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4348 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (set_nomacro && which_alternative == 1)
    return ".set\tmacro\n\t.cprestore\t%0\n\t.set\tnomacro";
  else
    return ".cprestore\t%0";
}
}

static const char *
output_284 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4402 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return "%(%<bal\t1f\n"
         "\tnop\n"
         "1:\taddiu\t$31,$31,12\n"
         "\tjr.hb\t$31\n"
         "\tnop%>%)";
}
}

static const char *
output_286 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4427 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_COMPARE_AND_SWAP ("", "li");
  else
    return MIPS_COMPARE_AND_SWAP ("", "move");
}
}

static const char *
output_287 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4427 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_COMPARE_AND_SWAP ("d", "li");
  else
    return MIPS_COMPARE_AND_SWAP ("d", "move");
}
}

static const char *
output_288 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4442 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OP ("", "addiu");	
  else
    return MIPS_SYNC_OP ("", "addu");	
}
}

static const char *
output_289 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4442 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OP ("d", "daddiu");	
  else
    return MIPS_SYNC_OP ("d", "daddu");	
}
}

static const char *
output_290 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4457 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return MIPS_SYNC_OP ("", "subu");	
}
}

static const char *
output_291 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4457 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return MIPS_SYNC_OP ("d", "dsubu");	
}
}

static const char *
output_292 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4471 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_OP ("", "addiu");	
  else
    return MIPS_SYNC_OLD_OP ("", "addu");	
}
}

static const char *
output_293 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4471 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_OP ("d", "daddiu");	
  else
    return MIPS_SYNC_OLD_OP ("d", "daddu");	
}
}

static const char *
output_294 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4488 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return MIPS_SYNC_OLD_OP ("", "subu");	
}
}

static const char *
output_295 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4488 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return MIPS_SYNC_OLD_OP ("d", "dsubu");	
}
}

static const char *
output_296 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4502 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_OP ("", "addiu");	
  else
    return MIPS_SYNC_NEW_OP ("", "addu");	
}
}

static const char *
output_297 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4502 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_OP ("d", "daddiu");	
  else
    return MIPS_SYNC_NEW_OP ("d", "daddu");	
}
}

static const char *
output_298 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4519 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return MIPS_SYNC_NEW_OP ("", "subu");	
}
}

static const char *
output_299 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4519 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return MIPS_SYNC_NEW_OP ("d", "dsubu");	
}
}

static const char *
output_300 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4531 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OP ("", "ori");	
  else
    return MIPS_SYNC_OP ("", "or");	
}
}

static const char *
output_301 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4531 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OP ("", "xori");	
  else
    return MIPS_SYNC_OP ("", "xor");	
}
}

static const char *
output_302 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4531 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OP ("", "andi");	
  else
    return MIPS_SYNC_OP ("", "and");	
}
}

static const char *
output_303 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4531 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OP ("d", "ori");	
  else
    return MIPS_SYNC_OP ("d", "or");	
}
}

static const char *
output_304 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4531 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OP ("d", "xori");	
  else
    return MIPS_SYNC_OP ("d", "xor");	
}
}

static const char *
output_305 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4531 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OP ("d", "andi");	
  else
    return MIPS_SYNC_OP ("d", "and");	
}
}

static const char *
output_306 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4548 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_OP ("", "ori");	
  else
    return MIPS_SYNC_OLD_OP ("", "or");	
}
}

static const char *
output_307 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4548 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_OP ("", "xori");	
  else
    return MIPS_SYNC_OLD_OP ("", "xor");	
}
}

static const char *
output_308 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4548 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_OP ("", "andi");	
  else
    return MIPS_SYNC_OLD_OP ("", "and");	
}
}

static const char *
output_309 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4548 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_OP ("d", "ori");	
  else
    return MIPS_SYNC_OLD_OP ("d", "or");	
}
}

static const char *
output_310 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4548 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_OP ("d", "xori");	
  else
    return MIPS_SYNC_OLD_OP ("d", "xor");	
}
}

static const char *
output_311 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4548 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_OP ("d", "andi");	
  else
    return MIPS_SYNC_OLD_OP ("d", "and");	
}
}

static const char *
output_312 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4565 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_OP ("", "ori");	
  else
    return MIPS_SYNC_NEW_OP ("", "or");	
}
}

static const char *
output_313 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4565 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_OP ("", "xori");	
  else
    return MIPS_SYNC_NEW_OP ("", "xor");	
}
}

static const char *
output_314 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4565 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_OP ("", "andi");	
  else
    return MIPS_SYNC_NEW_OP ("", "and");	
}
}

static const char *
output_315 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4565 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_OP ("d", "ori");	
  else
    return MIPS_SYNC_NEW_OP ("d", "or");	
}
}

static const char *
output_316 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4565 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_OP ("d", "xori");	
  else
    return MIPS_SYNC_NEW_OP ("d", "xor");	
}
}

static const char *
output_317 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4565 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_OP ("d", "andi");	
  else
    return MIPS_SYNC_NEW_OP ("d", "and");	
}
}

static const char *
output_318 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4578 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NAND ("", "andi");	
  else
    return MIPS_SYNC_NAND ("", "and");	
}
}

static const char *
output_319 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4578 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NAND ("d", "andi");	
  else
    return MIPS_SYNC_NAND ("d", "and");	
}
}

static const char *
output_320 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4593 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_NAND ("", "andi");	
  else
    return MIPS_SYNC_OLD_NAND ("", "and");	
}
}

static const char *
output_321 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4593 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_OLD_NAND ("d", "andi");	
  else
    return MIPS_SYNC_OLD_NAND ("d", "and");	
}
}

static const char *
output_322 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4608 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_NAND ("", "andi");	
  else
    return MIPS_SYNC_NEW_NAND ("", "and");	
}
}

static const char *
output_323 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4608 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_NEW_NAND ("d", "andi");	
  else
    return MIPS_SYNC_NEW_NAND ("d", "and");	
}
}

static const char *
output_324 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4623 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_EXCHANGE ("", "li");
  else
    return MIPS_SYNC_EXCHANGE ("", "move");
}
}

static const char *
output_325 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4623 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return MIPS_SYNC_EXCHANGE ("d", "li");
  else
    return MIPS_SYNC_EXCHANGE ("d", "move");
}
}

static const char *
output_326 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4692 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2])
			   & (GET_MODE_BITSIZE (SImode) - 1));

  return "sll\t%0,%1,%2";
}
}

static const char *
output_327 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4692 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2])
			   & (GET_MODE_BITSIZE (SImode) - 1));

  return "sra\t%0,%1,%2";
}
}

static const char *
output_328 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4692 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2])
			   & (GET_MODE_BITSIZE (SImode) - 1));

  return "srl\t%0,%1,%2";
}
}

static const char *
output_329 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4692 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2])
			   & (GET_MODE_BITSIZE (DImode) - 1));

  return "dsll\t%0,%1,%2";
}
}

static const char *
output_330 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4692 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2])
			   & (GET_MODE_BITSIZE (DImode) - 1));

  return "dsra\t%0,%1,%2";
}
}

static const char *
output_331 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4692 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2])
			   & (GET_MODE_BITSIZE (DImode) - 1));

  return "dsrl\t%0,%1,%2";
}
}

static const char *
output_332 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4708 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);

  return "sll\t%0,%1,%2";
}
}

static const char *
output_333 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4708 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);

  return "sra\t%0,%1,%2";
}
}

static const char *
output_334 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4708 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);

  return "srl\t%0,%1,%2";
}
}

static const char *
output_335 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4722 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return "sll\t%0,%2";

  operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
  return "sll\t%0,%1,%2";
}
}

static const char *
output_336 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4722 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return "sra\t%0,%2";

  operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
  return "sra\t%0,%1,%2";
}
}

static const char *
output_337 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4722 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return "srl\t%0,%2";

  operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
  return "srl\t%0,%1,%2";
}
}

static const char *
output_338 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4744 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (which_alternative == 0)
    return "dsll\t%0,%2";

  operands[2] = GEN_INT (INTVAL (operands[2]) & 0x3f);
  return "dsll\t%0,%1,%2";
}
}

static const char *
output_339 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4764 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x3f);

  return "dsra\t%0,%2";
}
}

static const char *
output_340 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4783 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 0x3f);

  return "dsrl\t%0,%2";
}
}

static const char *
output_342 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4839 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    gcc_assert (INTVAL (operands[2]) >= 0
		&& INTVAL (operands[2]) < GET_MODE_BITSIZE (SImode));

  return "ror\t%0,%1,%2";
}
}

static const char *
output_343 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4839 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (GET_CODE (operands[2]) == CONST_INT)
    gcc_assert (INTVAL (operands[2]) >= 0
		&& INTVAL (operands[2]) < GET_MODE_BITSIZE (DImode));

  return "dror\t%0,%1,%2";
}
}

static const char *
output_344 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4911 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("b%F0", "%Z2%1"),
					 MIPS_BRANCH ("b%W0", "%Z2%1"));
}
}

static const char *
output_345 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4928 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("b%W0", "%Z2%1"),
					 MIPS_BRANCH ("b%F0", "%Z2%1"));
}
}

static const char *
output_346 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4947 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_order_conditional_branch (insn, operands, false); }
}

static const char *
output_347 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4947 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_order_conditional_branch (insn, operands, false); }
}

static const char *
output_348 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4960 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_order_conditional_branch (insn, operands, true); }
}

static const char *
output_349 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4960 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips_output_order_conditional_branch (insn, operands, true); }
}

static const char *
output_350 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4975 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("b%C0", "%2,%z3,%1"),
					 MIPS_BRANCH ("b%N0", "%2,%z3,%1"));
}
}

static const char *
output_351 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4975 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("b%C0", "%2,%z3,%1"),
					 MIPS_BRANCH ("b%N0", "%2,%z3,%1"));
}
}

static const char *
output_352 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4992 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("b%N0", "%2,%z3,%1"),
					 MIPS_BRANCH ("b%C0", "%2,%z3,%1"));
}
}

static const char *
output_353 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 4992 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("b%N0", "%2,%z3,%1"),
					 MIPS_BRANCH ("b%C0", "%2,%z3,%1"));
}
}

static const char *
output_354 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5011 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (operands[2] != pc_rtx)
    {
      if (which_alternative == 0)
	return "b%C0z\t%1,%2";
      else
	return "bt%C0z\t%2";
    }
  else
    {
      if (which_alternative == 0)
	return "b%N0z\t%1,%3";
      else
	return "bt%N0z\t%3";
    }
}
}

static const char *
output_355 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5011 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (operands[2] != pc_rtx)
    {
      if (which_alternative == 0)
	return "b%C0z\t%1,%2";
      else
	return "bt%C0z\t%2";
    }
  else
    {
      if (which_alternative == 0)
	return "b%N0z\t%1,%3";
      else
	return "bt%N0z\t%3";
    }
}
}

static const char *
output_372 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5184 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = GEN_INT (INTVAL (operands[2]) + 1);
  return "slt\t%0,%1,%2";
}
}

static const char *
output_373 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5184 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = GEN_INT (INTVAL (operands[2]) + 1);
  return "slt\t%0,%1,%2";
}
}

static const char *
output_374 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5196 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = GEN_INT (INTVAL (operands[2]) + 1);
  return "slt\t%1,%2";
}
}

static const char *
output_375 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5196 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = GEN_INT (INTVAL (operands[2]) + 1);
  return "slt\t%1,%2";
}
}

static const char *
output_386 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5289 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = GEN_INT (INTVAL (operands[2]) + 1);
  return "sltu\t%0,%1,%2";
}
}

static const char *
output_387 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5289 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = GEN_INT (INTVAL (operands[2]) + 1);
  return "sltu\t%0,%1,%2";
}
}

static const char *
output_388 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5301 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = GEN_INT (INTVAL (operands[2]) + 1);
  return "sltu\t%1,%2";
}
}

static const char *
output_389 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5301 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = GEN_INT (INTVAL (operands[2]) + 1);
  return "sltu\t%1,%2";
}
}

static const char *
output_412 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5349 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  if (flag_pic)
    {
      if (get_attr_length (insn) <= 8)
	return "%*b\t%l0%/";
      else
	{
	  output_asm_insn (mips_output_load_label (), operands);
	  return "%*jr\t%@%/%]";
	}
    }
  else
    return "%*j\t%l0%/";
}
}

static const char *
output_428 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5742 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return MIPS_CALL ("j", operands, 0); }
}

static const char *
output_429 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5762 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return MIPS_CALL ("j", operands, 1); }
}

static const char *
output_430 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5773 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return MIPS_CALL ("j", operands, 1); }
}

static const char *
output_431 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5829 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return TARGET_SPLIT_CALLS ? "#" : MIPS_CALL ("jal", operands, 0); }
}

static const char *
output_432 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5851 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return MIPS_CALL ("jal", operands, 0); }
}

static const char *
output_433 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5859 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return MIPS_CALL ("jal", operands, 0); }
}

static const char *
output_434 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5881 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return TARGET_SPLIT_CALLS ? "#" : MIPS_CALL ("jal", operands, 1); }
}

static const char *
output_435 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5901 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return MIPS_CALL ("jal", operands, 1); }
}

static const char *
output_436 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5912 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return MIPS_CALL ("jal", operands, 1); }
}

static const char *
output_437 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5924 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return TARGET_SPLIT_CALLS ? "#" : MIPS_CALL ("jal", operands, 1); }
}

static const char *
output_438 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5947 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return MIPS_CALL ("jal", operands, 1); }
}

static const char *
output_439 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5987 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[1] = mips_prefetch_cookie (operands[1], operands[2]);
  return "pref\t%1,%a0";
}
}

static const char *
output_440 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5999 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = mips_prefetch_cookie (operands[2], operands[3]);
  return "prefx\t%2,%1(%0)";
}
}

static const char *
output_441 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 5999 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  operands[2] = mips_prefetch_cookie (operands[2], operands[3]);
  return "prefx\t%2,%1(%0)";
}
}

static const char *
output_443 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 6016 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
    if (set_noreorder)
      return "nop";
    else
      return "#nop";
  }
}

static const char * const output_444[] = {
  "mov%T4\t%0,%z2,%1",
  "mov%t4\t%0,%z3,%1",
};

static const char * const output_445[] = {
  "mov%T4\t%0,%z2,%1",
  "mov%t4\t%0,%z3,%1",
};

static const char * const output_446[] = {
  "mov%T4\t%0,%z2,%1",
  "mov%t4\t%0,%z3,%1",
};

static const char * const output_447[] = {
  "mov%T4\t%0,%z2,%1",
  "mov%t4\t%0,%z3,%1",
};

static const char * const output_448[] = {
  "mov%T4\t%0,%z2,%1",
  "mov%t4\t%0,%z3,%1",
};

static const char * const output_449[] = {
  "mov%T4\t%0,%z2,%1",
  "mov%t4\t%0,%z3,%1",
};

static const char * const output_450[] = {
  "mov%T4.s\t%0,%2,%1",
  "mov%t4.s\t%0,%3,%1",
};

static const char * const output_451[] = {
  "mov%T4.d\t%0,%2,%1",
  "mov%t4.d\t%0,%3,%1",
};

static const char * const output_452[] = {
  "mov%T4.s\t%0,%2,%1",
  "mov%t4.s\t%0,%3,%1",
};

static const char * const output_453[] = {
  "mov%T4.d\t%0,%2,%1",
  "mov%t4.d\t%0,%3,%1",
};

static const char * const output_454[] = {
  "mov%T4.s\t%0,%2,%1",
  "mov%t4.s\t%0,%3,%1",
};

static const char * const output_455[] = {
  "mov%T4.d\t%0,%2,%1",
  "mov%t4.d\t%0,%3,%1",
};

static const char *
output_456 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 6095 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  assemble_integer (operands[0], INTVAL (operands[1]),
		    BITS_PER_UNIT * INTVAL (operands[1]), 1);
  return "";
}
}

static const char *
output_457 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 6106 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{
  REAL_VALUE_TYPE d;

  gcc_assert (GET_CODE (operands[0]) == CONST_DOUBLE);
  REAL_VALUE_FROM_CONST_DOUBLE (d, operands[0]);
  assemble_real (d, GET_MODE (operands[0]),
		 GET_MODE_BITSIZE (GET_MODE (operands[0])));
  return "";
}
}

static const char *
output_459 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 6145 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md"
{ return mips16e_output_save_restore (operands[0], INTVAL (operands[2])); }
}

static const char * const output_460[] = {
  "mov%T4.ps\t%0,%2,%1",
  "mov%t4.ps\t%0,%3,%1",
};

static const char * const output_461[] = {
  "mov%T4.ps\t%0,%2,%1",
  "mov%t4.ps\t%0,%3,%1",
};

static const char * const output_462[] = {
  "movt.ps\t%0,%1,%3",
  "movf.ps\t%0,%2,%3",
};

static const char *
output_467 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 142 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  if (BYTES_BIG_ENDIAN)
    return "cvt.ps.s\t%0,%1,%2";
  else
    return "cvt.ps.s\t%0,%2,%1";
}
}

static const char *
output_468 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 161 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  if (INTVAL (operands[2]) == !BYTES_BIG_ENDIAN)
    return "cvt.s.pu\t%0,%1";
  else
    return "cvt.s.pl\t%0,%1";
}
}

static const char *
output_496 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 508 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  operands[2]
    = gen_rtx_REG (CCmode, REGNO (operands[2]) + INTVAL (operands[3]));
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("b%F0", "%2,%1"),
					 MIPS_BRANCH ("b%W0", "%2,%1"));
}
}

static const char *
output_497 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 530 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md"
{
  operands[2]
    = gen_rtx_REG (CCmode, REGNO (operands[2]) + INTVAL (operands[3]));
  return mips_output_conditional_branch (insn, operands,
					 MIPS_BRANCH ("b%W0", "%2,%1"),
					 MIPS_BRANCH ("b%F0", "%2,%1"));
}
}

static const char *
output_540 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 306 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
	  & ~(unsigned HOST_WIDE_INT) 0xf)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0xf);
      return "shll.ph\t%0,%1,%2";
    }
  return "shllv.ph\t%0,%1,%2";
}
}

static const char *
output_541 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 306 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
	  & ~(unsigned HOST_WIDE_INT) 0x7)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x7);
      return "shll.qb\t%0,%1,%2";
    }
  return "shllv.qb\t%0,%1,%2";
}
}

static const char *
output_542 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 328 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
          & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "shll_s.w\t%0,%1,%2";
    }
  return "shllv_s.w\t%0,%1,%2";
}
}

static const char *
output_543 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 328 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
          & ~(unsigned HOST_WIDE_INT) 0xf)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0xf);
      return "shll_s.ph\t%0,%1,%2";
    }
  return "shllv_s.ph\t%0,%1,%2";
}
}

static const char *
output_544 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 348 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x7)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x7);
      return "shrl.qb\t%0,%1,%2";
    }
  return "shrlv.qb\t%0,%1,%2";
}
}

static const char *
output_545 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 367 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0xf)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0xf);
      return "shra.ph\t%0,%1,%2";
    }
  return "shrav.ph\t%0,%1,%2";
}
}

static const char *
output_546 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 385 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
	  & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "shra_r.w\t%0,%1,%2";
    }
  return "shrav_r.w\t%0,%1,%2";
}
}

static const char *
output_547 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 385 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
	  & ~(unsigned HOST_WIDE_INT) 0xf)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0xf);
      return "shra_r.ph\t%0,%1,%2";
    }
  return "shrav_r.ph\t%0,%1,%2";
}
}

static const char *
output_568 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 690 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[1]) & ~(unsigned HOST_WIDE_INT) 0xff)
	operands[1] = GEN_INT (INTVAL (operands[1]) & 0xff);
      return "repl.qb\t%0,%1";
    }
  return "replv.qb\t%0,%1";
}
}

static const char * const output_569[] = {
  "repl.ph\t%0,%1",
  "replv.ph\t%0,%1",
};

static const char *
output_582 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 812 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extr.w\t%0,%q1,%2";
    }
  return "extrv.w\t%0,%q1,%2";
}
}

static const char *
output_583 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 833 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extr_r.w\t%0,%q1,%2";
    }
  return "extrv_r.w\t%0,%q1,%2";
}
}

static const char *
output_584 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 854 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extr_rs.w\t%0,%q1,%2";
    }
  return "extrv_rs.w\t%0,%q1,%2";
}
}

static const char *
output_585 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 876 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extr_s.h\t%0,%q1,%2";
    }
  return "extrv_s.h\t%0,%q1,%2";
}
}

static const char *
output_586 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 899 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extp\t%0,%q1,%2";
    }
  return "extpv\t%0,%q1,%2";
}
}

static const char *
output_587 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 924 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extpdp\t%0,%q1,%2";
    }
  return "extpdpv\t%0,%q1,%2";
}
}

static const char *
output_588 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 943 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) < -32 || INTVAL (operands[2]) > 31)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x3f);
      return "shilo\t%q0,%2";
    }
  return "shilov\t%q0,%2";
}
}

static const char *
output_601 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 67 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md"
{
  if (INTVAL (operands[3]) & ~(unsigned HOST_WIDE_INT) 31)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 31);
  return "append\t%0,%z2,%3";
}
}

static const char *
output_602 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 82 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md"
{
  if (INTVAL (operands[3]) & ~(unsigned HOST_WIDE_INT) 3)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 3);
  return "balign\t%0,%z2,%3";
}
}

static const char *
output_617 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 290 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md"
{
  if (INTVAL (operands[3]) & ~(unsigned HOST_WIDE_INT) 31)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 31);
  return "precr_sra.ph.w\t%0,%z2,%3";
}
}

static const char *
output_618 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 305 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md"
{
  if (INTVAL (operands[3]) & ~(unsigned HOST_WIDE_INT) 31)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 31);
  return "precr_sra_r.ph.w\t%0,%z2,%3";
}
}

static const char *
output_619 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 320 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md"
{
  if (INTVAL (operands[3]) & ~(unsigned HOST_WIDE_INT) 31)
    operands[2] = GEN_INT (INTVAL (operands[2]) & 31);
  return "prepend\t%0,%z2,%3";
}
}

static const char *
output_620 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 334 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 7)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 7);
      return "shra.qb\t%0,%z1,%2";
    }
  return "shrav.qb\t%0,%z1,%2";
}
}

static const char *
output_621 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 353 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 7)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 7);
      return "shra_r.qb\t%0,%z1,%2";
    }
  return "shrav_r.qb\t%0,%z1,%2";
}
}

static const char *
output_622 (rtx *operands ATTRIBUTE_UNUSED, rtx insn ATTRIBUTE_UNUSED)
{
#line 371 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 15)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 15);
      return "shrl.ph\t%0,%z1,%2";
    }
  return "shrlv.ph\t%0,%z1,%2";
}
}



static const struct insn_operand_data operand_data[] = 
{
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    trap_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "dI",
    SImode,
    0,
    1
  },
  {
    trap_comparison_operator,
    "",
    DImode,
    0,
    0
  },
  {
    reg_or_0_operand,
    "dJ",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "dI",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "d,Q",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "d,Q",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=ks,d,d,d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "ks,ks,0,d,d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "Q,Q,Q,O,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=ks,d,d,d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "ks,ks,0,d,d",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "Q,Q,Q,O,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "d,Q",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0,d,d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "Q,O,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h,h",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=l,X",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=l",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h",
    DImode,
    0,
    0
  },
  {
    scratch_operand,
    "=l",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=l,*d,*d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0,l,*d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h,h,h",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=X,3,l",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=X,X,&d",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0,l",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h,h",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=X,3",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0,l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h,h",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=X,1",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=d,d",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l,*d,*d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0,l,*d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d,d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h,h,h",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=X,1,l",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=X,X,&d",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h,h",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=X,l",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=x",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=l",
    DImode,
    0,
    0
  },
  {
    scratch_operand,
    "=h",
    DImode,
    0,
    0
  },
  {
    scratch_operand,
    "=d",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=l",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=h",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=ka",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=h",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=l",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=h,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=l,l",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=X,h",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=h",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    scratch_operand,
    "=l",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "+l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=h",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    const_1_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    const_1_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    V2SFmode,
    0,
    1
  },
  {
    const_1_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=h",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=h",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "%d,d",
    SImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "d,K",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "%d,d",
    DImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "d,K",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "%0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d,t,t",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "%0,d,d",
    SImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "d,K,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,t,t",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "%0,d,d",
    DImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "d,K,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    const_arith_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    const_arith_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    const_arith_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "d,W",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "d,W",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "d,m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "d,m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "d,m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "d,m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    HImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "d,m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    HImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "0,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "0,m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "0,m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "0,m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "0,m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    HImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "0,m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    scratch_operand,
    "=d",
    DFmode,
    0,
    0
  },
  {
    register_operand,
    "=f",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    scratch_operand,
    "=d",
    SFmode,
    0,
    0
  },
  {
    register_operand,
    "=f",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "+d",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "+d",
    DImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    BLKmode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    BLKmode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    BLKmode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    BLKmode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "=m",
    BLKmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    memory_operand,
    "=m",
    BLKmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    memory_operand,
    "+m",
    BLKmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    memory_operand,
    "+m",
    BLKmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    absolute_symbolic_operand,
    "",
    DImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&d",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    got_disp_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    got_disp_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    got_disp_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    got_disp_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    got_page_ofst_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    got_page_ofst_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    DImode,
    0,
    1
  },
  {
    stack_operand,
    "=m",
    SImode,
    0,
    1
  },
  {
    stack_operand,
    "=m",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,d,d,m,*a,*d,*B*C*D,*B*C*D,*d,*m",
    DImode,
    0,
    1
  },
  {
    move_operand,
    "d,i,m,d,*J*d,*a,*d,*m,*B*C*D,*B*C*D",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,d,d,m,*a,*d,*f,*f,*d,*m",
    DImode,
    0,
    1
  },
  {
    move_operand,
    "d,i,m,d,*J*d,*a,*J*d,*m,*f,*f",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,y,d,d,d,d,m,*d",
    DImode,
    0,
    1
  },
  {
    move_operand,
    "d,d,y,K,N,m,d,*x",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,d,e,d,m,*f,*f,*d,*m,*x,*B*C*D,*B*C*D,*d,*m",
    DImode,
    0,
    1
  },
  {
    move_operand,
    "d,U,T,m,dJ,*d*J,*m,*f,*f,*J*d,*d,*m,*B*C*D,*B*C*D",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,y,d,d,d,d,d,d,m",
    DImode,
    0,
    1
  },
  {
    move_operand,
    "d,d,y,K,N,kf,U,m,d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,d,e,d,m,*f,*f,*d,*m,*d,*z,*a,*d,*B*C*D,*B*C*D,*d,*m",
    SImode,
    0,
    1
  },
  {
    move_operand,
    "d,U,T,m,dJ,*d*J,*m,*f,*f,*z,*d,*J*d,*A,*d,*m,*B*C*D,*B*C*D",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,y,d,d,d,d,d,d,m",
    SImode,
    0,
    1
  },
  {
    move_operand,
    "d,d,y,K,N,kf,U,m,d",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,*d,*d,*m,*d,*f,*f,*f,*m",
    CCmode,
    0,
    1
  },
  {
    general_operand,
    "z,*d,*m,*d,*f,*d,*f,*m,*f",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,d,d,m,*x",
    HImode,
    0,
    1
  },
  {
    move_operand,
    "d,I,m,dJ,*d",
    HImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,y,d,d,d,d,m",
    HImode,
    0,
    1
  },
  {
    move_operand,
    "d,d,y,K,N,m,d",
    HImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,d,d,m,*x",
    QImode,
    0,
    1
  },
  {
    move_operand,
    "d,I,m,dJ,*d",
    QImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,y,d,d,d,d,m",
    QImode,
    0,
    1
  },
  {
    move_operand,
    "d,d,y,K,N,m,d",
    QImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=f,f,f,m,m,*f,*d,*d,*d,*m",
    SFmode,
    0,
    1
  },
  {
    move_operand,
    "f,G,m,f,G,*d,*f,*G*d,*m,*d",
    SFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,d,m",
    SFmode,
    0,
    1
  },
  {
    move_operand,
    "Gd,m,d",
    SFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,y,d,d,m",
    SFmode,
    0,
    1
  },
  {
    move_operand,
    "d,d,y,m,d",
    SFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=f,f,f,m,m,*f,*d,*d,*d,*m",
    DFmode,
    0,
    1
  },
  {
    move_operand,
    "f,G,m,f,G,*d,*f,*d*G,*m,*d",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,d,m,d,f,f",
    DFmode,
    0,
    1
  },
  {
    move_operand,
    "dG,m,dG,f,d,f",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,y,d,d,m",
    DFmode,
    0,
    1
  },
  {
    move_operand,
    "d,d,y,m,d",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,R,f,dR",
    TFmode,
    0,
    1
  },
  {
    move_operand,
    "dGR,dG,dGR,f",
    TFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=f,f,f,m,m,*f,*d,*d,*d,*m",
    V2SFmode,
    0,
    1
  },
  {
    move_operand,
    "f,YG,m,f,YG,*d,*f,*d*YG,*m,*d",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "h,l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "l,h",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "h,l",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "l,h",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    DImode,
    0,
    1
  },
  {
    general_operand,
    "dJ,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    V2SFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    TFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ,m",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0,0",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    DImode,
    0,
    1
  },
  {
    general_operand,
    "dJ,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0,0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    V2SFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0,0",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    TFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ,m",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0,0",
    TFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f,f",
    DFmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f,f",
    DImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,m",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f,f",
    V2SFmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=d,m",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f,f",
    TFmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    DImode,
    0,
    1
  },
  {
    general_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    V2SFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    TFmode,
    0,
    1
  },
  {
    general_operand,
    "dJ",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    TFmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f",
    TFmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    0,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    symbol_ref_operand,
    "",
    SImode,
    0,
    1
  },
  {
    symbol_ref_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    symbol_ref_operand,
    "",
    DImode,
    0,
    1
  },
  {
    symbol_ref_operand,
    "",
    DImode,
    0,
    1
  },
  {
    const_int_operand,
    "I,i",
    VOIDmode,
    0,
    1
  },
  {
    pmode_register_operand,
    "d",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=&d,&d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ,dJ",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=&d,&d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ,dJ",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "+R",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "+R",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=&d,&d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=&d,&d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=&d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "+R",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=&d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "+R",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    SImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "K,d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    DImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "K,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=&d,&d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    SImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "K,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=&d,&d",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "+R,R",
    DImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "K,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "dI",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "dI",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "dI",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0,d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "d,I",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0,d",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "d,I",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0,0",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "d,I",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "z",
    CCmode,
    0,
    1
  },
  {
    order_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    order_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    DImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    register_operand,
    "d,t",
    SImode,
    0,
    1
  },
  {
    pc_or_label_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    pc_or_label_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    register_operand,
    "d,t",
    DImode,
    0,
    1
  },
  {
    pc_or_label_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    pc_or_label_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=t",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=t",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=t",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=t",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "dI",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=t,t",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "d,I",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=t,t",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "d,I",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    sle_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    sle_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=t",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    sle_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=t",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    sle_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    sleu_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    sleu_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=t",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    sleu_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=t",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    sleu_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=z",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "=z",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    pmode_register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&d",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&d",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "r",
    DImode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    DImode,
    0,
    1
  },
  {
    call_insn_operand,
    "j,S",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    call_insn_operand,
    "j,S",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    call_insn_operand,
    "c,S",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    const_call_insn_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    call_insn_operand,
    "cS",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    call_insn_operand,
    "cS",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    const_call_insn_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    call_insn_operand,
    "c,S",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    address_operand,
    "p",
    QImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    VOIDmode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    VOIDmode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    VOIDmode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ,0",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "0,dJ",
    SImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ,0",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "0,dJ",
    DImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ,0",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "0,dJ",
    SImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ,0",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "0,dJ",
    DImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "z,z",
    CCmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ,0",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "0,dJ",
    SImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    CCmode,
    0,
    0
  },
  {
    register_operand,
    "=d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "z,z",
    CCmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ,0",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "0,dJ",
    DImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    CCmode,
    0,
    0
  },
  {
    register_operand,
    "=f,f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    SFmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    DFmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=f,f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    SFmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    DFmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=f,f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "z,z",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    SFmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    CCmode,
    0,
    0
  },
  {
    register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "z,z",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    DFmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    CCmode,
    0,
    0
  },
  {
    consttable_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=f,f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    V2SFmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=f,f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    V2SFmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=f,f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f,0",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "0,f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "z,z",
    CCV2mode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    const_0_or_1_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=z",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=z",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=z",
    CCV4mode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=z",
    CCV2mode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "f",
    V2SFmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "z",
    CCV4mode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "z",
    CCV2mode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    equality_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "z",
    CCV2mode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    V2HImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    V4QImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d,d",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=a",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "=a",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "=a",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    V4QImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    V2HImode,
    0,
    1
  },
  {
    reg_imm10_operand,
    "YB,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "a,a",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=a,a",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0,0",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    const_uimm6_operand,
    "YA",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    const_uimm6_operand,
    "YA",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "I",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V4QImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V2HImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V4QImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V4QImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V4QImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V4QImode,
    0,
    1
  },
  {
    register_operand,
    "=a",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V2HImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V2HImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V2HImode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=a",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V4QImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V2HImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dJ",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    V4QImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG,dYG",
    V4QImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d,d",
    V2HImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "dYG,dYG",
    V2HImode,
    0,
    1
  },
  {
    arith_operand,
    "I,d",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    QQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    QQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    QQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    HQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    HQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    HQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    UQQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UQQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UQQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    UHQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UHQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UHQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    USQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    USQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    USQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    UDQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UDQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UDQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    HAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    HAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    HAmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    SAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SAmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    DAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    DAmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    UHAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UHAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UHAmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    USAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    USAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    USAmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    UDAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UDAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    UDAmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V4UQQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4UQQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V4UQQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2UHQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2UHQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2UHQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2UHAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2UHAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2UHAmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HQmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HAmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HAmode,
    0,
    1
  },
  {
    register_operand,
    "=d",
    V2HQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    V2HQmode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    HQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    HQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    HQmode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    SQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SQmode,
    0,
    1
  },
  {
    scratch_operand,
    "=x",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=a",
    DQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SQmode,
    0,
    1
  },
  {
    register_operand,
    "d",
    SQmode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DQmode,
    0,
    1
  },
  {
    comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arith_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    macc_msac_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "d",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    macc_msac_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "d",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    macc_msac_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "",
    DImode,
    0,
    0
  },
  {
    scratch_operand,
    "",
    DImode,
    0,
    0
  },
  {
    scratch_operand,
    "",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    reg_or_1_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    reg_or_1_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    reg_or_1_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    uns_arith_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "",
    HImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    memory_operand,
    "",
    QImode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    absolute_symbolic_operand,
    "",
    DImode,
    0,
    1
  },
  {
    scratch_operand,
    "d",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    absolute_symbolic_operand,
    "",
    DImode,
    0,
    1
  },
  {
    scratch_operand,
    "",
    DImode,
    0,
    0
  },
  {
    register_operand,
    "=d",
    SImode,
    0,
    1
  },
  {
    absolute_symbolic_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    got_disp_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    got_disp_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    got_disp_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    got_disp_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    got_page_ofst_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    got_page_ofst_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    splittable_const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    splittable_const_int_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    0,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    0,
    "",
    DImode,
    0,
    1
  },
  {
    0,
    "",
    DImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    fcc_reload_operand,
    "=z",
    CCmode,
    0,
    1
  },
  {
    general_operand,
    "",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "=&f",
    TFmode,
    0,
    1
  },
  {
    fcc_reload_operand,
    "=z",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "",
    CCmode,
    0,
    1
  },
  {
    register_operand,
    "=&f",
    TFmode,
    0,
    1
  },
  {
    0,
    "",
    HImode,
    0,
    1
  },
  {
    0,
    "",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "",
    HImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    QImode,
    0,
    1
  },
  {
    0,
    "",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "",
    QImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SFmode,
    0,
    1
  },
  {
    0,
    "",
    SFmode,
    0,
    1
  },
  {
    0,
    "",
    DFmode,
    0,
    1
  },
  {
    0,
    "",
    DFmode,
    0,
    1
  },
  {
    0,
    "",
    TFmode,
    0,
    1
  },
  {
    0,
    "",
    TFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    TFmode,
    0,
    1
  },
  {
    move_operand,
    "",
    TFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    DImode,
    0,
    1
  },
  {
    move_operand,
    "",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    move_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    move_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    V2SFmode,
    0,
    1
  },
  {
    0,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    symbol_ref_operand,
    "",
    SImode,
    0,
    1
  },
  {
    symbol_ref_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    symbol_ref_operand,
    "",
    DImode,
    0,
    1
  },
  {
    symbol_ref_operand,
    "",
    DImode,
    0,
    1
  },
  {
    pmode_register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    pmode_register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    general_operand,
    "",
    BLKmode,
    0,
    1
  },
  {
    general_operand,
    "",
    BLKmode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    arith_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "",
    SImode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    nonmemory_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    nonmemory_operand,
    "",
    DImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    general_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    scratch_operand,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    call_insn_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    call_insn_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    comparison_operator,
    "",
    VOIDmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "",
    SImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    comparison_operator,
    "",
    VOIDmode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "",
    DImode,
    0,
    1
  },
  {
    reg_or_0_operand,
    "",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    comparison_operator,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    comparison_operator,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    small_data_pattern,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    comparison_operator,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    0,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    const_0_or_1_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    CCV4mode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    CCV2mode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
  {
    register_operand,
    "",
    V2SFmode,
    0,
    1
  },
};


#if GCC_VERSION >= 2007
__extension__
#endif

const struct insn_data insn_data[] = 
{
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:734 */
  {
    "trap",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_0 },
#else
    { 0, 0, output_0 },
#endif
    (insn_gen_fn) gen_trap,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:762 */
  {
    "*conditional_trapsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "t%C0\t%z1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[1],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:762 */
  {
    "*conditional_trapdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "t%C0\t%z1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[4],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:779 */
  {
    "addsf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "add.s\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addsf3,
    &operand_data[7],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:779 */
  {
    "adddf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "add.d\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_adddf3,
    &operand_data[10],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:779 */
  {
    "addv2sf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "add.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addv2sf3,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:794 */
  {
    "*addsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_6 },
#else
    { 0, output_6, 0 },
#endif
    0,
    &operand_data[16],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:794 */
  {
    "*adddi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_7 },
#else
    { 0, output_7, 0 },
#endif
    0,
    &operand_data[19],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:805 */
  {
    "*addsi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_8 },
#else
    { 0, output_8, 0 },
#endif
    0,
    &operand_data[22],
    3,
    0,
    5,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:805 */
  {
    "*adddi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_9 },
#else
    { 0, output_9, 0 },
#endif
    0,
    &operand_data[25],
    3,
    0,
    5,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:961 */
  {
    "*addsi3_extended",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_10 },
#else
    { 0, output_10, 0 },
#endif
    0,
    &operand_data[28],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:975 */
  {
    "*addsi3_extended_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[31],
    3,
    0,
    3,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:997 */
  {
    "subsf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sub.s\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subsf3,
    &operand_data[7],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:997 */
  {
    "subdf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sub.d\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subdf3,
    &operand_data[10],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:997 */
  {
    "subv2sf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sub.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subv2sf3,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1006 */
  {
    "subsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subsi3,
    &operand_data[34],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1006 */
  {
    "subdi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsubu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subdi3,
    &operand_data[37],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1015 */
  {
    "*subsi3_extended",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[40],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1040 */
  {
    "*mulsf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mul.s\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[7],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1040 */
  {
    "*muldf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mul.d\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[10],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1053 */
  {
    "*mulsf3_r4300",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mul.s\t%0,%1,%2\n\tnop",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[7],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1053 */
  {
    "*muldf3_r4300",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mul.d\t%0,%1,%2\n\tnop",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[10],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1063 */
  {
    "mulv2sf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mul.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mulv2sf3,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1146 */
  {
    "mulsi3_mult3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_23 },
#else
    { 0, 0, output_23 },
#endif
    (insn_gen_fn) gen_mulsi3_mult3,
    &operand_data[43],
    5,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1189 */
  {
    "mulsi3_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mult\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mulsi3_internal,
    &operand_data[48],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1189 */
  {
    "muldi3_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dmult\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_muldi3_internal,
    &operand_data[52],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1199 */
  {
    "mulsi3_r4000",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mult\t%1,%2\n\tmflo\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mulsi3_r4000,
    &operand_data[56],
    5,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1199 */
  {
    "muldi3_r4000",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dmult\t%1,%2\n\tmflo\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_muldi3_r4000,
    &operand_data[61],
    5,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1254 */
  {
    "*mul_acc_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_28 },
#else
    { 0, 0, output_28 },
#endif
    0,
    &operand_data[66],
    7,
    0,
    3,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1317 */
  {
    "*macc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_29 },
#else
    { 0, 0, output_29 },
#endif
    0,
    &operand_data[73],
    6,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1339 */
  {
    "*msac",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_30 },
#else
    { 0, 0, output_30 },
#endif
    0,
    &operand_data[79],
    6,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1359 */
  {
    "*msac_using_macc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[79],
    7,
    0,
    2,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1385 */
  {
    "*macc2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "macc\t%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[86],
    5,
    4,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1400 */
  {
    "*msac2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "msac\t%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[86],
    5,
    4,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1515 */
  {
    "*mul_sub_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_34 },
#else
    { 0, output_34, 0 },
#endif
    0,
    &operand_data[91],
    7,
    0,
    3,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1572 */
  {
    "*muls",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_35 },
#else
    { 0, output_35, 0 },
#endif
    0,
    &operand_data[98],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1609 */
  {
    "mulsidi3_32bit_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mult\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mulsidi3_32bit_internal,
    &operand_data[103],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1609 */
  {
    "umulsidi3_32bit_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "multu\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_umulsidi3_32bit_internal,
    &operand_data[103],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1618 */
  {
    "mulsidi3_32bit_r4000",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mult\t%1,%2\n\tmflo\t%L0;mfhi\t%M0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mulsidi3_32bit_r4000,
    &operand_data[106],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1618 */
  {
    "umulsidi3_32bit_r4000",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "multu\t%1,%2\n\tmflo\t%L0;mfhi\t%M0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_umulsidi3_32bit_r4000,
    &operand_data[106],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1629 */
  {
    "*mulsidi3_64bit",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[110],
    6,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1629 */
  {
    "*umulsidi3_64bit",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[110],
    6,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1676 */
  {
    "*mulsidi3_64bit_parts",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mult\t%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[116],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1676 */
  {
    "*umulsidi3_64bit_parts",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "multu\t%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[116],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1692 */
  {
    "*muls_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "muls\t$0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[103],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1692 */
  {
    "*mulsu_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulsu\t$0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[103],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1703 */
  {
    "msubsidi4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_46 },
#else
    { 0, 0, output_46 },
#endif
    (insn_gen_fn) gen_msubsidi4,
    &operand_data[120],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1703 */
  {
    "umsubsidi4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_47 },
#else
    { 0, 0, output_47 },
#endif
    (insn_gen_fn) gen_umsubsidi4,
    &operand_data[120],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1743 */
  {
    "smulsi3_highpart_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mult\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_smulsi3_highpart_internal,
    &operand_data[124],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1743 */
  {
    "umulsi3_highpart_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "multu\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_umulsi3_highpart_internal,
    &operand_data[124],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1756 */
  {
    "smulsi3_highpart_mulhi_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_50 },
#else
    { 0, output_50, 0 },
#endif
    (insn_gen_fn) gen_smulsi3_highpart_mulhi_internal,
    &operand_data[128],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1756 */
  {
    "umulsi3_highpart_mulhi_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_51 },
#else
    { 0, output_51, 0 },
#endif
    (insn_gen_fn) gen_umulsi3_highpart_mulhi_internal,
    &operand_data[128],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1773 */
  {
    "*smulsi3_highpart_neg_mulhi_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_52 },
#else
    { 0, output_52, 0 },
#endif
    0,
    &operand_data[128],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1773 */
  {
    "*umulsi3_highpart_neg_mulhi_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_53 },
#else
    { 0, output_53, 0 },
#endif
    0,
    &operand_data[128],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1794 */
  {
    "smuldi3_highpart",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dmult\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_smuldi3_highpart,
    &operand_data[133],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1794 */
  {
    "umuldi3_highpart",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dmultu\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_umuldi3_highpart,
    &operand_data[133],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1812 */
  {
    "madsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mad\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_madsi,
    &operand_data[137],
    4,
    1,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1823 */
  {
    "maddsidi4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_57 },
#else
    { 0, 0, output_57 },
#endif
    (insn_gen_fn) gen_maddsidi4,
    &operand_data[120],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1823 */
  {
    "umaddsidi4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_58 },
#else
    { 0, 0, output_58 },
#endif
    (insn_gen_fn) gen_umaddsidi4,
    &operand_data[120],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1847 */
  {
    "*maddsf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "madd.s\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[141],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1847 */
  {
    "*madddf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "madd.d\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[145],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1847 */
  {
    "*maddv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "madd.ps\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[149],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1857 */
  {
    "*msubsf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "msub.s\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[141],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1857 */
  {
    "*msubdf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "msub.d\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[145],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1857 */
  {
    "*msubv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "msub.ps\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[149],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1867 */
  {
    "*nmaddsf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmadd.s\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[141],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1867 */
  {
    "*nmadddf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmadd.d\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[145],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1867 */
  {
    "*nmaddv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmadd.ps\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[149],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1881 */
  {
    "*nmaddsf_fastmath",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmadd.s\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[141],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1881 */
  {
    "*nmadddf_fastmath",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmadd.d\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[145],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1881 */
  {
    "*nmaddv2sf_fastmath",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmadd.ps\t%0,%3,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[149],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1895 */
  {
    "*nmsubsf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmsub.s\t%0,%1,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[141],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1895 */
  {
    "*nmsubdf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmsub.d\t%0,%1,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[145],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1895 */
  {
    "*nmsubv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmsub.ps\t%0,%1,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[149],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1909 */
  {
    "*nmsubsf_fastmath",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmsub.s\t%0,%1,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[141],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1909 */
  {
    "*nmsubdf_fastmath",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmsub.d\t%0,%1,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[145],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1909 */
  {
    "*nmsubv2sf_fastmath",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nmsub.ps\t%0,%1,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[149],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1954 */
  {
    "*divsf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_77 },
#else
    { 0, 0, output_77 },
#endif
    0,
    &operand_data[7],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1954 */
  {
    "*divdf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_78 },
#else
    { 0, 0, output_78 },
#endif
    0,
    &operand_data[10],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1954 */
  {
    "*divv2sf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_79 },
#else
    { 0, 0, output_79 },
#endif
    0,
    &operand_data[13],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1972 */
  {
    "*recipsf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_80 },
#else
    { 0, 0, output_80 },
#endif
    0,
    &operand_data[153],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1972 */
  {
    "*recipdf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_81 },
#else
    { 0, 0, output_81 },
#endif
    0,
    &operand_data[156],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1972 */
  {
    "*recipv2sf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_82 },
#else
    { 0, 0, output_82 },
#endif
    0,
    &operand_data[159],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1992 */
  {
    "divmodsi4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_83 },
#else
    { 0, 0, output_83 },
#endif
    (insn_gen_fn) gen_divmodsi4,
    &operand_data[162],
    4,
    2,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1992 */
  {
    "divmoddi4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_84 },
#else
    { 0, 0, output_84 },
#endif
    (insn_gen_fn) gen_divmoddi4,
    &operand_data[166],
    4,
    2,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2004 */
  {
    "udivmodsi4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_85 },
#else
    { 0, 0, output_85 },
#endif
    (insn_gen_fn) gen_udivmodsi4,
    &operand_data[162],
    4,
    2,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2004 */
  {
    "udivmoddi4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_86 },
#else
    { 0, 0, output_86 },
#endif
    (insn_gen_fn) gen_udivmoddi4,
    &operand_data[166],
    4,
    2,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2026 */
  {
    "sqrtsf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_87 },
#else
    { 0, 0, output_87 },
#endif
    (insn_gen_fn) gen_sqrtsf2,
    &operand_data[7],
    2,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2026 */
  {
    "sqrtdf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_88 },
#else
    { 0, 0, output_88 },
#endif
    (insn_gen_fn) gen_sqrtdf2,
    &operand_data[10],
    2,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2026 */
  {
    "sqrtv2sf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_89 },
#else
    { 0, 0, output_89 },
#endif
    (insn_gen_fn) gen_sqrtv2sf2,
    &operand_data[13],
    2,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2043 */
  {
    "*rsqrtsfa",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_90 },
#else
    { 0, 0, output_90 },
#endif
    0,
    &operand_data[153],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2043 */
  {
    "*rsqrtdfa",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_91 },
#else
    { 0, 0, output_91 },
#endif
    0,
    &operand_data[156],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2043 */
  {
    "*rsqrtv2sfa",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_92 },
#else
    { 0, 0, output_92 },
#endif
    0,
    &operand_data[159],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2061 */
  {
    "*rsqrtsfb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_93 },
#else
    { 0, 0, output_93 },
#endif
    0,
    &operand_data[153],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2061 */
  {
    "*rsqrtdfb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_94 },
#else
    { 0, 0, output_94 },
#endif
    0,
    &operand_data[156],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2061 */
  {
    "*rsqrtv2sfb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_95 },
#else
    { 0, 0, output_95 },
#endif
    0,
    &operand_data[159],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2093 */
  {
    "abssf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "abs.s\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_abssf2,
    &operand_data[7],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2093 */
  {
    "absdf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "abs.d\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_absdf2,
    &operand_data[10],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2093 */
  {
    "absv2sf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "abs.ps\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_absv2sf2,
    &operand_data[13],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2109 */
  {
    "clzsi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "clz\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_clzsi2,
    &operand_data[34],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2109 */
  {
    "clzdi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dclz\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_clzdi2,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2124 */
  {
    "negsi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_101 },
#else
    { 0, 0, output_101 },
#endif
    (insn_gen_fn) gen_negsi2,
    &operand_data[34],
    2,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2137 */
  {
    "negdi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsubu\t%0,%.,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_negdi2,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2149 */
  {
    "negsf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "neg.s\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_negsf2,
    &operand_data[7],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2149 */
  {
    "negdf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "neg.d\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_negdf2,
    &operand_data[10],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2149 */
  {
    "negv2sf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "neg.ps\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_negv2sf2,
    &operand_data[13],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2157 */
  {
    "one_cmplsi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_106 },
#else
    { 0, 0, output_106 },
#endif
    (insn_gen_fn) gen_one_cmplsi2,
    &operand_data[34],
    2,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2157 */
  {
    "one_cmpldi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_107 },
#else
    { 0, 0, output_107 },
#endif
    (insn_gen_fn) gen_one_cmpldi2,
    &operand_data[37],
    2,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2191 */
  {
    "*andsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_108 },
#else
    { 0, output_108, 0 },
#endif
    0,
    &operand_data[170],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2191 */
  {
    "*anddi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_109 },
#else
    { 0, output_109, 0 },
#endif
    0,
    &operand_data[173],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2202 */
  {
    "*andsi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "and\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[176],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2202 */
  {
    "*anddi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "and\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[179],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2221 */
  {
    "*iorsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_112 },
#else
    { 0, output_112, 0 },
#endif
    0,
    &operand_data[170],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2221 */
  {
    "*iordi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_113 },
#else
    { 0, output_113, 0 },
#endif
    0,
    &operand_data[173],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2232 */
  {
    "*iorsi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "or\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[176],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2232 */
  {
    "*iordi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "or\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[179],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2248 */
  {
    "*mips.md:2248",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_116 },
#else
    { 0, output_116, 0 },
#endif
    0,
    &operand_data[170],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2248 */
  {
    "*mips.md:2248",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_117 },
#else
    { 0, output_117, 0 },
#endif
    0,
    &operand_data[173],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2259 */
  {
    "*mips.md:2259",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_118 },
#else
    { 0, output_118, 0 },
#endif
    0,
    &operand_data[182],
    3,
    0,
    3,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2259 */
  {
    "*mips.md:2259",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_119 },
#else
    { 0, output_119, 0 },
#endif
    0,
    &operand_data[185],
    3,
    0,
    3,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2277 */
  {
    "*norsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nor\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[34],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2277 */
  {
    "*nordi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "nor\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2295 */
  {
    "truncdfsf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cvt.s.d\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_truncdfsf2,
    &operand_data[188],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2317 */
  {
    "truncdisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_123 },
#else
    { 0, output_123, 0 },
#endif
    (insn_gen_fn) gen_truncdisi2,
    &operand_data[190],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2328 */
  {
    "truncdihi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_124 },
#else
    { 0, output_124, 0 },
#endif
    (insn_gen_fn) gen_truncdihi2,
    &operand_data[192],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2339 */
  {
    "truncdiqi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_125 },
#else
    { 0, output_125, 0 },
#endif
    (insn_gen_fn) gen_truncdiqi2,
    &operand_data[194],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2352 */
  {
    "*mips.md:2352",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsra\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[196],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2352 */
  {
    "*mips.md:2352",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsra\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[199],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2352 */
  {
    "*mips.md:2352",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsra\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[202],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2362 */
  {
    "*mips.md:2362",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsra\t%0,%1,32",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[196],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2362 */
  {
    "*mips.md:2362",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsra\t%0,%1,32",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[199],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2362 */
  {
    "*mips.md:2362",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsra\t%0,%1,32",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[202],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2376 */
  {
    "*extenddi_truncateqi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2376 */
  {
    "*extenddi_truncatehi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2394 */
  {
    "*extendsi_truncateqi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[202],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2394 */
  {
    "*extendsi_truncatehi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[202],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2414 */
  {
    "*zero_extendsi_trunchi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "andi\t%0,%1,0xffff",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[202],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2414 */
  {
    "*zero_extenddi_trunchi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "andi\t%0,%1,0xffff",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2423 */
  {
    "*zero_extendsi_truncqi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "andi\t%0,%1,0xff",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[202],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2423 */
  {
    "*zero_extenddi_truncqi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "andi\t%0,%1,0xff",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2432 */
  {
    "*mips.md:2432",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "andi\t%0,%1,0xff",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[199],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2450 */
  {
    "zero_extendsidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_141 },
#else
    { 0, output_141, 0 },
#endif
    (insn_gen_fn) gen_zero_extendsidi2,
    &operand_data[205],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2470 */
  {
    "*clear_upper32",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_142 },
#else
    { 0, 0, output_142 },
#endif
    0,
    &operand_data[207],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2508 */
  {
    "*zero_extendqisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_143 },
#else
    { 0, output_143, 0 },
#endif
    0,
    &operand_data[209],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2508 */
  {
    "*zero_extendqidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_144 },
#else
    { 0, output_144, 0 },
#endif
    0,
    &operand_data[211],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2508 */
  {
    "*zero_extendhisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_145 },
#else
    { 0, output_145, 0 },
#endif
    0,
    &operand_data[213],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2508 */
  {
    "*zero_extendhidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_146 },
#else
    { 0, output_146, 0 },
#endif
    0,
    &operand_data[215],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2519 */
  {
    "*zero_extendqisi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "zeb\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[217],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2519 */
  {
    "*zero_extendqidi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "zeb\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[219],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2519 */
  {
    "*zero_extendhisi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "zeh\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[221],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2519 */
  {
    "*zero_extendhidi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "zeh\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[223],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2527 */
  {
    "*zero_extendqisi2_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lbu\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[225],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2527 */
  {
    "*zero_extendqidi2_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lbu\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[227],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2527 */
  {
    "*zero_extendhisi2_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lhu\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[229],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2527 */
  {
    "*zero_extendhidi2_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lhu\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[231],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2548 */
  {
    "*zero_extendqihi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_155 },
#else
    { 0, output_155, 0 },
#endif
    0,
    &operand_data[233],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2558 */
  {
    "*zero_extendqihi2_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lbu\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[235],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2584 */
  {
    "extendsidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_157 },
#else
    { 0, output_157, 0 },
#endif
    (insn_gen_fn) gen_extendsidi2,
    &operand_data[237],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2605 */
  {
    "*extendqisi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_158 },
#else
    { 0, output_158, 0 },
#endif
    0,
    &operand_data[239],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2605 */
  {
    "*extendqidi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_159 },
#else
    { 0, output_159, 0 },
#endif
    0,
    &operand_data[241],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2605 */
  {
    "*extendhisi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_160 },
#else
    { 0, output_160, 0 },
#endif
    0,
    &operand_data[243],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2605 */
  {
    "*extendhidi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_161 },
#else
    { 0, output_161, 0 },
#endif
    0,
    &operand_data[245],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
  {
    "*extendqisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_162 },
#else
    { 0, output_162, 0 },
#endif
    0,
    &operand_data[209],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
  {
    "*extendqidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_163 },
#else
    { 0, output_163, 0 },
#endif
    0,
    &operand_data[211],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
  {
    "*extendhisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_164 },
#else
    { 0, output_164, 0 },
#endif
    0,
    &operand_data[213],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
  {
    "*extendhidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_165 },
#else
    { 0, output_165, 0 },
#endif
    0,
    &operand_data[215],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2635 */
  {
    "*extendqisi2_seb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_166 },
#else
    { 0, output_166, 0 },
#endif
    0,
    &operand_data[209],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2635 */
  {
    "*extendqidi2_seb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_167 },
#else
    { 0, output_167, 0 },
#endif
    0,
    &operand_data[211],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2635 */
  {
    "*extendhisi2_seh",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_168 },
#else
    { 0, output_168, 0 },
#endif
    0,
    &operand_data[213],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2635 */
  {
    "*extendhidi2_seh",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_169 },
#else
    { 0, output_169, 0 },
#endif
    0,
    &operand_data[215],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2651 */
  {
    "*extendqihi2_mips16e",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_170 },
#else
    { 0, output_170, 0 },
#endif
    0,
    &operand_data[247],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2661 */
  {
    "*extendqihi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_171 },
#else
    { 0, output_171, 0 },
#endif
    0,
    &operand_data[233],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2682 */
  {
    "*extendqihi2_seb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_172 },
#else
    { 0, output_172, 0 },
#endif
    0,
    &operand_data[233],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2693 */
  {
    "extendsfdf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cvt.d.s\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_extendsfdf2,
    &operand_data[249],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2721 */
  {
    "fix_truncdfsi2_insn",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "trunc.w.d %0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_fix_truncdfsi2_insn,
    &operand_data[251],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2731 */
  {
    "fix_truncdfsi2_macro",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_175 },
#else
    { 0, 0, output_175 },
#endif
    (insn_gen_fn) gen_fix_truncdfsi2_macro,
    &operand_data[251],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2759 */
  {
    "fix_truncsfsi2_insn",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "trunc.w.s %0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_fix_truncsfsi2_insn,
    &operand_data[254],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2769 */
  {
    "fix_truncsfsi2_macro",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_177 },
#else
    { 0, 0, output_177 },
#endif
    (insn_gen_fn) gen_fix_truncsfsi2_macro,
    &operand_data[254],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2786 */
  {
    "fix_truncdfdi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "trunc.l.d %0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_fix_truncdfdi2,
    &operand_data[257],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2797 */
  {
    "fix_truncsfdi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "trunc.l.s %0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_fix_truncsfdi2,
    &operand_data[259],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2808 */
  {
    "floatsidf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cvt.d.w\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_floatsidf2,
    &operand_data[261],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2819 */
  {
    "floatdidf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cvt.d.l\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_floatdidf2,
    &operand_data[263],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2830 */
  {
    "floatsisf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cvt.s.w\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_floatsisf2,
    &operand_data[265],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2841 */
  {
    "floatdisf2",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cvt.s.l\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_floatdisf2,
    &operand_data[267],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3072 */
  {
    "extzvsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ext\t%0,%1,%3,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_extzvsi,
    &operand_data[269],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3072 */
  {
    "extzvdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dext\t%0,%1,%3,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_extzvdi,
    &operand_data[273],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3110 */
  {
    "insvsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ins\t%0,%z3,%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_insvsi,
    &operand_data[277],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3110 */
  {
    "insvdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dins\t%0,%z3,%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_insvdi,
    &operand_data[281],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3133 */
  {
    "mov_lwl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lwl\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mov_lwl,
    &operand_data[285],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3133 */
  {
    "mov_ldl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ldl\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mov_ldl,
    &operand_data[288],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3143 */
  {
    "mov_lwr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lwr\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mov_lwr,
    &operand_data[291],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3143 */
  {
    "mov_ldr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ldr\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mov_ldr,
    &operand_data[295],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3154 */
  {
    "mov_swl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "swl\t%z1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mov_swl,
    &operand_data[299],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3154 */
  {
    "mov_sdl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sdl\t%z1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mov_sdl,
    &operand_data[302],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3164 */
  {
    "mov_swr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "swr\t%z1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mov_swr,
    &operand_data[305],
    3,
    1,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3164 */
  {
    "mov_sdr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sdr\t%z1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mov_sdr,
    &operand_data[308],
    3,
    1,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3190 */
  {
    "*lea_high64",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[311],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3243 */
  {
    "*lea64",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[311],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3280 */
  {
    "*xgot_hisi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[314],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3280 */
  {
    "*xgot_hidi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[316],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3295 */
  {
    "*xgot_losi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[318],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3295 */
  {
    "*xgot_lodi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[321],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3310 */
  {
    "*got_dispsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[314],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3310 */
  {
    "*got_dispdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[316],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3327 */
  {
    "*got_pagesi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[324],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3327 */
  {
    "*got_pagedi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[326],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3346 */
  {
    "load_gotsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lw\t%0,%R2(%1)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_load_gotsi,
    &operand_data[328],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3346 */
  {
    "load_gotdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ld\t%0,%R2(%1)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_load_gotdi,
    &operand_data[331],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3361 */
  {
    "*lowsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addiu\t%0,%1,%R2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[328],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3361 */
  {
    "*lowdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "daddiu\t%0,%1,%R2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[331],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3370 */
  {
    "*lowsi_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addiu\t%0,%R2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[334],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3370 */
  {
    "*lowdi_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "daddiu\t%0,%R2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[337],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3424 */
  {
    "*movsi_ra",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sw\t$31,%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[340],
    1,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3424 */
  {
    "*movdi_ra",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sd\t$31,%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[341],
    1,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3432 */
  {
    "*movdi_32bit",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_214 },
#else
    { 0, 0, output_214 },
#endif
    0,
    &operand_data[342],
    2,
    0,
    10,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3443 */
  {
    "*movdi_gp32_fp64",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_215 },
#else
    { 0, 0, output_215 },
#endif
    0,
    &operand_data[344],
    2,
    0,
    10,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3454 */
  {
    "*movdi_32bit_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_216 },
#else
    { 0, 0, output_216 },
#endif
    0,
    &operand_data[346],
    2,
    0,
    8,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3465 */
  {
    "*movdi_64bit",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_217 },
#else
    { 0, 0, output_217 },
#endif
    0,
    &operand_data[348],
    2,
    0,
    14,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3476 */
  {
    "*movdi_64bit_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_218 },
#else
    { 0, 0, output_218 },
#endif
    0,
    &operand_data[350],
    2,
    0,
    9,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3562 */
  {
    "*movsi_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_219 },
#else
    { 0, 0, output_219 },
#endif
    0,
    &operand_data[352],
    2,
    0,
    17,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3573 */
  {
    "*movsi_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_220 },
#else
    { 0, 0, output_220 },
#endif
    0,
    &operand_data[354],
    2,
    0,
    9,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3666 */
  {
    "movcc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_221 },
#else
    { 0, 0, output_221 },
#endif
    (insn_gen_fn) gen_movcc,
    &operand_data[356],
    2,
    0,
    9,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3722 */
  {
    "*lwxc1_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lwxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[358],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3722 */
  {
    "*ldxc1_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ldxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[361],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3722 */
  {
    "*ldxc1_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ldxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[364],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3722 */
  {
    "*lwxc1_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lwxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[367],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3722 */
  {
    "*ldxc1_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ldxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[370],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3722 */
  {
    "*ldxc1_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ldxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[373],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3731 */
  {
    "*swxc1_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "swxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[376],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3731 */
  {
    "*sdxc1_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sdxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[379],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3731 */
  {
    "*sdxc1_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sdxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[382],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3731 */
  {
    "*swxc1_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "swxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[385],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3731 */
  {
    "*sdxc1_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sdxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[388],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3731 */
  {
    "*sdxc1_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sdxc1\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[391],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3744 */
  {
    "*lwxs",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lwxs\t%0,%1(%2)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[34],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3771 */
  {
    "*movhi_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_235 },
#else
    { 0, output_235, 0 },
#endif
    0,
    &operand_data[394],
    2,
    0,
    5,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3787 */
  {
    "*movhi_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_236 },
#else
    { 0, output_236, 0 },
#endif
    0,
    &operand_data[396],
    2,
    0,
    7,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3875 */
  {
    "*movqi_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_237 },
#else
    { 0, output_237, 0 },
#endif
    0,
    &operand_data[398],
    2,
    0,
    5,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3891 */
  {
    "*movqi_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_238 },
#else
    { 0, output_238, 0 },
#endif
    0,
    &operand_data[400],
    2,
    0,
    7,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3950 */
  {
    "*movsf_hardfloat",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_239 },
#else
    { 0, 0, output_239 },
#endif
    0,
    &operand_data[402],
    2,
    0,
    10,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3961 */
  {
    "*movsf_softfloat",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_240 },
#else
    { 0, 0, output_240 },
#endif
    0,
    &operand_data[404],
    2,
    0,
    3,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3972 */
  {
    "*movsf_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_241 },
#else
    { 0, 0, output_241 },
#endif
    0,
    &operand_data[406],
    2,
    0,
    5,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3995 */
  {
    "*movdf_hardfloat_64bit",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_242 },
#else
    { 0, 0, output_242 },
#endif
    0,
    &operand_data[408],
    2,
    0,
    10,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4007 */
  {
    "*movdf_hardfloat_32bit",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_243 },
#else
    { 0, 0, output_243 },
#endif
    0,
    &operand_data[408],
    2,
    0,
    10,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4018 */
  {
    "*movdf_softfloat",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_244 },
#else
    { 0, 0, output_244 },
#endif
    0,
    &operand_data[410],
    2,
    0,
    6,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4029 */
  {
    "*movdf_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_245 },
#else
    { 0, 0, output_245 },
#endif
    0,
    &operand_data[412],
    2,
    0,
    5,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4052 */
  {
    "*movtf_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[414],
    2,
    0,
    4,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4103 */
  {
    "movv2sf_hardfloat_64bit",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_247 },
#else
    { 0, 0, output_247 },
#endif
    (insn_gen_fn) gen_movv2sf_hardfloat_64bit,
    &operand_data[416],
    2,
    0,
    10,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4116 */
  {
    "movv2sf_hardfloat_32bit",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_248 },
#else
    { 0, 0, output_248 },
#endif
    (insn_gen_fn) gen_movv2sf_hardfloat_32bit,
    &operand_data[416],
    2,
    0,
    10,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4146 */
  {
    "*mfhilo_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mf%1\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[418],
    3,
    0,
    2,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4146 */
  {
    "*mfhilo_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mf%1\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[421],
    3,
    0,
    2,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4156 */
  {
    "*mfhilo_si_macc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_251 },
#else
    { 0, output_251, 0 },
#endif
    0,
    &operand_data[418],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4156 */
  {
    "*mfhilo_di_macc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_252 },
#else
    { 0, output_252, 0 },
#endif
    0,
    &operand_data[421],
    3,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4204 */
  {
    "load_lowdf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_253 },
#else
    { 0, 0, output_253 },
#endif
    (insn_gen_fn) gen_load_lowdf,
    &operand_data[424],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4204 */
  {
    "load_lowdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_254 },
#else
    { 0, 0, output_254 },
#endif
    (insn_gen_fn) gen_load_lowdi,
    &operand_data[426],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4204 */
  {
    "load_lowv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_255 },
#else
    { 0, 0, output_255 },
#endif
    (insn_gen_fn) gen_load_lowv2sf,
    &operand_data[428],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4204 */
  {
    "load_lowtf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_256 },
#else
    { 0, 0, output_256 },
#endif
    (insn_gen_fn) gen_load_lowtf,
    &operand_data[430],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4218 */
  {
    "load_highdf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_257 },
#else
    { 0, 0, output_257 },
#endif
    (insn_gen_fn) gen_load_highdf,
    &operand_data[432],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4218 */
  {
    "load_highdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_258 },
#else
    { 0, 0, output_258 },
#endif
    (insn_gen_fn) gen_load_highdi,
    &operand_data[435],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4218 */
  {
    "load_highv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_259 },
#else
    { 0, 0, output_259 },
#endif
    (insn_gen_fn) gen_load_highv2sf,
    &operand_data[438],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4218 */
  {
    "load_hightf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_260 },
#else
    { 0, 0, output_260 },
#endif
    (insn_gen_fn) gen_load_hightf,
    &operand_data[441],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4233 */
  {
    "store_worddf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_261 },
#else
    { 0, 0, output_261 },
#endif
    (insn_gen_fn) gen_store_worddf,
    &operand_data[444],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4233 */
  {
    "store_worddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_262 },
#else
    { 0, 0, output_262 },
#endif
    (insn_gen_fn) gen_store_worddi,
    &operand_data[447],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4233 */
  {
    "store_wordv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_263 },
#else
    { 0, 0, output_263 },
#endif
    (insn_gen_fn) gen_store_wordv2sf,
    &operand_data[450],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4233 */
  {
    "store_wordtf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_264 },
#else
    { 0, 0, output_264 },
#endif
    (insn_gen_fn) gen_store_wordtf,
    &operand_data[453],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4248 */
  {
    "mthc1df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mthc1\t%z1,%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mthc1df,
    &operand_data[456],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4248 */
  {
    "mthc1di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mthc1\t%z1,%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mthc1di,
    &operand_data[459],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4248 */
  {
    "mthc1v2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mthc1\t%z1,%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mthc1v2sf,
    &operand_data[462],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4248 */
  {
    "mthc1tf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mthc1\t%z1,%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mthc1tf,
    &operand_data[465],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4259 */
  {
    "mfhc1df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mfhc1\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mfhc1df,
    &operand_data[468],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4259 */
  {
    "mfhc1di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mfhc1\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mfhc1di,
    &operand_data[470],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4259 */
  {
    "mfhc1v2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mfhc1\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mfhc1v2sf,
    &operand_data[472],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4259 */
  {
    "mfhc1tf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mfhc1\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mfhc1tf,
    &operand_data[474],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4276 */
  {
    "loadgp_newabi_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_loadgp_newabi_si,
    &operand_data[476],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4276 */
  {
    "loadgp_newabi_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_loadgp_newabi_di,
    &operand_data[479],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4295 */
  {
    "loadgp_absolute_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_loadgp_absolute_si,
    &operand_data[476],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4295 */
  {
    "loadgp_absolute_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_loadgp_absolute_di,
    &operand_data[479],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4311 */
  {
    "loadgp_blockage",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_loadgp_blockage,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4321 */
  {
    "loadgp_rtp_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_loadgp_rtp_si,
    &operand_data[482],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4321 */
  {
    "loadgp_rtp_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_loadgp_rtp_di,
    &operand_data[485],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4343 */
  {
    "cprestore",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_280 },
#else
    { 0, 0, output_280 },
#endif
    (insn_gen_fn) gen_cprestore,
    &operand_data[488],
    1,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4380 */
  {
    "sync",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%|sync%-",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sync,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4385 */
  {
    "synci",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "synci\t0(%0)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_synci,
    &operand_data[489],
    1,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4391 */
  {
    "rdhwr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "rdhwr\t%0,$%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_rdhwr,
    &operand_data[490],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4398 */
  {
    "clear_hazard",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_284 },
#else
    { 0, 0, output_284 },
#endif
    (insn_gen_fn) gen_clear_hazard,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4413 */
  {
    "memory_barrier",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%|sync%-",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_memory_barrier,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4419 */
  {
    "sync_compare_and_swapsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_286 },
#else
    { 0, 0, output_286 },
#endif
    (insn_gen_fn) gen_sync_compare_and_swapsi,
    &operand_data[492],
    4,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4419 */
  {
    "sync_compare_and_swapdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_287 },
#else
    { 0, 0, output_287 },
#endif
    (insn_gen_fn) gen_sync_compare_and_swapdi,
    &operand_data[496],
    4,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4435 */
  {
    "sync_addsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_288 },
#else
    { 0, 0, output_288 },
#endif
    (insn_gen_fn) gen_sync_addsi,
    &operand_data[500],
    2,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4435 */
  {
    "sync_adddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_289 },
#else
    { 0, 0, output_289 },
#endif
    (insn_gen_fn) gen_sync_adddi,
    &operand_data[502],
    2,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4450 */
  {
    "sync_subsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_290 },
#else
    { 0, 0, output_290 },
#endif
    (insn_gen_fn) gen_sync_subsi,
    &operand_data[504],
    2,
    1,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4450 */
  {
    "sync_subdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_291 },
#else
    { 0, 0, output_291 },
#endif
    (insn_gen_fn) gen_sync_subdi,
    &operand_data[506],
    2,
    1,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4462 */
  {
    "sync_old_addsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_292 },
#else
    { 0, 0, output_292 },
#endif
    (insn_gen_fn) gen_sync_old_addsi,
    &operand_data[508],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4462 */
  {
    "sync_old_adddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_293 },
#else
    { 0, 0, output_293 },
#endif
    (insn_gen_fn) gen_sync_old_adddi,
    &operand_data[511],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4479 */
  {
    "sync_old_subsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_294 },
#else
    { 0, 0, output_294 },
#endif
    (insn_gen_fn) gen_sync_old_subsi,
    &operand_data[514],
    3,
    2,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4479 */
  {
    "sync_old_subdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_295 },
#else
    { 0, 0, output_295 },
#endif
    (insn_gen_fn) gen_sync_old_subdi,
    &operand_data[517],
    3,
    2,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4493 */
  {
    "sync_new_addsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_296 },
#else
    { 0, 0, output_296 },
#endif
    (insn_gen_fn) gen_sync_new_addsi,
    &operand_data[508],
    3,
    3,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4493 */
  {
    "sync_new_adddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_297 },
#else
    { 0, 0, output_297 },
#endif
    (insn_gen_fn) gen_sync_new_adddi,
    &operand_data[511],
    3,
    3,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4510 */
  {
    "sync_new_subsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_298 },
#else
    { 0, 0, output_298 },
#endif
    (insn_gen_fn) gen_sync_new_subsi,
    &operand_data[514],
    3,
    3,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4510 */
  {
    "sync_new_subdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_299 },
#else
    { 0, 0, output_299 },
#endif
    (insn_gen_fn) gen_sync_new_subdi,
    &operand_data[517],
    3,
    3,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
  {
    "sync_iorsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_300 },
#else
    { 0, 0, output_300 },
#endif
    (insn_gen_fn) gen_sync_iorsi,
    &operand_data[520],
    2,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
  {
    "sync_xorsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_301 },
#else
    { 0, 0, output_301 },
#endif
    (insn_gen_fn) gen_sync_xorsi,
    &operand_data[520],
    2,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
  {
    "sync_andsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_302 },
#else
    { 0, 0, output_302 },
#endif
    (insn_gen_fn) gen_sync_andsi,
    &operand_data[520],
    2,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
  {
    "sync_iordi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_303 },
#else
    { 0, 0, output_303 },
#endif
    (insn_gen_fn) gen_sync_iordi,
    &operand_data[522],
    2,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
  {
    "sync_xordi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_304 },
#else
    { 0, 0, output_304 },
#endif
    (insn_gen_fn) gen_sync_xordi,
    &operand_data[522],
    2,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4524 */
  {
    "sync_anddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_305 },
#else
    { 0, 0, output_305 },
#endif
    (insn_gen_fn) gen_sync_anddi,
    &operand_data[522],
    2,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
  {
    "sync_old_iorsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_306 },
#else
    { 0, 0, output_306 },
#endif
    (insn_gen_fn) gen_sync_old_iorsi,
    &operand_data[524],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
  {
    "sync_old_xorsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_307 },
#else
    { 0, 0, output_307 },
#endif
    (insn_gen_fn) gen_sync_old_xorsi,
    &operand_data[524],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
  {
    "sync_old_andsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_308 },
#else
    { 0, 0, output_308 },
#endif
    (insn_gen_fn) gen_sync_old_andsi,
    &operand_data[524],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
  {
    "sync_old_iordi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_309 },
#else
    { 0, 0, output_309 },
#endif
    (insn_gen_fn) gen_sync_old_iordi,
    &operand_data[527],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
  {
    "sync_old_xordi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_310 },
#else
    { 0, 0, output_310 },
#endif
    (insn_gen_fn) gen_sync_old_xordi,
    &operand_data[527],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4539 */
  {
    "sync_old_anddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_311 },
#else
    { 0, 0, output_311 },
#endif
    (insn_gen_fn) gen_sync_old_anddi,
    &operand_data[527],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
  {
    "sync_new_iorsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_312 },
#else
    { 0, 0, output_312 },
#endif
    (insn_gen_fn) gen_sync_new_iorsi,
    &operand_data[524],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
  {
    "sync_new_xorsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_313 },
#else
    { 0, 0, output_313 },
#endif
    (insn_gen_fn) gen_sync_new_xorsi,
    &operand_data[524],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
  {
    "sync_new_andsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_314 },
#else
    { 0, 0, output_314 },
#endif
    (insn_gen_fn) gen_sync_new_andsi,
    &operand_data[524],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
  {
    "sync_new_iordi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_315 },
#else
    { 0, 0, output_315 },
#endif
    (insn_gen_fn) gen_sync_new_iordi,
    &operand_data[527],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
  {
    "sync_new_xordi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_316 },
#else
    { 0, 0, output_316 },
#endif
    (insn_gen_fn) gen_sync_new_xordi,
    &operand_data[527],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4556 */
  {
    "sync_new_anddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_317 },
#else
    { 0, 0, output_317 },
#endif
    (insn_gen_fn) gen_sync_new_anddi,
    &operand_data[527],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4573 */
  {
    "sync_nandsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_318 },
#else
    { 0, 0, output_318 },
#endif
    (insn_gen_fn) gen_sync_nandsi,
    &operand_data[520],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4573 */
  {
    "sync_nanddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_319 },
#else
    { 0, 0, output_319 },
#endif
    (insn_gen_fn) gen_sync_nanddi,
    &operand_data[522],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4586 */
  {
    "sync_old_nandsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_320 },
#else
    { 0, 0, output_320 },
#endif
    (insn_gen_fn) gen_sync_old_nandsi,
    &operand_data[524],
    3,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4586 */
  {
    "sync_old_nanddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_321 },
#else
    { 0, 0, output_321 },
#endif
    (insn_gen_fn) gen_sync_old_nanddi,
    &operand_data[527],
    3,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4601 */
  {
    "sync_new_nandsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_322 },
#else
    { 0, 0, output_322 },
#endif
    (insn_gen_fn) gen_sync_new_nandsi,
    &operand_data[524],
    3,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4601 */
  {
    "sync_new_nanddi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_323 },
#else
    { 0, 0, output_323 },
#endif
    (insn_gen_fn) gen_sync_new_nanddi,
    &operand_data[527],
    3,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4616 */
  {
    "sync_lock_test_and_setsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_324 },
#else
    { 0, 0, output_324 },
#endif
    (insn_gen_fn) gen_sync_lock_test_and_setsi,
    &operand_data[508],
    3,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4616 */
  {
    "sync_lock_test_and_setdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_325 },
#else
    { 0, 0, output_325 },
#endif
    (insn_gen_fn) gen_sync_lock_test_and_setdi,
    &operand_data[511],
    3,
    1,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4687 */
  {
    "*ashlsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_326 },
#else
    { 0, 0, output_326 },
#endif
    0,
    &operand_data[530],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4687 */
  {
    "*ashrsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_327 },
#else
    { 0, 0, output_327 },
#endif
    0,
    &operand_data[530],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4687 */
  {
    "*lshrsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_328 },
#else
    { 0, 0, output_328 },
#endif
    0,
    &operand_data[530],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4687 */
  {
    "*ashldi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_329 },
#else
    { 0, 0, output_329 },
#endif
    0,
    &operand_data[533],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4687 */
  {
    "*ashrdi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_330 },
#else
    { 0, 0, output_330 },
#endif
    0,
    &operand_data[533],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4687 */
  {
    "*lshrdi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_331 },
#else
    { 0, 0, output_331 },
#endif
    0,
    &operand_data[533],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4702 */
  {
    "*ashlsi3_extend",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_332 },
#else
    { 0, 0, output_332 },
#endif
    0,
    &operand_data[536],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4702 */
  {
    "*ashrsi3_extend",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_333 },
#else
    { 0, 0, output_333 },
#endif
    0,
    &operand_data[536],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4702 */
  {
    "*lshrsi3_extend",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_334 },
#else
    { 0, 0, output_334 },
#endif
    0,
    &operand_data[536],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4717 */
  {
    "*ashlsi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_335 },
#else
    { 0, 0, output_335 },
#endif
    0,
    &operand_data[539],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4717 */
  {
    "*ashrsi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_336 },
#else
    { 0, 0, output_336 },
#endif
    0,
    &operand_data[539],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4717 */
  {
    "*lshrsi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_337 },
#else
    { 0, 0, output_337 },
#endif
    0,
    &operand_data[539],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4739 */
  {
    "*ashldi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_338 },
#else
    { 0, 0, output_338 },
#endif
    0,
    &operand_data[542],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4759 */
  {
    "*ashrdi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_339 },
#else
    { 0, 0, output_339 },
#endif
    0,
    &operand_data[545],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4778 */
  {
    "*lshrdi3_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_340 },
#else
    { 0, 0, output_340 },
#endif
    0,
    &operand_data[545],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4820 */
  {
    "*mips.md:4820",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[548],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4834 */
  {
    "rotrsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_342 },
#else
    { 0, 0, output_342 },
#endif
    (insn_gen_fn) gen_rotrsi3,
    &operand_data[530],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4834 */
  {
    "rotrdi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_343 },
#else
    { 0, 0, output_343 },
#endif
    (insn_gen_fn) gen_rotrdi3,
    &operand_data[533],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4902 */
  {
    "*branch_fp",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_344 },
#else
    { 0, 0, output_344 },
#endif
    0,
    &operand_data[551],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4919 */
  {
    "*branch_fp_inverted",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_345 },
#else
    { 0, 0, output_345 },
#endif
    0,
    &operand_data[551],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4938 */
  {
    "*branch_ordersi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_346 },
#else
    { 0, 0, output_346 },
#endif
    0,
    &operand_data[554],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4938 */
  {
    "*branch_orderdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_347 },
#else
    { 0, 0, output_347 },
#endif
    0,
    &operand_data[557],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4951 */
  {
    "*branch_ordersi_inverted",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_348 },
#else
    { 0, 0, output_348 },
#endif
    0,
    &operand_data[554],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4951 */
  {
    "*branch_orderdi_inverted",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_349 },
#else
    { 0, 0, output_349 },
#endif
    0,
    &operand_data[557],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4966 */
  {
    "*branch_equalitysi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_350 },
#else
    { 0, 0, output_350 },
#endif
    0,
    &operand_data[560],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4966 */
  {
    "*branch_equalitydi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_351 },
#else
    { 0, 0, output_351 },
#endif
    0,
    &operand_data[564],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4983 */
  {
    "*branch_equalitysi_inverted",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_352 },
#else
    { 0, 0, output_352 },
#endif
    0,
    &operand_data[560],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4983 */
  {
    "*branch_equalitydi_inverted",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_353 },
#else
    { 0, 0, output_353 },
#endif
    0,
    &operand_data[564],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5002 */
  {
    "*branch_equalitysi_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_354 },
#else
    { 0, 0, output_354 },
#endif
    0,
    &operand_data[568],
    4,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5002 */
  {
    "*branch_equalitydi_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_355 },
#else
    { 0, 0, output_355 },
#endif
    0,
    &operand_data[572],
    4,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5064 */
  {
    "*seq_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%1,1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[34],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5064 */
  {
    "*seq_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%1,1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5073 */
  {
    "*seq_si_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%1,1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[576],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5073 */
  {
    "*seq_di_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%1,1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[578],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5092 */
  {
    "*sne_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%.,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[34],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5092 */
  {
    "*sne_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%.,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5108 */
  {
    "*sgt_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%0,%z2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[580],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5108 */
  {
    "*sgt_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%0,%z2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[583],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5117 */
  {
    "*sgt_si_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[586],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5117 */
  {
    "*sgt_di_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[589],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5133 */
  {
    "*sge_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%0,%.,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[34],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5133 */
  {
    "*sge_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%0,%.,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5149 */
  {
    "*slt_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[530],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5149 */
  {
    "*slt_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[592],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5158 */
  {
    "*slt_si_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[595],
    3,
    0,
    2,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5158 */
  {
    "*slt_di_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "slt\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[598],
    3,
    0,
    2,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5179 */
  {
    "*sle_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_372 },
#else
    { 0, 0, output_372 },
#endif
    0,
    &operand_data[601],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5179 */
  {
    "*sle_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_373 },
#else
    { 0, 0, output_373 },
#endif
    0,
    &operand_data[604],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5191 */
  {
    "*sle_si_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_374 },
#else
    { 0, 0, output_374 },
#endif
    0,
    &operand_data[607],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5191 */
  {
    "*sle_di_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_375 },
#else
    { 0, 0, output_375 },
#endif
    0,
    &operand_data[610],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5213 */
  {
    "*sgtu_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%z2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[580],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5213 */
  {
    "*sgtu_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%z2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[583],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5222 */
  {
    "*sgtu_si_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[586],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5222 */
  {
    "*sgtu_di_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[589],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5238 */
  {
    "*sge_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%.,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[34],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5238 */
  {
    "*sge_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%.,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[37],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5254 */
  {
    "*sltu_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[530],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5254 */
  {
    "*sltu_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[592],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5263 */
  {
    "*sltu_si_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[595],
    3,
    0,
    2,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5263 */
  {
    "*sltu_di_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "sltu\t%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[598],
    3,
    0,
    2,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5284 */
  {
    "*sleu_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_386 },
#else
    { 0, 0, output_386 },
#endif
    0,
    &operand_data[613],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5284 */
  {
    "*sleu_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_387 },
#else
    { 0, 0, output_387 },
#endif
    0,
    &operand_data[616],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5296 */
  {
    "*sleu_si_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_388 },
#else
    { 0, 0, output_388 },
#endif
    0,
    &operand_data[619],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5296 */
  {
    "*sleu_di_mips16",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_389 },
#else
    { 0, 0, output_389 },
#endif
    0,
    &operand_data[622],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "sunordered_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.un.s\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunordered_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "suneq_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ueq.s\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_suneq_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "sunlt_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ult.s\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunlt_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "sunle_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ule.s\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunle_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "seq_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.eq.s\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_seq_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "slt_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.lt.s\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_slt_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "sle_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.le.s\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sle_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "sunordered_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.un.d\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunordered_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "suneq_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ueq.d\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_suneq_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "sunlt_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ult.d\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunlt_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "sunle_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ule.d\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunle_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "seq_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.eq.d\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_seq_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "slt_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.lt.d\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_slt_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5318 */
  {
    "sle_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.le.d\t%Z0%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sle_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
  {
    "sge_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.le.s\t%Z0%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sge_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
  {
    "sgt_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.lt.s\t%Z0%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sgt_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
  {
    "sunge_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ule.s\t%Z0%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunge_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
  {
    "sungt_sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ult.s\t%Z0%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sungt_sf,
    &operand_data[625],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
  {
    "sge_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.le.d\t%Z0%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sge_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
  {
    "sgt_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.lt.d\t%Z0%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sgt_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
  {
    "sunge_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ule.d\t%Z0%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunge_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5327 */
  {
    "sungt_df",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ult.d\t%Z0%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sungt_df,
    &operand_data[628],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5345 */
  {
    "jump",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_412 },
#else
    { 0, 0, output_412 },
#endif
    (insn_gen_fn) gen_jump,
    &operand_data[552],
    1,
    0,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5379 */
  {
    "*mips.md:5379",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "b\t%l0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[552],
    1,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5400 */
  {
    "indirect_jumpsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*j\t%0%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_indirect_jumpsi,
    &operand_data[35],
    1,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5400 */
  {
    "indirect_jumpdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*j\t%0%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_indirect_jumpdi,
    &operand_data[38],
    1,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5438 */
  {
    "tablejumpsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*j\t%0%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_tablejumpsi,
    &operand_data[631],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5438 */
  {
    "tablejumpdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*j\t%0%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_tablejumpdi,
    &operand_data[633],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5516 */
  {
    "blockage",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_blockage,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5543 */
  {
    "return",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*j\t$31%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_return,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5552 */
  {
    "return_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*j\t%0%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_return_internal,
    &operand_data[635],
    1,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5577 */
  {
    "eh_set_lr_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_eh_set_lr_si,
    &operand_data[636],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5583 */
  {
    "eh_set_lr_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_eh_set_lr_di,
    &operand_data[638],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5623 */
  {
    "restore_gp",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_restore_gp,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5687 */
  {
    "load_callsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lw\t%0,%R2(%1)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_load_callsi,
    &operand_data[640],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5687 */
  {
    "load_calldi",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "ld\t%0,%R2(%1)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_load_calldi,
    &operand_data[643],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5698 */
  {
    "set_got_version",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_set_got_version,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5706 */
  {
    "update_got_version",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_update_got_version,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5738 */
  {
    "sibcall_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_428 },
#else
    { 0, 0, output_428 },
#endif
    (insn_gen_fn) gen_sibcall_internal,
    &operand_data[646],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5757 */
  {
    "sibcall_value_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_429 },
#else
    { 0, 0, output_429 },
#endif
    (insn_gen_fn) gen_sibcall_value_internal,
    &operand_data[648],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5765 */
  {
    "sibcall_value_multiple_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_430 },
#else
    { 0, 0, output_430 },
#endif
    (insn_gen_fn) gen_sibcall_value_multiple_internal,
    &operand_data[648],
    4,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5824 */
  {
    "call_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_431 },
#else
    { 0, 0, output_431 },
#endif
    (insn_gen_fn) gen_call_internal,
    &operand_data[652],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5845 */
  {
    "call_internal_direct",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_432 },
#else
    { 0, 0, output_432 },
#endif
    (insn_gen_fn) gen_call_internal_direct,
    &operand_data[654],
    2,
    0,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5853 */
  {
    "call_split",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_433 },
#else
    { 0, 0, output_433 },
#endif
    (insn_gen_fn) gen_call_split,
    &operand_data[656],
    2,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5875 */
  {
    "call_value_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_434 },
#else
    { 0, 0, output_434 },
#endif
    (insn_gen_fn) gen_call_value_internal,
    &operand_data[651],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5894 */
  {
    "call_value_split",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_435 },
#else
    { 0, 0, output_435 },
#endif
    (insn_gen_fn) gen_call_value_split,
    &operand_data[658],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5905 */
  {
    "call_value_internal_direct",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_436 },
#else
    { 0, 0, output_436 },
#endif
    (insn_gen_fn) gen_call_value_internal_direct,
    &operand_data[661],
    3,
    0,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5915 */
  {
    "call_value_multiple_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_437 },
#else
    { 0, 0, output_437 },
#endif
    (insn_gen_fn) gen_call_value_multiple_internal,
    &operand_data[664],
    4,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5937 */
  {
    "call_value_multiple_split",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_438 },
#else
    { 0, 0, output_438 },
#endif
    (insn_gen_fn) gen_call_value_multiple_split,
    &operand_data[658],
    4,
    2,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5982 */
  {
    "prefetch",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_439 },
#else
    { 0, 0, output_439 },
#endif
    (insn_gen_fn) gen_prefetch,
    &operand_data[668],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5993 */
  {
    "*prefetch_indexed_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_440 },
#else
    { 0, 0, output_440 },
#endif
    0,
    &operand_data[671],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5993 */
  {
    "*prefetch_indexed_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_441 },
#else
    { 0, 0, output_441 },
#endif
    0,
    &operand_data[675],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6005 */
  {
    "nop",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%(nop%)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_nop,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6013 */
  {
    "hazard_nop",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_443 },
#else
    { 0, 0, output_443 },
#endif
    (insn_gen_fn) gen_hazard_nop,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6026 */
  {
    "*movsi_on_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_444 },
#else
    { 0, output_444, 0 },
#endif
    0,
    &operand_data[679],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6026 */
  {
    "*movdi_on_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_445 },
#else
    { 0, output_445, 0 },
#endif
    0,
    &operand_data[684],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6026 */
  {
    "*movsi_on_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_446 },
#else
    { 0, output_446, 0 },
#endif
    0,
    &operand_data[689],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6026 */
  {
    "*movdi_on_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_447 },
#else
    { 0, output_447, 0 },
#endif
    0,
    &operand_data[694],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6026 */
  {
    "*movsi_on_cc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_448 },
#else
    { 0, output_448, 0 },
#endif
    0,
    &operand_data[699],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6026 */
  {
    "*movdi_on_cc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_449 },
#else
    { 0, output_449, 0 },
#endif
    0,
    &operand_data[704],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6041 */
  {
    "*movsf_on_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_450 },
#else
    { 0, output_450, 0 },
#endif
    0,
    &operand_data[709],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6041 */
  {
    "*movdf_on_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_451 },
#else
    { 0, output_451, 0 },
#endif
    0,
    &operand_data[714],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6041 */
  {
    "*movsf_on_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_452 },
#else
    { 0, output_452, 0 },
#endif
    0,
    &operand_data[719],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6041 */
  {
    "*movdf_on_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_453 },
#else
    { 0, output_453, 0 },
#endif
    0,
    &operand_data[724],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6041 */
  {
    "*movsf_on_cc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_454 },
#else
    { 0, output_454, 0 },
#endif
    0,
    &operand_data[729],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6041 */
  {
    "*movdf_on_cc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_455 },
#else
    { 0, output_455, 0 },
#endif
    0,
    &operand_data[734],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6090 */
  {
    "consttable_int",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_456 },
#else
    { 0, 0, output_456 },
#endif
    (insn_gen_fn) gen_consttable_int,
    &operand_data[739],
    2,
    0,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6102 */
  {
    "consttable_float",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_457 },
#else
    { 0, 0, output_457 },
#endif
    (insn_gen_fn) gen_consttable_float,
    &operand_data[739],
    1,
    0,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6118 */
  {
    "align",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    ".align\t%0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_align,
    &operand_data[446],
    1,
    0,
    0,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6138 */
  {
    "*mips16e_save_restore",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_459 },
#else
    { 0, 0, output_459 },
#endif
    0,
    &operand_data[741],
    3,
    1,
    0,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:20 */
  {
    "*movcc_v2sf_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_460 },
#else
    { 0, output_460, 0 },
#endif
    0,
    &operand_data[744],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:20 */
  {
    "*movcc_v2sf_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_461 },
#else
    { 0, output_461, 0 },
#endif
    0,
    &operand_data[749],
    5,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:35 */
  {
    "mips_cond_move_tf_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_462 },
#else
    { 0, output_462, 0 },
#endif
    (insn_gen_fn) gen_mips_cond_move_tf_ps,
    &operand_data[754],
    4,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:69 */
  {
    "mips_pul_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "pul.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_pul_ps,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:81 */
  {
    "mips_puu_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "puu.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_puu_ps,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:95 */
  {
    "mips_pll_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "pll.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_pll_ps,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:109 */
  {
    "mips_plu_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "plu.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_plu_ps,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:136 */
  {
    "vec_initv2sf_internal",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_467 },
#else
    { 0, 0, output_467 },
#endif
    (insn_gen_fn) gen_vec_initv2sf_internal,
    &operand_data[758],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:155 */
  {
    "vec_extractv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_468 },
#else
    { 0, 0, output_468 },
#endif
    (insn_gen_fn) gen_vec_extractv2sf,
    &operand_data[761],
    3,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:225 */
  {
    "mips_alnv_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "alnv.ps\t%0,%1,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_alnv_ps,
    &operand_data[764],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:237 */
  {
    "mips_addr_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addr.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addr_ps,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:248 */
  {
    "mips_cvt_pw_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cvt.pw.ps\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cvt_pw_ps,
    &operand_data[13],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:258 */
  {
    "mips_cvt_ps_pw",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cvt.ps.pw\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cvt_ps_pw,
    &operand_data[13],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:268 */
  {
    "mips_mulr_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulr.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mulr_ps,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:294 */
  {
    "*mips_abs_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "abs.ps\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    0,
    &operand_data[13],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:307 */
  {
    "mips_cabs_cond_s",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cabs.%Y3.s\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cabs_cond_s,
    &operand_data[768],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:307 */
  {
    "mips_cabs_cond_d",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cabs.%Y3.d\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cabs_cond_d,
    &operand_data[772],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:323 */
  {
    "mips_c_cond_4s",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_c_cond_4s,
    &operand_data[776],
    6,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:352 */
  {
    "mips_cabs_cond_4s",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "#",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cabs_cond_4s,
    &operand_data[776],
    6,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:386 */
  {
    "mips_c_cond_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.%Y3.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_c_cond_ps,
    &operand_data[782],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:397 */
  {
    "mips_cabs_cond_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cabs.%Y3.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cabs_cond_ps,
    &operand_data[782],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
  {
    "sunordered_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.un.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunordered_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
  {
    "suneq_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ueq.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_suneq_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
  {
    "sunlt_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ult.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunlt_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
  {
    "sunle_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ule.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunle_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
  {
    "seq_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.eq.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_seq_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
  {
    "slt_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.lt.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_slt_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:413 */
  {
    "sle_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.le.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sle_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:424 */
  {
    "sge_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.le.ps\t%0,%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sge_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:424 */
  {
    "sgt_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.lt.ps\t%0,%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sgt_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:424 */
  {
    "sunge_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ule.ps\t%0,%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sunge_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:424 */
  {
    "sungt_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "c.ult.ps\t%0,%2,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sungt_ps,
    &operand_data[782],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:440 */
  {
    "bc1any4t",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*bc1any4t\t%0,%1%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_bc1any4t,
    &operand_data[786],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:452 */
  {
    "bc1any4f",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*bc1any4f\t%0,%1%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_bc1any4f,
    &operand_data[786],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:464 */
  {
    "bc1any2t",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*bc1any2t\t%0,%1%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_bc1any2t,
    &operand_data[788],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:476 */
  {
    "bc1any2f",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*bc1any2f\t%0,%1%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_bc1any2f,
    &operand_data[788],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:497 */
  {
    "*branch_upper_lower",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_496 },
#else
    { 0, 0, output_496 },
#endif
    0,
    &operand_data[790],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:519 */
  {
    "*branch_upper_lower_inverted",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_497 },
#else
    { 0, 0, output_497 },
#endif
    0,
    &operand_data[790],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:544 */
  {
    "mips_rsqrt1_s",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "rsqrt1.s\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_rsqrt1_s,
    &operand_data[7],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:544 */
  {
    "mips_rsqrt1_d",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "rsqrt1.d\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_rsqrt1_d,
    &operand_data[10],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:544 */
  {
    "mips_rsqrt1_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "rsqrt1.ps\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_rsqrt1_ps,
    &operand_data[13],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:553 */
  {
    "mips_rsqrt2_s",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "rsqrt2.s\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_rsqrt2_s,
    &operand_data[7],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:553 */
  {
    "mips_rsqrt2_d",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "rsqrt2.d\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_rsqrt2_d,
    &operand_data[10],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:553 */
  {
    "mips_rsqrt2_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "rsqrt2.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_rsqrt2_ps,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:563 */
  {
    "mips_recip1_s",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "recip1.s\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_recip1_s,
    &operand_data[7],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:563 */
  {
    "mips_recip1_d",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "recip1.d\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_recip1_d,
    &operand_data[10],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:563 */
  {
    "mips_recip1_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "recip1.ps\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_recip1_ps,
    &operand_data[13],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:572 */
  {
    "mips_recip2_s",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "recip2.s\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_recip2_s,
    &operand_data[7],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:572 */
  {
    "mips_recip2_d",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "recip2.d\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_recip2_d,
    &operand_data[10],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:572 */
  {
    "mips_recip2_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "recip2.ps\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_recip2_ps,
    &operand_data[13],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1 */
  {
    "addv2hi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addv2hi3,
    &operand_data[794],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1 */
  {
    "addv4qi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addv4qi3,
    &operand_data[797],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:50 */
  {
    "mips_addq_s_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq_s.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addq_s_w,
    &operand_data[34],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:50 */
  {
    "mips_addq_s_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addq_s_ph,
    &operand_data[794],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:50 */
  {
    "mips_addu_s_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu_s.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addu_s_qb,
    &operand_data[797],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:64 */
  {
    "subv2hi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subv2hi3,
    &operand_data[794],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:64 */
  {
    "subv4qi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subv4qi3,
    &operand_data[797],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:76 */
  {
    "mips_subq_s_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq_s.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subq_s_w,
    &operand_data[34],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:76 */
  {
    "mips_subq_s_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subq_s_ph,
    &operand_data[794],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:76 */
  {
    "mips_subu_s_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu_s.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subu_s_qb,
    &operand_data[797],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:90 */
  {
    "mips_addsc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addsc\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addsc,
    &operand_data[34],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:104 */
  {
    "mips_addwc",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addwc\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addwc,
    &operand_data[34],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:119 */
  {
    "mips_modsub",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "modsub\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_modsub,
    &operand_data[34],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:130 */
  {
    "mips_raddu_w_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "raddu.w.qb\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_raddu_w_qb,
    &operand_data[800],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:140 */
  {
    "mips_absq_s_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "absq_s.w\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_absq_s_w,
    &operand_data[34],
    2,
    1,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:140 */
  {
    "mips_absq_s_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "absq_s.ph\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_absq_s_ph,
    &operand_data[794],
    2,
    1,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:153 */
  {
    "mips_precrq_qb_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precrq.qb.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precrq_qb_ph,
    &operand_data[802],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:163 */
  {
    "mips_precrq_ph_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precrq.ph.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precrq_ph_w,
    &operand_data[805],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:173 */
  {
    "mips_precrq_rs_ph_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precrq_rs.ph.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precrq_rs_ph_w,
    &operand_data[805],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:188 */
  {
    "mips_precrqu_s_qb_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precrqu_s.qb.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precrqu_s_qb_ph,
    &operand_data[802],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:203 */
  {
    "mips_preceq_w_phl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "preceq.w.phl\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_preceq_w_phl,
    &operand_data[808],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:212 */
  {
    "mips_preceq_w_phr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "preceq.w.phr\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_preceq_w_phr,
    &operand_data[808],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:222 */
  {
    "mips_precequ_ph_qbl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precequ.ph.qbl\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precequ_ph_qbl,
    &operand_data[810],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:231 */
  {
    "mips_precequ_ph_qbr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precequ.ph.qbr\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precequ_ph_qbr,
    &operand_data[810],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:240 */
  {
    "mips_precequ_ph_qbla",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precequ.ph.qbla\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precequ_ph_qbla,
    &operand_data[810],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:249 */
  {
    "mips_precequ_ph_qbra",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precequ.ph.qbra\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precequ_ph_qbra,
    &operand_data[810],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:259 */
  {
    "mips_preceu_ph_qbl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "preceu.ph.qbl\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_preceu_ph_qbl,
    &operand_data[810],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:268 */
  {
    "mips_preceu_ph_qbr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "preceu.ph.qbr\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_preceu_ph_qbr,
    &operand_data[810],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:277 */
  {
    "mips_preceu_ph_qbla",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "preceu.ph.qbla\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_preceu_ph_qbla,
    &operand_data[810],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:286 */
  {
    "mips_preceu_ph_qbra",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "preceu.ph.qbra\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_preceu_ph_qbra,
    &operand_data[810],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:297 */
  {
    "mips_shll_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_540 },
#else
    { 0, 0, output_540 },
#endif
    (insn_gen_fn) gen_mips_shll_ph,
    &operand_data[812],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:297 */
  {
    "mips_shll_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_541 },
#else
    { 0, 0, output_541 },
#endif
    (insn_gen_fn) gen_mips_shll_qb,
    &operand_data[815],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:319 */
  {
    "mips_shll_s_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_542 },
#else
    { 0, 0, output_542 },
#endif
    (insn_gen_fn) gen_mips_shll_s_w,
    &operand_data[818],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:319 */
  {
    "mips_shll_s_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_543 },
#else
    { 0, 0, output_543 },
#endif
    (insn_gen_fn) gen_mips_shll_s_ph,
    &operand_data[812],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:342 */
  {
    "mips_shrl_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_544 },
#else
    { 0, 0, output_544 },
#endif
    (insn_gen_fn) gen_mips_shrl_qb,
    &operand_data[815],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:361 */
  {
    "mips_shra_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_545 },
#else
    { 0, 0, output_545 },
#endif
    (insn_gen_fn) gen_mips_shra_ph,
    &operand_data[812],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:379 */
  {
    "mips_shra_r_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_546 },
#else
    { 0, 0, output_546 },
#endif
    (insn_gen_fn) gen_mips_shra_r_w,
    &operand_data[818],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:379 */
  {
    "mips_shra_r_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_547 },
#else
    { 0, 0, output_547 },
#endif
    (insn_gen_fn) gen_mips_shra_r_ph,
    &operand_data[812],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:400 */
  {
    "mips_muleu_s_ph_qbl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "muleu_s.ph.qbl\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_muleu_s_ph_qbl,
    &operand_data[821],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:414 */
  {
    "mips_muleu_s_ph_qbr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "muleu_s.ph.qbr\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_muleu_s_ph_qbr,
    &operand_data[821],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:429 */
  {
    "mips_mulq_rs_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulq_rs.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mulq_rs_ph,
    &operand_data[825],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:444 */
  {
    "mips_muleq_s_w_phl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "muleq_s.w.phl\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_muleq_s_w_phl,
    &operand_data[829],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:458 */
  {
    "mips_muleq_s_w_phr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "muleq_s.w.phr\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_muleq_s_w_phr,
    &operand_data[829],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:473 */
  {
    "mips_dpau_h_qbl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpau.h.qbl\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpau_h_qbl,
    &operand_data[833],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:484 */
  {
    "mips_dpau_h_qbr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpau.h.qbr\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpau_h_qbr,
    &operand_data[833],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:496 */
  {
    "mips_dpsu_h_qbl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpsu.h.qbl\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpsu_h_qbl,
    &operand_data[833],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:507 */
  {
    "mips_dpsu_h_qbr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpsu.h.qbr\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpsu_h_qbr,
    &operand_data[833],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:519 */
  {
    "mips_dpaq_s_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpaq_s.w.ph\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpaq_s_w_ph,
    &operand_data[837],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:535 */
  {
    "mips_dpsq_s_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpsq_s.w.ph\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpsq_s_w_ph,
    &operand_data[837],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:551 */
  {
    "mips_mulsaq_s_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulsaq_s.w.ph\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mulsaq_s_w_ph,
    &operand_data[837],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:567 */
  {
    "mips_dpaq_sa_l_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpaq_sa.l.w\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpaq_sa_l_w,
    &operand_data[841],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:583 */
  {
    "mips_dpsq_sa_l_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpsq_sa.l.w\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpsq_sa_l_w,
    &operand_data[841],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:599 */
  {
    "mips_maq_s_w_phl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "maq_s.w.phl\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_maq_s_w_phl,
    &operand_data[837],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:614 */
  {
    "mips_maq_s_w_phr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "maq_s.w.phr\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_maq_s_w_phr,
    &operand_data[837],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:630 */
  {
    "mips_maq_sa_w_phl",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "maq_sa.w.phl\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_maq_sa_w_phl,
    &operand_data[837],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:645 */
  {
    "mips_maq_sa_w_phr",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "maq_sa.w.phr\t%q0,%2,%3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_maq_sa_w_phr,
    &operand_data[837],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:662 */
  {
    "mips_bitrev",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "bitrev\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_bitrev,
    &operand_data[34],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:672 */
  {
    "mips_insv",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "insv\t%0,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_insv,
    &operand_data[845],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:685 */
  {
    "mips_repl_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_568 },
#else
    { 0, 0, output_568 },
#endif
    (insn_gen_fn) gen_mips_repl_qb,
    &operand_data[848],
    2,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:702 */
  {
    "mips_repl_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .multi = output_569 },
#else
    { 0, output_569, 0 },
#endif
    (insn_gen_fn) gen_mips_repl_ph,
    &operand_data[850],
    2,
    0,
    2,
    2
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:715 */
  {
    "mips_cmp_eq_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmp.eq.ph\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmp_eq_ph,
    &operand_data[795],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:715 */
  {
    "mips_cmpu_eq_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpu.eq.qb\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpu_eq_qb,
    &operand_data[798],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:726 */
  {
    "mips_cmp_lt_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmp.lt.ph\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmp_lt_ph,
    &operand_data[795],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:726 */
  {
    "mips_cmpu_lt_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpu.lt.qb\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpu_lt_qb,
    &operand_data[798],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:737 */
  {
    "mips_cmp_le_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmp.le.ph\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmp_le_ph,
    &operand_data[795],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:737 */
  {
    "mips_cmpu_le_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpu.le.qb\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpu_le_qb,
    &operand_data[798],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:748 */
  {
    "mips_cmpgu_eq_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpgu.eq.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpgu_eq_qb,
    &operand_data[852],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:758 */
  {
    "mips_cmpgu_lt_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpgu.lt.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpgu_lt_qb,
    &operand_data[852],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:768 */
  {
    "mips_cmpgu_le_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpgu.le.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpgu_le_qb,
    &operand_data[852],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:779 */
  {
    "mips_pick_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "pick.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_pick_ph,
    &operand_data[794],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:779 */
  {
    "mips_pick_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "pick.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_pick_qb,
    &operand_data[797],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:791 */
  {
    "mips_packrl_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "packrl.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_packrl_ph,
    &operand_data[794],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:803 */
  {
    "mips_extr_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_582 },
#else
    { 0, 0, output_582 },
#endif
    (insn_gen_fn) gen_mips_extr_w,
    &operand_data[855],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:824 */
  {
    "mips_extr_r_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_583 },
#else
    { 0, 0, output_583 },
#endif
    (insn_gen_fn) gen_mips_extr_r_w,
    &operand_data[855],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:845 */
  {
    "mips_extr_rs_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_584 },
#else
    { 0, 0, output_584 },
#endif
    (insn_gen_fn) gen_mips_extr_rs_w,
    &operand_data[855],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:867 */
  {
    "mips_extr_s_h",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_585 },
#else
    { 0, 0, output_585 },
#endif
    (insn_gen_fn) gen_mips_extr_s_h,
    &operand_data[855],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:889 */
  {
    "mips_extp",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_586 },
#else
    { 0, 0, output_586 },
#endif
    (insn_gen_fn) gen_mips_extp,
    &operand_data[855],
    3,
    2,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:911 */
  {
    "mips_extpdp",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_587 },
#else
    { 0, 0, output_587 },
#endif
    (insn_gen_fn) gen_mips_extpdp,
    &operand_data[855],
    3,
    4,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:937 */
  {
    "mips_shilo",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_588 },
#else
    { 0, 0, output_588 },
#endif
    (insn_gen_fn) gen_mips_shilo,
    &operand_data[858],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:956 */
  {
    "mips_mthlip",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mthlip\t%2,%q0",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mthlip,
    &operand_data[841],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:972 */
  {
    "mips_wrdsp",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "wrdsp\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_wrdsp,
    &operand_data[861],
    2,
    10,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:994 */
  {
    "mips_rddsp",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "rddsp\t%0,%1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_rddsp,
    &operand_data[863],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1011 */
  {
    "mips_lbux",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lbux\t%0,%2(%1)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_lbux,
    &operand_data[34],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1023 */
  {
    "mips_lhx",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lhx\t%0,%2(%1)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_lhx,
    &operand_data[34],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1035 */
  {
    "mips_lwx",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "lwx\t%0,%2(%1)",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_lwx,
    &operand_data[34],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dsp.md:1047 */
  {
    "mips_bposge",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "%*bposge%0\t%1%/",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_bposge,
    &operand_data[865],
    2,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:3 */
  {
    "mips_absq_s_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "absq_s.qb\t%0,%z1",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_absq_s_qb,
    &operand_data[867],
    2,
    1,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:15 */
  {
    "mips_addu_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addu_ph,
    &operand_data[869],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:27 */
  {
    "mips_addu_s_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu_s.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addu_s_ph,
    &operand_data[869],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:40 */
  {
    "mips_adduh_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "adduh.qb\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_adduh_qb,
    &operand_data[872],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:50 */
  {
    "mips_adduh_r_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "adduh_r.qb\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_adduh_r_qb,
    &operand_data[872],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:60 */
  {
    "mips_append",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_601 },
#else
    { 0, 0, output_601 },
#endif
    (insn_gen_fn) gen_mips_append,
    &operand_data[875],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:75 */
  {
    "mips_balign",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_602 },
#else
    { 0, 0, output_602 },
#endif
    (insn_gen_fn) gen_mips_balign,
    &operand_data[875],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:90 */
  {
    "mips_cmpgdu_eq_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpgdu.eq.qb\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpgdu_eq_qb,
    &operand_data[879],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:105 */
  {
    "mips_cmpgdu_lt_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpgdu.lt.qb\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpgdu_lt_qb,
    &operand_data[879],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:120 */
  {
    "mips_cmpgdu_le_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "cmpgdu.le.qb\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_cmpgdu_le_qb,
    &operand_data[879],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:135 */
  {
    "mips_dpa_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpa.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpa_w_ph,
    &operand_data[882],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:146 */
  {
    "mips_dps_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dps.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dps_w_ph,
    &operand_data[882],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:173 */
  {
    "mulv2hi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mul.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mulv2hi3,
    &operand_data[825],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:186 */
  {
    "mips_mul_s_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mul_s.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mul_s_ph,
    &operand_data[886],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:200 */
  {
    "mips_mulq_rs_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulq_rs.w\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mulq_rs_w,
    &operand_data[890],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:214 */
  {
    "mips_mulq_s_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulq_s.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mulq_s_ph,
    &operand_data[886],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:228 */
  {
    "mips_mulq_s_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulq_s.w\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mulq_s_w,
    &operand_data[890],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:242 */
  {
    "mips_mulsa_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulsa.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mulsa_w_ph,
    &operand_data[882],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:253 */
  {
    "mips_mult",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mult\t%q0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_mult,
    &operand_data[894],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:263 */
  {
    "mips_multu",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "multu\t%q0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_multu,
    &operand_data[894],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:273 */
  {
    "mips_precr_qb_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "precr.qb.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_precr_qb_ph,
    &operand_data[897],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:283 */
  {
    "mips_precr_sra_ph_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_617 },
#else
    { 0, 0, output_617 },
#endif
    (insn_gen_fn) gen_mips_precr_sra_ph_w,
    &operand_data[900],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:298 */
  {
    "mips_precr_sra_r_ph_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_618 },
#else
    { 0, 0, output_618 },
#endif
    (insn_gen_fn) gen_mips_precr_sra_r_ph_w,
    &operand_data[900],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:313 */
  {
    "mips_prepend",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_619 },
#else
    { 0, 0, output_619 },
#endif
    (insn_gen_fn) gen_mips_prepend,
    &operand_data[875],
    4,
    0,
    1,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:328 */
  {
    "mips_shra_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_620 },
#else
    { 0, 0, output_620 },
#endif
    (insn_gen_fn) gen_mips_shra_qb,
    &operand_data[904],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:347 */
  {
    "mips_shra_r_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_621 },
#else
    { 0, 0, output_621 },
#endif
    (insn_gen_fn) gen_mips_shra_r_qb,
    &operand_data[904],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:365 */
  {
    "mips_shrl_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .function = output_622 },
#else
    { 0, 0, output_622 },
#endif
    (insn_gen_fn) gen_mips_shrl_ph,
    &operand_data[907],
    3,
    0,
    2,
    3
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:383 */
  {
    "mips_subu_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subu_ph,
    &operand_data[869],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:396 */
  {
    "mips_subu_s_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu_s.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subu_s_ph,
    &operand_data[869],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:409 */
  {
    "mips_subuh_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subuh.qb\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subuh_qb,
    &operand_data[872],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:419 */
  {
    "mips_subuh_r_qb",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subuh_r.qb\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subuh_r_qb,
    &operand_data[872],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:429 */
  {
    "mips_addqh_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addqh.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addqh_ph,
    &operand_data[869],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:439 */
  {
    "mips_addqh_r_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addqh_r.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addqh_r_ph,
    &operand_data[869],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:449 */
  {
    "mips_addqh_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addqh.w\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addqh_w,
    &operand_data[890],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:459 */
  {
    "mips_addqh_r_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addqh_r.w\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_addqh_r_w,
    &operand_data[890],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:469 */
  {
    "mips_subqh_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subqh.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subqh_ph,
    &operand_data[869],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:479 */
  {
    "mips_subqh_r_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subqh_r.ph\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subqh_r_ph,
    &operand_data[869],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:489 */
  {
    "mips_subqh_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subqh.w\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subqh_w,
    &operand_data[890],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:499 */
  {
    "mips_subqh_r_w",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subqh_r.w\t%0,%z1,%z2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_subqh_r_w,
    &operand_data[890],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:509 */
  {
    "mips_dpax_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpax.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpax_w_ph,
    &operand_data[882],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:520 */
  {
    "mips_dpsx_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpsx.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpsx_w_ph,
    &operand_data[882],
    4,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:531 */
  {
    "mips_dpaqx_s_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpaqx_s.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpaqx_s_w_ph,
    &operand_data[882],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:546 */
  {
    "mips_dpaqx_sa_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpaqx_sa.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpaqx_sa_w_ph,
    &operand_data[882],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:561 */
  {
    "mips_dpsqx_s_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpsqx_s.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpsqx_s_w_ph,
    &operand_data[882],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:576 */
  {
    "mips_dpsqx_sa_w_ph",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpsqx_sa.w.ph\t%q0,%z2,%z3",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_mips_dpsqx_sa_w_ph,
    &operand_data[882],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addqq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addqq3,
    &operand_data[910],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addhq3,
    &operand_data[913],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addsq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addsq3,
    &operand_data[916],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "adddq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "daddu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_adddq3,
    &operand_data[919],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "adduqq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_adduqq3,
    &operand_data[922],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "adduhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_adduhq3,
    &operand_data[925],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addusq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addusq3,
    &operand_data[928],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addudq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "daddu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addudq3,
    &operand_data[931],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addha3,
    &operand_data[934],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addsa3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addsa3,
    &operand_data[937],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addda3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "daddu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addda3,
    &operand_data[940],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "adduha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_adduha3,
    &operand_data[943],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "addusa3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_addusa3,
    &operand_data[946],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:4 */
  {
    "adduda3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "daddu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_adduda3,
    &operand_data[949],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
  {
    "usadduqq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu_s.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_usadduqq3,
    &operand_data[922],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
  {
    "usadduhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_usadduhq3,
    &operand_data[925],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
  {
    "usadduha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_usadduha3,
    &operand_data[943],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
  {
    "usaddv4uqq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu_s.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_usaddv4uqq3,
    &operand_data[952],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
  {
    "usaddv2uhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_usaddv2uhq3,
    &operand_data[955],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:36 */
  {
    "usaddv2uha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addu_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_usaddv2uha3,
    &operand_data[958],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
  {
    "ssaddhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssaddhq3,
    &operand_data[913],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
  {
    "ssaddsq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq_s.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssaddsq3,
    &operand_data[916],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
  {
    "ssaddha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssaddha3,
    &operand_data[934],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
  {
    "ssaddsa3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq_s.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssaddsa3,
    &operand_data[937],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
  {
    "ssaddv2hq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssaddv2hq3,
    &operand_data[961],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:48 */
  {
    "ssaddv2ha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "addq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssaddv2ha3,
    &operand_data[964],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subqq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subqq3,
    &operand_data[910],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subhq3,
    &operand_data[913],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subsq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subsq3,
    &operand_data[916],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subdq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsubu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subdq3,
    &operand_data[919],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subuqq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subuqq3,
    &operand_data[922],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subuhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subuhq3,
    &operand_data[925],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subusq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subusq3,
    &operand_data[928],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subudq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsubu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subudq3,
    &operand_data[931],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subha3,
    &operand_data[934],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subsa3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subsa3,
    &operand_data[937],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subda3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsubu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subda3,
    &operand_data[940],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subuha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subuha3,
    &operand_data[943],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subusa3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subusa3,
    &operand_data[946],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:60 */
  {
    "subuda3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dsubu\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_subuda3,
    &operand_data[949],
    3,
    0,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
  {
    "ussubuqq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu_s.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ussubuqq3,
    &operand_data[922],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
  {
    "ussubuhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ussubuhq3,
    &operand_data[925],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
  {
    "ussubuha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ussubuha3,
    &operand_data[943],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
  {
    "ussubv4uqq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu_s.qb\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ussubv4uqq3,
    &operand_data[952],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
  {
    "ussubv2uhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ussubv2uhq3,
    &operand_data[955],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:69 */
  {
    "ussubv2uha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subu_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ussubv2uha3,
    &operand_data[958],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
  {
    "sssubhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sssubhq3,
    &operand_data[913],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
  {
    "sssubsq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq_s.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sssubsq3,
    &operand_data[916],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
  {
    "sssubha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sssubha3,
    &operand_data[934],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
  {
    "sssubsa3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq_s.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sssubsa3,
    &operand_data[937],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
  {
    "sssubv2hq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sssubv2hq3,
    &operand_data[961],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:81 */
  {
    "sssubv2ha3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "subq_s.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_sssubv2ha3,
    &operand_data[964],
    3,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:93 */
  {
    "ssmulv2hq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulq_rs.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssmulv2hq3,
    &operand_data[967],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:93 */
  {
    "ssmulhq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulq_rs.ph\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssmulhq3,
    &operand_data[971],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:93 */
  {
    "ssmulsq3",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "mulq_rs.w\t%0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssmulsq3,
    &operand_data[975],
    4,
    2,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:106 */
  {
    "ssmaddsqdq4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpaq_sa.l.w\t%q0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssmaddsqdq4,
    &operand_data[979],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-fixed.md:123 */
  {
    "ssmsubsqdq4",
#if HAVE_DESIGNATED_INITIALIZERS
    { .single =
#else
    {
#endif
    "dpsq_sa.l.w\t%q0,%1,%2",
#if HAVE_DESIGNATED_INITIALIZERS
    },
#else
    0, 0 },
#endif
    (insn_gen_fn) gen_ssmsubsqdq4,
    &operand_data[979],
    4,
    3,
    1,
    1
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:747 */
  {
    "conditional_trap",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_conditional_trap,
    &operand_data[983],
    2,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:788 */
  {
    "addsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_addsi3,
    &operand_data[985],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:788 */
  {
    "adddi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_adddi3,
    &operand_data[988],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:839 */
  {
    "adddi3+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[742],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:868 */
  {
    "adddi3+2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[991],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:900 */
  {
    "adddi3+3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[994],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:929 */
  {
    "mulsf3-2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[996],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:975 */
  {
    "mulsf3-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[999],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1033 */
  {
    "mulsf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mulsf3,
    &operand_data[1002],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1033 */
  {
    "muldf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_muldf3,
    &operand_data[1005],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1118 */
  {
    "mulsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mulsi3,
    &operand_data[1008],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1133 */
  {
    "muldi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_muldi3,
    &operand_data[1011],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1172 */
  {
    "muldi3+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1014],
    5,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1220 */
  {
    "muldi3+2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1014],
    5,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1278 */
  {
    "muldi3+3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1015],
    7,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1297 */
  {
    "muldi3+4",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1015],
    7,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1359 */
  {
    "muldi3+5",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1015],
    7,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1422 */
  {
    "mulsidi3-5",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1022],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1453 */
  {
    "mulsidi3-4",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1026],
    6,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1489 */
  {
    "mulsidi3-3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1032],
    6,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1533 */
  {
    "mulsidi3-2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1015],
    7,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1552 */
  {
    "mulsidi3-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1015],
    7,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1587 */
  {
    "mulsidi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mulsidi3,
    &operand_data[1013],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1587 */
  {
    "umulsidi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_umulsidi3,
    &operand_data[1013],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1629 */
  {
    "umulsidi3+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1038],
    6,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1629 */
  {
    "smulsi3_highpart-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1038],
    6,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1724 */
  {
    "smulsi3_highpart",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_smulsi3_highpart,
    &operand_data[1008],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1724 */
  {
    "umulsi3_highpart",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_umulsi3_highpart,
    &operand_data[1008],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1931 */
  {
    "divsf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_divsf3,
    &operand_data[1044],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1931 */
  {
    "divdf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_divdf3,
    &operand_data[1047],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:1931 */
  {
    "divv2sf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_divv2sf3,
    &operand_data[1050],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2181 */
  {
    "andsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_andsi3,
    &operand_data[1053],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2181 */
  {
    "anddi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_anddi3,
    &operand_data[1056],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2211 */
  {
    "iorsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_iorsi3,
    &operand_data[1053],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2211 */
  {
    "iordi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_iordi3,
    &operand_data[1056],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2241 */
  {
    "xorsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_xorsi3,
    &operand_data[1053],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2241 */
  {
    "xordi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_xordi3,
    &operand_data[1056],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2376 */
  {
    "xordi3+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[988],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2376 */
  {
    "xordi3+2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[988],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2394 */
  {
    "xordi3+3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1010],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2394 */
  {
    "zero_extendqisi2-3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1010],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2450 */
  {
    "zero_extendqisi2-2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1059],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2470 */
  {
    "zero_extendqisi2-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1061],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2492 */
  {
    "zero_extendqisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_zero_extendqisi2,
    &operand_data[1063],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2492 */
  {
    "zero_extendqidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_zero_extendqidi2,
    &operand_data[1065],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2492 */
  {
    "zero_extendhisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_zero_extendhisi2,
    &operand_data[1067],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2492 */
  {
    "zero_extendhidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_zero_extendhidi2,
    &operand_data[1069],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2535 */
  {
    "zero_extendqihi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_zero_extendqihi2,
    &operand_data[1071],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2584 */
  {
    "zero_extendqihi2+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1059],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2600 */
  {
    "extendqisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_extendqisi2,
    &operand_data[1063],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2600 */
  {
    "extendqidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_extendqidi2,
    &operand_data[1065],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2600 */
  {
    "extendhisi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_extendhisi2,
    &operand_data[1067],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2600 */
  {
    "extendhidi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_extendhidi2,
    &operand_data[1069],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
  {
    "extendhidi2+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1063],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
  {
    "extendhidi2+2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1065],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
  {
    "extendqihi2-2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1067],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2615 */
  {
    "extendqihi2-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1069],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2646 */
  {
    "extendqihi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_extendqihi2,
    &operand_data[1071],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2661 */
  {
    "extendqihi2+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1071],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2709 */
  {
    "fix_truncdfsi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_fix_truncdfsi2,
    &operand_data[1073],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2747 */
  {
    "fix_truncsfsi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_fix_truncsfsi2,
    &operand_data[1075],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2852 */
  {
    "fixuns_truncdfsi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_fixuns_truncdfsi2,
    &operand_data[1073],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2897 */
  {
    "fixuns_truncdfdi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_fixuns_truncdfdi2,
    &operand_data[1077],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2939 */
  {
    "fixuns_truncsfsi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_fixuns_truncsfsi2,
    &operand_data[1075],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:2981 */
  {
    "fixuns_truncsfdi2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_fixuns_truncsfdi2,
    &operand_data[1079],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3031 */
  {
    "extv",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_extv,
    &operand_data[1081],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3046 */
  {
    "extzv",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_extzv,
    &operand_data[1085],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3084 */
  {
    "insv",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_insv,
    &operand_data[1086],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3190 */
  {
    "insv+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1090],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3215 */
  {
    "insv+2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1092],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3243 */
  {
    "insv+3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1093],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3268 */
  {
    "insv+4",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1096],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3280 */
  {
    "insv+5",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1098],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3280 */
  {
    "insv+6",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1100],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3295 */
  {
    "insv+7",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1102],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3295 */
  {
    "insv+8",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1105],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3310 */
  {
    "movdi-8",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1098],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3310 */
  {
    "movdi-7",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1100],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3327 */
  {
    "movdi-6",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1108],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3327 */
  {
    "movdi-5",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1110],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3382 */
  {
    "movdi-4",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1112],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3382 */
  {
    "movdi-3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1115],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3394 */
  {
    "movdi-2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1118],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3394 */
  {
    "movdi-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1121],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3411 */
  {
    "movdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movdi,
    &operand_data[1124],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3505 */
  {
    "movdi+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[994],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3550 */
  {
    "movsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movsi,
    &operand_data[1126],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3601 */
  {
    "movsi+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[742],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3643 */
  {
    "reload_incc-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[742],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3689 */
  {
    "reload_incc",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_reload_incc,
    &operand_data[1128],
    3,
    0,
    1,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3699 */
  {
    "reload_outcc",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_reload_outcc,
    &operand_data[1131],
    3,
    0,
    1,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3762 */
  {
    "movhi",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movhi,
    &operand_data[1134],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3821 */
  {
    "movhi+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1136],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3866 */
  {
    "movqi",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movqi,
    &operand_data[1138],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3913 */
  {
    "movqi+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1140],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3941 */
  {
    "movsf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movsf,
    &operand_data[1142],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:3986 */
  {
    "movdf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movdf,
    &operand_data[1144],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4042 */
  {
    "movtf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movtf,
    &operand_data[1146],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4052 */
  {
    "movtf+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1148],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4066 */
  {
    "movtf+2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1150],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4066 */
  {
    "movtf+3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1152],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4066 */
  {
    "movv2sf-2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1154],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4079 */
  {
    "movv2sf-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1156],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4094 */
  {
    "movv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movv2sf,
    &operand_data[1158],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4140 */
  {
    "mfhilo_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mfhilo_si,
    &operand_data[1008],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4140 */
  {
    "mfhilo_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mfhilo_di,
    &operand_data[1011],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4175 */
  {
    "move_doubleword_fprdf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_move_doubleword_fprdf,
    &operand_data[1144],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4175 */
  {
    "move_doubleword_fprdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_move_doubleword_fprdi,
    &operand_data[1124],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4175 */
  {
    "move_doubleword_fprv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_move_doubleword_fprv2sf,
    &operand_data[1158],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4175 */
  {
    "move_doubleword_fprtf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_move_doubleword_fprtf,
    &operand_data[1146],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4269 */
  {
    "load_const_gp_si",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_load_const_gp_si,
    &operand_data[34],
    1,
    0,
    1,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4269 */
  {
    "load_const_gp_di",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_load_const_gp_di,
    &operand_data[37],
    1,
    0,
    1,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4276 */
  {
    "load_const_gp_di+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1118],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4276 */
  {
    "load_const_gp_di+2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1121],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4295 */
  {
    "load_const_gp_di+3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1118],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4295 */
  {
    "clear_cache-3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1121],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4321 */
  {
    "clear_cache-2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1160],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4321 */
  {
    "clear_cache-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1163],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4359 */
  {
    "clear_cache",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_clear_cache,
    &operand_data[1166],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4637 */
  {
    "movmemsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movmemsi,
    &operand_data[1168],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
  {
    "ashlsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_ashlsi3,
    &operand_data[985],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
  {
    "ashrsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_ashrsi3,
    &operand_data[985],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
  {
    "lshrsi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_lshrsi3,
    &operand_data[985],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
  {
    "ashldi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_ashldi3,
    &operand_data[1172],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
  {
    "ashrdi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_ashrdi3,
    &operand_data[1172],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4657 */
  {
    "lshrdi3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_lshrdi3,
    &operand_data[1172],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
  {
    "lshrdi3+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[991],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
  {
    "lshrdi3+2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[991],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
  {
    "lshrdi3+3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[991],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
  {
    "lshrdi3+4",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[996],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
  {
    "cmpsi-3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[996],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4799 */
  {
    "cmpsi-2",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[996],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4820 */
  {
    "cmpsi-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1175],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4871 */
  {
    "cmpsi",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_cmpsi,
    &operand_data[1178],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4871 */
  {
    "cmpdi",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_cmpdi,
    &operand_data[1180],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4882 */
  {
    "cmpsf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_cmpsf,
    &operand_data[1002],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:4882 */
  {
    "cmpdf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_cmpdf,
    &operand_data[1005],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bunordered",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bunordered,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bordered",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bordered,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bunlt",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bunlt,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bunge",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bunge,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "buneq",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_buneq,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bltgt",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bltgt,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bunle",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bunle,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bungt",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bungt,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "beq",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_beq,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bne",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bne,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bgt",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bgt,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bge",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bge,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "blt",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_blt,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "ble",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_ble,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bgtu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bgtu,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bgeu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bgeu,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bltu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bltu,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5031 */
  {
    "bleu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_bleu,
    &operand_data[552],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5044 */
  {
    "condjump",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_condjump,
    &operand_data[1182],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5057 */
  {
    "seq",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_seq,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5085 */
  {
    "sne",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sne,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5101 */
  {
    "sgt",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sgt,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5126 */
  {
    "sge",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sge,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5142 */
  {
    "slt",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_slt,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5172 */
  {
    "sle",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sle,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5206 */
  {
    "sgtu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sgtu,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5231 */
  {
    "sgeu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sgeu,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5247 */
  {
    "sltu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sltu,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5277 */
  {
    "sleu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sleu,
    &operand_data[742],
    1,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5388 */
  {
    "indirect_jump",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_indirect_jump,
    &operand_data[648],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5407 */
  {
    "tablejump",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_tablejump,
    &operand_data[1184],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5452 */
  {
    "builtin_setjmp_setup",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_builtin_setjmp_setup,
    &operand_data[648],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5467 */
  {
    "builtin_longjmp",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_builtin_longjmp,
    &operand_data[648],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5504 */
  {
    "prologue",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_prologue,
    &operand_data[0],
    0,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5524 */
  {
    "epilogue",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_epilogue,
    &operand_data[0],
    0,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5532 */
  {
    "sibcall_epilogue",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sibcall_epilogue,
    &operand_data[0],
    0,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5561 */
  {
    "eh_return",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_eh_return,
    &operand_data[1186],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5589 */
  {
    "eh_return+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1187],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5599 */
  {
    "exception_receiver",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_exception_receiver,
    &operand_data[0],
    0,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5612 */
  {
    "nonlocal_goto_receiver",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_nonlocal_goto_receiver,
    &operand_data[0],
    0,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5623 */
  {
    "nonlocal_goto_receiver+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[0],
    0,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5727 */
  {
    "sibcall",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sibcall,
    &operand_data[1189],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5745 */
  {
    "sibcall_value",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sibcall_value,
    &operand_data[1189],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5776 */
  {
    "call",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_call,
    &operand_data[1189],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5824 */
  {
    "call+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1193],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5862 */
  {
    "call_value",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_call_value,
    &operand_data[1189],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5875 */
  {
    "call_value+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1195],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5915 */
  {
    "untyped_call-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1195],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:5952 */
  {
    "untyped_call",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_untyped_call,
    &operand_data[1189],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6058 */
  {
    "movsicc",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movsicc,
    &operand_data[1199],
    4,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6058 */
  {
    "movdicc",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movdicc,
    &operand_data[1203],
    4,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6070 */
  {
    "movsfcc",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movsfcc,
    &operand_data[1207],
    4,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6070 */
  {
    "movdfcc",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movdfcc,
    &operand_data[1211],
    4,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md:6124 */
  {
    "movdfcc+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1215],
    1,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:48 */
  {
    "movv2sfcc",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_movv2sfcc,
    &operand_data[1216],
    4,
    2,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:125 */
  {
    "vec_initv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_vec_initv2sf,
    &operand_data[1219],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:173 */
  {
    "vec_setv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_vec_setv2sf,
    &operand_data[1221],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:193 */
  {
    "mips_cvt_ps_s",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mips_cvt_ps_s,
    &operand_data[1224],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:209 */
  {
    "mips_cvt_s_pl",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mips_cvt_s_pl,
    &operand_data[1226],
    2,
    1,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:217 */
  {
    "mips_cvt_s_pu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mips_cvt_s_pu,
    &operand_data[1226],
    2,
    1,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:279 */
  {
    "mips_abs_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mips_abs_ps,
    &operand_data[1218],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:323 */
  {
    "mips_abs_ps+1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1228],
    6,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:352 */
  {
    "scc_ps-1",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    0,
    &operand_data[1228],
    6,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:409 */
  {
    "scc_ps",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_scc_ps,
    &operand_data[1234],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:489 */
  {
    "single_cc",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_single_cc,
    &operand_data[1182],
    2,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:582 */
  {
    "vcondv2sf",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_vcondv2sf,
    &operand_data[1236],
    6,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:597 */
  {
    "sminv2sf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_sminv2sf3,
    &operand_data[1229],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-ps-3d.md:608 */
  {
    "smaxv2sf3",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_smaxv2sf3,
    &operand_data[1229],
    3,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:157 */
  {
    "mips_madd",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mips_madd,
    &operand_data[1012],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:157 */
  {
    "mips_maddu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mips_maddu,
    &operand_data[1012],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:165 */
  {
    "mips_msub",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mips_msub,
    &operand_data[1012],
    4,
    0,
    0,
    0
  },
  /* /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips-dspr2.md:165 */
  {
    "mips_msubu",
#if HAVE_DESIGNATED_INITIALIZERS
    { 0 },
#else
    { 0, 0, 0 },
#endif
    (insn_gen_fn) gen_mips_msubu,
    &operand_data[1012],
    4,
    0,
    0,
    0
  },
};


const char *
get_insn_name (int code)
{
  if (code == NOOP_MOVE_INSN_CODE)
    return "NOOP_MOVE";
  else
    return insn_data[code].name;
}
