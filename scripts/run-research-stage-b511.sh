#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
result_dir="${1:-${repo_dir}/research/results/b511}"

make -C "${repo_dir}" _build/eve_tests _build/eve_research_stage_b511
"${repo_dir}/_build/eve_tests"
mkdir -p "${result_dir}"
"${repo_dir}/_build/eve_research_stage_b511" "${result_dir}"

for raw in network-results.csv response-matrix.csv v1-v10-dimensions.csv \
        component-overlap.csv transfer-c.csv \
        historical-network-reproduction.csv; do
    if [[ -f "${result_dir}/${raw}" ]]; then
        gzip -n -f "${result_dir}/${raw}"
    fi
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

echo "B.5.11 results: ${result_dir}"
