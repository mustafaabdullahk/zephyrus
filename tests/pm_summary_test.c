#include "prometheus/pm_summary.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_summary_creation()
{
	// Create summary
	pm_label_t labels[] = {{"endpoint", "/metrics"}};
	pm_summary_t *summary = pm_summary_create("request_size_bytes", "Request size", labels, 1);

	// Check if summary is not NULL
	assert(summary != NULL);
	assert(summary->base.type == PM_SUMMARY);
	assert(strcmp(summary->base.name, "request_size_bytes") == 0);
	assert(strcmp(summary->base.description, "Request size") == 0);
	assert(summary->base.num_labels == 1);
	assert(strcmp(summary->base.labels[0].key, "endpoint") == 0);
	assert(strcmp(summary->base.labels[0].value, "/metrics") == 0);
	assert(summary->num_quantiles == 0);
	assert(summary->quantiles == NULL);
	assert(summary->sum == 0);
	assert(summary->count == 0);

	pm_summary_destroy(summary);
}

void test_summary_observe()
{
	// Create summary
	pm_label_t labels[] = {{"endpoint", "/metrics"}};
	pm_summary_t *summary = pm_summary_create("request_size_bytes", "Request size", labels, 1);

	// Observe values
	pm_summary_observe(summary, 100);
	pm_summary_observe(summary, 200);
	pm_summary_observe(summary, 300);

	// Check counts
	assert(summary->count == 3);
	assert(summary->sum == 600);
	assert(summary->num_quantiles == 0); // No quantiles added yet

	pm_summary_destroy(summary);
}

int main()
{
	test_summary_creation();
	test_summary_observe();

	printf("Summary tests passed successfully!\n");

	return 0;
}
