/* Generated automatically by the program `genextract'
   from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "insn-config.h"
#include "recog.h"
#include "toplev.h"

/* This variable is used as the "location" of any missing operand
   whose numbers are skipped by a given pattern.  */
static rtx junk ATTRIBUTE_UNUSED;

void
insn_extract (rtx insn)
{
  rtx *ro = recog_data.operand;
  rtx **ro_loc = recog_data.operand_loc;
  rtx pat = PATTERN (insn);
  int i ATTRIBUTE_UNUSED; /* only for peepholes */

#ifdef ENABLE_CHECKING
  memset (ro, 0xab, sizeof (*ro) * MAX_RECOG_OPERANDS);
  memset (ro_loc, 0xab, sizeof (*ro_loc) * MAX_RECOG_OPERANDS);
#endif

  switch (INSN_CODE (insn))
    {
    default:
      /* Control reaches here if insn_extract has been called with an
         unrecognizable insn (code -1), or an insn whose INSN_CODE
         corresponds to a DEFINE_EXPAND in the machine description;
         either way, a bug.  */
      if (INSN_CODE (insn) < 0)
        fatal_insn ("unrecognizable insn:", insn);
      else
        fatal_insn ("insn with invalid code number:", insn);

    case 697:  /* ssmsubsqdq4 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 2);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 696:  /* ssmaddsqdq4 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 2);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 695:  /* ssmulsq3 */
    case 694:  /* ssmulhq3 */
    case 693:  /* ssmulv2hq3 */
    case 608:  /* mulv2hi3 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 2), 0));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[1] = 1;
      break;

    case 595:  /* mips_bposge */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 593:  /* mips_lhx */
    case 592:  /* mips_lbux */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      break;

    case 590:  /* mips_wrdsp */
      ro[0] = *(ro_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 1));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 5), 1), 0, 1);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 5), 1), 0, 0);
      recog_data.dup_num[1] = 0;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 4), 1), 0, 1);
      recog_data.dup_num[2] = 1;
      recog_data.dup_loc[3] = &XVECEXP (XEXP (XVECEXP (pat, 0, 4), 1), 0, 0);
      recog_data.dup_num[3] = 0;
      recog_data.dup_loc[4] = &XVECEXP (XEXP (XVECEXP (pat, 0, 3), 1), 0, 1);
      recog_data.dup_num[4] = 1;
      recog_data.dup_loc[5] = &XVECEXP (XEXP (XVECEXP (pat, 0, 3), 1), 0, 0);
      recog_data.dup_num[5] = 0;
      recog_data.dup_loc[6] = &XVECEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0, 1);
      recog_data.dup_num[6] = 1;
      recog_data.dup_loc[7] = &XVECEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0, 0);
      recog_data.dup_num[7] = 0;
      recog_data.dup_loc[8] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[8] = 1;
      recog_data.dup_loc[9] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[9] = 0;
      break;

    case 587:  /* mips_extpdp */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 1));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0, 1);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 2), 1), 0, 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[2] = 2;
      recog_data.dup_loc[3] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[3] = 1;
      break;

    case 575:  /* mips_cmpu_le_qb */
    case 574:  /* mips_cmp_le_ph */
    case 573:  /* mips_cmpu_lt_qb */
    case 572:  /* mips_cmp_lt_ph */
    case 571:  /* mips_cmpu_eq_qb */
    case 570:  /* mips_cmp_eq_ph */
      ro[0] = *(ro_loc[0] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 1));
      break;

    case 640:  /* mips_dpsqx_sa_w_ph */
    case 639:  /* mips_dpsqx_s_w_ph */
    case 638:  /* mips_dpaqx_sa_w_ph */
    case 637:  /* mips_dpaqx_s_w_ph */
    case 565:  /* mips_maq_sa_w_phr */
    case 564:  /* mips_maq_sa_w_phl */
    case 563:  /* mips_maq_s_w_phr */
    case 562:  /* mips_maq_s_w_phl */
    case 561:  /* mips_dpsq_sa_l_w */
    case 560:  /* mips_dpaq_sa_l_w */
    case 559:  /* mips_mulsaq_s_w_ph */
    case 558:  /* mips_dpsq_s_w_ph */
    case 557:  /* mips_dpaq_s_w_ph */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 2));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 2);
      recog_data.dup_num[0] = 3;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[2] = 1;
      break;

    case 612:  /* mips_mulq_s_w */
    case 611:  /* mips_mulq_s_ph */
    case 610:  /* mips_mulq_rs_w */
    case 609:  /* mips_mul_s_ph */
    case 552:  /* mips_muleq_s_w_phr */
    case 551:  /* mips_muleq_s_w_phl */
    case 550:  /* mips_mulq_rs_ph */
    case 549:  /* mips_muleu_s_ph_qbr */
    case 548:  /* mips_muleu_s_ph_qbl */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 2), 0));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[1] = 1;
      break;

    case 596:  /* mips_absq_s_qb */
    case 525:  /* mips_absq_s_ph */
    case 524:  /* mips_absq_s_w */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[0] = 1;
      break;

    case 624:  /* mips_subu_s_ph */
    case 623:  /* mips_subu_ph */
    case 605:  /* mips_cmpgdu_le_qb */
    case 604:  /* mips_cmpgdu_lt_qb */
    case 603:  /* mips_cmpgdu_eq_qb */
    case 598:  /* mips_addu_s_ph */
    case 589:  /* mips_mthlip */
    case 586:  /* mips_extp */
    case 585:  /* mips_extr_s_h */
    case 584:  /* mips_extr_rs_w */
    case 583:  /* mips_extr_r_w */
    case 582:  /* mips_extr_w */
    case 543:  /* mips_shll_s_ph */
    case 542:  /* mips_shll_s_w */
    case 541:  /* mips_shll_qb */
    case 540:  /* mips_shll_ph */
    case 529:  /* mips_precrqu_s_qb_ph */
    case 528:  /* mips_precrq_rs_ph_w */
    case 521:  /* mips_addwc */
    case 520:  /* mips_addsc */
    case 519:  /* mips_subu_s_qb */
    case 518:  /* mips_subq_s_ph */
    case 517:  /* mips_subq_s_w */
    case 514:  /* mips_addu_s_qb */
    case 513:  /* mips_addq_s_ph */
    case 512:  /* mips_addq_s_w */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0, 1));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[1] = 1;
      break;

    case 692:  /* sssubv2ha3 */
    case 691:  /* sssubv2hq3 */
    case 690:  /* sssubsa3 */
    case 689:  /* sssubha3 */
    case 688:  /* sssubsq3 */
    case 687:  /* sssubhq3 */
    case 686:  /* ussubv2uha3 */
    case 685:  /* ussubv2uhq3 */
    case 684:  /* ussubv4uqq3 */
    case 683:  /* ussubuha3 */
    case 682:  /* ussubuhq3 */
    case 681:  /* ussubuqq3 */
    case 666:  /* ssaddv2ha3 */
    case 665:  /* ssaddv2hq3 */
    case 664:  /* ssaddsa3 */
    case 663:  /* ssaddha3 */
    case 662:  /* ssaddsq3 */
    case 661:  /* ssaddhq3 */
    case 660:  /* usaddv2uha3 */
    case 659:  /* usaddv2uhq3 */
    case 658:  /* usaddv4uqq3 */
    case 657:  /* usadduha3 */
    case 656:  /* usadduhq3 */
    case 655:  /* usadduqq3 */
    case 597:  /* mips_addu_ph */
    case 516:  /* subv4qi3 */
    case 515:  /* subv2hi3 */
    case 511:  /* addv4qi3 */
    case 510:  /* addv2hi3 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0);
      recog_data.dup_num[1] = 1;
      break;

    case 497:  /* *branch_upper_lower_inverted */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0, 0));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0, 1));
      break;

    case 496:  /* *branch_upper_lower */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0, 0));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0, 1));
      break;

    case 495:  /* bc1any2f */
    case 494:  /* bc1any2t */
    case 493:  /* bc1any4f */
    case 492:  /* bc1any4t */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 491:  /* sungt_ps */
    case 490:  /* sunge_ps */
    case 489:  /* sgt_ps */
    case 488:  /* sge_ps */
    case 487:  /* sle_ps */
    case 486:  /* slt_ps */
    case 485:  /* seq_ps */
    case 484:  /* sunle_ps */
    case 483:  /* sunlt_ps */
    case 482:  /* suneq_ps */
    case 481:  /* sunordered_ps */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1));
      break;

    case 478:  /* mips_cabs_cond_4s */
    case 477:  /* mips_c_cond_4s */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (pat, 1), 0, 2));
      ro[4] = *(ro_loc[4] = &XVECEXP (XEXP (pat, 1), 0, 3));
      ro[5] = *(ro_loc[5] = &XVECEXP (XEXP (pat, 1), 0, 4));
      break;

    case 468:  /* vec_extractv2sf */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XEXP (pat, 1), 1), 0, 0));
      break;

    case 459:  /* *mips16e_save_restore */
      ro[0] = *(ro_loc[0] = &PATTERN (insn));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 456:  /* consttable_int */
      ro[0] = *(ro_loc[0] = &XVECEXP (pat, 0, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (pat, 0, 1));
      break;

    case 461:  /* *movcc_v2sf_di */
    case 460:  /* *movcc_v2sf_si */
    case 455:  /* *movdf_on_cc */
    case 454:  /* *movsf_on_cc */
    case 453:  /* *movdf_on_di */
    case 452:  /* *movsf_on_di */
    case 451:  /* *movdf_on_si */
    case 450:  /* *movsf_on_si */
    case 449:  /* *movdi_on_cc */
    case 448:  /* *movsi_on_cc */
    case 447:  /* *movdi_on_di */
    case 446:  /* *movsi_on_di */
    case 445:  /* *movdi_on_si */
    case 444:  /* *movsi_on_si */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 2));
      ro[4] = *(ro_loc[4] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 441:  /* *prefetch_indexed_di */
    case 440:  /* *prefetch_indexed_si */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 1));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 2));
      break;

    case 439:  /* prefetch */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 1));
      ro[2] = *(ro_loc[2] = &XEXP (pat, 2));
      break;

    case 436:  /* call_value_internal_direct */
    case 435:  /* call_value_split */
    case 434:  /* call_value_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      break;

    case 433:  /* call_split */
    case 432:  /* call_internal_direct */
    case 431:  /* call_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      break;

    case 438:  /* call_value_multiple_split */
    case 437:  /* call_value_multiple_internal */
    case 430:  /* sibcall_value_multiple_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      break;

    case 465:  /* mips_pll_ps */
    case 429:  /* sibcall_value_internal */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 428:  /* sibcall_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 1));
      break;

    case 422:  /* eh_set_lr_di */
    case 421:  /* eh_set_lr_si */
      ro[0] = *(ro_loc[0] = &XVECEXP (XVECEXP (pat, 0, 0), 0, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 420:  /* return_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 417:  /* tablejumpdi */
    case 416:  /* tablejumpsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 0), 0));
      break;

    case 415:  /* indirect_jumpdi */
    case 414:  /* indirect_jumpsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      break;

    case 413:  /* *mips.md:5379 */
    case 412:  /* jump */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 355:  /* *branch_equalitydi_mips16 */
    case 354:  /* *branch_equalitysi_mips16 */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 2));
      break;

    case 353:  /* *branch_equalitydi_inverted */
    case 352:  /* *branch_equalitysi_inverted */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 351:  /* *branch_equalitydi */
    case 350:  /* *branch_equalitysi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 349:  /* *branch_orderdi_inverted */
    case 348:  /* *branch_ordersi_inverted */
    case 345:  /* *branch_fp_inverted */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 2), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 347:  /* *branch_orderdi */
    case 346:  /* *branch_ordersi */
    case 344:  /* *branch_fp */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 1), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 325:  /* sync_lock_test_and_setdi */
    case 324:  /* sync_lock_test_and_setsi */
    case 323:  /* sync_new_nanddi */
    case 322:  /* sync_new_nandsi */
    case 321:  /* sync_old_nanddi */
    case 320:  /* sync_old_nandsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 317:  /* sync_new_anddi */
    case 316:  /* sync_new_xordi */
    case 315:  /* sync_new_iordi */
    case 314:  /* sync_new_andsi */
    case 313:  /* sync_new_xorsi */
    case 312:  /* sync_new_iorsi */
    case 311:  /* sync_old_anddi */
    case 310:  /* sync_old_xordi */
    case 309:  /* sync_old_iordi */
    case 308:  /* sync_old_andsi */
    case 307:  /* sync_old_xorsi */
    case 306:  /* sync_old_iorsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0), 1);
      recog_data.dup_num[1] = 1;
      break;

    case 305:  /* sync_anddi */
    case 304:  /* sync_xordi */
    case 303:  /* sync_iordi */
    case 302:  /* sync_andsi */
    case 301:  /* sync_xorsi */
    case 300:  /* sync_iorsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1);
      recog_data.dup_num[0] = 0;
      break;

    case 299:  /* sync_new_subdi */
    case 298:  /* sync_new_subsi */
    case 297:  /* sync_new_adddi */
    case 296:  /* sync_new_addsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0), 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0), 1);
      recog_data.dup_num[2] = 2;
      break;

    case 295:  /* sync_old_subdi */
    case 294:  /* sync_old_subsi */
    case 293:  /* sync_old_adddi */
    case 292:  /* sync_old_addsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0), 1));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0), 0);
      recog_data.dup_num[1] = 1;
      break;

    case 291:  /* sync_subdi */
    case 290:  /* sync_subsi */
    case 289:  /* sync_adddi */
    case 288:  /* sync_addsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 1));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (XEXP (pat, 1), 0, 0), 0);
      recog_data.dup_num[0] = 0;
      break;

    case 287:  /* sync_compare_and_swapdi */
    case 286:  /* sync_compare_and_swapsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 0));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0, 1));
      recog_data.dup_loc[0] = &XEXP (XVECEXP (pat, 0, 1), 0);
      recog_data.dup_num[0] = 1;
      break;

    case 458:  /* align */
    case 457:  /* consttable_float */
    case 282:  /* synci */
    case 280:  /* cprestore */
      ro[0] = *(ro_loc[0] = &XVECEXP (pat, 0, 0));
      break;

    case 591:  /* mips_rddsp */
    case 569:  /* mips_repl_ph */
    case 568:  /* mips_repl_qb */
    case 566:  /* mips_bitrev */
    case 539:  /* mips_preceu_ph_qbra */
    case 538:  /* mips_preceu_ph_qbla */
    case 537:  /* mips_preceu_ph_qbr */
    case 536:  /* mips_preceu_ph_qbl */
    case 535:  /* mips_precequ_ph_qbra */
    case 534:  /* mips_precequ_ph_qbla */
    case 533:  /* mips_precequ_ph_qbr */
    case 532:  /* mips_precequ_ph_qbl */
    case 531:  /* mips_preceq_w_phr */
    case 530:  /* mips_preceq_w_phl */
    case 523:  /* mips_raddu_w_qb */
    case 506:  /* mips_recip1_ps */
    case 505:  /* mips_recip1_d */
    case 504:  /* mips_recip1_s */
    case 500:  /* mips_rsqrt1_ps */
    case 499:  /* mips_rsqrt1_d */
    case 498:  /* mips_rsqrt1_s */
    case 474:  /* *mips_abs_ps */
    case 472:  /* mips_cvt_ps_pw */
    case 471:  /* mips_cvt_pw_ps */
    case 319:  /* sync_nanddi */
    case 318:  /* sync_nandsi */
    case 283:  /* rdhwr */
    case 276:  /* loadgp_absolute_di */
    case 275:  /* loadgp_absolute_si */
    case 272:  /* mfhc1tf */
    case 271:  /* mfhc1v2sf */
    case 270:  /* mfhc1di */
    case 269:  /* mfhc1df */
    case 256:  /* load_lowtf */
    case 255:  /* load_lowv2sf */
    case 254:  /* load_lowdi */
    case 253:  /* load_lowdf */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      break;

    case 234:  /* *lwxs */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 233:  /* *sdxc1_di */
    case 232:  /* *sdxc1_di */
    case 231:  /* *swxc1_di */
    case 230:  /* *sdxc1_si */
    case 229:  /* *sdxc1_si */
    case 228:  /* *swxc1_si */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 1));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 0), 0), 1));
      break;

    case 213:  /* *movdi_ra */
    case 212:  /* *movsi_ra */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      break;

    case 248:  /* movv2sf_hardfloat_32bit */
    case 247:  /* movv2sf_hardfloat_64bit */
    case 246:  /* *movtf_internal */
    case 245:  /* *movdf_mips16 */
    case 244:  /* *movdf_softfloat */
    case 243:  /* *movdf_hardfloat_32bit */
    case 242:  /* *movdf_hardfloat_64bit */
    case 241:  /* *movsf_mips16 */
    case 240:  /* *movsf_softfloat */
    case 239:  /* *movsf_hardfloat */
    case 238:  /* *movqi_mips16 */
    case 237:  /* *movqi_internal */
    case 236:  /* *movhi_mips16 */
    case 235:  /* *movhi_internal */
    case 221:  /* movcc */
    case 220:  /* *movsi_mips16 */
    case 219:  /* *movsi_internal */
    case 218:  /* *movdi_64bit_mips16 */
    case 217:  /* *movdi_64bit */
    case 216:  /* *movdi_32bit_mips16 */
    case 215:  /* *movdi_gp32_fp64 */
    case 214:  /* *movdi_32bit */
    case 203:  /* *got_dispdi */
    case 202:  /* *got_dispsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (pat, 1));
      break;

    case 197:  /* *lea64 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 195:  /* mov_sdr */
    case 194:  /* mov_swr */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      recog_data.dup_loc[0] = &XVECEXP (XEXP (pat, 1), 0, 2);
      recog_data.dup_num[0] = 0;
      break;

    case 636:  /* mips_dpsx_w_ph */
    case 635:  /* mips_dpax_w_ph */
    case 619:  /* mips_prepend */
    case 618:  /* mips_precr_sra_r_ph_w */
    case 617:  /* mips_precr_sra_ph_w */
    case 613:  /* mips_mulsa_w_ph */
    case 607:  /* mips_dps_w_ph */
    case 606:  /* mips_dpa_w_ph */
    case 602:  /* mips_balign */
    case 601:  /* mips_append */
    case 556:  /* mips_dpsu_h_qbr */
    case 555:  /* mips_dpsu_h_qbl */
    case 554:  /* mips_dpau_h_qbr */
    case 553:  /* mips_dpau_h_qbl */
    case 480:  /* mips_cabs_cond_ps */
    case 479:  /* mips_c_cond_ps */
    case 476:  /* mips_cabs_cond_d */
    case 475:  /* mips_cabs_cond_s */
    case 469:  /* mips_alnv_ps */
    case 462:  /* mips_cond_move_tf_ps */
    case 191:  /* mov_ldr */
    case 190:  /* mov_lwr */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      ro[3] = *(ro_loc[3] = &XVECEXP (XEXP (pat, 1), 0, 2));
      break;

    case 634:  /* mips_subqh_r_w */
    case 633:  /* mips_subqh_w */
    case 632:  /* mips_subqh_r_ph */
    case 631:  /* mips_subqh_ph */
    case 630:  /* mips_addqh_r_w */
    case 629:  /* mips_addqh_w */
    case 628:  /* mips_addqh_r_ph */
    case 627:  /* mips_addqh_ph */
    case 626:  /* mips_subuh_r_qb */
    case 625:  /* mips_subuh_qb */
    case 622:  /* mips_shrl_ph */
    case 621:  /* mips_shra_r_qb */
    case 620:  /* mips_shra_qb */
    case 616:  /* mips_precr_qb_ph */
    case 600:  /* mips_adduh_r_qb */
    case 599:  /* mips_adduh_qb */
    case 588:  /* mips_shilo */
    case 581:  /* mips_packrl_ph */
    case 580:  /* mips_pick_qb */
    case 579:  /* mips_pick_ph */
    case 578:  /* mips_cmpgu_le_qb */
    case 577:  /* mips_cmpgu_lt_qb */
    case 576:  /* mips_cmpgu_eq_qb */
    case 567:  /* mips_insv */
    case 547:  /* mips_shra_r_ph */
    case 546:  /* mips_shra_r_w */
    case 545:  /* mips_shra_ph */
    case 544:  /* mips_shrl_qb */
    case 527:  /* mips_precrq_ph_w */
    case 526:  /* mips_precrq_qb_ph */
    case 522:  /* mips_modsub */
    case 509:  /* mips_recip2_ps */
    case 508:  /* mips_recip2_d */
    case 507:  /* mips_recip2_s */
    case 503:  /* mips_rsqrt2_ps */
    case 502:  /* mips_rsqrt2_d */
    case 501:  /* mips_rsqrt2_s */
    case 473:  /* mips_mulr_ps */
    case 470:  /* mips_addr_ps */
    case 425:  /* load_calldi */
    case 424:  /* load_callsi */
    case 279:  /* loadgp_rtp_di */
    case 278:  /* loadgp_rtp_si */
    case 274:  /* loadgp_newabi_di */
    case 273:  /* loadgp_newabi_si */
    case 268:  /* mthc1tf */
    case 267:  /* mthc1v2sf */
    case 266:  /* mthc1di */
    case 265:  /* mthc1df */
    case 264:  /* store_wordtf */
    case 263:  /* store_wordv2sf */
    case 262:  /* store_worddi */
    case 261:  /* store_worddf */
    case 260:  /* load_hightf */
    case 259:  /* load_highv2sf */
    case 258:  /* load_highdi */
    case 257:  /* load_highdf */
    case 252:  /* *mfhilo_di_macc */
    case 251:  /* *mfhilo_si_macc */
    case 250:  /* *mfhilo_di */
    case 249:  /* *mfhilo_si */
    case 207:  /* load_gotdi */
    case 206:  /* load_gotsi */
    case 193:  /* mov_sdl */
    case 192:  /* mov_swl */
    case 189:  /* mov_ldl */
    case 188:  /* mov_lwl */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XVECEXP (XEXP (pat, 1), 0, 0));
      ro[2] = *(ro_loc[2] = &XVECEXP (XEXP (pat, 1), 0, 1));
      break;

    case 187:  /* insvdi */
    case 186:  /* insvsi */
      ro[0] = *(ro_loc[0] = &XEXP (XEXP (pat, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 0), 2));
      ro[3] = *(ro_loc[3] = &XEXP (pat, 1));
      break;

    case 185:  /* extzvdi */
    case 184:  /* extzvsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 2));
      break;

    case 177:  /* fix_truncsfsi2_macro */
    case 175:  /* fix_truncdfsi2_macro */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 140:  /* *mips.md:2432 */
    case 139:  /* *zero_extenddi_truncqi */
    case 138:  /* *zero_extendsi_truncqi */
    case 137:  /* *zero_extenddi_trunchi */
    case 136:  /* *zero_extendsi_trunchi */
    case 135:  /* *extendsi_truncatehi */
    case 134:  /* *extendsi_truncateqi */
    case 133:  /* *extenddi_truncatehi */
    case 132:  /* *extenddi_truncateqi */
    case 131:  /* *mips.md:2362 */
    case 130:  /* *mips.md:2362 */
    case 129:  /* *mips.md:2362 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      break;

    case 464:  /* mips_puu_ps */
    case 92:  /* *rsqrtv2sfa */
    case 91:  /* *rsqrtdfa */
    case 90:  /* *rsqrtsfa */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 381:  /* *sge_di */
    case 380:  /* *sge_si */
    case 367:  /* *sge_di */
    case 366:  /* *sge_si */
    case 361:  /* *sne_di */
    case 360:  /* *sne_si */
    case 359:  /* *seq_di_mips16 */
    case 358:  /* *seq_si_mips16 */
    case 357:  /* *seq_di */
    case 356:  /* *seq_si */
    case 205:  /* *got_pagedi */
    case 204:  /* *got_pagesi */
    case 199:  /* *xgot_hidi */
    case 198:  /* *xgot_hisi */
    case 196:  /* *lea_high64 */
    case 183:  /* floatdisf2 */
    case 182:  /* floatsisf2 */
    case 181:  /* floatdidf2 */
    case 180:  /* floatsidf2 */
    case 179:  /* fix_truncsfdi2 */
    case 178:  /* fix_truncdfdi2 */
    case 176:  /* fix_truncsfsi2_insn */
    case 174:  /* fix_truncdfsi2_insn */
    case 173:  /* extendsfdf2 */
    case 172:  /* *extendqihi2_seb */
    case 171:  /* *extendqihi2 */
    case 170:  /* *extendqihi2_mips16e */
    case 169:  /* *extendhidi2_seh */
    case 168:  /* *extendhisi2_seh */
    case 167:  /* *extendqidi2_seb */
    case 166:  /* *extendqisi2_seb */
    case 165:  /* *extendhidi2 */
    case 164:  /* *extendhisi2 */
    case 163:  /* *extendqidi2 */
    case 162:  /* *extendqisi2 */
    case 161:  /* *extendhidi2_mips16e */
    case 160:  /* *extendhisi2_mips16e */
    case 159:  /* *extendqidi2_mips16e */
    case 158:  /* *extendqisi2_mips16e */
    case 157:  /* extendsidi2 */
    case 156:  /* *zero_extendqihi2_mips16 */
    case 155:  /* *zero_extendqihi2 */
    case 154:  /* *zero_extendhidi2_mips16 */
    case 153:  /* *zero_extendhisi2_mips16 */
    case 152:  /* *zero_extendqidi2_mips16 */
    case 151:  /* *zero_extendqisi2_mips16 */
    case 150:  /* *zero_extendhidi2_mips16e */
    case 149:  /* *zero_extendhisi2_mips16e */
    case 148:  /* *zero_extendqidi2_mips16e */
    case 147:  /* *zero_extendqisi2_mips16e */
    case 146:  /* *zero_extendhidi2 */
    case 145:  /* *zero_extendhisi2 */
    case 144:  /* *zero_extendqidi2 */
    case 143:  /* *zero_extendqisi2 */
    case 142:  /* *clear_upper32 */
    case 141:  /* zero_extendsidi2 */
    case 125:  /* truncdiqi2 */
    case 124:  /* truncdihi2 */
    case 123:  /* truncdisi2 */
    case 122:  /* truncdfsf2 */
    case 107:  /* one_cmpldi2 */
    case 106:  /* one_cmplsi2 */
    case 105:  /* negv2sf2 */
    case 104:  /* negdf2 */
    case 103:  /* negsf2 */
    case 102:  /* negdi2 */
    case 101:  /* negsi2 */
    case 100:  /* clzdi2 */
    case 99:  /* clzsi2 */
    case 98:  /* absv2sf2 */
    case 97:  /* absdf2 */
    case 96:  /* abssf2 */
    case 89:  /* sqrtv2sf2 */
    case 88:  /* sqrtdf2 */
    case 87:  /* sqrtsf2 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 86:  /* udivmoddi4 */
    case 85:  /* udivmodsi4 */
    case 84:  /* divmoddi4 */
    case 83:  /* divmodsi4 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[1] = 2;
      break;

    case 76:  /* *nmsubv2sf_fastmath */
    case 75:  /* *nmsubdf_fastmath */
    case 74:  /* *nmsubsf_fastmath */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 1), 1));
      break;

    case 73:  /* *nmsubv2sf */
    case 72:  /* *nmsubdf */
    case 71:  /* *nmsubsf */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      break;

    case 70:  /* *nmaddv2sf_fastmath */
    case 69:  /* *nmadddf_fastmath */
    case 68:  /* *nmaddsf_fastmath */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 67:  /* *nmaddv2sf */
    case 66:  /* *nmadddf */
    case 65:  /* *nmaddsf */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 64:  /* *msubv2sf */
    case 63:  /* *msubdf */
    case 62:  /* *msubsf */
    case 61:  /* *maddv2sf */
    case 60:  /* *madddf */
    case 59:  /* *maddsf */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 58:  /* umaddsidi4 */
    case 57:  /* maddsidi4 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 56:  /* madsi */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1);
      recog_data.dup_num[0] = 0;
      break;

    case 53:  /* *umulsi3_highpart_neg_mulhi_internal */
    case 52:  /* *smulsi3_highpart_neg_mulhi_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      break;

    case 51:  /* umulsi3_highpart_mulhi_internal */
    case 50:  /* smulsi3_highpart_mulhi_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      break;

    case 55:  /* umuldi3_highpart */
    case 54:  /* smuldi3_highpart */
    case 49:  /* umulsi3_highpart_internal */
    case 48:  /* smulsi3_highpart_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 47:  /* umsubsidi4 */
    case 46:  /* msubsidi4 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (pat, 1), 0));
      break;

    case 45:  /* *mulsu_di */
    case 44:  /* *muls_di */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XEXP (pat, 1), 0), 1), 0));
      break;

    case 43:  /* *umulsidi3_64bit_parts */
    case 42:  /* *mulsidi3_64bit_parts */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0), 0);
      recog_data.dup_num[0] = 2;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 1), 0);
      recog_data.dup_num[1] = 3;
      break;

    case 41:  /* *umulsidi3_64bit */
    case 40:  /* *mulsidi3_64bit */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 3), 0));
      break;

    case 39:  /* umulsidi3_32bit_r4000 */
    case 38:  /* mulsidi3_32bit_r4000 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 615:  /* mips_multu */
    case 614:  /* mips_mult */
    case 466:  /* mips_plu_ps */
    case 121:  /* *nordi3 */
    case 120:  /* *norsi3 */
    case 37:  /* umulsidi3_32bit_internal */
    case 36:  /* mulsidi3_32bit_internal */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 1), 0));
      break;

    case 35:  /* *muls */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      break;

    case 33:  /* *msac2 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0);
      recog_data.dup_num[0] = 0;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1), 0);
      recog_data.dup_num[1] = 1;
      recog_data.dup_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1), 1);
      recog_data.dup_num[2] = 2;
      recog_data.dup_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0);
      recog_data.dup_num[3] = 0;
      break;

    case 32:  /* *macc2 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      recog_data.dup_loc[0] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 0);
      recog_data.dup_num[0] = 1;
      recog_data.dup_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 0), 1);
      recog_data.dup_num[1] = 2;
      recog_data.dup_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 1), 1), 1);
      recog_data.dup_num[2] = 0;
      recog_data.dup_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1);
      recog_data.dup_num[3] = 0;
      break;

    case 34:  /* *mul_sub_si */
    case 31:  /* *msac_using_macc */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XVECEXP (pat, 0, 3), 0));
      break;

    case 30:  /* *msac */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 0));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 2), 0));
      break;

    case 29:  /* *macc */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 2), 0));
      break;

    case 28:  /* *mul_acc_si */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[5] = *(ro_loc[5] = &XEXP (XVECEXP (pat, 0, 2), 0));
      ro[6] = *(ro_loc[6] = &XEXP (XVECEXP (pat, 0, 3), 0));
      break;

    case 25:  /* muldi3_internal */
    case 24:  /* mulsi3_internal */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      break;

    case 27:  /* muldi3_r4000 */
    case 26:  /* mulsi3_r4000 */
    case 23:  /* mulsi3_mult3 */
      ro[0] = *(ro_loc[0] = &XEXP (XVECEXP (pat, 0, 0), 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1));
      ro[3] = *(ro_loc[3] = &XEXP (XVECEXP (pat, 0, 1), 0));
      ro[4] = *(ro_loc[4] = &XEXP (XVECEXP (pat, 0, 2), 0));
      break;

    case 594:  /* mips_lwx */
    case 334:  /* *lshrsi3_extend */
    case 333:  /* *ashrsi3_extend */
    case 332:  /* *ashlsi3_extend */
    case 227:  /* *ldxc1_di */
    case 226:  /* *ldxc1_di */
    case 225:  /* *lwxc1_di */
    case 224:  /* *ldxc1_si */
    case 223:  /* *ldxc1_si */
    case 222:  /* *lwxc1_si */
    case 128:  /* *mips.md:2352 */
    case 127:  /* *mips.md:2352 */
    case 126:  /* *mips.md:2352 */
    case 95:  /* *rsqrtv2sfb */
    case 94:  /* *rsqrtdfb */
    case 93:  /* *rsqrtsfb */
    case 17:  /* *subsi3_extended */
    case 11:  /* *addsi3_extended_mips16 */
    case 10:  /* *addsi3_extended */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (XEXP (pat, 1), 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (XEXP (pat, 1), 0), 1));
      break;

    case 680:  /* subuda3 */
    case 679:  /* subusa3 */
    case 678:  /* subuha3 */
    case 677:  /* subda3 */
    case 676:  /* subsa3 */
    case 675:  /* subha3 */
    case 674:  /* subudq3 */
    case 673:  /* subusq3 */
    case 672:  /* subuhq3 */
    case 671:  /* subuqq3 */
    case 670:  /* subdq3 */
    case 669:  /* subsq3 */
    case 668:  /* subhq3 */
    case 667:  /* subqq3 */
    case 654:  /* adduda3 */
    case 653:  /* addusa3 */
    case 652:  /* adduha3 */
    case 651:  /* addda3 */
    case 650:  /* addsa3 */
    case 649:  /* addha3 */
    case 648:  /* addudq3 */
    case 647:  /* addusq3 */
    case 646:  /* adduhq3 */
    case 645:  /* adduqq3 */
    case 644:  /* adddq3 */
    case 643:  /* addsq3 */
    case 642:  /* addhq3 */
    case 641:  /* addqq3 */
    case 467:  /* vec_initv2sf_internal */
    case 463:  /* mips_pul_ps */
    case 411:  /* sungt_df */
    case 410:  /* sunge_df */
    case 409:  /* sgt_df */
    case 408:  /* sge_df */
    case 407:  /* sungt_sf */
    case 406:  /* sunge_sf */
    case 405:  /* sgt_sf */
    case 404:  /* sge_sf */
    case 403:  /* sle_df */
    case 402:  /* slt_df */
    case 401:  /* seq_df */
    case 400:  /* sunle_df */
    case 399:  /* sunlt_df */
    case 398:  /* suneq_df */
    case 397:  /* sunordered_df */
    case 396:  /* sle_sf */
    case 395:  /* slt_sf */
    case 394:  /* seq_sf */
    case 393:  /* sunle_sf */
    case 392:  /* sunlt_sf */
    case 391:  /* suneq_sf */
    case 390:  /* sunordered_sf */
    case 389:  /* *sleu_di_mips16 */
    case 388:  /* *sleu_si_mips16 */
    case 387:  /* *sleu_di */
    case 386:  /* *sleu_si */
    case 385:  /* *sltu_di_mips16 */
    case 384:  /* *sltu_si_mips16 */
    case 383:  /* *sltu_di */
    case 382:  /* *sltu_si */
    case 379:  /* *sgtu_di_mips16 */
    case 378:  /* *sgtu_si_mips16 */
    case 377:  /* *sgtu_di */
    case 376:  /* *sgtu_si */
    case 375:  /* *sle_di_mips16 */
    case 374:  /* *sle_si_mips16 */
    case 373:  /* *sle_di */
    case 372:  /* *sle_si */
    case 371:  /* *slt_di_mips16 */
    case 370:  /* *slt_si_mips16 */
    case 369:  /* *slt_di */
    case 368:  /* *slt_si */
    case 365:  /* *sgt_di_mips16 */
    case 364:  /* *sgt_si_mips16 */
    case 363:  /* *sgt_di */
    case 362:  /* *sgt_si */
    case 343:  /* rotrdi3 */
    case 342:  /* rotrsi3 */
    case 341:  /* *mips.md:4820 */
    case 340:  /* *lshrdi3_mips16 */
    case 339:  /* *ashrdi3_mips16 */
    case 338:  /* *ashldi3_mips16 */
    case 337:  /* *lshrsi3_mips16 */
    case 336:  /* *ashrsi3_mips16 */
    case 335:  /* *ashlsi3_mips16 */
    case 331:  /* *lshrdi3 */
    case 330:  /* *ashrdi3 */
    case 329:  /* *ashldi3 */
    case 328:  /* *lshrsi3 */
    case 327:  /* *ashrsi3 */
    case 326:  /* *ashlsi3 */
    case 211:  /* *lowdi_mips16 */
    case 210:  /* *lowsi_mips16 */
    case 209:  /* *lowdi */
    case 208:  /* *lowsi */
    case 201:  /* *xgot_lodi */
    case 200:  /* *xgot_losi */
    case 119:  /* *mips.md:2259 */
    case 118:  /* *mips.md:2259 */
    case 117:  /* *mips.md:2248 */
    case 116:  /* *mips.md:2248 */
    case 115:  /* *iordi3_mips16 */
    case 114:  /* *iorsi3_mips16 */
    case 113:  /* *iordi3 */
    case 112:  /* *iorsi3 */
    case 111:  /* *anddi3_mips16 */
    case 110:  /* *andsi3_mips16 */
    case 109:  /* *anddi3 */
    case 108:  /* *andsi3 */
    case 82:  /* *recipv2sf3 */
    case 81:  /* *recipdf3 */
    case 80:  /* *recipsf3 */
    case 79:  /* *divv2sf3 */
    case 78:  /* *divdf3 */
    case 77:  /* *divsf3 */
    case 22:  /* mulv2sf3 */
    case 21:  /* *muldf3_r4300 */
    case 20:  /* *mulsf3_r4300 */
    case 19:  /* *muldf3 */
    case 18:  /* *mulsf3 */
    case 16:  /* subdi3 */
    case 15:  /* subsi3 */
    case 14:  /* subv2sf3 */
    case 13:  /* subdf3 */
    case 12:  /* subsf3 */
    case 9:  /* *adddi3_mips16 */
    case 8:  /* *addsi3_mips16 */
    case 7:  /* *adddi3 */
    case 6:  /* *addsi3 */
    case 5:  /* addv2sf3 */
    case 4:  /* adddf3 */
    case 3:  /* addsf3 */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 1), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 1), 1));
      break;

    case 2:  /* *conditional_trapdi */
    case 1:  /* *conditional_trapsi */
      ro[0] = *(ro_loc[0] = &XEXP (pat, 0));
      ro[1] = *(ro_loc[1] = &XEXP (XEXP (pat, 0), 0));
      ro[2] = *(ro_loc[2] = &XEXP (XEXP (pat, 0), 1));
      break;

    case 443:  /* hazard_nop */
    case 442:  /* nop */
    case 427:  /* update_got_version */
    case 426:  /* set_got_version */
    case 423:  /* restore_gp */
    case 419:  /* return */
    case 418:  /* blockage */
    case 285:  /* memory_barrier */
    case 284:  /* clear_hazard */
    case 281:  /* sync */
    case 277:  /* loadgp_blockage */
    case 0:  /* trap */
      break;

    }
}
