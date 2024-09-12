# Chat Application

A cross-platform chat application utilizing multithreading and socket programming for communication over a local network or localhost. This project is designed to work on both Linux/macOS and Windows, with separate implementations provided for each platform.

## Features
- **Cross-platform support**: Compatible with Linux/macOS and Windows.
- **Multithreaded server**: Handles multiple client connections simultaneously using multithreading.
- **Multithreaded client**:Handles sending as well as receiving messages from server
- **Socket programming**: Efficient communication between server and clients over TCP/IP sockets.
- **Simple text-based chat**: Clients can exchange text messages in real-time.

## Folder Structure

- `src/`
  - `linux-mac/`: Contains the source files for Linux and macOS (`client.cpp`, `server.cpp`).
  - `windows/`: Contains the source files for Windows (`client.cpp`, `server.cpp`).
- `build/`: (Optional) Directory for compiled binaries.
  - `linux-mac/`: Compiled binaries for Linux/macOS (`client`, `server`).
  - `windows/`: Compiled binaries for Windows (`client.exe`, `server.exe`).
- `docs/`: Additional documentation, diagrams, etc. (if any).
- `include/`: Header files shared across platforms (if any).
- `.gitignore`: Specifies files and directories to ignore in the repository.
- `LICENSE`: The license under which the project is distributed.
- `README.md`: This file.

## Prerequisites
- A C++ compiler (e.g., g++, clang, MSVC)
- CMake (optional, if using a CMake build system)

## Installation

### Cloning the Repository
Clone the repository to your local machine using the following command:

```bash
git clone https://github.com/your-username/chat-application.git
cd chat-application
## Building the application
-For windows
 use msvs command prompt - cl server.cpp/cl client.exe and run exe
-For mac/Linux
 to build use clang++/g++ -o server server.cpp same for client and run executable
