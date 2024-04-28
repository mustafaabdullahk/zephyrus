# add source files
set(SRC_FILES
    pm_counter.c
    pm_gauge.c
    pm_histogram.c
    # pm_label.c
    pm_metric.c
    pm_collector.c
    pm_summary.c
    pm_formatter.c
    pm_exposer.c
)

# create library
add_library(prometheus ${SRC_FILES})
