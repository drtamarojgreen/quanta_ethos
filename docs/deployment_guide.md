# Build and Deployment Strategy

## Unified Build System

A top-level `Makefile` or `build_all.sh` script will orchestrate the build process for the entire ecosystem:
-   Run `cmake` and `make` for all C++ components.
-   Create Python virtual environments and install dependencies via `pip` for all Python components.
-   Run `R` dependency installation scripts.

To build the entire system, run the `build_all.sh` script from the root of the project:
```bash
./build_all.sh
```

## Unified Testing

A `test_all.sh` script will run unit and integration tests for each module, using mock components where necessary to ensure isolated and repeatable testing.

To run the test suite, run the `test_all.sh` script from the root of the project:
```bash
./test_all.sh
```

## Master Control Script

A `start_system.sh` script will launch all necessary components in the correct order (e.g., monitoring daemons, `QuantaLista` in a watch loop).

To start the PrismQuanta ecosystem, run the `start_system.sh` script from the root of the project:
```bash
./start_system.sh
```
