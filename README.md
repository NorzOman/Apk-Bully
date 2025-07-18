# Apk-Bully

**Apk-Bully** is a security analysis tool for Android apps. It scans `.apk` files, decompiles them, and hunts for hardcoded secrets, API keys, tokens, and other sensitive data leaks in the app's source code.

---

## Features

- **Automated APK decompilation** using [jadx](https://github.com/skylot/jadx)
- **Pattern-based search** for secrets, tokens, and sensitive information
- **Fast C++20 core** for efficient scanning
- **Dockerized workflow** for easy setup and reproducibility

---

##  Quick Start

You can use Apk-Bully in **two ways**:  
**1. Full Dockerized Workflow (Recommended)**  
**2. Native Build & Run (No Docker)**

---

## ( Option - 1 ). Full Dockerized Workflow (Recommended)

### Prerequisites

- [Docker](https://docs.docker.com/get-docker/) installed

### Build the Docker Image

```bash
git clone https://github.com/NorzOman/Apk-Bully.git
cd Apk-Bully
docker build -t apkbully .
```

### 1. Run with the Helper Script

This is the easiest way. The script handles all Docker mounting and arguments.

```bash
chmod +x apkbully-run.sh
./apkbully-run.sh --input /absolute/path/to/your.apk --out results.json
```

- Your APK is mounted into the container.
- Results are saved as `output/results.json` on your host.

### 2. Run Docker Manually

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

## ( Option - 2 ) Native Build & Run (No Docker)

### Prerequisites

- Linux
- `g++` with C++20 support
- Java Runtime (for JADX)

### Build the Scanner

```bash
g++ -std=c++20 -O2 -o apkbully-exec apkbully-exec.cpp
```

### Install JADX

```bash
wget https://github.com/skylot/jadx/releases/download/v1.5.2/jadx-1.5.2.zip
unzip jadx-1.5.2.zip -d jadx
sudo ln -sf $(pwd)/jadx/jadx-1.5.2/bin/jadx /usr/local/bin/jadx
```

### Run the Scanner

```bash
mkdir -p output
./apkbully-exec --src /absolute/path/to/your.apk --out output/results.json
```

- The output will be saved as `output/results.json`.

---

## 📦 Output

- Results are saved as a JSON file in the `output/` directory.
- Review the output for any detected secrets or sensitive data.

---

## 🤝 Contributing

Pull requests and issues are welcome!

---

## 📄 License

No driver's license needed for the code

---
