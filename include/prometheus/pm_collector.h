#ifndef PM_COLLECTOR_H
#define PM_COLLECTOR_H

#include "pm_metric.h"

#include <stddef.h>

typedef struct {
	pm_metric_t *metric;
	struct pm_collector_node *next;
} pm_collector_node_t;

typedef struct {
	pm_collector_node_t *head;
	size_t size;
} pm_collector_t;

pm_collector_t *pm_collector_create();
void pm_collector_register_metric(pm_collector_t *collector, pm_metric_t *metric);
const pm_metric_t *pm_collector_get_metric(const pm_collector_t *collector, const char *name);
void pm_collector_destroy(pm_collector_t *collector);

#endif /* PM_COLLECTOR_H */
