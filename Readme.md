# Typing Test TUI

A simple terminal-based typing test application with a text user interface (TUI).

## Installation

Clone the repository and run the setup script:

```bash
git clone https://github.com/yourusername/typi.git
cd typi
chmod +x run.sh
./run.sh
```

> ⚠️ The script will compile the program and move the binary to `/usr/local/bin`.

## Usage

Once installed, you can run the typing test from anywhere by typing:

```bash
typi
```

## Requirements

Make sure you have the following installed:

- `cmake`
- `g++` or `clang++`
- `make`

### 🐧 Ubuntu / Debian

```bash
sudo apt update
sudo apt install build-essential cmake
```

### 🐧 Arch Linux / Manjaro

```bash
sudo pacman -S base-devel cmake
```

### 🐧 Fedora

```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake
```

### 🐧 Gentoo

```bash
sudo emerge --ask sys-devel/gcc cmake
```

## Project Structure

```
typi/
├── run.sh           # Setup script
├── src/             # Source files
├── CMakeLists.txt   # Build configuration
└── README.md
```
