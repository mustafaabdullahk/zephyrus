#include "prometheus/pm_metric.h"

#include <string.h>
#include <stdlib.h>

pm_metric_t *pm_metric_create(pm_metric_type_t type, const char *name, const char *desc,
			      pm_label_t *labels, int num_labels)
{
	pm_metric_t *metric = (pm_metric_t *)malloc(sizeof(pm_metric_t));
	if (metric) {
		metric->type = type;
		strncpy(metric->name, name, MAX_METRIC_NAME_LENGTH - 1);
		metric->name[MAX_METRIC_NAME_LENGTH - 1] = '\0';
		strncpy(metric->description, desc, MAX_METRIC_DESCRIPTION_LENGTH - 1);
		metric->description[MAX_METRIC_DESCRIPTION_LENGTH - 1] = '\0';
		metric->num_labels = num_labels;
		if (num_labels > 0) {
			memcpy(metric->labels, labels, num_labels * sizeof(pm_label_t));
		}
	}
	return metric;
}

void pm_metric_destroy(pm_metric_t *metric)
{
	if (metric) {
		free(metric);
	}
}
