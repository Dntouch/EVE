#!/usr/bin/env sh
set -eu

make _build/eve_research_stage_b2
exec _build/eve_research_stage_b2 "${1:-research/results}"
