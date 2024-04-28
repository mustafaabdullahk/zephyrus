#ifndef PM_EXPOSITION_FORMATTER_H
#define PM_EXPOSITION_FORMATTER_H

#include "pm_collector.h"

// Function to format metric data according to Prometheus text-based format
void pm_format_exposition(const pm_collector_t *collector, char *buffer, size_t buffer_size);

#endif /* PM_EXPOSITION_FORMATTER_H */
