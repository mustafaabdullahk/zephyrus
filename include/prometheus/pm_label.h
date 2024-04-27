#ifndef PM_LABEL_H
#define PM_LABEL_H

#define MAX_LABEL_KEY_LENGTH   16
#define MAX_LABEL_VALUE_LENGTH 16
#define MAX_LABELS_PER_METRIC  5 // Maximum number of labels per metric

typedef struct pm_label {
	char key[MAX_LABEL_KEY_LENGTH];
	char value[MAX_LABEL_VALUE_LENGTH];
} pm_label_t;

#endif /* PM_LABEL_H */
