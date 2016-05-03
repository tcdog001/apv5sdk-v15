(define_conditions [
  (-1 "TARGET_64BIT && !TARGET_FIX_R4000
   && !(ZERO_EXTEND == ZERO_EXTEND && TARGET_FIX_VR4120)")
  (-1 "(!ISA_MIPS1) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "(TARGET_MIPS16 && reload_completed && !TARGET_DEBUG_D_MODE
   && GET_CODE (operands[2]) == CONST_INT
   && INTVAL (operands[2]) > 8
   && INTVAL (operands[2]) <= 16) && (TARGET_64BIT)")
  (-1 "(mips_current_loadgp_style () == LOADGP_ABSOLUTE) && (Pmode == DImode)")
  (-1 "(ISA_HAS_NMADD_NMSUB (V2SFmode)
   && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (V2SFmode)
   && !HONOR_NANS (V2SFmode)) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "TARGET_SIBCALLS && SIBLING_CALL_P (insn)")
  (-1 "TARGET_64BIT && !TARGET_FIX_R4000")
  (-1 "(!ISA_HAS_MACCHI) && (TARGET_64BIT)")
  (-1 "((ISA_HAS_FP4) && (Pmode == SImode)) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "(ISA_HAS_PREFETCHX && TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT) && (Pmode == DImode)")
  (-1 "(TARGET_HARD_FLOAT) && (!TARGET_64BIT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "!TARGET_MIPS16
   && (register_operand (operands[0], QImode)
       || reg_or_0_operand (operands[1], QImode))")
  (-1 "ISA_HAS_MUL3")
  (-1 "TARGET_64BIT && !TARGET_MIPS16")
  (-1 "(TARGET_4300_MUL_FIX) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "(TARGET_HARD_FLOAT && ISA_HAS_MXHC1) && (!TARGET_64BIT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "(TARGET_SOFT_FLOAT || TARGET_SINGLE_FLOAT) && !TARGET_MIPS16
   && (register_operand (operands[0], DFmode)
       || reg_or_0_operand (operands[1], DFmode))")
  (-1 "(TARGET_EXPLICIT_RELOCS && !TARGET_XGOT) && (Pmode == SImode)")
  (-1 "TARGET_HARD_FLOAT
   && TARGET_PAIRED_SINGLE_FLOAT
   && TARGET_64BIT
   && (register_operand (operands[0], V2SFmode)
       || reg_or_0_operand (operands[1], V2SFmode))")
  (-1 "reload_completed && !TARGET_DEBUG_D_MODE
   && GP_REG_P (true_regnum (operands[0]))
   && true_regnum (operands[3]) == LO_REGNUM")
  (-1 "TARGET_HARD_FLOAT && TARGET_FLOAT64 && TARGET_DOUBLE_FLOAT")
  (-1 "(ISA_HAS_FP4 && TARGET_FLOAT64 && flag_unsafe_math_optimizations) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "TARGET_CALL_CLOBBERED_GP")
  (-1 "(TARGET_HARD_FLOAT && ISA_HAS_MXHC1) && (TARGET_64BIT && TARGET_FLOAT64)")
  (-1 "ISA_HAS_MULHI || !TARGET_FIX_R4000")
  (-1 "(ISA_HAS_ROR) && (TARGET_64BIT)")
  (-1 "TARGET_MIPS16 && reload_completed && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == CONST_INT
   && ((INTVAL (operands[1]) < 0
	&& INTVAL (operands[1]) >= -0x80)
       || (INTVAL (operands[1]) >= 32
	   && INTVAL (operands[1]) <= 31 + 0x7f))")
  (-1 "(TARGET_64BIT && !TARGET_MIPS16) && ( reload_completed)")
  (-1 "GENERATE_MADD_MSUB")
  (-1 "(!TARGET_MIPS16) && (TARGET_64BIT)")
  (-1 "ISA_HAS_COND_TRAP")
  (-1 "!ISA_HAS_MACCHI")
  (-1 "(TARGET_EXPLICIT_RELOCS && ABI_HAS_64BIT_SYMBOLS && cse_not_expected) && ( reload_completed)")
  (-1 "TARGET_64BIT && !TARGET_MIPS16 && INTVAL (operands[2]) >= 32")
  (0 "(HAVE_AS_TLS && !TARGET_MIPS16) && (Pmode == SImode)")
  (-1 "((!ISA_HAS_SEB_SEH && !GENERATE_MIPS16E) && (TARGET_64BIT)) && ( reload_completed && REG_P (operands[1]))")
  (-1 "TARGET_FIX_R4000")
  (-1 "(TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT) && (TARGET_HARD_FLOAT)")
  (-1 "TARGET_MIPS16 && reload_completed && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == CONST_INT
   && INTVAL (operands[1]) >= 0x100
   && INTVAL (operands[1]) <= 0xff + 0x7f")
  (-1 "(!ISA_HAS_SEB_SEH && !GENERATE_MIPS16E) && ( reload_completed && REG_P (operands[1]))")
  (-1 "(!ISA_MIPS1) && (TARGET_HARD_FLOAT)")
  (-1 "(ISA_HAS_COND_TRAP) && (TARGET_64BIT)")
  (-1 "(!HONOR_NANS (V2SFmode)) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "(ISA_HAS_CLZ_CLO) && (TARGET_64BIT)")
  (-1 "!TARGET_64BIT && TARGET_DOUBLE_FLOAT")
  (-1 "GENERATE_MIPS16E")
  (-1 "(ISA_HAS_FP4 && TARGET_FUSED_MADD) && (TARGET_HARD_FLOAT)")
  (-1 "!TARGET_64BIT && TARGET_MIPS16
   && (register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))")
  (-1 "(TARGET_EXPLICIT_RELOCS && !TARGET_XGOT) && (Pmode == DImode)")
  (-1 "TARGET_MIPS16 && reload_completed && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == CONST_INT
   && ((INTVAL (operands[1]) < 0
	&& INTVAL (operands[1]) >= -0x80)
       || (INTVAL (operands[1]) >= 32 * 2
	   && INTVAL (operands[1]) <= 31 * 2 + 0x7e)
       || (INTVAL (operands[1]) >= 0
	   && INTVAL (operands[1]) < 32 * 2
	   && (INTVAL (operands[1]) & 1) != 0))")
  (-1 "ISA_HAS_DSP")
  (-1 "ISA_HAS_CLZ_CLO")
  (-1 "TARGET_64BIT && TARGET_MIPS16")
  (-1 "(TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "(ISA_HAS_NMADD_NMSUB (DFmode)
   && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (DFmode)
   && !HONOR_NANS (DFmode)) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "TARGET_MIPS16
   && (register_operand (operands[0], SFmode)
       || register_operand (operands[1], SFmode))")
  (-1 "ISA_HAS_MUL3 && peep2_reg_dead_p (2, operands[0])")
  (-1 "(mips_current_loadgp_style () == LOADGP_RTP) && (Pmode == SImode)")
  (-1 "(TARGET_MIPS16) && (Pmode == DImode)")
  (-1 "(TARGET_EXPLICIT_RELOCS) && (Pmode == DImode)")
  (-1 "ISA_HAS_MULHI")
  (-1 "reload_completed && TARGET_SPLIT_CALLS && (operands[2] = insn)")
  (-1 "!ISA_HAS_MULHI && !TARGET_FIX_R4000")
  (-1 "ISA_HAS_MACC && !ISA_HAS_MUL3")
  (-1 "(ISA_HAS_CONDMOVE) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "!TARGET_64BIT && ISA_HAS_MULS")
  (-1 "(TARGET_64BIT && TARGET_MIPS16) && ( reload_completed)")
  (-1 "reload_completed && !TARGET_DEBUG_D_MODE")
  (-1 "(TARGET_EXPLICIT_RELOCS && ABI_HAS_64BIT_SYMBOLS) && ( epilogue_completed)")
  (-1 "TARGET_USE_GOT")
  (-1 "(ISA_HAS_FP4 && TARGET_FUSED_MADD) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "TARGET_HARD_FLOAT && !ISA_HAS_TRUNC_W")
  (-1 "!TARGET_MIPS16 && mips_mem_fits_mode_p (SImode, operands[0])")
  (-1 "! TARGET_64BIT")
  (-1 "ISA_HAS_MACC")
  (-1 "(ISA_HAS_CONDMOVE) && (TARGET_HARD_FLOAT)")
  (-1 "(!TARGET_FIX_VR4120) && (TARGET_64BIT)")
  (-1 "reload_completed && !TARGET_DEBUG_D_MODE
   && GP_REG_P (true_regnum (operands[0]))
   && true_regnum (operands[1]) == LO_REGNUM")
  (-1 "((TARGET_EXPLICIT_RELOCS && TARGET_XGOT) && (Pmode == DImode)) && ( reload_completed)")
  (-1 "TARGET_HARD_FLOAT
   && TARGET_PAIRED_SINGLE_FLOAT
   && !TARGET_64BIT
   && (register_operand (operands[0], V2SFmode)
       || reg_or_0_operand (operands[1], V2SFmode))")
  (-1 "((TARGET_EXPLICIT_RELOCS) && (Pmode == DImode)) && ( reload_completed)")
  (-1 "TARGET_EXPLICIT_RELOCS && ABI_HAS_64BIT_SYMBOLS && cse_not_expected")
  (-1 "(TARGET_EXPLICIT_RELOCS && TARGET_XGOT) && (Pmode == SImode)")
  (-1 "reload_completed && !TARGET_DEBUG_D_MODE
   && GP_REG_P (true_regnum (operands[0]))
   && GP_REG_P (true_regnum (operands[3]))")
  (-1 "(!ISA_HAS_SEB_SEH && !GENERATE_MIPS16E) && (TARGET_64BIT)")
  (-1 "((ISA_HAS_CONDMOVE) && (TARGET_HARD_FLOAT)) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "(GENERATE_LL_SC) && (TARGET_64BIT)")
  (-1 "(!HONOR_NANS (SFmode)) && (TARGET_HARD_FLOAT)")
  (-1 "(ISA_HAS_SEB_SEH) && (TARGET_64BIT)")
  (-1 "!TARGET_64BIT && TARGET_FIX_R4000")
  (-1 "(TARGET_SB1) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "(ISA_HAS_MACCHI) && (TARGET_64BIT)")
  (0 "((HAVE_AS_TLS && !TARGET_MIPS16) && (Pmode == DImode)) && ( reload_completed)")
  (-1 "reload_completed && !TARGET_DEBUG_D_MODE
   && GP_REG_P (true_regnum (operands[0]))
   && GP_REG_P (true_regnum (operands[1]))")
  (-1 "ISA_HAS_DSPR2 && !TARGET_64BIT")
  (-1 "TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT && ISA_HAS_TRUNC_W")
  (-1 "!TARGET_FIX_VR4120")
  (-1 "(TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT) && (TARGET_64BIT)")
  (-1 "((ISA_HAS_CONDMOVE) && (TARGET_64BIT)) && (TARGET_64BIT)")
  (-1 "TARGET_MIPS16
   && (register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))")
  (-1 "TARGET_MIPS16 && reload_completed && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == CONST_INT
   && ((INTVAL (operands[1]) > 0x7f
	&& INTVAL (operands[1]) <= 0x7f + 0x7f)
       || (INTVAL (operands[1]) < - 0x80
	   && INTVAL (operands[1]) >= - 0x80 - 0x80))")
  (-1 "(mips_current_loadgp_style () == LOADGP_RTP) && (Pmode == DImode)")
  (-1 "(!TARGET_FIX_R4000) && (TARGET_64BIT)")
  (-1 "(TARGET_MIPS16) && (TARGET_64BIT)")
  (-1 "((ISA_HAS_FP4) && (Pmode == DImode)) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "(TARGET_HARD_FLOAT) && (!TARGET_64BIT && TARGET_DOUBLE_FLOAT)")
  (-1 "ISA_HAS_SEB_SEH")
  (-1 "(!TARGET_4300_MUL_FIX) && (TARGET_HARD_FLOAT)")
  (-1 "!TARGET_MIPS16")
  (-1 "((ISA_HAS_FP4) && (Pmode == SImode)) && (TARGET_HARD_FLOAT)")
  (-1 "mips_can_use_return_insn ()")
  (-1 "(!TARGET_MIPS16 && mips_mem_fits_mode_p (DImode, operands[0])) && (TARGET_64BIT)")
  (-1 "ISA_HAS_DSP && !TARGET_64BIT")
  (-1 "(TARGET_CALL_CLOBBERED_GP) && ( reload_completed)")
  (-1 "ISA_HAS_SYNCI")
  (-1 "(!TARGET_MIPS16) && (Pmode == DImode)")
  (-1 "(TARGET_SB1 && (!TARGET_FIX_SB1 || flag_unsafe_math_optimizations)) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "ISA_HAS_PREFETCH && TARGET_EXPLICIT_RELOCS")
  (-1 "((ISA_HAS_CONDMOVE) && (TARGET_HARD_FLOAT)) && (TARGET_HARD_FLOAT)")
  (-1 "TARGET_MIPS16
   && (register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))")
  (-1 "TARGET_HARD_FLOAT && TARGET_64BIT && TARGET_DOUBLE_FLOAT")
  (-1 "TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT && !TARGET_64BIT
   && (register_operand (operands[0], DFmode)
       || reg_or_0_operand (operands[1], DFmode))")
  (-1 "(ISA_HAS_NMADD_NMSUB (DFmode)
   && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (DFmode)
   && !HONOR_NANS (DFmode)) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "(TARGET_HARD_FLOAT && ISA_HAS_MXHC1) && (!TARGET_64BIT && TARGET_DOUBLE_FLOAT)")
  (-1 "TARGET_64BIT && TARGET_FLOAT64")
  (-1 "!TARGET_MIPS16 && mips_mem_fits_mode_p (SImode, operands[1])")
  (-1 "TARGET_SIBCALLS")
  (-1 "(mips_use_ins_ext_p (operands[1], INTVAL (operands[2]),
		       INTVAL (operands[3]))) && (TARGET_64BIT)")
  (-1 "TARGET_MIPS16 && reload_completed && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == CONST_INT
   && ((INTVAL (operands[1]) < 0
	&& INTVAL (operands[1]) >= -0x80)
       || (INTVAL (operands[1]) >= 32 * 4
	   && INTVAL (operands[1]) <= 31 * 4 + 0x7c)
       || (INTVAL (operands[1]) >= 0
	   && INTVAL (operands[1]) < 32 * 4
	   && (INTVAL (operands[1]) & 3) != 0))")
  (-1 "ISA_HAS_MUL3 && peep2_reg_dead_p (3, operands[1])")
  (-1 "(TARGET_HARD_FLOAT) && (TARGET_64BIT && TARGET_FLOAT64)")
  (-1 "(!TARGET_MIPS16) && (Pmode == SImode)")
  (-1 "TARGET_MIPS16 && TARGET_64BIT && reload_completed && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && REG_P (operands[1])
   && M16_REG_P (REGNO (operands[1]))
   && REGNO (operands[0]) != REGNO (operands[1])
   && GET_CODE (operands[2]) == CONST_INT
   && ((INTVAL (operands[2]) > 0x7
	&& INTVAL (operands[2]) <= 0x7 + 0xf)
       || (INTVAL (operands[2]) < - 0x8
	   && INTVAL (operands[2]) >= - 0x8 - 0x10))")
  (-1 "ISA_HAS_MACCHI")
  (-1 "ISA_HAS_CONDMOVE")
  (-1 "ISA_HAS_LWXS")
  (-1 "(mips_use_ins_ext_p (operands[0], INTVAL (operands[1]),
		       INTVAL (operands[2]))) && (TARGET_64BIT)")
  (-1 "!TARGET_MIPS16
   && (register_operand (operands[0], SImode)
       || reg_or_0_operand (operands[1], SImode))")
  (-1 "!TARGET_MIPS16 && !TARGET_MEMCPY")
  (-1 "GENERATE_SYNC")
  (-1 "TARGET_64BIT && !TARGET_FIX_R4000
   && !(SIGN_EXTEND == ZERO_EXTEND && TARGET_FIX_VR4120)")
  (-1 "!TARGET_MIPS16
   && (register_operand (operands[0], HImode)
       || reg_or_0_operand (operands[1], HImode))")
  (-1 "(ISA_HAS_CONDMOVE) && (TARGET_64BIT)")
  (-1 "(!HONOR_NANS (DFmode)) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "((ISA_HAS_FP4) && (Pmode == SImode)) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "reload_completed && TARGET_SPLIT_CALLS && (operands[4] = insn)")
  (-1 "(!TARGET_4300_MUL_FIX) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "(TARGET_64BIT && !TARGET_FIX_R4000) && ( reload_completed)")
  (-1 "TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT")
  (-1 "TARGET_SPLIT_CALLS")
  (-1 "ISA_HAS_MUL3
   && true_regnum (operands[1]) == LO_REGNUM
   && peep2_reg_dead_p (2, operands[1])
   && GP_REG_P (true_regnum (operands[3]))")
  (-1 "(TARGET_EXPLICIT_RELOCS && TARGET_XGOT) && (Pmode == DImode)")
  (-1 "TARGET_64BIT && TARGET_MIPS16 && reload_completed
   && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == CONST_INT
   && ((INTVAL (operands[1]) < 0
	&& INTVAL (operands[1]) >= -0x10)
       || (INTVAL (operands[1]) >= 32 * 8
	   && INTVAL (operands[1]) <= 31 * 8 + 0x8)
       || (INTVAL (operands[1]) >= 0
	   && INTVAL (operands[1]) < 32 * 8
	   && (INTVAL (operands[1]) & 7) != 0))")
  (-1 "!TARGET_64BIT && TARGET_FLOAT64 && !TARGET_MIPS16
   && (register_operand (operands[0], DImode)
       || reg_or_0_operand (operands[1], DImode))")
  (-1 "(ISA_HAS_NMADD_NMSUB (SFmode)
   && TARGET_FUSED_MADD
   && HONOR_SIGNED_ZEROS (SFmode)
   && !HONOR_NANS (SFmode)) && (TARGET_HARD_FLOAT)")
  (-1 "TARGET_64BIT && TARGET_MIPS16
   && (register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))")
  (-1 "!TARGET_64BIT || !TARGET_FIX_R4000")
  (-1 "TARGET_SOFT_FLOAT && !TARGET_MIPS16
   && (register_operand (operands[0], SFmode)
       || reg_or_0_operand (operands[1], SFmode))")
  (-1 "TARGET_HARD_FLOAT")
  (-1 "(ISA_HAS_FP4 && flag_unsafe_math_optimizations) && (TARGET_HARD_FLOAT)")
  (-1 "reload_completed && !TARGET_64BIT
   && mips_split_64bit_move_p (operands[0], operands[1])")
  (-1 "ISA_HAS_MACC && reload_completed")
  (-1 "reload_completed && TARGET_SPLIT_CALLS && (operands[3] = insn)")
  (-1 "TARGET_MIPS16 && reload_completed && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && REG_P (operands[1])
   && M16_REG_P (REGNO (operands[1]))
   && REGNO (operands[0]) != REGNO (operands[1])
   && GET_CODE (operands[2]) == CONST_INT
   && ((INTVAL (operands[2]) > 0x7
	&& INTVAL (operands[2]) <= 0x7 + 0x7f)
       || (INTVAL (operands[2]) < - 0x8
	   && INTVAL (operands[2]) >= - 0x8 - 0x80))")
  (-1 "(GENERATE_MIPS16E) && (TARGET_64BIT)")
  (-1 "(ISA_HAS_FP4 && TARGET_FUSED_MADD) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "mips_use_ins_ext_p (operands[1], INTVAL (operands[2]),
		       INTVAL (operands[3]))")
  (-1 "TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT")
  (-1 "ISA_HAS_8CC && TARGET_HARD_FLOAT")
  (-1 "GENERATE_LL_SC")
  (-1 "((TARGET_EXPLICIT_RELOCS && !TARGET_XGOT) && (Pmode == DImode)) && ( reload_completed)")
  (-1 "(mips_split_symbol (operands[2], operands[1], MAX_MACHINE_MODE, NULL)) && (Pmode == SImode)")
  (-1 "!TARGET_64BIT && (ISA_HAS_MSAC || GENERATE_MADD_MSUB || ISA_HAS_DSPR2)")
  (-1 "(TARGET_MAD || ISA_HAS_MACC || GENERATE_MADD_MSUB || ISA_HAS_DSPR2)
   && !TARGET_64BIT")
  (-1 "TARGET_MIPS16 && TARGET_64BIT && reload_completed && !TARGET_DEBUG_D_MODE
   && REG_P (operands[0])
   && M16_REG_P (REGNO (operands[0]))
   && GET_CODE (operands[1]) == CONST_INT
   && ((INTVAL (operands[1]) > 0xf
	&& INTVAL (operands[1]) <= 0xf + 0xf)
       || (INTVAL (operands[1]) < - 0x10
	   && INTVAL (operands[1]) >= - 0x10 - 0x10))")
  (-1 "((ISA_HAS_CONDMOVE) && (TARGET_HARD_FLOAT)) && (TARGET_64BIT)")
  (-1 "!TARGET_FIX_R4000")
  (-1 "(TARGET_USE_GOT) && (Pmode == DImode)")
  (-1 "((ISA_HAS_FP4) && (Pmode == DImode)) && (TARGET_HARD_FLOAT)")
  (-1 "((ISA_HAS_FP4) && (Pmode == DImode)) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "(TARGET_MIPS16) && (Pmode == SImode)")
  (-1 "TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT && !ISA_HAS_TRUNC_W")
  (-1 " reload_completed")
  (-1 "!TARGET_64BIT && !TARGET_FLOAT64 && !TARGET_MIPS16
   && (register_operand (operands[0], DImode)
       || reg_or_0_operand (operands[1], DImode))")
  (-1 "TARGET_64BIT && !TARGET_MIPS16
   && (register_operand (operands[0], DImode)
       || reg_or_0_operand (operands[1], DImode))")
  (-1 "TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT && TARGET_64BIT
   && (register_operand (operands[0], DFmode)
       || reg_or_0_operand (operands[1], DFmode))")
  (-1 "(mips_current_loadgp_style () == LOADGP_ABSOLUTE) && (Pmode == SImode)")
  (-1 "(TARGET_64BIT && !TARGET_MIPS16) && (TARGET_64BIT)")
  (-1 "(!TARGET_MIPS16 && mips_mem_fits_mode_p (DImode, operands[1])) && (TARGET_64BIT)")
  (-1 "((ISA_HAS_CONDMOVE) && (TARGET_64BIT)) && (TARGET_HARD_FLOAT)")
  (-1 "(!TARGET_FIX_SB1 || flag_unsafe_math_optimizations) && (TARGET_HARD_FLOAT)")
  (-1 "!TARGET_64BIT && !TARGET_FIX_R4000 && !ISA_HAS_DSPR2")
  (-1 "(ISA_HAS_PREFETCHX && TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT) && (Pmode == SImode)")
  (-1 "TARGET_EXPLICIT_RELOCS && ABI_HAS_64BIT_SYMBOLS")
  (-1 "TARGET_HARD_FLOAT
   && (register_operand (operands[0], SFmode)
       || reg_or_0_operand (operands[1], SFmode))")
  (-1 "TARGET_MAD")
  (-1 "(ISA_HAS_NMADD_NMSUB (SFmode)
   && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (SFmode)
   && !HONOR_NANS (SFmode)) && (TARGET_HARD_FLOAT)")
  (-1 "(reload_completed && !TARGET_64BIT
   && mips_split_64bit_move_p (operands[0], operands[1])) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "((TARGET_EXPLICIT_RELOCS && TARGET_XGOT) && (Pmode == SImode)) && ( reload_completed)")
  (-1 "((ISA_HAS_CONDMOVE) && (TARGET_64BIT)) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "TARGET_MIPS16
   && (register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode))")
  (-1 "(TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT) && ( reload_completed)")
  (-1 "(TARGET_64BIT) && ( reload_completed && REG_P (operands[1]))")
  (-1 "(TARGET_FIX_R4000) && (TARGET_64BIT)")
  (-1 "(ISA_HAS_NMADD_NMSUB (V2SFmode)
   && TARGET_FUSED_MADD
   && !HONOR_SIGNED_ZEROS (V2SFmode)
   && !HONOR_NANS (V2SFmode)) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "ISA_HAS_MACC && !ISA_HAS_MSAC")
  (-1 "((TARGET_EXPLICIT_RELOCS && !TARGET_XGOT) && (Pmode == SImode)) && ( reload_completed)")
  (-1 "operands[1] == stack_pointer_rtx
   && mips16e_save_restore_pattern_p (operands[0], INTVAL (operands[2]), NULL)")
  (-1 "(TARGET_USE_GOT) && (Pmode == SImode)")
  (-1 "Pmode == SImode")
  (-1 "(mips_current_loadgp_style () == LOADGP_NEWABI) && (Pmode == SImode)")
  (-1 "mips_use_ins_ext_p (operands[0], INTVAL (operands[1]),
		       INTVAL (operands[2]))")
  (-1 "((TARGET_EXPLICIT_RELOCS) && (Pmode == SImode)) && ( reload_completed)")
  (0 "((HAVE_AS_TLS && !TARGET_MIPS16) && (Pmode == SImode)) && ( reload_completed)")
  (-1 "Pmode == DImode")
  (-1 "ISA_HAS_DSPR2")
  (-1 "TARGET_MIPS16 && reload_completed && !TARGET_DEBUG_D_MODE
   && GET_CODE (operands[2]) == CONST_INT
   && INTVAL (operands[2]) > 8
   && INTVAL (operands[2]) <= 16")
  (-1 "!TARGET_64BIT && TARGET_PAIRED_SINGLE_FLOAT")
  (-1 "!ISA_HAS_SEB_SEH && !GENERATE_MIPS16E")
  (-1 "TARGET_MIPS16
   && (register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))")
  (-1 "(TARGET_SB1 && flag_unsafe_math_optimizations) && (TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT)")
  (-1 "ISA_HAS_ROR")
  (-1 "(ISA_HAS_MACC && !ISA_HAS_MSAC) && ( reload_completed)")
  (-1 "TARGET_HARD_FLOAT && ISA_HAS_TRUNC_W")
  (-1 "(TARGET_4300_MUL_FIX) && (TARGET_HARD_FLOAT)")
  (-1 "(mips_current_loadgp_style () == LOADGP_NEWABI) && (Pmode == DImode)")
  (-1 "ISA_HAS_MSAC")
  (-1 "TARGET_MIPS16 && reload_completed")
  (-1 "TARGET_MIPS16 && reload_completed && INTVAL (operands[1]) < 0")
  (-1 "(TARGET_MIPS3900
   || GENERATE_MADD_MSUB)
   && !TARGET_MIPS16")
  (-1 "ISA_HAS_MSAC && reload_completed")
  (-1 "(TARGET_HARD_FLOAT && TARGET_PAIRED_SINGLE_FLOAT) && (TARGET_HARD_FLOAT && TARGET_DOUBLE_FLOAT)")
  (-1 "(ISA_HAS_DSP) && (ISA_HAS_DSP)")
  (-1 "(TARGET_64BIT) && ( reload_completed && register_operand (operands[1], VOIDmode))")
  (0 "(HAVE_AS_TLS && !TARGET_MIPS16) && (Pmode == DImode)")
  (-1 "(TARGET_EXPLICIT_RELOCS) && (Pmode == SImode)")
  (-1 "ISA_HAS_MULS")
  (-1 "TARGET_MIPS16")
  (-1 "(mips_split_symbol (operands[2], operands[1], MAX_MACHINE_MODE, NULL)) && (Pmode == DImode)")
  (-1 "TARGET_64BIT")
  (-1 "reload_completed")
])
