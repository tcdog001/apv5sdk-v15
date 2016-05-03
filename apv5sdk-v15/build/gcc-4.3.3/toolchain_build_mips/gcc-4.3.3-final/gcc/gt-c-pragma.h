/* Type information for c-pragma.c.
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
gt_ggc_mx_def_pragma_macro (void *x_p)
{
  struct def_pragma_macro * const x = (struct def_pragma_macro *)x_p;
  if (ggc_test_and_set_mark (x))
    {
      gt_ggc_m_22def_pragma_macro_value ((*x).value.prev);
      gt_ggc_m_9cpp_macro ((*x).value.value);
    }
}

void
gt_ggc_mx_def_pragma_macro_value (void *x_p)
{
  struct def_pragma_macro_value * const x = (struct def_pragma_macro_value *)x_p;
  if (ggc_test_and_set_mark (x))
    {
      gt_ggc_m_22def_pragma_macro_value ((*x).prev);
      gt_ggc_m_9cpp_macro ((*x).value);
    }
}

void
gt_ggc_mx_align_stack (void *x_p)
{
  struct align_stack * const x = (struct align_stack *)x_p;
  if (ggc_test_and_set_mark (x))
    {
      gt_ggc_m_9tree_node ((*x).id);
      gt_ggc_m_11align_stack ((*x).prev);
    }
}

void
gt_ggc_m_P16def_pragma_macro4htab (void *x_p)
{
  struct htab * const x = (struct htab *)x_p;
  if (ggc_test_and_set_mark (x))
    {
      if ((*x).entries != NULL) {
        size_t i0;
        for (i0 = 0; i0 != (size_t)(((*x)).size); i0++) {
          gt_ggc_m_16def_pragma_macro ((*x).entries[i0]);
        }
        ggc_mark ((*x).entries);
      }
    }
}

void
gt_pch_nx_def_pragma_macro (void *x_p)
{
  struct def_pragma_macro * const x = (struct def_pragma_macro *)x_p;
  if (gt_pch_note_object (x, x, gt_pch_p_16def_pragma_macro, gt_ggc_e_16def_pragma_macro))
    {
      gt_pch_n_S ((*x).name);
      gt_pch_n_22def_pragma_macro_value ((*x).value.prev);
      gt_pch_n_9cpp_macro ((*x).value.value);
    }
}

void
gt_pch_nx_def_pragma_macro_value (void *x_p)
{
  struct def_pragma_macro_value * const x = (struct def_pragma_macro_value *)x_p;
  if (gt_pch_note_object (x, x, gt_pch_p_22def_pragma_macro_value, gt_ggc_e_22def_pragma_macro_value))
    {
      gt_pch_n_22def_pragma_macro_value ((*x).prev);
      gt_pch_n_9cpp_macro ((*x).value);
    }
}

void
gt_pch_nx_align_stack (void *x_p)
{
  struct align_stack * const x = (struct align_stack *)x_p;
  if (gt_pch_note_object (x, x, gt_pch_p_11align_stack, gt_ggc_e_11align_stack))
    {
      gt_pch_n_9tree_node ((*x).id);
      gt_pch_n_11align_stack ((*x).prev);
    }
}

void
gt_pch_n_P16def_pragma_macro4htab (void *x_p)
{
  struct htab * const x = (struct htab *)x_p;
  if (gt_pch_note_object (x, x, gt_pch_p_P16def_pragma_macro4htab, gt_e_P16def_pragma_macro4htab))
    {
      if ((*x).entries != NULL) {
        size_t i0;
        for (i0 = 0; i0 != (size_t)(((*x)).size); i0++) {
          gt_pch_n_16def_pragma_macro ((*x).entries[i0]);
        }
        gt_pch_note_object ((*x).entries, x, gt_pch_p_P16def_pragma_macro4htab, gt_types_enum_last);
      }
    }
}

void
gt_pch_p_16def_pragma_macro (ATTRIBUTE_UNUSED void *this_obj,
	void *x_p,
	ATTRIBUTE_UNUSED gt_pointer_operator op,
	ATTRIBUTE_UNUSED void *cookie)
{
  struct def_pragma_macro * const x ATTRIBUTE_UNUSED = (struct def_pragma_macro *)x_p;
  if ((void *)(x) == this_obj)
    op (&((*x).name), cookie);
  if ((void *)(x) == this_obj)
    op (&((*x).value.prev), cookie);
  if ((void *)(x) == this_obj)
    op (&((*x).value.value), cookie);
}

void
gt_pch_p_22def_pragma_macro_value (ATTRIBUTE_UNUSED void *this_obj,
	void *x_p,
	ATTRIBUTE_UNUSED gt_pointer_operator op,
	ATTRIBUTE_UNUSED void *cookie)
{
  struct def_pragma_macro_value * const x ATTRIBUTE_UNUSED = (struct def_pragma_macro_value *)x_p;
  if ((void *)(x) == this_obj)
    op (&((*x).prev), cookie);
  if ((void *)(x) == this_obj)
    op (&((*x).value), cookie);
}

void
gt_pch_p_11align_stack (ATTRIBUTE_UNUSED void *this_obj,
	void *x_p,
	ATTRIBUTE_UNUSED gt_pointer_operator op,
	ATTRIBUTE_UNUSED void *cookie)
{
  struct align_stack * const x ATTRIBUTE_UNUSED = (struct align_stack *)x_p;
  if ((void *)(x) == this_obj)
    op (&((*x).id), cookie);
  if ((void *)(x) == this_obj)
    op (&((*x).prev), cookie);
}

void
gt_pch_p_P16def_pragma_macro4htab (ATTRIBUTE_UNUSED void *this_obj,
	void *x_p,
	ATTRIBUTE_UNUSED gt_pointer_operator op,
	ATTRIBUTE_UNUSED void *cookie)
{
  struct htab * const x ATTRIBUTE_UNUSED = (struct htab *)x_p;
  if ((*x).entries != NULL) {
    size_t i0;
    for (i0 = 0; i0 != (size_t)(((*x)).size); i0++) {
      if ((void *)((*x).entries) == this_obj)
        op (&((*x).entries[i0]), cookie);
    }
    if ((void *)(x) == this_obj)
      op (&((*x).entries), cookie);
  }
}

/* GC roots.  */

const struct ggc_root_tab gt_ggc_r_gt_c_pragma_h[] = {
  {
    &pragma_extern_prefix,
    1,
    sizeof (pragma_extern_prefix),
    &gt_ggc_mx_tree_node,
    &gt_pch_nx_tree_node
  },
  {
    &pending_redefine_extname,
    1,
    sizeof (pending_redefine_extname),
    &gt_ggc_mx_tree_node,
    &gt_pch_nx_tree_node
  },
  {
    &pending_weaks,
    1,
    sizeof (pending_weaks),
    &gt_ggc_mx_tree_node,
    &gt_pch_nx_tree_node
  },
  {
    &pushed_macro_table,
    1,
    sizeof (pushed_macro_table),
    &gt_ggc_m_P16def_pragma_macro4htab,
    &gt_pch_n_P16def_pragma_macro4htab
  },
  {
    &alignment_stack,
    1,
    sizeof (alignment_stack),
    &gt_ggc_mx_align_stack,
    &gt_pch_nx_align_stack
  },
  LAST_GGC_ROOT_TAB
};

