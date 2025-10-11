credit to nubiza for the get device id function 

# Autholas C++ Authentication System for Termux (Android)

A C++ implementation for Autholas authentication service with Android hardware ID verification, specifically designed to work in Termux environment.

## Features

- User authentication via Autholas API
- Android hardware ID generation using system properties
- Termux-optimized compilation
- Built-in JSON parsing with jsoncpp
- Comprehensive error handling for mobile environment
- Session management with token support

## Prerequisites

This guide assumes you have Termux installed on your Android device. If not, install it from:
- [F-Droid](https://f-droid.org/packages/com.termux/) (Recommended)
- [Google Play Store](https://play.google.com/store/apps/details?id=com.termux)

## Installation Guide

### Step 1: Update Termux Packages

```bash
# Update package repositories
pkg update && pkg upgrade -y
```

### Step 2: Install Required Dependencies

```bash
# Install essential build tools
pkg install -y clang make cmake git

# Install required libraries
pkg install -y libcurl-dev jsoncpp-dev

# Install additional utilities (optional but recommended)
pkg install -y nano vim wget
```

### Step 3: Verify Installation

```bash
# Check if compiler is working
clang++ --version

# Check if curl is available
curl --version

# Verify jsoncpp installation
pkg list-installed | grep jsoncpp
```

## Project Setup

### Method 1: Clone from Repository

```bash
# Navigate to home directory
cd ~

# Clone your repository (replace with your actual repo URL)
git clone https://github.com/yourusername/autholas-android.git
cd autholas-android
```

### Method 2: Manual Setup

```bash
# Create project directory
mkdir ~/autholas-auth
cd ~/autholas-auth

# Create source files (you'll need to copy the content manually)
touch Autholas.h Example.cpp android_device_id.h obfuscate.h
```

## Building the Project

### Compilation Command

```bash
# Basic compilation
clang++ -std=c++17 Example.cpp -lcurl -ljsoncpp -o autholas_auth

# With debug information
clang++ -std=c++17 -g -O0 Example.cpp -lcurl -ljsoncpp -o autholas_auth

# Optimized release build
clang++ -std=c++17 -O2 Example.cpp -lcurl -ljsoncpp -o autholas_auth
```

### Using Makefile (Recommended)

Create a `Makefile`:

```makefile
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
LIBS = -lcurl -ljsoncpp
TARGET = autholas_auth
SOURCE = Example.cpp

# Debug build
debug: CXXFLAGS += -g -O0 -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O2 -DNDEBUG
release: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $< $(LIBS) -o $@

clean:
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(PREFIX)/bin/

.PHONY: debug release clean install
```

Then build with:
```bash
# Debug build
make debug

# Release build
make release
```

## Configuration

### 1. Set Your API Key

Edit `Autholas.h` and replace the API key:

```cpp
const char* ApiKey = OBF("YOUR_ACTUAL_API_KEY_HERE");
```

### 2. Configure obfuscate.h

Make sure you have the obfuscation header. If not, you can create a simple version:

```cpp
// obfuscate.h - Simple version
#pragma once
#define AY_OBFUSCATE(str) str
```

### 3. Android Permissions (if needed)

Some Android versions might require additional permissions. If you encounter issues:

```bash
# Check available system properties
getprop | grep "ro.product"

# Test device ID generation
./autholas_auth --test-hwid  # if you implement this flag
```

## Usage

### Running the Application

**Important**: After compilation, the executable file needs execute permission:

```bash
# Grant execute permission to the file
chmod +x autholas_auth

# Run the application
./autholas_auth
```

**Explanation:**
- `chmod +x` grants execute permission to the file
- `./` runs the file from current directory
- Without `chmod +x`, you'll get "Permission denied" error

### Example Session

```
========================
Autholas Login System
Android C++ Example Code
========================
Username: your_username
Password: your_password

Attempting login...

✓ Authentication successful!
Session token: abcd1234567890123456...
Session expires: 2024-12-31T23:59:59Z

=== APPLICATION STARTED ===
You can now run your main application logic here.
User 'your_username' is authenticated and ready to use the application.

Press Enter to exit...
```

## Troubleshooting

### Common Issues and Solutions

#### 1. Compilation Errors

```bash
# If jsoncpp headers not found
pkg install jsoncpp-dev

# If libcurl not found
pkg install libcurl-dev

# Check library locations
pkg-config --cflags --libs jsoncpp libcurl
```

#### 2. Runtime Errors

```bash
# If program crashes on startup
export TMPDIR=$PREFIX/tmp
export LD_LIBRARY_PATH=$PREFIX/lib

# Check system properties access
ls -la /system/build.prop
```

#### 3. Network Issues

```bash
# Test network connectivity
curl -I https://autholas.nicholasdevs.my.id

# Check DNS resolution
nslookup autholas.nicholasdevs.my.id
```

#### 4. Hardware ID Issues

```bash
# Check system property access
getprop ro.product.device
getprop ro.product.name
getprop ro.product.brand
```

### Android-Specific Considerations

1. **Storage Access**: Termux apps run in a sandboxed environment
2. **Network Permissions**: Usually granted by default in Termux
3. **System Properties**: Some properties might be restricted on newer Android versions
4. **SSL Certificates**: Ensure system CA certificates are accessible

## File Structure

```
autholas-android/
├── Autholas.h              # Main authentication class header
├── Example.cpp             # Main application entry point
├── android_device_id.h     # Android hardware ID generation
├── obfuscate.h            # String obfuscation utilities
├── Makefile               # Build configuration
├── README.md              # This documentation
└── .gitignore            # Git ignore file
```

## Advanced Configuration

### Custom Build Script

Create `build.sh`:

```bash
#!/bin/bash

echo "Building Autholas Authentication System..."

# Clean previous builds
make clean

# Build release version
make release

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    echo "Granting execute permission..."
    chmod +x autholas_auth
    echo "✓ Ready to run!"
    echo "Run with: ./autholas_auth"
else
    echo "✗ Build failed!"
    exit 1
fi
```

Make script executable and run:
```bash
# Grant permission to build script
chmod +x build.sh

# Run build script
./build.sh
```

### Environment Setup Script

Create `setup_env.sh`:

```bash
#!/bin/bash

# Set up environment variables for optimal performance
export TMPDIR=$PREFIX/tmp
export LD_LIBRARY_PATH=$PREFIX/lib
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig

# Create necessary directories
mkdir -p $TMPDIR

echo "Environment configured for Autholas authentication system"
echo "To use: source ./setup_env.sh"
```

Usage:
```bash
# Grant execute permission
chmod +x setup_env.sh

# Load environment (use source, not ./)
source ./setup_env.sh
```

## Error Handling

The system handles various authentication errors:

- `INVALID_API_KEY` - Incorrect API key configuration
- `INVALID_CREDENTIALS` - Wrong username/password
- `USER_BANNED` - Account suspended
- `SUBSCRIPTION_EXPIRED` - Subscription ended
- `HWID_BANNED` - Device hardware ID banned
- `MAX_DEVICES_REACHED` - Device limit exceeded
- `RATE_LIMIT_EXCEEDED` - Too many authentication attempts
- `DEVELOPER_SUSPENDED` - API developer account suspended

## Security Features

- Android hardware ID generation using system properties
- Secure HTTPS communication with Autholas API
- String obfuscation for sensitive data
- Session token management
- No sensitive data stored in plain text

## Performance Optimization

### Compilation Optimizations

```bash
# Optimize for size (good for mobile)
clang++ -std=c++17 -Os Example.cpp -lcurl -ljsoncpp -o autholas_auth

# Link-time optimization
clang++ -std=c++17 -O2 -flto Example.cpp -lcurl -ljsoncpp -o autholas_auth

# Strip debugging symbols (smaller binary)
strip autholas_auth
```

### Runtime Optimizations

```bash
# Set CPU affinity (if needed)
taskset -c 0-3 ./autholas_auth

# Monitor resource usage
top -p $(pgrep autholas_auth)
```

## Dependencies

- **clang++**: C++ compiler (comes with Termux clang package)
- **libcurl**: HTTP client library for API communication
- **jsoncpp**: JSON parsing library for API responses
- **Android system properties**: For hardware ID generation

## Compatibility

- **Android**: 5.0+ (API level 21+)
- **Termux**: Latest version recommended
- **Architecture**: ARM64, ARM, x86_64 (depending on device)

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Test on multiple Android devices if possible
4. Commit your changes (`git commit -m 'Add amazing feature'`)
5. Push to the branch (`git push origin feature/amazing-feature`)
6. Open a Pull Request

## Testing

### Unit Testing

```bash
# Create simple test
cat > test_hwid.cpp << 'EOF'
#include "android_device_id.h"
#include <iostream>

int main() {
    std::string hwid = get_device_id();
    std::cout << "Hardware ID: " << hwid << std::endl;
    std::cout << "Length: " << hwid.length() << std::endl;
    return hwid.empty() ? 1 : 0;
}
EOF

# Compile test
clang++ -std=c++17 test_hwid.cpp -o test_hwid

# Grant permission and run
chmod +x test_hwid
./test_hwid
```

### API Testing

```bash
# Test API connectivity
curl -X POST https://autholas.nicholasdevs.my.id/api/auth \
  -H "Content-Type: application/json" \
  -d '{"api_key":"test","username":"test","password":"test","hwid":"test"}'
```

## License

This project is provided as-is for educational and development purposes. Please respect the terms of service of the Autholas API.

## Support

For issues related to:
- **Autholas API**: Contact Autholas support
- **Termux-specific issues**: Check [Termux Wiki](https://wiki.termux.com/)
- **Code issues**: Create an issue in this repository

## Changelog

### v1.0.0
- Initial release for Termux
- Android hardware ID support
- Full authentication flow
- Session management

---

**Note**: This application is designed to work within Termux's sandboxed environment. Some system properties might be limited on newer Android versions due to security restrictions.

