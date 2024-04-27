#include "prometheus/pm_summary.h"

#include <stdlib.h>
#include <string.h>

pm_summary_t *pm_summary_create(const char *name, const char *description, pm_label_t *labels,
				size_t num_labels)
{
	pm_summary_t *summary = (pm_summary_t *)malloc(sizeof(pm_summary_t));
	if (summary == NULL) {
		return NULL; // Allocation failed
	}

	summary->base.type = PM_SUMMARY;
	strncpy(summary->base.name, name, MAX_METRIC_NAME_LENGTH - 1);
	summary->base.name[MAX_METRIC_NAME_LENGTH - 1] = '\0';
	strncpy(summary->base.description, description, MAX_METRIC_DESCRIPTION_LENGTH - 1);
	summary->base.description[MAX_METRIC_DESCRIPTION_LENGTH - 1] = '\0';
	summary->base.num_labels = num_labels;
	if (num_labels > 0) {
		memcpy(summary->base.labels, labels, num_labels * sizeof(pm_label_t));
	}

	// Initialize summary specific fields
	summary->quantiles = NULL;
	summary->num_quantiles = 0;
	summary->sum = 0;
	summary->count = 0;

	return summary;
}

void pm_summary_observe(pm_summary_t *summary, double value)
{
	// Increment count
	summary->count++;

	// Update sum
	summary->sum += value;
}

void pm_summary_destroy(pm_summary_t *summary)
{
	// Free quantiles array
	free(summary->quantiles);

	// Free summary struct
	free(summary);
}