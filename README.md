
# SmartOrder — Smart Order Router Simulator

A C++14-based trading simulator that processes buy and sell orders, routes them across simulated exchanges, and generates execution reports with transaction fees and order status.

The project is being developed to explore core C++ programming, trading system architecture, order routing, and concurrent order processing.

## Current Features

### 1. Order Processing
- Processes simulated BUY and SELL orders.
- Assigns order IDs.
- Tracks requested, filled, and unfilled quantities.
- Reports order execution status.

### 2. Smart Order Routing
- Supports multiple simulated exchanges.
- Routes orders across Exchange A, Exchange B, and Exchange C.
- Considers exchange prices and fees in the routing logic.

### 3. Execution Reports
Each processed order generates a report containing:
- Order ID and side (BUY/SELL)
- Requested quantity
- Selected exchange and execution details
- Execution price and transaction fee
- Filled and unfilled quantities
- Total cost or net proceeds
- Order status (FILLED)

### 4. Multithreaded Order Processing
- Uses worker threads for concurrent order processing.
- Implements a producer-consumer style order processing architecture.
- Uses C++ concurrency primitives in the order processor.

### 5. Performance Summary
The simulator reports:
- Total orders submitted
- Orders completed
- Total processing time
- Simulator throughput in orders per second

## Technology Stack

- C++14
- GCC (MinGW-w64 / MSYS2 UCRT64)
- Visual Studio Code
- C++ Standard Library
- STL containers and concurrency primitives

## Project Structure

```text
SmartOrder/
│
├── .vscode/
│   ├── c_cpp_properties.json
│   ├── launch.json
│   ├── settings.json
│   └── tasks.json
│
├── include/
│   ├── Order.h
│   ├── Exchange.h
│   ├── OrderProcessor.h
│   ├── RiskManager.h
│   └── SmartRouter.h
│
├── src/
│   ├── main.cpp
│   ├── Exchange.cpp
│   ├── OrderProcessor.cpp
│   ├── RiskManager.cpp
│   └── SmartRouter.cpp
│
└── README.md
```

## How to Build and Run

### Prerequisites

- A C++14-compatible GCC compiler
- Visual Studio Code (optional, for editing and debugging)

### Compile

Open the terminal in the project root directory and run:

```powershell
& "C:\msys64\ucrt64\bin\g++.exe" -std=c++14 -pthread -Iinclude src/main.cpp src/Exchange.cpp src/RiskManager.cpp src/SmartRouter.cpp src/OrderProcessor.cpp -o router.exe
```

### Run

```powershell
.\router.exe
```

Note: The compiler path may differ depending on your local installation.

## Sample Output

The following is based on the current simulator output:

```text
================================
Order ID: 1
Side: BUY
Requested: 10
Executions:
  Exchange_B | Quantity: 10 | Price: 249.00 | Fee: 2.00
Filled: 10
Unfilled: 0
Total Cost incl. fees: 2492.00
Status: FILLED
================================
```

The simulator also prints a summary:

```text
========== SUMMARY ==========
Total orders submitted: 20
Orders completed: 20
Time taken: 0.03 seconds
Simulator throughput: 687.97 orders/second
=============================
```

## Current Benchmark

| Metric | Observed Result |
|---|---:|
| Orders submitted | 20 |
| Orders completed | 20 |
| Reported processing time | 0.03 seconds |
| Reported throughput (Run 1) | 687.97 orders/second |
| Reported throughput (Run 2) | 664.30 orders/second |

These are initial simulator measurements from small test runs, not production or high-frequency trading performance benchmarks.


## Project Status

**Current stage:** Working simulator prototype.

The current implementation demonstrates simulated order submission, exchange routing, execution reporting, and initial concurrent processing and throughput measurements.

The simulator uses in-memory simulated exchanges and does not connect to live exchanges or execute real trades.
