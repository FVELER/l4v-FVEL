(*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *)

(*
   Test proofs for corres methods. Builds on AInvs image.
*)

theory CorresK_Test
imports "Refine.VSpace_R" "Lib.CorresK_Method"
begin

chapter \<open>The Corres Method\<close>

section \<open>Introduction\<close>

text \<open>The @{method corresK} method tries to do for corres-style refinement proofs what
@{method wp} did for hoare logic proofs. The intention is to automate the application
of corres calculational rules, so that the bulk of the manual proof is now handling
a verification condition. In general refinement proofs are difficult to automate, so here we
exploit the fact that in l4v the abstract and executable specifications tend to be structurally
similar. Corres proofs are based on the @{const corres_underlying} constant, which takes a number
of parameters that allow it to be specialized for different flavours of refinement.

A corres statement has the following form: @{term "corres_underlying sr nf nf' r P P' f f'"}, where
@{term sr} is a state-relation, @{term nf} and @{term nf'} refer to whether or not the left and
right hand functions may fail, @{term r} is a return value relation between the functions, @{term P}
and @{term P'} are preconditions for the functions @{term f} and @{term f'} respectively. Informally
the statement says that: under the given preconditions, for every execution of @{term f'} there exists
an execution of @{term f} that is related by the given state relation @{term sr} and return-value
relation @{term r}.

If the left and right side of a corres statement share similar structure, we can "unzip" the function
into one corres obligation for each atomic function. This is done through the application of
  @{thm corres_split}.
\<close>

thm corres_split[no_vars]

text \<open>Briefly this states that: given a corres goal proving refinement between @{term "a >>= b"} and
  @{term "c >>= d"}, we can decompose this into a proof showing refinement between @{term a} and
@{term c}, and between @{term a} and @{term c}. Additionally @{term a} and @{term c} must establish
appropriate postconditions to satisfy the obligations of proving refinement between @{term b} and @{term d}.

The first subgoal that is produced has an important characteristic: the preconditions for each
side may only discuss the return value of its respective side. This means that rules such as
@{term "corres_underlying sr nf nf' r (\<lambda>s. x = x') (\<lambda>_. True) (f x) (f' x')"} will not apply to a goal
 if @{term x} and @{term x'} are variables generated by applying @{thm corres_split} (i.e. the
return values of functions).

This means that any such conditions must instead be phrased as an assumption to the rule, and our rule must be
rephrased as follows:
  @{term "x = x' \<Longrightarrow> corres_underlying sr nf nf' r (\<lambda>_. True) (\<lambda>_. True) (f x) (f' x')"}.
The result is that we must solve @{term "x = x'"} immediately after applying our rule. While this
is not a major concern for a manual proof, it proves to be a significant obstacle if we're trying
to focus on automating the "corres" part of the refinement.
\<close>

section \<open>corres_underlyingK and corres_rv\<close>

text \<open>To remedy this situation, we augment the @{const corres_underlying} definition to include
yet another flag: a single boolean. This new constant: @{const corres_underlyingK},
will form the basis of the calculus for our corres method.\<close>

thm corres_underlyingK_def[no_vars]

text \<open>The boolean in @{const corres_underlyingK} can be thought of as a stateless precondition. It
is used to propagate additional proof obligations for rules that either do not need to discuss
either the left or right hand state, or must discuss bound variables from both sides.\<close>

thm corresK_split[no_vars]

text \<open>In this split rule for @{const corres_underlyingK} we see that the additional precondition @{term F'}
may discuss both @{term rv} and @{term rv'}. To show that this condition is satisified, however,
we can't use hoare logic and instead need a new definition: @{const corres_rv}.\<close>

thm corres_rv_def_I_know_what_I'm_doing[no_vars]

text \<open>This is a weaker form of @{const corres_underlying} that is only interested in the return value
of the functions. In essence, it states the given functions will establish @{term Q} after executing,
assuming the given return-value relation @{term r} holds, along with the given stateless precondition
@{term F} and left/right preconditions @{term P} and @{term P'}.

The assumption in general is that corresK_rv rules should never be written, instead corresK_rv obligations
should be propagated into either the stateless precondition (@{term F} from @{term corres_underlyingK}),
the left precondition (@{term P}) or the right precondition @{term P'}. This is implicitly handled
by @{method corresK_rv} (called from @{method corresK}) by applying one of the following rules to each conjunct:\<close>

thm corres_rv_defer
thm corres_rv_wp_left
thm corres_rv_wp_right

text \<open>If none of these rules can be safely applied, then @{method corresK_rv} will leave the
  obligation untouched. The user can manually apply one of them if desired, but this is liable to
  create unsolvable proof obligations. In the worst case, the user may manually solve the goal in-place.\<close>

thm corres_rv_proveT[no_vars]

section \<open>The corres method\<close>

text \<open>The core algorithm of the corres method is simple:
  1) start by applying any necessary weakening rules to ensure the goal has schematic preconditions
  2) apply a known @{thm corres} or @{thm corresK} rule (see next section)
  3) if unsuccessful, apply a split rule (i.e. @{thm corresK_split}) and go to 2

Importantly, @{method corresK} will not split a goal if it ultimately is not able to apply at least
one @{thm corres} or @{thm corresK} rule.
\<close>

subsection \<open>The corres and corresK named_theorems\<close>

text \<open>To address the fact that existing refinement rules are phrased as @{const corres_underlying}
and not @{const corres_underlyingK} there are two different named_theorems that are used for different
kind of rules @{thm corres} and @{thm corresK}. A @{thm corres} rule is understood to be phrased
with @{const corres_underlying} and may have additional assumptions. These assumptions will be
propagated through the additional @{term F} flag in @{const corres_underlyingK}, rather than presented
as proof obligations immediately. A @{thm corresK} rule is understood to be phrased with
@{const corres_underlyingK}, and is meant for calculational rules which may have proper assumptions that
should not be propagated.
\<close>
thm corresK
thm corres

subsection \<open>The corresc method\<close>

text \<open>Similar to @{method wpc}, @{method corresKc} can handle case statements in @{const corres_underlyingK}
proof goals. Importantly, however, it is split into two sub-methods @{method corresKc_left} and
@{method corresKc_right}, which perform case-splitting on each side respectively. The combined method
@{method corresKc}, however, attempts to discharge the contradictions that arise from the quadratic
blowup of a case analysis on both the left and right sides.\<close>

subsection \<open>corres_concrete_r, corres_concrete_rE\<close>

text \<open>Some @{thm corresK} rules should only be applied if certain variables are concrete
(i.e. not schematic) in the goal. These are classified separately with the named_theorems
@{thm corresK_concrete_r} and @{thm corresK_concrete_rER}. The first
indicates that the return value relation of the goal must be concrete, the second indicates that
only the left side of the error relation must be concrete.\<close>

thm corresK_concrete_r
thm corresK_concrete_rER

subsection \<open>The corresK_search method\<close>

text \<open>The purpose of @{method corresK_search} is to address cases where there is non-trivial control flow.
In particular: in the case where there is an "if" statement or either side needs to be symbolically
executed. The core idea is that corresK_search should be provided with a "search" rule that acts
as an anchoring point. Symbolic execution and control flow is decomposed until either the given
rule is successfully applied or all search branches are exhausted.\<close>

subsubsection \<open>Symbolic Execution\<close>

text \<open>Symbolic execution is handled by two named theorems:
 @{thm corresK_symb_exec_ls} and @{thm corresK_symb_exec_rs}, which perform symbolic execution on
the left and right hand sides of a corres goal.\<close>

thm corresK_symb_exec_ls
thm corresK_symb_exec_rs

text \<open>A function may be symbolically executed if it does not modify the state, i.e. its only purpose
is to compute some value and return it. After being symbolically executed,
this value can only be discussed by the precondition of the associated side or the stateless
precondition of corresK. The resulting @{const corres_rv} goal has @{const corres_noop} as the
function on the alternate side. This gives @{method corresK_rv} a hint that the resulting obligation
should be aggressively re-written into a hoare triple over @{term m} if it can't be propagated
back statelessly safely.
\<close>


section \<open>Demo\<close>


context begin interpretation Arch .

(* VSpace_R *)


lemmas load_hw_asid_corres_args[corres] =
  loadHWASID_corres[@lift_corres_args]

lemmas invalidate_asid_corres_args[corres] =
  invalidateASID_corres[@lift_corres_args]

lemmas invalidate_hw_asid_entry_corres_args[corres] =
  invalidateHWASIDEntry_corres[@lift_corres_args]

lemma invalidateASIDEntry_corres:
  "corres dc (valid_vspace_objs and valid_asid_map
                and K (asid \<le> mask asid_bits \<and> asid \<noteq> 0)
                and vspace_at_asid asid pd and valid_vs_lookup
                and unique_table_refs o caps_of_state
                and valid_global_objs and valid_arch_state
                and pspace_aligned and pspace_distinct)
             (pspace_aligned' and pspace_distinct' and no_0_obj')
             (invalidate_asid_entry asid) (invalidateASIDEntry asid)"
  apply (simp add: invalidate_asid_entry_def invalidateASIDEntry_def)
  apply_debug (trace) (* apply_trace between steps *)
   (tags "corres") (* break at breakpoints labelled "corres" *)
   corresK (* weaken precondition *)
   continue (* split *)
   continue (* solve load_hw_asid *)
   continue (* split *)
   continue (* apply corres_when *)
   continue (* trivial simplification *)
   continue (* invalidate _hw_asid_entry *)
   finish (* invalidate_asid *)

  apply (corresKsimp wp: load_hw_asid_wp)+
  apply (fastforce simp: pd_at_asid_uniq)
  done


crunch typ_at'[wp]: invalidateASIDEntry, flushSpace "typ_at' T t"
crunch ksCurThread[wp]: invalidateASIDEntry, flushSpace "\<lambda>s. P (ksCurThread s)"
crunch obj_at'[wp]: invalidateASIDEntry, flushSpace "obj_at' P p"

lemmas flush_space_corres_args[corres] =
  flushSpace_corres[@lift_corres_args]

lemmas invalidate_asid_entry_corres_args[corres] =
  invalidateASIDEntry_corres[@lift_corres_args]


lemma corres_inst_eq_ext:
  "(\<And>x. corres_inst_eq (f x) (f' x)) \<Longrightarrow> corres_inst_eq f f'"
  by (auto simp add: corres_inst_eq_def)

lemma delete_asid_corresb:
  notes [corres] = corres_gets_asid getCurThread_corres setObject_ASIDPool_corres and
    [@lift_corres_args, corres] =  get_asid_pool_corres_inv'
    invalidateASIDEntry_corres
    setVMRoot_corres
  notes [wp] = set_asid_pool_asid_map_unmap set_asid_pool_vs_lookup_unmap'
    set_asid_pool_vspace_objs_unmap'
    invalidate_asid_entry_invalidates
    getASID_wp
  notes if_weak_cong[cong] option.case_cong_weak[cong]
  shows
    "corres dc
          (invs and valid_etcbs and K (asid \<le> mask asid_bits \<and> asid \<noteq> 0))
          (pspace_aligned' and pspace_distinct' and no_0_obj'
              and valid_arch_state' and cur_tcb')
          (delete_asid asid pd) (deleteASID asid pd)"
  apply (simp add: delete_asid_def deleteASID_def)
  apply_debug (trace) (* apply_trace between steps *)
    (tags "corres") (* break at breakpoints labelled "corres" *)
    corresK (* weaken precondition *)
   continue (* split *)
       continue (* gets rule *)
      continue (* corresc *)
       continue (* return rule *)
      continue (* split *)
          continue (* function application *)
          continue (* liftM rule *)
          continue (* get_asid_pool_corres_inv' *)
         continue (* function application *)
         continue (* function application *)
         continue (* corresK_when *)
         continue (* split *)
             continue (* flushSpace_corres *)
            continue (* K_bind *)
            continue (* K_bind *)
            continue (* split *)
                continue (* invalidateASIDEntry_corres *)
               continue (* K_bind *)
               continue (* return bind *)
               continue (* K_bind *)
               continue (* split *)
                   continue (* backtracking *)
               continue (* split *)
                   continue (* function application *)
                   continue (* setObject_ASIDPool_corres *)
                  continue (* K_bind *)
                  continue (* K_bind *)
                  continue (* split *)
                      continue (* getCurThread_corres *)
                     continue (* setVMRoot_corres *)
                    finish (* backtracking? *)
                    apply (corresKsimp simp: mask_asid_low_bits_ucast_ucast
      | fold cur_tcb_def | wps)+
  apply (frule arm_asid_table_related,clarsimp)
  apply (rule conjI)
   apply (intro impI allI)
    apply (rule conjI)
     apply (safe; assumption?)
     apply (rule ext)
     apply (fastforce simp: inv_def dest: ucast_ucast_eq)
    apply (rule context_conjI)
    apply (fastforce simp: o_def dest: valid_asid_tableD invs_valid_asid_table)
   apply (intro allI impI)
   apply (subgoal_tac "vspace_at_asid asid pd s")
    prefer 2
    apply (simp add: vspace_at_asid_def)
    apply (rule vs_lookupI)
     apply (simp add: vs_asid_refs_def)
     apply (rule image_eqI[OF refl])
     apply (rule graph_ofI)
     apply fastforce
    apply (rule r_into_rtrancl)
    apply simp
    apply (rule vs_lookup1I [OF _ _ refl], assumption)
    apply (simp add: vs_refs_def)
    apply (rule image_eqI[rotated], erule graph_ofI)
    apply (simp add: mask_asid_low_bits_ucast_ucast)
   prefer 2
   apply (intro allI impI context_conjI; assumption?)
    apply (rule aligned_distinct_relation_asid_pool_atI'; fastforce?)
    apply (fastforce simp: o_def dest: valid_asid_tableD invs_valid_asid_table)
    apply (simp add: cur_tcb'_def)
    apply (safe; assumption?)
    apply (erule ko_at_weakenE)
    apply (clarsimp simp: graph_of_def)
    apply (fastforce split: if_split_asm)
   apply (frule invs_vspace_objs)
   apply (drule (2) valid_vspace_objsD)
   apply (erule ranE)
   apply (fastforce split: if_split_asm)
  apply (erule ko_at_weakenE)
  apply (clarsimp simp: graph_of_def)
  apply (fastforce split: if_split_asm)
  done

lemma cte_wp_at_ex:
  "cte_wp_at (\<lambda>_. True) p s \<Longrightarrow> (\<exists>cap. cte_wp_at ((=) cap) p s)"
  by (simp add: cte_wp_at_def)

(* Sadly broken:
lemma setVMRootForFlush_corres:
  notes [corres] = getCurThread_corres getSlotCap_corres
  shows
  "corres (=)
          (cur_tcb and vspace_at_asid asid pd
           and K (asid \<noteq> 0 \<and> asid \<le> mask asid_bits)
           and valid_asid_map and valid_vs_lookup
           and valid_vspace_objs and valid_global_objs
           and unique_table_refs o caps_of_state
           and valid_arch_state
           and pspace_aligned and pspace_distinct)
          (pspace_aligned' and pspace_distinct' and no_0_obj')
          (set_vm_root_for_flush pd asid)
          (setVMRootForFlush pd asid)"
  apply (simp add: set_vm_root_for_flush_def setVMRootForFlush_def getThreadVSpaceRoot_def locateSlot_conv)
  apply corres
         apply_debug (trace) (tags "corresK_search") (corresK_search search: armv_contextSwitch_corres)
  continue (* step left *)
  continue (* if rule *)
  continue (* failed corres on first subgoal, trying next *)
  continue (* fail corres on last subgoal, trying reverse if rule *)
  continue (* can't make corres progress here, trying other goal *)
  finish (* successful goal discharged by corres *)

  apply (corresKsimp wp: get_cap_wp getSlotCap_wp)+
  apply (rule context_conjI)
  subgoal by (simp add: cte_map_def objBits_simps tcb_cnode_index_def
                        tcbVTableSlot_def to_bl_1 cte_level_bits_def)
  apply (rule context_conjI)
  subgoal by (fastforce simp: cur_tcb_def intro!: tcb_at_cte_at_1[simplified])
  apply (rule conjI)
   subgoal by (fastforce simp: isCap_simps)
  apply (drule cte_wp_at_ex)
  apply clarsimp
  apply (drule (1) pspace_relation_cte_wp_at[rotated 1]; (assumption | clarsimp)?)
  apply (drule cte_wp_at_norm')
  apply clarsimp
  apply (rule_tac x="cteCap cte" in exI)
  apply (auto elim: cte_wp_at_weakenE' dest!: curthread_relation)
  done

text \<open>Note we can wrap it all up in corresKsimp\<close>

lemma setVMRootForFlush_corres':
  notes [corres] = getCurThread_corres getSlotCap_corres
  shows
  "corres (=)
          (cur_tcb and vspace_at_asid asid pd
           and K (asid \<noteq> 0 \<and> asid \<le> mask asid_bits)
           and valid_asid_map and valid_vs_lookup
           and valid_vspace_objs and valid_global_objs
           and unique_table_refs o caps_of_state
           and valid_arch_state
           and pspace_aligned and pspace_distinct)
          (pspace_aligned' and pspace_distinct' and no_0_obj')
          (set_vm_root_for_flush pd asid)
          (setVMRootForFlush pd asid)"
  apply (simp add: set_vm_root_for_flush_def setVMRootForFlush_def getThreadVSpaceRoot_def locateSlot_conv)
  apply (corresKsimp search: armv_contextSwitch_corres
                        wp: get_cap_wp getSlotCap_wp
                      simp: isCap_simps)
  apply (rule context_conjI)
  subgoal by (simp add: cte_map_def objBits_simps tcb_cnode_index_def
                        tcbVTableSlot_def to_bl_1 cte_level_bits_def)
  apply (rule context_conjI)
  subgoal by (fastforce simp: cur_tcb_def intro!: tcb_at_cte_at_1[simplified])
  apply (rule conjI)
   subgoal by (fastforce)
  apply (drule cte_wp_at_ex)
  apply clarsimp
  apply (drule (1) pspace_relation_cte_wp_at[rotated 1]; (assumption | clarsimp)?)
  apply (drule cte_wp_at_norm')
  apply clarsimp
  apply (rule_tac x="cteCap cte" in exI)
  apply (auto elim: cte_wp_at_weakenE' dest!: curthread_relation)
  done
*)

end
end
