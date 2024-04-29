#include "prometheus/pm_exposer.h"
#include "prometheus/pm_formatter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __ZEPHYR__

#include <zephyr/posix/unistd.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/netinet/in.h>
#include <zephyr/posix/arpa/inet.h>

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif

#define BUFFER_SIZE 2048

void handle_connection(int client_socket, const pm_collector_t *collector)
{
	char request[BUFFER_SIZE];
	memset(request, 0, BUFFER_SIZE);

	printf("Handling connection\n");

	// Read request from client
	ssize_t bytes_received = recv(client_socket, request, BUFFER_SIZE, 0);
	if (bytes_received < 0) {
		printf("Error receiving data from client");
		close(client_socket);
		return;
	}

	// TO DO: Implement request parsing
	// Check if request contains metrics path
	// if (strstr(request, "GET ") == NULL || strstr(request, "HTTP/1.1") == NULL ||
	// strstr(request, "Connection: close") == NULL) {
	//     close(client_socket);
	//     return;
	// }

	// Prepare response headers
	const char *response_headers = "HTTP/1.1 200 OK\r\nContent-Type: text/plain; "
				       "version=0.0.4\r\nConnection: close\r\n\r\n";

	// Format metrics into buffer
	char metrics_buffer[BUFFER_SIZE];
	memset(metrics_buffer, 0, BUFFER_SIZE);
	pm_format_exposition(collector, metrics_buffer, BUFFER_SIZE);

	// Send response
	send(client_socket, response_headers, strlen(response_headers), 0);
	send(client_socket, metrics_buffer, strlen(metrics_buffer), 0);

	// Close client socket
	close(client_socket);
}

void pm_exposer_start(int port, const char *metrics_path, const pm_collector_t *collector)
{
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	// Create socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	int option = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (server_socket < 0) {
		perror("Error opening socket");
		exit(EXIT_FAILURE);
	}

	// Set up server address
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	// Bind socket
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Error binding socket");
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	if (listen(server_socket, 5) < 0) {
		perror("Error listening for connections");
		exit(EXIT_FAILURE);
	}

	printf("Prometheus exposer listening on port %d\n", port);

	// Accept connections and handle requests
	while (1) {
		client_socket =
			accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client_socket < 0) {
			printf("Error accepting connection");
			continue;
		}

		// Handle connection in a separate function
		handle_connection(client_socket, collector);
	}

	// Close server socket
	close(server_socket);
}
