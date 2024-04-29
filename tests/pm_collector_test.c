#include "prometheus/pm_collector.h"

#include "prometheus/pm_counter.h"
#include "prometheus/pm_gauge.h"
#include "prometheus/pm_histogram.h"
#include "prometheus/pm_summary.h"

#include <stdio.h>
#include <assert.h>

void test_collector_create()
{
	pm_collector_t *collector = pm_collector_create();
	assert(collector != NULL);
	assert(collector->size == 0);
	pm_collector_destroy(collector);
}

void test_register_get_counter()
{
	pm_collector_t *collector = pm_collector_create();
	assert(collector != NULL);

	pm_label_t labels[] = {{"label1", "value1"}, {"label2", "value2"}};
	pm_counter_t *counter = pm_counter_create("test_counter", "Test counter", labels, 2);
	pm_collector_register_metric(collector, (pm_metric_t *)counter);
	const pm_metric_t *retrieved_counter = pm_collector_get_metric(collector, "test_counter");
	assert(retrieved_counter != NULL && "error: counter not found");
	assert(retrieved_counter->type == PM_COUNTER);

	pm_counter_destroy(counter);
	pm_collector_destroy(collector);
}

void test_register_get_gauge()
{
	// to do: gauge registration and retrieval test
}

void test_register_get_histogram()
{
	pm_collector_t *collector = pm_collector_create();
	assert(collector != NULL);

	// Create histogram
	pm_label_t labels[] = {{"endpoint", "/api"}, {"method", "POST"}};
	pm_histogram_t *histogram =
		pm_histogram_create("request_duration_seconds", "Request duration", labels, 2);
	pm_collector_register_metric(collector, (pm_metric_t *)histogram);

	// Retrieve histogram
	const pm_metric_t *retrieved_histogram =
		pm_collector_get_metric(collector, "request_duration_seconds");
	assert(retrieved_histogram != NULL);
	assert(retrieved_histogram->type == PM_HISTOGRAM);

	pm_histogram_destroy(histogram);
	pm_collector_destroy(collector);
}

void test_register_get_summary()
{
	pm_collector_t *collector = pm_collector_create();
	assert(collector != NULL);

	// Create summary
	pm_label_t labels[] = {{"endpoint", "/metrics"}};
	pm_summary_t *summary = pm_summary_create("request_size_bytes", "Request size", labels, 1);
	pm_collector_register_metric(collector, (pm_metric_t *)summary);

	// Retrieve summary
	const pm_metric_t *retrieved_summary =
		pm_collector_get_metric(collector, "request_size_bytes");
	assert(retrieved_summary != NULL);
	assert(retrieved_summary->type == PM_SUMMARY);

	pm_summary_destroy(summary);
	pm_collector_destroy(collector);
}

int main()
{
	test_collector_create();
	test_register_get_counter();
	test_register_get_gauge();
	test_register_get_histogram();
	test_register_get_summary();

	printf("pm_collector tests passed successfully!\n");

	return 0;
}
