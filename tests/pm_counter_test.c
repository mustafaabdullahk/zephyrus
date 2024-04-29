#include "prometheus/pm_counter.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_counter_creation()
{
	pm_label_t labels[] = {{"label1", "value1"}, {"label2", "value2"}};
	pm_counter_t *counter = pm_counter_create("test_counter", "Test counter", labels, 2);

	assert(counter != NULL);
	assert(counter->base.type == PM_COUNTER);
	assert(strcmp(counter->base.name, "test_counter") == 0);
	assert(strcmp(counter->base.description, "Test counter") == 0);
	assert(counter->base.num_labels == 2);
	assert(strcmp(counter->base.labels[0].key, "label1") == 0);
	assert(strcmp(counter->base.labels[0].value, "value1") == 0);
	assert(strcmp(counter->base.labels[1].key, "label2") == 0);
	assert(strcmp(counter->base.labels[1].value, "value2") == 0);
	assert(counter->value == 0);

	pm_counter_destroy(counter);
}

void test_counter_increment()
{
	pm_counter_t *counter = pm_counter_create("test_counter", "Test counter", NULL, 0);
	assert(counter != NULL);

	pm_counter_inc(counter);
	assert(counter->value == 1);

	pm_counter_inc(counter);
	assert(counter->value == 2);

	pm_counter_destroy(counter);
}

int main()
{
	test_counter_creation();
	test_counter_increment();

	printf("All tests passed successfully!\n");

	return 0;
}
