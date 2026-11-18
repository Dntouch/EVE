#!/usr/bin/env sh
set -eu

destination="${1:-research/results/b45}"
make _build/eve_research_stage_b45
./_build/eve_research_stage_b45 "$destination"
