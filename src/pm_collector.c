#include "prometheus/pm_collector.h"

#include "prometheus/pm_metric.h"
#include "prometheus/pm_histogram.h"
#include "prometheus/pm_summary.h"
#include "prometheus/pm_counter.h"
#include "prometheus/pm_gauge.h"

#include <stdlib.h>
#include <string.h>

pm_collector_t *pm_collector_create()
{
	pm_collector_t *collector = (pm_collector_t *)malloc(sizeof(pm_collector_t));
	if (collector == NULL) {
		return NULL; // Allocation failed
	}

	collector->head = NULL;
	collector->size = 0;

	return collector;
}

void pm_collector_register_metric(pm_collector_t *collector, const pm_metric_t *metric)
{
	// Allocate memory for the new node
	pm_collector_node_t *node = (pm_collector_node_t *)malloc(sizeof(pm_collector_node_t));
	if (node == NULL) {
		return; // Allocation failed
	}

	// Allocate memory for the specific metric type and copy the metric data
	switch (metric->type) {
	case PM_COUNTER:
		node->metric = (pm_metric_t *)malloc(sizeof(pm_counter_t));
		if (node->metric != NULL) {
			memcpy(node->metric, metric, sizeof(pm_counter_t));
		}
		break;
	case PM_GAUGE:
		node->metric = (pm_metric_t *)malloc(sizeof(pm_gauge_t));
		if (node->metric != NULL) {
			memcpy(node->metric, metric, sizeof(pm_gauge_t));
		}
		break;
	case PM_HISTOGRAM:
		node->metric = (pm_metric_t *)malloc(sizeof(pm_histogram_t));
		if (node->metric != NULL) {
			memcpy(node->metric, metric, sizeof(pm_histogram_t));
		}
		break;
	case PM_SUMMARY:
		node->metric = (pm_metric_t *)malloc(sizeof(pm_summary_t));
		if (node->metric != NULL) {
			memcpy(node->metric, metric, sizeof(pm_summary_t));
		}
		break;
	default:
		node->metric = NULL; // Unsupported metric type
		break;
	}

	if (node->metric == NULL) {
		free(node); // Free the node if allocation failed for the specific metric type
		return;
	}

	// Insert the new node at the beginning of the list
	node->next = collector->head;
	collector->head = node;
	collector->size++;
}

const pm_metric_t *pm_collector_get_metric(const pm_collector_t *collector, const char *name)
{
	// Traverse the list and find the metric by name
	pm_collector_node_t *current = collector->head;
	while (current != NULL) {
		if (strcmp(current->metric->name, name) == 0) {
			return current->metric;
		}
		current = current->next;
	}
	return NULL; // Metric not found
}

void pm_collector_destroy(pm_collector_t *collector)
{
	// Free memory for each node and its metric
	pm_collector_node_t *current = collector->head;
	while (current != NULL) {
		pm_collector_node_t *next = current->next;
		free(current->metric); // Free metric-specific memory
		free(current);         // Free node
		current = next;
	}

	// Free collector struct
	free(collector);
}
