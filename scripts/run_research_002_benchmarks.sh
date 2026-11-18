#!/usr/bin/env bash
set -euo pipefail

repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
output_path=${1:-"${repo_dir}/research/results/research-002-performance.csv"}
benchmark="${repo_dir}/_build/eve_performance_benchmark"
summary_path="${output_path%.csv}-summary.csv"

mkdir -p -- "$(dirname -- "${output_path}")"
make -C "${repo_dir}" "${benchmark#"${repo_dir}/"}"

LC_ALL=C "${benchmark}" --header >"${output_path}"
for neurons in 256 1024 10000 100000 1000000; do
    for trial in 1 2 3; do
        LC_ALL=C "${benchmark}" \
            --neurons "${neurons}" \
            --trial "${trial}" \
            --seed 20261117 \
            --synaptic-events 1000000 >>"${output_path}"
    done
done

LC_ALL=C awk -f "${repo_dir}/scripts/summarize_research_002_performance.awk" \
    "${output_path}" >"${summary_path}"
echo "Wrote ${output_path}"
echo "Wrote ${summary_path}"
