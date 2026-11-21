#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
result_dir="${1:-${repo_dir}/research/results/b512}"
reproduction_dir="$(mktemp -d)"
trap 'rm -rf "${reproduction_dir}"' EXIT

make -C "${repo_dir}" _build/eve_tests \
    _build/eve_research_stage_b511 _build/eve_research_stage_b512
"${repo_dir}/_build/eve_tests"

"${repo_dir}/_build/eve_research_stage_b511" "${reproduction_dir}"
for raw in network-results.csv response-matrix.csv v1-v10-dimensions.csv \
        component-overlap.csv transfer-c.csv \
        historical-network-reproduction.csv; do
    gzip -n -f "${reproduction_dir}/${raw}"
done

mkdir -p "${result_dir}"
printf 'file,status\n' > "${result_dir}/b511-reproduction.csv"
for expected in "${repo_dir}"/research/results/b511/*; do
    name="$(basename "${expected}")"
    if [[ "${name}" == resources.csv ]]; then
        continue
    fi
    cmp -s "${expected}" "${reproduction_dir}/${name}"
    printf '%s,exact\n' "${name}" >> "${result_dir}/b511-reproduction.csv"
done

"${repo_dir}/_build/eve_research_stage_b512" "${result_dir}"
for raw in learning-curves.csv latent-learning.csv simple-reference.csv \
        consolidation.csv impairment-reacquisition.csv multi-learning.csv; do
    gzip -n -f "${result_dir}/${raw}"
done

raw_bytes=0
compressed_bytes=0
while IFS= read -r -d '' path; do
    size="$(stat -c %s "${path}")"
    if [[ "${path}" == *.gz ]]; then
        original_size="$(gzip -l "${path}" | awk 'NR==2 {print $2}')"
        raw_bytes=$((raw_bytes + original_size))
        compressed_bytes=$((compressed_bytes + size))
    else
        raw_bytes=$((raw_bytes + size))
        compressed_bytes=$((compressed_bytes + size))
    fi
done < <(find "${result_dir}" -maxdepth 1 -type f -print0)
printf 'raw_result_bytes,%s\ncompressed_result_bytes,%s\n' \
    "${raw_bytes}" "${compressed_bytes}" >> "${result_dir}/resources.csv"

echo "B.5.12 results: ${result_dir}"
