#ifndef PM_COUNTER_H
#define PM_COUNTER_H

#include "pm_metric.h"

typedef struct pm_counter {
	pm_metric_t base;
	unsigned long value;
} pm_counter_t;

pm_counter_t *pm_counter_create(const char *name, const char *desc, pm_label_t *labels,
				int num_labels);
void pm_counter_inc(pm_counter_t *counter);
void pm_counter_destroy(pm_counter_t *counter);

#endif /* PM_COUNTER_H */
