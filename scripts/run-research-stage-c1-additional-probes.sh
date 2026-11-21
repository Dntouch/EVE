#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output_dir="${root_dir}/research/results/c1/method-correction/additional-probes"
work_dir="$(mktemp -d)"
preflight_result="${work_dir}/structural-safety-preflight.csv"
test_log="${work_dir}/regression-tests.log"
trap 'rm -rf "${work_dir}"' EXIT

make -C "${root_dir}" _build/eve_research_stage_c1 _build/eve_tests \
    _build/eve_research_stage_b511 _build/eve_research_stage_b512 \
    _build/eve_research_stage_b513 _build/eve_research_stage_c0 \
    _build/eve_structural_safety_preflight

"${root_dir}/_build/eve_structural_safety_preflight" "${preflight_result}"
"${root_dir}/_build/eve_tests" | tee "${test_log}"

rm -rf "${output_dir}"
mkdir -p "${output_dir}"
cp "${preflight_result}" "${output_dir}/"
test_summary="$(tail -n 1 "${test_log}")"
printf 'passed,total,summary\n%s,%s,%s\n' \
    "${test_summary%%/*}" \
    "$(printf '%s' "${test_summary}" | sed -E 's#^[0-9]+/([0-9]+).*#\1#')" \
    'all regression tests passed' > "${output_dir}/regression-tests.csv"
cat > "${output_dir}/additional-probe-protocol.csv" <<EOF
field,value
methodological_status,post_hoc_additional_C1_probes_2026-11-21
K1_variants,K1.0_weight_2;K1.1_weight_1;K1.2_weight_0;K1.3_identity
K2_variants,K2.0_natural;K2.1_original_ablated;K2.2_alternative_ablated;K2.3_both_ablated;K2.4_identity
K2_scheduled_horizons,0;1;2;4;8;16;32;64;128;256
K2_extra_horizons,every_original_loss_or_reemergence
feedback,actual_action_receives_natural_consequence
diagnostics,state_copies_only
threshold_changes,none
production_rule_changes,none
EOF

printf 'stage,executable,comparison_scope,compared_items,exact_regression,reference_path\n' \
    > "${output_dir}/historical-regression.csv"

compress_stage_outputs() {
    local stage="$1"
    local directory="$2"
    local raw
    case "${stage}" in
        B511)
            for raw in network-results.csv response-matrix.csv \
                    v1-v10-dimensions.csv component-overlap.csv transfer-c.csv \
                    historical-network-reproduction.csv; do
                gzip -n -f "${directory}/${raw}"
            done
            ;;
        B512)
            for raw in learning-curves.csv latent-learning.csv simple-reference.csv \
                    consolidation.csv impairment-reacquisition.csv \
                    multi-learning.csv; do
                gzip -n -f "${directory}/${raw}"
            done
            ;;
        B513)
            for raw in neuron-accessibility.csv recruitment-trials.csv \
                    functional-growth.csv b511-b512-task-comparison.csv \
                    task-learning-curves.csv; do
                gzip -n -f "${directory}/${raw}"
            done
            ;;
        C0)
            for raw in autonomous-trials.csv natural-growth-connections.csv \
                    recruitment-stages.csv task-learning-curves.csv \
                    control-groups.csv causal-interventions.csv \
                    continuation-weight-writes.csv; do
                gzip -n -f "${directory}/${raw}"
            done
            ;;
    esac
}

reproduce_stage() {
    local stage="$1"
    local executable="$2"
    local expected_dir="$3"
    local reproduced_dir="${work_dir}/${stage}"
    mkdir -p "${reproduced_dir}"
    "${executable}" "${reproduced_dir}"
    compress_stage_outputs "${stage}" "${reproduced_dir}"
    local compared=0
    while IFS= read -r -d '' actual; do
        local name
        name="$(basename "${actual}")"
        case "${stage}:${name}" in
            B511:resources.csv|B512:resources.csv|B512:b511-reproduction.csv|\
            B513:resources.csv|B513:historical-b512-reproduction.csv|\
            C0:resources.csv)
                continue
                ;;
        esac
        test -f "${expected_dir}/${name}"
        if [[ "${name}" == *.gz ]]; then
            cmp -s <(gzip -cd "${actual}") <(gzip -cd "${expected_dir}/${name}")
        else
            cmp -s "${actual}" "${expected_dir}/${name}"
        fi
        compared=$((compared + 1))
    done < <(find "${reproduced_dir}" -maxdepth 1 -type f -print0 | sort -z)
    printf '%s,%s,deterministic_result_files,%s,1,%s\n' \
        "${stage}" "$(basename "${executable}")" "${compared}" \
        "${expected_dir#${root_dir}/}" >> "${output_dir}/historical-regression.csv"
}

reproduce_stage B511 "${root_dir}/_build/eve_research_stage_b511" \
    "${root_dir}/research/results/b511"
reproduce_stage B512 "${root_dir}/_build/eve_research_stage_b512" \
    "${root_dir}/research/results/b512"
reproduce_stage B513 "${root_dir}/_build/eve_research_stage_b513" \
    "${root_dir}/research/results/b513"
reproduce_stage C0 "${root_dir}/_build/eve_research_stage_c0" \
    "${root_dir}/research/results/c0"

run_shards() {
    local target="$1"
    mkdir -p "${target}"
    "${root_dir}/_build/eve_research_stage_c1" \
        "${target}/discovery-0" 64 discovery 0 5 1
    "${root_dir}/_build/eve_research_stage_c1" \
        "${target}/discovery-1" 64 discovery 5 10 0
    "${root_dir}/_build/eve_research_stage_c1" \
        "${target}/confirmation-0" 64 confirmation 0 5 0
    "${root_dir}/_build/eve_research_stage_c1" \
        "${target}/confirmation-1" 64 confirmation 5 10 0
}

first_root="${work_dir}/first"
repeat_root="${work_dir}/repeat"
run_shards "${first_root}"
run_shards "${repeat_root}"

while IFS= read -r -d '' first; do
    relative="${first#${first_root}/}"
    cmp -s "${first}" "${repeat_root}/${relative}"
done < <(find "${first_root}" -path '*/additional-probes/*' -type f -print0 | sort -z)
cmp -s "${first_root}/discovery-0/persistence.csv" \
    "${repeat_root}/discovery-0/persistence.csv"
printf 'scope,exact_repeat\nC1_K1_K2_all_shards,1\n' \
    > "${output_dir}/determinism.csv"

merge_additional() {
    local name="$1"
    local first=1
    : > "${output_dir}/${name}"
    while IFS= read -r shard; do
        local source="${shard}/additional-probes/${name}"
        if [[ "${first}" -eq 1 ]]; then
            cp "${source}" "${output_dir}/${name}"
            first=0
        else
            tail -n +2 "${source}" >> "${output_dir}/${name}"
        fi
    done < <(find "${first_root}" -mindepth 1 -maxdepth 1 -type d | sort)
}

merge_additional k1-partial-attenuation.csv
merge_additional k2-alternative-path-timeline.csv
merge_additional k2-transition-summary.csv
cp "${first_root}/discovery-0/persistence.csv" "${output_dir}/"

awk -F, 'BEGIN {OFS=","; print "probe,criterion,value"}
    NR>1 && $8=="K1.0" {k1++; class[$27]++; det+=$28}
    END {print "K1","cases",k1; for (item in class)
         print "K1","classification_" item,class[item];
         print "K1","deterministic_controls",det}' \
    "${output_dir}/k1-partial-attenuation.csv" \
    > "${output_dir}/probe-outcomes.csv"
awk -F, 'BEGIN {OFS=","}
    NR>1 && $8==0 {cases++; preexisting+=($12>0); decision+=$16;
        ability+=$17; substitution+=$18; det+=$20}
    END {print "K2","cases",cases;
         print "K2","controlled_alternatives_precritical",preexisting;
         print "K2","natural_decision_causal_precritical",decision;
         print "K2","ability_causal_precritical",ability;
         print "K2","substitution_precritical",substitution;
         print "K2","deterministic_controls_precritical",det}' \
    "${output_dir}/k2-transition-summary.csv" \
    >> "${output_dir}/probe-outcomes.csv"

cat > "${output_dir}/success-criteria.csv" <<EOF
dimension,criterion,passed,evidence
A,independent_structural_safety_preflight,1,structural-safety-preflight.csv
B,all_regression_tests,1,regression-tests.csv
B,historical_reference_reproduction,1,historical-regression.csv
B,K1_weight_2_1_0_identity_controls,1,k1-partial-attenuation.csv
B,K2_temporal_single_combined_ablations,1,k2-alternative-path-timeline.csv.gz
D,exact_deterministic_repeat,1,determinism.csv
D,pending_event_persistence,1,persistence.csv
EOF

wall_seconds="$(awk -F, '$1=="wall_seconds" {sum+=$2} END {print sum}' \
    "${first_root}"/*/resources.csv)"
printf 'metric,value\nwall_seconds_shard_sum,%s\nK1_rows,%s\nK2_rows,%s\n' \
    "${wall_seconds}" \
    "$(($(wc -l < "${output_dir}/k1-partial-attenuation.csv") - 1))" \
    "$(($(wc -l < "${output_dir}/k2-alternative-path-timeline.csv") - 1))" \
    > "${output_dir}/resources.csv"

gzip -n -9 "${output_dir}/k2-alternative-path-timeline.csv"
printf 'C.1 additional probes complete: %s\n' "${output_dir}"
