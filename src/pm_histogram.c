#include "prometheus/pm_histogram.h"

#include <stdlib.h>
#include <string.h>

pm_histogram_t *pm_histogram_create(const char *name, const char *description, pm_label_t *labels,
				    size_t num_labels)
{
	pm_histogram_t *histogram = (pm_histogram_t *)malloc(sizeof(pm_histogram_t));
	if (histogram == NULL) {
		return NULL; // Allocation failed
	}

	histogram->base.type = PM_HISTOGRAM;
	strncpy(histogram->base.name, name, MAX_METRIC_NAME_LENGTH - 1);
	histogram->base.name[MAX_METRIC_NAME_LENGTH - 1] = '\0';
	strncpy(histogram->base.description, description, MAX_METRIC_DESCRIPTION_LENGTH - 1);
	histogram->base.description[MAX_METRIC_DESCRIPTION_LENGTH - 1] = '\0';
	histogram->base.num_labels = num_labels;
	if (num_labels > 0) {
		memcpy(histogram->base.labels, labels, num_labels * sizeof(pm_label_t));
	}

	// Initialize histogram specific fields
	histogram->buckets = NULL;
	histogram->num_buckets = 0;
	histogram->sum = 0;
	histogram->count = 0;

	return histogram;
}

void pm_histogram_observe(pm_histogram_t *histogram, double value)
{
	// Increment count
	histogram->count++;

	// Update sum
	histogram->sum += value;

	// Find appropriate bucket
	for (size_t i = 0; i < histogram->num_buckets; ++i) {
		if (value <= histogram->buckets[i].upper_bound) {
			// Increment count for the bucket
			histogram->buckets[i].count++;
			break;
		}
	}
}

void pm_histogram_destroy(pm_histogram_t *histogram)
{
	// Free buckets array
	free(histogram->buckets);

	// Free histogram struct
	free(histogram);
}
