BEGIN {
    FS = ","
    OFS = ","
    split("256 1024 10000 100000 1000000", sizes, " ")
}

NR > 1 {
    key = $1
    runs[key]++
    init[key] += $6
    init_squared[key] += $6 * $6
    processing[key] += $9
    processing_squared[key] += $9 * $9
    rate[key] += $16
    rate_squared[key] += $16 * $16
    fire_rate[key] += $17
    peak_rss[key] += $22
    estimated[key] = $19
    bytes_per_neuron[key] += $23
}

END {
    print "neurons,runs,init_mean_s,init_sd_s,processing_mean_s,processing_sd_s," \
          "syn_events_per_s_mean,syn_events_per_s_sd,fire_events_per_s_mean," \
          "peak_rss_mean_bytes,estimated_bytes,bytes_per_neuron_rss_mean"
    for (i = 1; i <= length(sizes); i++) {
        key = sizes[i]
        count = runs[key]
        init_mean = init[key] / count
        processing_mean = processing[key] / count
        rate_mean = rate[key] / count
        init_sd = sqrt((init_squared[key] - count * init_mean * init_mean) / (count - 1))
        processing_sd = sqrt((processing_squared[key] - count * processing_mean * processing_mean) / (count - 1))
        rate_sd = sqrt((rate_squared[key] - count * rate_mean * rate_mean) / (count - 1))
        printf "%s,%d,%.9f,%.9f,%.9f,%.9f,%.3f,%.3f,%.3f,%.0f,%.0f,%.6f\n", \
               key, count, init_mean, init_sd, processing_mean, processing_sd, \
               rate_mean, rate_sd, fire_rate[key] / count, peak_rss[key] / count, \
               estimated[key], bytes_per_neuron[key] / count
    }
}
