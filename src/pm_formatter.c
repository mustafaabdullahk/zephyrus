#include "prometheus/pm_formatter.h"
#include "prometheus/pm_collector.h"
#include "prometheus/pm_metric.h"
#include "prometheus/pm_histogram.h"
#include "prometheus/pm_summary.h"
#include "prometheus/pm_gauge.h"
#include "prometheus/pm_counter.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// Helper function to write formatted string to buffer, handling buffer size
static int write_to_buffer(char *buffer, size_t buffer_size, const char *str)
{
	int len = snprintf(buffer + strlen(buffer), buffer_size - strlen(buffer), "%s", str);
	if (len >= 0 && len < buffer_size - strlen(buffer)) {
		return 1; // Success
	}

	return 0; // Error (insufficient buffer size)
}

// Helper function to write formatted metric to buffer
static int write_metric_to_buffer(char *buffer, size_t buffer_size, const char *format, ...)
{
	char temp_buffer[256];
	va_list args;

	va_start(args, format);
	vsnprintf(temp_buffer, sizeof(temp_buffer), format, args);
	va_end(args);

	return write_to_buffer(buffer, buffer_size, temp_buffer);
}

void pm_format_exposition(const pm_collector_t *collector, char *buffer, size_t buffer_size)
{
	if (collector == NULL || buffer == NULL || buffer_size == 0) {
		return;
	}

	int written = 0;

	// Iterate through each metric in the collector
	pm_collector_node_t *current = collector->head;
	while (current != NULL) {
		const pm_metric_t *metric = current->metric;

		// Write HELP line if available
		if (metric->description[0] != '\0') {
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "# HELP %s %s\n", metric->name,
						    metric->description)) {
				return; // Error writing to buffer
			}
		}
		printf("metric->name: %s\n", metric->name);
		printf("metric->type: %d\n", metric->type);

		// Write TYPE line
		switch (metric->type) {
		case PM_COUNTER:
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "# TYPE %s counter\n", metric->name)) {
				printf("error writing counter\n");
				return; // Error writing to buffer
			}
			break;
		case PM_GAUGE:
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "# TYPE %s gauge\n", metric->name)) {
				printf("error writing gauge\n");
				return; // Error writing to buffer
			}
			break;
		case PM_HISTOGRAM:
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "# TYPE %s histogram\n", metric->name)) {
				printf("error writing histogram\n");
				return; // Error writing to buffer
			}
			break;
		case PM_SUMMARY:
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "# TYPE %s summary\n", metric->name)) {
				printf("error writing summary\n");
				return; // Error writing to buffer
			}
			break;
		default:
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "# TYPE %s untyped\n", metric->name)) {
				return; // Error writing to buffer
			}
			break;
		}

		// Write metric-specific fields
		switch (metric->type) {
		case PM_COUNTER:
			const pm_counter_t *counter = (const pm_counter_t *)metric;
			printf("counter->value: %lu\n", counter->value);
			printf("counter->num_labels: %d\n", metric->num_labels);
			for (int i = 0; i < metric->num_labels; ++i) {
				if (!write_metric_to_buffer(buffer + written, buffer_size - written,
							    "%s{%s=\"%s\"} %lu\n", metric->name,
							    metric->labels[i].key,
							    metric->labels[i].value,
							    counter->value)) {
					return; // Error writing to buffer
				}
			}
			break;
		case PM_GAUGE:
			const pm_gauge_t *gauge = (const pm_gauge_t *)metric;
			printf("gauge->value: %f\n", gauge->value);
			printf("gauge->num_labels: %d\n", metric->num_labels);
			for (int i = 0; i < metric->num_labels; ++i) {
				if (!write_metric_to_buffer(buffer + written, buffer_size - written,
							    "%s{%s=\"%s\"} %f\n", metric->name,
							    metric->labels[i].key,
							    metric->labels[i].value,
							    gauge->value)) {
					return; // Error writing to buffer
				}
			}
			break;
		case PM_HISTOGRAM: {
			const pm_histogram_t *histogram = (const pm_histogram_t *)metric;
			for (int i = 0; i < histogram->num_buckets; ++i) {
				if (!write_metric_to_buffer(buffer + written, buffer_size - written,
							    "%s_bucket{le=\"%f\"} %lu\n",
							    metric->name,
							    histogram->buckets[i].upper_bound,
							    histogram->buckets[i].count)) {
					return; // Error writing to buffer
				}
			}
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "%s_sum %f\n", metric->name, histogram->sum)) {
				return; // Error writing to buffer
			}
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "%s_count %lu\n", metric->name,
						    histogram->count)) {
				return; // Error writing to buffer
			}
			break;
		}
		case PM_SUMMARY: {
			const pm_summary_t *summary = (const pm_summary_t *)metric;
			for (int i = 0; i < summary->num_quantiles; ++i) {
				if (!write_metric_to_buffer(buffer + written, buffer_size - written,
							    "%s{%s=\"%f\"} %f\n", metric->name,
							    "quantile",
							    summary->quantiles[i].quantile,
							    summary->quantiles[i].value)) {
					return; // Error writing to buffer
				}
			}
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "%s_sum %f\n", metric->name, summary->sum)) {
				return; // Error writing to buffer
			}
			if (!write_metric_to_buffer(buffer + written, buffer_size - written,
						    "%s_count %lu\n", metric->name,
						    summary->count)) {
				return; // Error writing to buffer
			}
			break;
		}
		default:
			// Should not happen
			break;
		}
		// Move to the next metric in the collector
		current = current->next;
	}
}
