#!/usr/bin/env sh
set -eu

destination="${1:-research/results/b41}"
make _build/eve_research_stage_b41
./_build/eve_research_stage_b41 "$destination"
