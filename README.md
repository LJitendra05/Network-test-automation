# Python Network Test Automation Framework

A beginner-friendly Python and pytest-based framework for automating network connectivity and basic network performance tests.

## Features

- Network connectivity testing
- Positive and negative test cases
- Data-driven testing using JSON configuration
- Network performance testing
- Packet loss measurement
- Latency measurement
- Configurable performance thresholds
- Logging of test execution
- Error handling
- Cross-platform ping command handling
- Git-based version control

## Project Structure

```text
network-test-automation/
│
├── config/
│   └── devices.json
│
├── tests/
│   ├── test_connectivity.py
│   └── test_performance.py
│
├── utils/
│   ├── config_loader.py
│   └── network_utils.py
│
├── .gitignore
├── requirements.txt
└── README.md
```
## Technologies

- Python
- pytest
- JSON
- Git
- Windows/Linux-compatible command handling

## Setup

Create a virtual environment:

    python -m venv venv

Activate it on Windows:

    .\venv\Scripts\activate

Install dependencies:

    pip install -r requirements.txt

## Running Tests

Run all tests:

    python -m pytest -v

## Test Coverage

The framework currently tests:

1. Network host connectivity
2. Expected unreachable hosts
3. Network performance
4. Packet loss
5. Average latency against configured thresholds

## Example

A device can be configured in `config/devices.json` with:

- Host/IP address
- Expected reachability
- Maximum allowed latency
- Maximum allowed packet loss

The test framework reads this configuration and executes the appropriate tests automatically.

## Learning Outcome

This project provided hands-on experience with Python test automation, pytest, network testing, test-case design, configuration-driven testing, logging, error handling, and Git-based development.
