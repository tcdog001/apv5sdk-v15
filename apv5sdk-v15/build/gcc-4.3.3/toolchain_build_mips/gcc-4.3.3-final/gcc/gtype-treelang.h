/* Type information for treelang.
   Copyright (C) 2004, 2007 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* This file is machine generated.  Do not edit.  */

void
gt_ggc_mx_prod_token_parm_item (void *x_p)
{
  struct prod_token_parm_item * const x = (struct prod_token_parm_item *)x_p;
  if (ggc_test_and_set_mark (x))
    {
      switch (((item *)&((*x)))->category)
        {
        case token_category:
          break;
        case production_category:
          gt_ggc_m_20prod_token_parm_item ((*x).tp.pro.main_token);
          {
            size_t i1;
            size_t l1 = (size_t)(SUB_COUNT);
            for (i1 = 0; i1 != l1; i1++) {
              gt_ggc_m_20prod_token_parm_item ((*x).tp.pro.sub[i1]);
            }
          }
          gt_ggc_m_9tree_node ((*x).tp.pro.code);
          gt_ggc_m_20prod_token_parm_item ((*x).tp.pro.next);
          break;
        case parameter_category:
          gt_ggc_m_20prod_token_parm_item ((*x).tp.par.next);
          break;
        default:
          break;
        }
    }
}

void
gt_pch_nx_prod_token_parm_item (void *x_p)
{
  struct prod_token_parm_item * const x = (struct prod_token_parm_item *)x_p;
  if (gt_pch_note_object (x, x, gt_pch_p_20prod_token_parm_item, gt_ggc_e_20prod_token_parm_item))
    {
      switch (((item *)&((*x)))->category)
        {
        case token_category:
          gt_pch_n_S ((*x).tp.tok.chars);
          break;
        case production_category:
          gt_pch_n_20prod_token_parm_item ((*x).tp.pro.main_token);
          {
            size_t i1;
            size_t l1 = (size_t)(SUB_COUNT);
            for (i1 = 0; i1 != l1; i1++) {
              gt_pch_n_20prod_token_parm_item ((*x).tp.pro.sub[i1]);
            }
          }
          gt_pch_n_9tree_node ((*x).tp.pro.code);
          gt_pch_n_20prod_token_parm_item ((*x).tp.pro.next);
          break;
        case parameter_category:
          gt_pch_n_20prod_token_parm_item ((*x).tp.par.next);
          gt_pch_n_S ((*x).tp.par.variable_name);
          break;
        default:
          break;
        }
    }
}

void
gt_pch_p_20prod_token_parm_item (ATTRIBUTE_UNUSED void *this_obj,
	void *x_p,
	ATTRIBUTE_UNUSED gt_pointer_operator op,
	ATTRIBUTE_UNUSED void *cookie)
{
  struct prod_token_parm_item * const x ATTRIBUTE_UNUSED = (struct prod_token_parm_item *)x_p;
  switch (((item *)&((*x)))->category)
    {
    case token_category:
      if ((void *)(x) == this_obj)
        op (&((*x).tp.tok.chars), cookie);
      break;
    case production_category:
      if ((void *)(x) == this_obj)
        op (&((*x).tp.pro.main_token), cookie);
      {
        size_t i1;
        size_t l1 = (size_t)(SUB_COUNT);
        for (i1 = 0; i1 != l1; i1++) {
          if ((void *)(x) == this_obj)
            op (&((*x).tp.pro.sub[i1]), cookie);
        }
      }
      if ((void *)(x) == this_obj)
        op (&((*x).tp.pro.code), cookie);
      if ((void *)(x) == this_obj)
        op (&((*x).tp.pro.next), cookie);
      break;
    case parameter_category:
      if ((void *)(x) == this_obj)
        op (&((*x).tp.par.next), cookie);
      if ((void *)(x) == this_obj)
        op (&((*x).tp.par.variable_name), cookie);
      break;
    default:
      break;
    }
}
extern const struct ggc_root_tab gt_ggc_r_gt_coverage_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_caller_save_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_alias_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_cselib_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_cgraph_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_ipa_inline_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_dbxout_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_dwarf2out_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_dwarf2asm_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_dojump_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_emit_rtl_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_except_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_explow_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_expr_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_function_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_gcse_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_optabs_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_regclass_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_cfglayout_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_sdbout_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_stor_layout_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_stringpool_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_varasm_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_mudflap_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_scalar_evolution_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_ssa_address_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_ssa_propagate_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_gimplify_h[];
extern const struct ggc_root_tab gt_ggc_r_gtype_desc_c[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_vect_generic_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_profile_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_nested_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_ipa_reference_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_tree_parloops_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_omp_low_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_varpool_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_targhooks_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_mips_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_passes_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_cgraphunit_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_treelang_tree1_h[];
extern const struct ggc_root_tab gt_ggc_r_gt_treelang_treetree_h[];
const struct ggc_root_tab * const gt_ggc_rtab[] = {
  gt_ggc_r_gt_coverage_h,
  gt_ggc_r_gt_caller_save_h,
  gt_ggc_r_gt_alias_h,
  gt_ggc_r_gt_cselib_h,
  gt_ggc_r_gt_cgraph_h,
  gt_ggc_r_gt_ipa_inline_h,
  gt_ggc_r_gt_dbxout_h,
  gt_ggc_r_gt_dwarf2out_h,
  gt_ggc_r_gt_dwarf2asm_h,
  gt_ggc_r_gt_dojump_h,
  gt_ggc_r_gt_emit_rtl_h,
  gt_ggc_r_gt_except_h,
  gt_ggc_r_gt_explow_h,
  gt_ggc_r_gt_expr_h,
  gt_ggc_r_gt_function_h,
  gt_ggc_r_gt_gcse_h,
  gt_ggc_r_gt_optabs_h,
  gt_ggc_r_gt_regclass_h,
  gt_ggc_r_gt_cfglayout_h,
  gt_ggc_r_gt_sdbout_h,
  gt_ggc_r_gt_stor_layout_h,
  gt_ggc_r_gt_stringpool_h,
  gt_ggc_r_gt_tree_h,
  gt_ggc_r_gt_varasm_h,
  gt_ggc_r_gt_tree_mudflap_h,
  gt_ggc_r_gt_tree_scalar_evolution_h,
  gt_ggc_r_gt_tree_ssa_address_h,
  gt_ggc_r_gt_tree_ssa_propagate_h,
  gt_ggc_r_gt_gimplify_h,
  gt_ggc_r_gtype_desc_c,
  gt_ggc_r_gt_tree_vect_generic_h,
  gt_ggc_r_gt_tree_profile_h,
  gt_ggc_r_gt_tree_nested_h,
  gt_ggc_r_gt_ipa_reference_h,
  gt_ggc_r_gt_tree_parloops_h,
  gt_ggc_r_gt_omp_low_h,
  gt_ggc_r_gt_varpool_h,
  gt_ggc_r_gt_targhooks_h,
  gt_ggc_r_gt_mips_h,
  gt_ggc_r_gt_passes_h,
  gt_ggc_r_gt_cgraphunit_h,
  gt_ggc_r_gt_treelang_tree1_h,
  gt_ggc_r_gt_treelang_treetree_h,
  NULL
};
extern const struct ggc_root_tab gt_ggc_rd_gt_alias_h[];
extern const struct ggc_root_tab gt_ggc_rd_gt_bitmap_h[];
extern const struct ggc_root_tab gt_ggc_rd_gt_emit_rtl_h[];
extern const struct ggc_root_tab gt_ggc_rd_gt_lists_h[];
extern const struct ggc_root_tab gt_ggc_rd_gt_tree_phinodes_h[];
extern const struct ggc_root_tab gt_ggc_rd_gt_tree_iterator_h[];
const struct ggc_root_tab * const gt_ggc_deletable_rtab[] = {
  gt_ggc_rd_gt_alias_h,
  gt_ggc_rd_gt_bitmap_h,
  gt_ggc_rd_gt_emit_rtl_h,
  gt_ggc_rd_gt_lists_h,
  gt_ggc_rd_gt_tree_phinodes_h,
  gt_ggc_rd_gt_tree_iterator_h,
  NULL
};
extern const struct ggc_cache_tab gt_ggc_rc_gt_emit_rtl_h[];
extern const struct ggc_cache_tab gt_ggc_rc_gt_tree_h[];
extern const struct ggc_cache_tab gt_ggc_rc_gt_varasm_h[];
extern const struct ggc_cache_tab gt_ggc_rc_gt_tree_ssa_structalias_h[];
const struct ggc_cache_tab * const gt_ggc_cache_rtab[] = {
  gt_ggc_rc_gt_emit_rtl_h,
  gt_ggc_rc_gt_tree_h,
  gt_ggc_rc_gt_varasm_h,
  gt_ggc_rc_gt_tree_ssa_structalias_h,
  NULL
};
extern const struct ggc_root_tab gt_pch_rc_gt_emit_rtl_h[];
extern const struct ggc_root_tab gt_pch_rc_gt_tree_h[];
extern const struct ggc_root_tab gt_pch_rc_gt_varasm_h[];
extern const struct ggc_root_tab gt_pch_rc_gt_tree_ssa_structalias_h[];
const struct ggc_root_tab * const gt_pch_cache_rtab[] = {
  gt_pch_rc_gt_emit_rtl_h,
  gt_pch_rc_gt_tree_h,
  gt_pch_rc_gt_varasm_h,
  gt_pch_rc_gt_tree_ssa_structalias_h,
  NULL
};
extern const struct ggc_root_tab gt_pch_rs_gt_alias_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_dbxout_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_dwarf2out_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_dwarf2asm_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_emit_rtl_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_except_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_function_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_sdbout_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_tree_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_varasm_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_tree_ssa_address_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_gimplify_h[];
extern const struct ggc_root_tab gt_pch_rs_gtype_desc_c[];
extern const struct ggc_root_tab gt_pch_rs_gt_tree_vect_generic_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_omp_low_h[];
extern const struct ggc_root_tab gt_pch_rs_gt_mips_h[];
const struct ggc_root_tab * const gt_pch_scalar_rtab[] = {
  gt_pch_rs_gt_alias_h,
  gt_pch_rs_gt_dbxout_h,
  gt_pch_rs_gt_dwarf2out_h,
  gt_pch_rs_gt_dwarf2asm_h,
  gt_pch_rs_gt_emit_rtl_h,
  gt_pch_rs_gt_except_h,
  gt_pch_rs_gt_function_h,
  gt_pch_rs_gt_sdbout_h,
  gt_pch_rs_gt_tree_h,
  gt_pch_rs_gt_varasm_h,
  gt_pch_rs_gt_tree_ssa_address_h,
  gt_pch_rs_gt_gimplify_h,
  gt_pch_rs_gtype_desc_c,
  gt_pch_rs_gt_tree_vect_generic_h,
  gt_pch_rs_gt_omp_low_h,
  gt_pch_rs_gt_mips_h,
  NULL
};
