#ifndef PM_EXPOSER_H
#define PM_EXPOSER_H

#include "pm_collector.h"

void pm_exposer_start(int port, const char *metrics_path, const pm_collector_t *collector);

#endif /* PM_EXPOSER_H */
