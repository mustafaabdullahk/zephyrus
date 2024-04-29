#include "prometheus/pm_exposer.h"
#include "prometheus/pm_collector.h"
#include "prometheus/pm_counter.h"
#include "prometheus/pm_gauge.h"
#include "prometheus/pm_histogram.h"
#include "prometheus/pm_summary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT         8080
#define METRICS_PATH "/metrics"

void *start_exposer(void *args)
{
	const pm_collector_t *collector = (const pm_collector_t *)args;
	pm_exposer_start(PORT, METRICS_PATH, collector);
	pthread_exit(NULL);
}

void test_exposer()
{
	// Create a mock collector with different types of metrics
	pm_collector_t *collector = pm_collector_create();

	pm_label_t labels[] = {{"label1", "value1"}, {"label2", "value2"}};
	pm_counter_t *counter = pm_counter_create("test_counter", "Test counter", labels, 2);
	pm_counter_inc(counter);
	pm_collector_register_metric(collector, (pm_metric_t *)counter);

	// TO DO: Add more metrics to the collector and test them

	pm_exposer_start(PORT, METRICS_PATH, collector);

	// Start exposer server in a separate thread
	pthread_t exposer_thread;
	if (pthread_create(&exposer_thread, NULL, start_exposer, (void *)collector) != 0) {
		perror("Error creating exposer thread");
		exit(EXIT_FAILURE);
	}

	// Sleep for a while to ensure server is started
	sleep(1);

	// Send HTTP GET request to exposer server
	printf("Sending HTTP GET request to exposer server...\n");
	FILE *curl_output = popen("curl -s localhost:9090/metrics", "r");
	if (curl_output == NULL) {
		perror("Error executing curl command");
		exit(EXIT_FAILURE);
	}

	// Read and print the response from the exposer server
	printf("Response from exposer server:\n");
	char response_buffer[2048];
	memset(response_buffer, 0, sizeof(response_buffer));
	fread(response_buffer, sizeof(char), sizeof(response_buffer) - 1, curl_output);
	printf("%s\n", response_buffer);

	// Close the curl process
	pclose(curl_output);

	// Cleanup
	pthread_cancel(exposer_thread);
	pm_collector_destroy(collector);
}

int main()
{
	test_exposer();
	printf("Exposer test passed successfully!\n");
	return 0;
}
