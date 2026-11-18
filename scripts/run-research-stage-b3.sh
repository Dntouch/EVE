#!/usr/bin/env sh
set -eu

make _build/eve_research_stage_b3
exec _build/eve_research_stage_b3 "${1:-research/results}"
