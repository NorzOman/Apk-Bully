# Apk-Bully

**Apk-Bully** is a security analysis tool for Android apps. It scans `.apk` files, decompiles them, and hunts for hardcoded secrets, API keys, tokens, and other sensitive data leaks in the app's source code.

---

## Features

- **Automated APK decompilation** using [jadx](https://github.com/skylot/jadx)
- **Pattern-based search** for secrets, tokens, and sensitive information
- **Fast C++20 core** for efficient scanning
- **Dockerized workflow** for easy setup and reproducibility

---

## 🚀 Usage Overview

You can use Apk-Bully in **two main ways**:

- **A. Dockerized (Recommended):**  
  - Use the provided helper script for easiest usage  
  - Or run the Docker container manually

- **B. Native (No Docker):**  
  - Build and run the scanner directly on your system

---

## 🐳 A. Dockerized Workflow (Recommended)

### 1. Prerequisites

- [Docker](https://docs.docker.com/get-docker/) installed

### 2. Build the Docker Image

```bash
git clone https://github.com/NorzOman/Apk-Bully.git
cd Apk-Bully
docker build -t apkbully .
```

### 3. Run the Scanner

You have **two options** for running the scanner in Docker:

#### Option 1: Using the Helper Script

This is the easiest way. The script handles all Docker mounting and arguments.

```bash
chmod +x apkbully-run.sh
./apkbully-run.sh --input /absolute/path/to/your.apk --out results.json
```

- Your APK is mounted into the container.
- Results are saved as `output/results.json` on your host.

#### Option 2: Using Raw Docker Command

If you want more control:

```bash
mkdir -p output
docker run --rm \
  -v /absolute/path/to/your.apk:/app/input.apk \
  -v $(pwd)/output:/app/output \
  apkbully --src /app/input.apk --out /app/output/results.json
```

**Notes:**
- Use **absolute paths** for volume mounts.
- The `output/` directory **must exist** before running.

---

## 🖥️ B. Native Build & Run (No Docker)

### 1. Prerequisites

- Linux
- `g++` with C++20 support
- Java Runtime (for JADX)

### 2. Build the Scanner

```bash
cd src
g++ -std=c++20 -O2 -o apkbully-exec apkbully-exec.cpp
```

### 3. Install JADX

```bash
wget https://github.com/skylot/jadx/releases/download/v1.5.2/jadx-1.5.2.zip
unzip jadx-1.5.2.zip -d jadx
sudo ln -sf $(pwd)/jadx/jadx-1.5.2/bin/jadx /usr/local/bin/jadx
```

### 4. Run the Scanner

```bash
mkdir -p output
./src/apkbully-exec --src /absolute/path/to/your.apk --out output/results.json
```

- The output will be saved as `output/results.json`.

---

## 📦 Output

- Results are saved as a JSON file in the `output/` directory.
- Review the output for any detected secrets or sensitive data.

---