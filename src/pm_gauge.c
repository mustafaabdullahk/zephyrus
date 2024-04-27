#include "prometheus/pm_gauge.h"

#include <stdlib.h>

pm_gauge_t *pm_gauge_create(const char *name, const char *desc, pm_label_t *labels, int num_labels)
{
	pm_gauge_t *gauge = (pm_gauge_t *)malloc(sizeof(pm_gauge_t));
	if (gauge) {
		gauge->base.type = PM_GAUGE;
		strncpy(gauge->base.name, name, MAX_METRIC_NAME_LENGTH - 1);
		gauge->base.name[MAX_METRIC_NAME_LENGTH - 1] = '\0';
		strncpy(gauge->base.description, desc, MAX_METRIC_DESCRIPTION_LENGTH - 1);
		gauge->base.description[MAX_METRIC_DESCRIPTION_LENGTH - 1] = '\0';
		gauge->base.num_labels = num_labels;
		if (num_labels > 0) {
			memcpy(gauge->base.labels, labels, num_labels * sizeof(pm_label_t));
		}
		gauge->value = 0;
	}
	return gauge;
}

void pm_gauge_set(pm_gauge_t *gauge, double value)
{
	if (gauge) {
		gauge->value = value;
	}
}

void pm_gauge_destroy(pm_gauge_t *gauge)
{
	if (gauge) {
		free(gauge);
	}
}
