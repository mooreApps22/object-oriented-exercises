# Bank Server with accompanied Customer Client 

A multi-client TCP bak simulation written in C++98.

This project implements a simple client-server banking system where multiple customers can connect to a central bank server and perform operations such as creating/managing Accountand Loans.

## Why I Built This

This project began as an object-oriented programming exercise focused on encapsulations and class design.

I expanded the original concept into a networked application to explore how OOP logic can interact with a TCP server-client relationship.

The Project is primarily a leanring exercise focused on:

- Object-oriented design and encapsulation
- TCP socket programming
- I/O multiplexing with poll()
- Multi-client server architecture
- Application protocol design
- Client session and state management
- Separation of networking and business logic

## Architecture

The project consists of two programs:

### Server

- TCP connections
- Multiple connected clients
- STL container use
- Request handling
- Customers, Bank accounts, Loans, and Banking Operations

### Client

The client acts as a "dumb" terminal interface for communicating with the server.

Most application logic remains on the server.

## Features

Current functionality includes:

- Multiple simultaneous clients
- Customer sessions
- Account creation
- Account modification and deletion
- Deposits
- Withdrawals
- Loan applications
- Loan repayments
- Customer, account, and loan information
- Bank liquidity management

## Building

The entire project runs in muliple terminals:

Run `make` to build the `server` and `client` program object files.

Once the two programs are compiled run `server` and as many `client` files as you want:
- `./server <port>`
- EXAMPLE: `./server 6666`
- `./client <port>`
- EXAMPLE: `./client 6666`

TIP: Open a new terminal for each client you want to run. All will interface with the same server, as long as they share the same port number.
