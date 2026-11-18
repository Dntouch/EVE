#!/usr/bin/env sh
set -eu

destination="${1:-research/results/b44}"
make _build/eve_research_stage_b44
./_build/eve_research_stage_b44 "$destination"
