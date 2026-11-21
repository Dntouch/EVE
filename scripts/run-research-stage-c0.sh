#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output_dir="${root_dir}/research/results/c0"
reproduction_root="$(mktemp -d)"
preflight_result="${reproduction_root}/structural-safety-preflight.csv"
trap 'rm -rf "${reproduction_root}"' EXIT

make -C "${root_dir}" _build/eve_research_stage_c0 _build/eve_tests \
    _build/eve_research_stage_b511 _build/eve_research_stage_b512 \
    _build/eve_research_stage_b513 _build/eve_structural_safety_preflight
"${root_dir}/_build/eve_structural_safety_preflight" "${preflight_result}"
"${root_dir}/_build/eve_tests"
rm -rf "${output_dir}"
mkdir -p "${output_dir}"
cp "${preflight_result}" "${output_dir}/structural-safety-preflight.csv"
"${root_dir}/_build/eve_research_stage_c0" "${output_dir}"

for file in autonomous-trials.csv natural-growth-connections.csv \
    recruitment-stages.csv task-learning-curves.csv control-groups.csv \
    causal-interventions.csv continuation-weight-writes.csv; do
    gzip -n -9 "${output_dir}/${file}"
done

printf 'assessment,criterion,passed,evidence\n' \
    > "${output_dir}/success-criteria.csv"
printf 'A,integrity_independent_structural_safety_preflight,1,structural-safety-preflight.csv\n' \
    >> "${output_dir}/success-criteria.csv"
tail -n +2 "${output_dir}/research-outcomes.csv" \
    >> "${output_dir}/success-criteria.csv"

reproduce_stage() {
    local stage="$1"
    local executable="$2"
    local expected_dir="$3"
    local reproduced_dir="${reproduction_root}/${stage}"
    mkdir -p "${reproduced_dir}"
    "${executable}" "${reproduced_dir}"

    case "${stage}" in
        B511)
            for raw in network-results.csv response-matrix.csv \
                    v1-v10-dimensions.csv component-overlap.csv transfer-c.csv \
                    historical-network-reproduction.csv; do
                gzip -n -f "${reproduced_dir}/${raw}"
            done
            ;;
        B512)
            for raw in learning-curves.csv latent-learning.csv simple-reference.csv \
                    consolidation.csv impairment-reacquisition.csv \
                    multi-learning.csv; do
                gzip -n -f "${reproduced_dir}/${raw}"
            done
            ;;
        B513)
            for raw in neuron-accessibility.csv recruitment-trials.csv \
                    functional-growth.csv b511-b512-task-comparison.csv \
                    task-learning-curves.csv; do
                gzip -n -f "${reproduced_dir}/${raw}"
            done
            ;;
    esac

    local compared=0
    while IFS= read -r -d '' expected; do
        local name
        name="$(basename "${expected}")"
        case "${stage}:${name}" in
            B511:resources.csv|B512:resources.csv|B512:b511-reproduction.csv|\
            B513:resources.csv|B513:historical-b512-reproduction.csv)
                continue
                ;;
        esac
        test -f "${reproduced_dir}/${name}"
        if [[ "${name}" == *.gz ]]; then
            if ! cmp -s <(gzip -cd "${expected}") \
                    <(gzip -cd "${reproduced_dir}/${name}"); then
                printf 'historical content mismatch: %s/%s\n' \
                    "${stage}" "${name}" >&2
                return 1
            fi
        elif ! cmp -s "${expected}" "${reproduced_dir}/${name}"; then
            printf 'historical content mismatch: %s/%s\n' \
                "${stage}" "${name}" >&2
            return 1
        fi
        compared=$((compared + 1))
    done < <(find "${expected_dir}" -maxdepth 1 -type f -print0 | sort -z)

    printf '%s,%s,1,%s,1,%s\n' \
        "${stage}" "$(basename "${executable}")" "${compared}" \
        "${expected_dir#${root_dir}/}" >> "${output_dir}/historical-regression.csv"
}

printf 'stage,executable,c0_mechanisms_disabled,compared_files,exact_regression,reference_path\n' \
    > "${output_dir}/historical-regression.csv"
reproduce_stage B511 "${root_dir}/_build/eve_research_stage_b511" \
    "${root_dir}/research/results/b511"
reproduce_stage B512 "${root_dir}/_build/eve_research_stage_b512" \
    "${root_dir}/research/results/b512"
reproduce_stage B513 "${root_dir}/_build/eve_research_stage_b513" \
    "${root_dir}/research/results/b513"
