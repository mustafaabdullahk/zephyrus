#include "prometheus/pm_counter.h"

#include <stdlib.h>
#include <string.h>

pm_counter_t *pm_counter_create(const char *name, const char *desc, pm_label_t *labels,
				int num_labels)
{
	pm_counter_t *counter = (pm_counter_t *)malloc(sizeof(pm_counter_t));
	if (counter) {
		counter->base.type = PM_COUNTER;
		strncpy(counter->base.name, name, MAX_METRIC_NAME_LENGTH - 1);
		counter->base.name[MAX_METRIC_NAME_LENGTH - 1] = '\0';
		strncpy(counter->base.description, desc, MAX_METRIC_DESCRIPTION_LENGTH - 1);
		counter->base.description[MAX_METRIC_DESCRIPTION_LENGTH - 1] = '\0';
		counter->base.num_labels = num_labels;
		if (num_labels > 0) {
			memcpy(counter->base.labels, labels, num_labels * sizeof(pm_label_t));
		}
		counter->value = 0;
	}
	return counter;
}

void pm_counter_inc(pm_counter_t *counter)
{
	if (counter) {
		counter->value++;
	}
}

void pm_counter_destroy(pm_counter_t *counter)
{
	if (counter) {
		free(counter);
	}
}
