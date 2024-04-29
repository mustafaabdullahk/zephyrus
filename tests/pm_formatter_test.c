#include "prometheus/pm_formatter.h"
#include "prometheus/pm_collector.h"
#include "prometheus/pm_counter.h"
#include "prometheus/pm_gauge.h"
#include "prometheus/pm_histogram.h"
#include "prometheus/pm_summary.h"
#include "prometheus/pm_exposer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT         9090
#define METRICS_PATH "/metrics"
#define BUFFER_SIZE  4096

void *start_exposer(void *args)
{
	const pm_collector_t *collector = (const pm_collector_t *)args;
	pm_exposer_start(PORT, METRICS_PATH, collector);
	pthread_exit(NULL);
}

void test_formatter()
{
	// Create a mock collector with different types of metrics
	pm_collector_t *collector = pm_collector_create();

	pm_counter_t *counter =
		pm_counter_create("http_requests_total", "Total HTTP requests", NULL, 0);
	pm_counter_inc(counter);
	pm_collector_register_metric(collector, (pm_metric_t *)counter);

	pm_gauge_t *gauge = pm_gauge_create("temperature", "Current temperature", NULL, 0);
	pm_gauge_set(gauge, 25.5);
	pm_collector_register_metric(collector, (pm_metric_t *)gauge);

	pm_histogram_t *histogram =
		pm_histogram_create("request_duration_seconds", "Request duration", NULL, 0);
	pm_histogram_observe(histogram, 0.5);
	pm_histogram_observe(histogram, 1.0);
	pm_collector_register_metric(collector, (pm_metric_t *)histogram);

	pm_summary_t *summary = pm_summary_create("response_size_bytes", "Response size", NULL, 0);
	pm_summary_observe(summary, 100);
	pm_summary_observe(summary, 200);
	pm_collector_register_metric(collector, (pm_metric_t *)summary);

	// Prepare buffer to hold formatted metrics
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);

	// Format metrics into buffer
	pm_format_exposition(collector, buffer, BUFFER_SIZE);

	// Print formatted metrics
	printf("Formatted metrics:\n%s\n", buffer);

	// Cleanup
	pm_collector_destroy(collector);
}

int main()
{
	test_formatter();
	printf("Formatter test passed successfully!\n");
	return 0;
}
