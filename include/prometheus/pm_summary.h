#ifndef PM_SUMMARY_H
#define PM_SUMMARY_H

#include "pm_metric.h"

#include <stddef.h>

typedef struct {
	double quantile;
	double value;
} pm_summary_quantile_t;

typedef struct {
	pm_metric_t base;
	pm_summary_quantile_t *quantiles;
	size_t num_quantiles;
	double sum;
	unsigned long count;
} pm_summary_t;

pm_summary_t *pm_summary_create(const char *name, const char *description, pm_label_t *labels,
				size_t num_labels);
void pm_summary_observe(pm_summary_t *summary, double value);
void pm_summary_destroy(pm_summary_t *summary);

#endif /* PM_SUMMARY_H */
