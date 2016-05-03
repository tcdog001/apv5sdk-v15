/* 
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the elliptic curve math library.
 *
 * The Initial Developer of the Original Code is
 * Sun Microsystems, Inc.
 * Portions created by the Initial Developer are Copyright (C) 2003
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Stephen Fung <fungstep@hotmail.com> and
 *   Douglas Stebila <douglas@stebila.ca>, Sun Microsystems Laboratories
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "mpi.h"
//#include "mp_gf2m.h"
#include "ecl-priv.h"
#include <stdlib.h>

/* Allocate memory for a new GFMethod object. */
GFMethod *
GFMethod_new()
{
	mp_err res = MP_OKAY;
	GFMethod *meth;
	meth = (GFMethod *) malloc(sizeof(GFMethod));
	if (meth == NULL)
		return NULL;
	meth->constructed = MP_YES;
	MP_CHECKOK(mp_init(&meth->irr));
	meth->extra_free = NULL;

  CLEANUP:
	if (res != MP_OKAY) {
		GFMethod_free(meth);
		return NULL;
	}
	return meth;
}

/* Construct a generic GFMethod for arithmetic over prime fields with
 * irreducible irr. */
GFMethod *
GFMethod_consGFp(const mp_int *irr)
{
	mp_err res = MP_OKAY;
	GFMethod *meth = NULL;

	meth = GFMethod_new();
	if (meth == NULL)
		return NULL;

	MP_CHECKOK(mp_copy(irr, &meth->irr));
	meth->irr_arr[0] = mpl_significant_bits(irr);
	meth->irr_arr[1] = meth->irr_arr[2] = meth->irr_arr[3] =
		meth->irr_arr[4] = 0;
	meth->field_add = &ec_GFp_add;
	meth->field_neg = &ec_GFp_neg;
	meth->field_sub = &ec_GFp_sub;
	meth->field_mod = &ec_GFp_mod;
	meth->field_mul = &ec_GFp_mul;
	meth->field_sqr = &ec_GFp_sqr;
	meth->field_div = &ec_GFp_div;
	meth->field_enc = NULL;
	meth->field_dec = NULL;
	meth->extra1 = NULL;
	meth->extra2 = NULL;
	meth->extra_free = NULL;

  CLEANUP:
	if (res != MP_OKAY) {
		GFMethod_free(meth);
		return NULL;
	}
	return meth;
}
/* Free the memory allocated (if any) to a GFMethod object. */
void
GFMethod_free(GFMethod *meth)
{
	if (meth == NULL)
		return;
	if (meth->constructed == MP_NO)
		return;
	if (meth->extra_free != NULL)
		meth->extra_free(meth);
	free(meth);
}

/* Wrapper functions for generic prime field arithmetic. */

/* Add two field elements.  Assumes that 0 <= a, b < meth->irr */
mp_err
ec_GFp_add(const mp_int *a, const mp_int *b, mp_int *r,
		   const GFMethod *meth)
{
	/* PRE: 0 <= a, b < p = meth->irr POST: 0 <= r < p, r = a + b (mod p) */
	mp_err res;

	if ((res = mp_add(a, b, r)) != MP_OKAY) {
		return res;
	}
	if (mp_cmp(r, &meth->irr) >= 0) {
		return mp_sub(r, &meth->irr, r);
	}
	return res;
}

/* Negates a field element.  Assumes that 0 <= a < meth->irr */
mp_err
ec_GFp_neg(const mp_int *a, mp_int *r, const GFMethod *meth)
{
	/* PRE: 0 <= a < p = meth->irr POST: 0 <= r < p, r = -a (mod p) */

	if (mp_cmp_z(a) == 0) {
		mp_zero(r);
		return MP_OKAY;
	}
	return mp_sub(&meth->irr, a, r);
}

/* Subtracts two field elements.  Assumes that 0 <= a, b < meth->irr */
mp_err
ec_GFp_sub(const mp_int *a, const mp_int *b, mp_int *r,
		   const GFMethod *meth)
{
	mp_err res = MP_OKAY;

	/* PRE: 0 <= a, b < p = meth->irr POST: 0 <= r < p, r = a - b (mod p) */
	res = mp_sub(a, b, r);
	if (res == MP_RANGE) {
		MP_CHECKOK(mp_sub(b, a, r));
		if (mp_cmp_z(r) < 0) {
			MP_CHECKOK(mp_add(r, &meth->irr, r));
		}
		MP_CHECKOK(ec_GFp_neg(r, r, meth));
	}
	if (mp_cmp_z(r) < 0) {
		MP_CHECKOK(mp_add(r, &meth->irr, r));
	}
  CLEANUP:
	return res;
}

/* Reduces an integer to a field element. */
mp_err
ec_GFp_mod(const mp_int *a, mp_int *r, const GFMethod *meth)
{
	return mp_mod(a, &meth->irr, r);
}

/* Multiplies two field elements. */
mp_err
ec_GFp_mul(const mp_int *a, const mp_int *b, mp_int *r,
		   const GFMethod *meth)
{
	return mp_mulmod(a, b, &meth->irr, r);
}

/* Squares a field element. */
mp_err
ec_GFp_sqr(const mp_int *a, mp_int *r, const GFMethod *meth)
{
	return mp_sqrmod(a, &meth->irr, r);
}

/* Divides two field elements. If a is NULL, then returns the inverse of
 * b. */
mp_err
ec_GFp_div(const mp_int *a, const mp_int *b, mp_int *r,
		   const GFMethod *meth)
{
	mp_err res = MP_OKAY;
	mp_int t;

	/* If a is NULL, then return the inverse of b, otherwise return a/b. */
	if (a == NULL) {
		return mp_invmod(b, &meth->irr, r);
	} else {
		/* MPI doesn't support divmod, so we implement it using invmod and 
		 * mulmod. */
		MP_CHECKOK(mp_init(&t));
		MP_CHECKOK(mp_invmod(b, &meth->irr, &t));
		MP_CHECKOK(mp_mulmod(a, &t, &meth->irr, r));
	  CLEANUP:
		mp_clear(&t);
		return res;
	}
}

