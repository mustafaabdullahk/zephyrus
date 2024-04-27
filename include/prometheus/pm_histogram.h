#ifndef PM_HISTOGRAM_H
#define PM_HISTOGRAM_H

#include "pm_metric.h"

#include <stddef.h>

typedef struct {
	double upper_bound;
	unsigned long count;
} pm_histogram_bucket_t;

typedef struct {
	pm_metric_t base;
	pm_histogram_bucket_t *buckets;
	size_t num_buckets;
	double sum;
	unsigned long count;
} pm_histogram_t;

pm_histogram_t *pm_histogram_create(const char *name, const char *description, pm_label_t *labels,
				    size_t num_labels);
void pm_histogram_observe(pm_histogram_t *histogram, double value);
void pm_histogram_destroy(pm_histogram_t *histogram);

#endif /* PM_HISTOGRAM_H */
