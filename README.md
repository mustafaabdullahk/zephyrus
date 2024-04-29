# Prometheus C Library (Zephyr Module)

A lightweight C library for exposing Prometheus metrics in Zephyr applications. This library allows Zephyr developers to easily integrate Prometheus metrics into their applications, enabling monitoring and observability in Zephyr-based systems.

## Description
Prometheus Exposer for Zephyr provides a simple and efficient way to expose metrics from Zephyr applications in the Prometheus format. It includes functionality to create various types of metrics such as counters, gauges, histograms, and summaries, register them with a collector, and expose them over HTTP for scraping by a Prometheus server.

## Getting Started
To get started with Prometheus C Library, follow these steps:

1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Build the library using the provided build system.
4. Integrate the library into your C project.

## Building and Running
To build the library and run the example applications, follow these steps:

1. Ensure that you have a C compiler (e.g., GCC) installed on your system.
2. Navigate to the project directory.
3. Run the build command following:
```bash
mkdir build && cd build
cmake ..
make all
```
4. After successful compilation, you can run the example applications to see the library in action.

## Usage
To use Prometheus C Library in your C project, follow these steps:
1. Include the necessary header files in your source files.
2. Initialize a collector to store your metrics.
3. Create various types of metrics (e.g., counters, gauges, histograms, summaries) and register them with the collector.
4. Expose the metrics via an HTTP endpoint using the provided exposer component.
5. Configure your Prometheus server to scrape the metrics from the endpoint.


```c
#include <prometheus.h>

// Initialize a counter metric
pm_counter_t *http_requests_total = pm_counter_create("http_requests_total", "Total HTTP requests", "method", "GET");

// Increment the counter
pm_counter_inc(http_requests_total);

// Register the metric with the collector
pm_collector_register_metric(collector, (pm_metric_t *)http_requests_total);
```

## Zephyr Module Usage

Repository create by taking advantage of following documents:

- [manifest](https://docs.zephyrproject.org/latest/develop/west/manifest.html#)
- [modules](https://docs.zephyrproject.org/latest/develop/modules.html#)

I suggest to read following zephyr issues:

- [add http server support pr](https://github.com/zephyrproject-rtos/zephyr/pull/64465)
- [replacement for civetweb](https://github.com/zephyrproject-rtos/zephyr/issues/46758)

### west.yml

- first one add mongoose module into main manifest(west.yaml) following way:

```yaml
  remotes:
    - name: zephyrproject-rtos
      url-base: https://github.com/zephyrproject-rtos
    - name: zephyrus
      url-base: https://github.com/mustafaabdullahk/zephyrus

  projects:
    - name: zephyr
      remote: zephyrproject-rtos
      revision: main
      import:
        # By using name-allowlist we can clone only the modules that are
        # strictly needed by the application.
        name-allowlist:
          - cmsis      # required by the ARM port
          - hal_nordic # required by the custom_plank board (Nordic based)
          - hal_stm32  # required by the nucleo_f302r8 board (STM32 based)
          - mbedtls

    - name: zephyrus
      remote: mustafaabdullahk # referred from remotes can change other forks or main
      submodules: true # prometheus add as submodule
      revision: main # can select different branch
      repo-path: .
      path: modules/lib/prometheus
```

- second one run `west update` and initialized mongoose module

- third one can add your prj.conf for using mongoose library.

```
CONFIG_PROMETHEUS=y

CONFIG_NETWORKING=y
CONFIG_NET_IPV4=y
CONFIG_NET_IPV6=y
CONFIG_NET_TCP=y
CONFIG_NET_UDP=y
CONFIG_NET_DHCPV4=y
CONFIG_NET_SOCKETS=y
CONFIG_NET_SOCKETS_POLL_MAX=32
CONFIG_POSIX_MAX_FDS=32
CONFIG_NET_MAX_CONN=10
CONFIG_NET_MAX_CONTEXTS=10
CONFIG_NET_CONFIG_SETTINGS=y
CONFIG_NET_CONNECTION_MANAGER=y
CONFIG_NET_LOG=y

CONFIG_LOG=y
CONFIG_ISR_STACK_SIZE=2048
CONFIG_MAIN_STACK_SIZE=8192
CONFIG_IDLE_STACK_SIZE=1024

CONFIG_MINIMAL_LIBC=y
CONFIG_MINIMAL_LIBC_RAND=y
CONFIG_MINIMAL_LIBC_MALLOC_ARENA_SIZE=32768
```

- add library and ready to start coding:

```c
#include <mongoose.h>
```

## Features
- Lightweight and efficient C library for instrumenting C-based applications.
- Supports various types of metrics, including counters, gauges, histograms, and summaries.
- Exposes metrics in the Prometheus exposition format via an HTTP endpoint.
- Easy to integrate into existing C projects.
- Well-documented API and example applications for reference.

## Contributing
Contributions to Prometheus C Library are welcome! If you would like to contribute, please follow these guidelines:
1. Fork the repository and create a new branch for your feature or bug fix.
2. Implement your changes and ensure that all tests pass.
3. Submit a pull request with a detailed description of your changes and the problem they solve.
4. Participate in code reviews and address any feedback from the maintainers.

## License
This project is licensed under the Apache 2.0 License - see the [LICENSE](LICENSE) file for details.
