/* Generated automatically by the program 'build/genpreds'
   from the machine description file '/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/mips.md'.  */

#ifndef GCC_TM_CONSTRS_H
#define GCC_TM_CONSTRS_H

static inline bool
satisfies_constraint_kf (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return force_to_mem_operand (op, mode);
}
static inline bool
satisfies_constraint_ks (rtx op)
{
  return (GET_CODE (op) == REG) && (
#line 97 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(REGNO (op) == STACK_POINTER_REGNUM));
}
static inline bool
satisfies_constraint_I (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 104 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(SMALL_OPERAND (ival)));
}
static inline bool
satisfies_constraint_J (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 109 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(ival == 0));
}
static inline bool
satisfies_constraint_K (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 114 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(SMALL_OPERAND_UNSIGNED (ival)));
}
static inline bool
satisfies_constraint_L (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 120 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(LUI_OPERAND (ival)));
}
static inline bool
satisfies_constraint_M (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && ((
#line 126 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(!SMALL_OPERAND (ival))) && ((
#line 127 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(!SMALL_OPERAND_UNSIGNED (ival))) && (
#line 128 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(!LUI_OPERAND (ival)))));
}
static inline bool
satisfies_constraint_N (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 133 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(ival >= -0xffff && ival < 0));
}
static inline bool
satisfies_constraint_O (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 138 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(ival >= -0x4000 && ival < 0x4000));
}
static inline bool
satisfies_constraint_P (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 143 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(ival > 0 && ival < 0x10000));
}
static inline bool
satisfies_constraint_G (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return (GET_CODE (op) == CONST_DOUBLE) && (
#line 150 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(op == CONST0_RTX (mode)));
}
static inline bool
satisfies_constraint_Q (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return const_arith_operand (op, mode);
}
static inline bool
satisfies_constraint_R (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return (GET_CODE (op) == MEM) && (
#line 161 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(mips_address_insns (XEXP (op, 0), mode, false) == 1));
}
static inline bool
satisfies_constraint_S (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return (call_insn_operand (op, mode)) && (
#line 167 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(CONSTANT_P (op)));
}
static inline bool
satisfies_constraint_T (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return (move_operand (op, mode)) && ((
#line 174 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(CONSTANT_P (op))) && (
#line 175 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(mips_dangerous_for_la25_p (op))));
}
static inline bool
satisfies_constraint_U (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return (move_operand (op, mode)) && ((
#line 182 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(CONSTANT_P (op))) && (
#line 183 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(!mips_dangerous_for_la25_p (op))));
}
static inline bool
satisfies_constraint_W (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return (GET_CODE (op) == MEM) && ((memory_operand (op, mode)) && ((
#line 193 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(!TARGET_MIPS16)) || ((!(stack_operand (op, mode))) && (!(
#line 195 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(CONSTANT_P (XEXP (op, 0))))))));
}
static inline bool
satisfies_constraint_YG (rtx op)
{
enum machine_mode mode = GET_MODE (op);
  return (GET_CODE (op) == CONST_VECTOR) && (
#line 201 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(op == CONST0_RTX (mode)));
}
static inline bool
satisfies_constraint_YA (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 207 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(UIMM6_OPERAND (ival)));
}
static inline bool
satisfies_constraint_YB (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (GET_CODE (op) == CONST_INT)
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 213 "/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/gcc-4.3.3/gcc/config/mips/constraints.md"
(IMM10_OPERAND (ival)));
}
#endif /* tm-constrs.h */
