#include "prometheus/pm_histogram.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_histogram_creation()
{
	// Create histogram
	pm_label_t labels[] = {{"endpoint", "/api"}, {"method", "POST"}};
	pm_histogram_t *histogram =
		pm_histogram_create("request_duration_seconds", "Request duration", labels, 2);

	// Check if histogram is not NULL
	assert(histogram != NULL);
	assert(histogram->base.type == PM_HISTOGRAM);
	assert(strcmp(histogram->base.name, "request_duration_seconds") == 0);
	assert(strcmp(histogram->base.description, "Request duration") == 0);
	assert(histogram->base.num_labels == 2);
	assert(strcmp(histogram->base.labels[0].key, "endpoint") == 0);
	assert(strcmp(histogram->base.labels[0].value, "/api") == 0);
	assert(strcmp(histogram->base.labels[1].key, "method") == 0);
	assert(strcmp(histogram->base.labels[1].value, "POST") == 0);
	assert(histogram->num_buckets == 0);
	assert(histogram->buckets == NULL);
	assert(histogram->sum == 0);
	assert(histogram->count == 0);

	pm_histogram_destroy(histogram);
}

void test_histogram_observe()
{
	// Create histogram
	pm_label_t labels[] = {{"endpoint", "/api"}, {"method", "POST"}};
	pm_histogram_t *histogram =
		pm_histogram_create("request_duration_seconds", "Request duration", labels, 2);

	// Observe values
	pm_histogram_observe(histogram, 0.5);
	pm_histogram_observe(histogram, 1.0);
	pm_histogram_observe(histogram, 2.0);

	// Check counts
	assert(histogram->count == 3);
	assert(histogram->sum == 3.5);
	assert(histogram->num_buckets == 3);
	assert(histogram->buckets != NULL);
	assert(histogram->buckets[0].upper_bound == 0.5);
	assert(histogram->buckets[0].count == 1);
	assert(histogram->buckets[1].upper_bound == 1.0);
	assert(histogram->buckets[1].count == 2);
	assert(histogram->buckets[2].upper_bound == 2.0);
	assert(histogram->buckets[2].count == 3);

	pm_histogram_destroy(histogram);
}

int main()
{
	test_histogram_creation();
	test_histogram_observe();
	printf("Histogram tests passed successfully!\n");
	return 0;
}
