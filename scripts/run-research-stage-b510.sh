#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="${1:-${root}/research/results/b510}"

make -C "${root}" _build/eve_tests _build/eve_research_stage_b510
"${root}/_build/eve_tests"
mkdir -p "${output}"
"${root}/_build/eve_research_stage_b510" "${output}"

for name in network-classifications stable-experiences plasticity-events \
        integration-events nonfire-decisions eligibility-integration \
        prospective-write-events natural-followups; do
    path="${output}/${name}.csv"
    if [[ -f "${path}" ]]; then
        gzip -n -f "${path}"
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
done < <(find "${output}" -maxdepth 1 -type f -print0)
printf 'raw_result_bytes,%s\ncompressed_result_bytes,%s\n' \
    "${raw_bytes}" "${compressed_bytes}" >> "${output}/resources.csv"
