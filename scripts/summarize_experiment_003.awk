BEGIN {
    FS = ","
    OFS = ","
    split("origin A1 A2 C5 C5+A1 C5+A2 C5+A3 C5+A4 C5+A5", order, " ")
}
NR == 1 { next }

{
    variant = $1
    key = variant SUBSEP $4 SUBSEP $5
    seen_variant[variant] = 1
    final_completed[key] = $7
    final_status[key] = $9
    final_events[key] = $12
    final_fires[key] = $14
    final_suppressed[key] = $16
    final_strengthenings[key] = $18
    final_rejected[key] = $20
    final_grown[key] = $22
    final_active[key] = $23
    final_queue[key] = $24
    final_synapses[key] = $25
    final_saturation[key] = $31
    final_average_weight[key] = $35
    final_memory[key] = $41
    final_seconds[key] += $42

    if ($34 > maximum_observed[variant]) {
        maximum_observed[variant] = $34
    }
    if ($9 != "unstable" && $7 >= $6) {
        completed[variant, $6]++
    }
}
END {
    print "variant,runs,completed_512,completed_1024,completed_4096," \
          "completed_16384,min_completed,max_completed,stable_final,inactive_final," \
          "unstable_final,total_events,total_fires,total_suppressed," \
          "total_strengthenings,total_rejected,total_grown,average_final_active_neurons," \
          "average_final_max_queue,average_final_synapses,average_final_saturation_percent," \
          "average_final_weight,maximum_observed_weight,average_final_memory_bytes," \
          "total_seconds"

    for (position = 1; position <= length(order); ++position) {
        variant = order[position]
        if (!seen_variant[variant]) {
            continue
        }
        runs = 0
        minimum = -1
        maximum = 0
        stable = 0
        inactive = 0
        unstable = 0
        events = fires = suppressed = strengthenings = rejected = grown = 0
        active = queue = synapses = saturation = average_weight = memory = seconds = 0

        for (key in final_completed) {
            split(key, parts, SUBSEP)
            if (parts[1] != variant) {
                continue
            }
            ++runs
            value = final_completed[key] + 0
            if (minimum < 0 || value < minimum) minimum = value
            if (value > maximum) maximum = value
            if (final_status[key] == "stable") ++stable
            else if (final_status[key] == "inactive") ++inactive
            else ++unstable
            events += final_events[key]
            fires += final_fires[key]
            suppressed += final_suppressed[key]
            strengthenings += final_strengthenings[key]
            rejected += final_rejected[key]
            grown += final_grown[key]
            active += final_active[key]
            queue += final_queue[key]
            synapses += final_synapses[key]
            saturation += final_saturation[key]
            average_weight += final_average_weight[key]
            memory += final_memory[key]
            seconds += final_seconds[key]
        }
        denominator = runs ? runs : 1
        print variant, runs, completed[variant, 512] + 0, completed[variant, 1024] + 0, \
              completed[variant, 4096] + 0, completed[variant, 16384] + 0, \
              minimum, maximum, stable, inactive, unstable, events, fires, suppressed, \
              strengthenings, rejected, grown, active / denominator, queue / denominator, \
              synapses / denominator, saturation / denominator, average_weight / denominator, \
              maximum_observed[variant] + 0, memory / denominator, seconds
    }
}
