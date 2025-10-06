# C Sockets Client-Server

A simple client-server application implemented in C using sockets. This project demonstrates basic TCP socket programming, allowing the client to connect to the server and exchange messages.

## Build Instructions

### Build with CMake

```bash
mkdir build

cd build

cmake ..

make
```

## Usage

### Running the Server

```bash
./server
```

The server will listen on port **8080** by default.

### Running the Client

```bash
./client
```

The client will connect to the server's IP address and port.
