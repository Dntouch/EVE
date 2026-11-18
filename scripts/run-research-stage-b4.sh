#!/usr/bin/env sh
set -eu

destination="${1:-research/results/b4}"
make _build/eve_research_stage_b4
./_build/eve_research_stage_b4 "$destination"
