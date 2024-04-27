#ifndef PM_METRIC_H
#define PM_METRIC_H

#include "pm_label.h"

typedef enum {
	PM_COUNTER,
	PM_GAUGE,
	PM_HISTOGRAM,
	PM_SUMMARY
} pm_metric_type_t;

#define MAX_METRIC_NAME_LENGTH        32
#define MAX_METRIC_DESCRIPTION_LENGTH 64

typedef struct pm_metric {
	pm_metric_type_t type;
	char name[MAX_METRIC_NAME_LENGTH];
	char description[MAX_METRIC_DESCRIPTION_LENGTH];
	pm_label_t labels[MAX_LABELS_PER_METRIC];
	int num_labels;
	// Add any other necessary fields
} pm_metric_t;

#endif /* PM_METRIC_H */
