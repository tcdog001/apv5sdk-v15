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
#include "mplogic.h"
#include "ecl.h"
#include "ecl-priv.h"
#include <stdlib.h>
#include <strings.h>

/* Elliptic curve scalar-point multiplication. Computes R(x, y) = k * P(x, 
 * y).  If x, y = NULL, then P is assumed to be the generator (base point) 
 * of the group of points on the elliptic curve. Input and output values
 * are assumed to be NOT field-encoded. */
mp_err
ECPoint_mul(const ECGroup *group, const mp_int *k, const mp_int *px,
			const mp_int *py, mp_int *rx, mp_int *ry)
{
	mp_err res = MP_OKAY;
	mp_int kt;

	ARGCHK((k != NULL) && (group != NULL), MP_BADARG);
	MP_DIGITS(&kt) = 0;

	/* want scalar to be less than or equal to group order */
	if (mp_cmp(k, &group->order) >= 0) {
		MP_CHECKOK(mp_init(&kt));
		MP_CHECKOK(mp_mod(k, &group->order, &kt));
	} else {
		MP_SIGN(&kt) = MP_ZPOS;
		MP_USED(&kt) = MP_USED(k);
		MP_ALLOC(&kt) = MP_ALLOC(k);
		MP_DIGITS(&kt) = MP_DIGITS(k);
	}

	if ((px == NULL) || (py == NULL)) {
		if (group->base_point_mul) {
			MP_CHECKOK(group->base_point_mul(&kt, rx, ry, group));
		} else {
			MP_CHECKOK(group->
					   point_mul(&kt, &group->genx, &group->geny, rx, ry,
								 group));
		}
	} else {
		if (group->meth->field_enc) {
			MP_CHECKOK(group->meth->field_enc(px, rx, group->meth));
			MP_CHECKOK(group->meth->field_enc(py, ry, group->meth));
			MP_CHECKOK(group->point_mul(&kt, rx, ry, rx, ry, group));
		} else {
			MP_CHECKOK(group->point_mul(&kt, px, py, rx, ry, group));
		}
	}
	if (group->meth->field_dec) {
		MP_CHECKOK(group->meth->field_dec(rx, rx, group->meth));
		MP_CHECKOK(group->meth->field_dec(ry, ry, group->meth));
	}

  CLEANUP:
	if (MP_DIGITS(&kt) != MP_DIGITS(k)) {
		mp_clear(&kt);
	}
	return res;
}


/* Elliptic curve scalar-point multiplication. Computes R(x, y) = k1 * G + 
 * k2 * P(x, y), where G is the generator (base point) of the group of
 * points on the elliptic curve. Allows k1 = NULL or { k2, P } = NULL.
 * Input and output values are assumed to be NOT field-encoded. */
mp_err
ECPoints_mul(const ECGroup *group, const mp_int *k1, const mp_int *k2,
			 const mp_int *px, const mp_int *py, mp_int *rx, mp_int *ry)
{
	mp_err res = MP_OKAY;
	mp_int k1t, k2t;
	const mp_int *k1p = NULL, *k2p = NULL;

	MP_DIGITS(&k1t) = 0;
	MP_DIGITS(&k2t) = 0;

	ARGCHK(group != NULL, MP_BADARG);

	/* want scalar to be less than or equal to group order */
	if (k1 != NULL) {
		if (mp_cmp(k1, &group->order) >= 0) {
			MP_CHECKOK(mp_init(&k1t));
			MP_CHECKOK(mp_mod(k1, &group->order, &k1t));
			k1p = &k1t;
		} else {
			k1p = k1;
		}
	} else {
		k1p = k1;
	}
	if (k2 != NULL) {
		if (mp_cmp(k2, &group->order) >= 0) {
			MP_CHECKOK(mp_init(&k2t));
			MP_CHECKOK(mp_mod(k2, &group->order, &k2t));
			k2p = &k2t;
		} else {
			k2p = k2;
		}
	} else {
		k2p = k2;
	}

	/* if points_mul is defined, then use it */
	if (group->points_mul) {
		return group->points_mul(k1p, k2p, px, py, rx, ry, group);
	} else {
		goto CLEANUP;
	}

  CLEANUP:
	if (k1 != k1p) {
		mp_clear(&k1t);
	}
	if (k2 != k2p) {
		mp_clear(&k2t);
	}
	return res;
}
