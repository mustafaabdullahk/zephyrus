#ifndef PM_GAUGE_H
#define PM_GAUGE_H

#include "pm_metric.h"

typedef struct pm_gauge {
	pm_metric_t base;
	double value;
} pm_gauge_t;

pm_gauge_t *pm_gauge_create(const char *name, const char *desc, pm_label_t *labels, int num_labels);
void pm_gauge_set(pm_gauge_t *gauge, double value);
void pm_gauge_destroy(pm_gauge_t *gauge);

#endif /* PM_GAUGE_H */
