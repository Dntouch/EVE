#!/usr/bin/env bash
set -euo pipefail

repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
output_dir=${1:-"${repo_dir}/research/results"}
runner="${repo_dir}/_build/eve_research_002"

mkdir -p -- "${output_dir}"
make -C "${repo_dir}" "${runner#"${repo_dir}/"}"
"${runner}" "${output_dir}"
