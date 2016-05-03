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

#include "ecl.h"
#include "ecl-curve.h"
#include "ecl-priv.h"
#include <stdlib.h>
#include <string.h>

#define CHECK(func) if ((func) == NULL) { res = 0; goto CLEANUP; }
/* mapping between ECCurveName enum and pointers to ECCurveParams */
static const ECCurveParams ecCurve_X9_62_PRIME_192V4 = {
	"X9.62 P-192V4", ECField_GFp, 192,
	"BDB6F4FE3E8B1D9E0DA8C0D46F4C318CEFE4AFE3B6B8551F",
	"BB8E5E8FBC115E139FE6A814FE48AAA6F0ADA1AA5DF91985",
	"1854BEBDC31B21B7AEFC80AB0ECD10D5B1B3308E6DBF11C1",
	"4AD5F7048DE709AD51236DE65E4D4B482C836DC6E4106640",
	"02BB3A02D4AAADACAE24817A4CA3A1B014B5270432DB27D2",
	"BDB6F4FE3E8B1D9E0DA8C0D40FC962195DFAE76F56564677",1,
	};

const ECCurveParams *ecCurve_map[] = {
	NULL,						/* ECCurve_noName */
	&ecCurve_X9_62_PRIME_192V4,	/* ECCurve_X9_62_PRIME_192V4 */
	NULL						/* ECCurve_pastLastCurve */
};

/* Duplicates an ECCurveParams */
ECCurveParams *
ECCurveParams_dup(const ECCurveParams * params)
{
	int res = 1;
	ECCurveParams *ret = NULL;

	CHECK(ret = (ECCurveParams *) malloc(sizeof(ECCurveParams)));
	if (params->text != NULL) {
		CHECK(ret->text = strdup(params->text));
	}
	ret->field = params->field;
	ret->size = params->size;
	if (params->irr != NULL) {
		CHECK(ret->irr = strdup(params->irr));
	}
	if (params->curvea != NULL) {
		CHECK(ret->curvea = strdup(params->curvea));
	}
	if (params->curveb != NULL) {
		CHECK(ret->curveb = strdup(params->curveb));
	}
	if (params->genx != NULL) {
		CHECK(ret->genx = strdup(params->genx));
	}
	if (params->geny != NULL) {
		CHECK(ret->geny = strdup(params->geny));
	}
	if (params->order != NULL) {
		CHECK(ret->order = strdup(params->order));
	}
	ret->cofactor = params->cofactor;

  CLEANUP:
	if (res != 1) {
		EC_FreeCurveParams(ret);
		return NULL;
	}
	return ret;
}

#undef CHECK

/* Construct ECCurveParams from an ECCurveName */
ECCurveParams *
EC_GetNamedCurveParams(const ECCurveName name)
{
	if ((name <= ECCurve_noName) || (ECCurve_pastLastCurve <= name)) {
		return NULL;
	} else {
		return ECCurveParams_dup(ecCurve_map[name]);
	}
}

/* Free the memory allocated (if any) to an ECCurveParams object. */
void
EC_FreeCurveParams(ECCurveParams * params)
{
	if (params == NULL)
		return;
	if (params->text != NULL)
		free(params->text);
	if (params->irr != NULL)
		free(params->irr);
	if (params->curvea != NULL)
		free(params->curvea);
	if (params->curveb != NULL)
		free(params->curveb);
	if (params->genx != NULL)
		free(params->genx);
	if (params->geny != NULL)
		free(params->geny);
	if (params->order != NULL)
		free(params->order);
	free(params);
}

