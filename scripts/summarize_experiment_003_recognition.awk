BEGIN {
    FS = ","
    OFS = ","
}
NR == 1 { next }

{
    key = $1 SUBSEP $3 SUBSEP $5
    variants[$1] = 1
    groups[$3] = 1
    tests[$5] = 1
    rows[key]++
    if ($6 == "complete") {
        completed[key]++
        events[key] += $9
        fires[key] += $10
        active[key] += $11
        sequence[key SUBSEP $13] = 1
    }
}
END {
    print "variant,group,test_input,rows,completed,average_events,average_fires," \
          "average_active_neurons,distinct_sequence_hashes"
    split("origin A1 A2 C5 C5+A1 C5+A2 C5+A3 C5+A4 C5+A5", variant_order, " ")
    split("trained-AB trained-BA untrained", group_order, " ")
    split("AB BA AC AA BB", test_order, " ")
    for (v = 1; v <= length(variant_order); ++v) {
        for (g = 1; g <= length(group_order); ++g) {
            for (t = 1; t <= length(test_order); ++t) {
                key = variant_order[v] SUBSEP group_order[g] SUBSEP test_order[t]
                if (!(key in rows)) continue
                distinct = 0
                prefix = key SUBSEP
                for (entry in sequence) {
                    if (index(entry, prefix) == 1) ++distinct
                }
                denominator = completed[key] ? completed[key] : 1
                print variant_order[v], group_order[g], test_order[t], rows[key], \
                      completed[key] + 0, events[key] / denominator, \
                      fires[key] / denominator, active[key] / denominator, distinct
            }
        }
    }
}
