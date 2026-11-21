#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
result_dir="${1:-${repo_dir}/research/results/b513}"
reproduction_dir="$(mktemp -d)"
trap 'rm -rf "${reproduction_dir}"' EXIT

make -C "${repo_dir}" _build/eve_tests \
    _build/eve_research_stage_b512 _build/eve_research_stage_b513
"${repo_dir}/_build/eve_tests"

"${repo_dir}/_build/eve_research_stage_b512" "${reproduction_dir}"
for raw in learning-curves.csv latent-learning.csv simple-reference.csv \
        consolidation.csv impairment-reacquisition.csv multi-learning.csv; do
    gzip -n -f "${reproduction_dir}/${raw}"
done

mkdir -p "${result_dir}"
printf 'file,status\n' > "${result_dir}/historical-b512-reproduction.csv"
for expected in "${repo_dir}"/research/results/b512/*; do
    name="$(basename "${expected}")"
    if [[ "${name}" == resources.csv || "${name}" == b511-reproduction.csv ]]; then
        continue
    fi
    cmp -s "${expected}" "${reproduction_dir}/${name}"
    printf '%s,exact\n' "${name}" >> \
        "${result_dir}/historical-b512-reproduction.csv"
done

"${repo_dir}/_build/eve_research_stage_b513" "${result_dir}"
for raw in neuron-accessibility.csv recruitment-trials.csv \
        functional-growth.csv b511-b512-task-comparison.csv \
        task-learning-curves.csv; do
    gzip -n -f "${result_dir}/${raw}"
done

raw_bytes=0
stored_bytes=0
while IFS= read -r -d '' path; do
    size="$(stat -c %s "${path}")"
    if [[ "${path}" == *.gz ]]; then
        original_size="$(gzip -l "${path}" | awk 'NR==2 {print $2}')"
        raw_bytes=$((raw_bytes + original_size))
        stored_bytes=$((stored_bytes + size))
    else
        raw_bytes=$((raw_bytes + size))
        stored_bytes=$((stored_bytes + size))
    fi
done < <(find "${result_dir}" -maxdepth 1 -type f -print0)
printf 'raw_result_bytes,%s\nstored_result_bytes,%s\n' \
    "${raw_bytes}" "${stored_bytes}" >> "${result_dir}/resources.csv"

echo "B.5.13 results: ${result_dir}"
