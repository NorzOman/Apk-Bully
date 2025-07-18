FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN echo "[1/6] Updating apt and installing dependencies..." && \
    apt-get update && apt-get install -y \
    default-jre-headless \
    bash \
    coreutils \
    findutils \
    grep \
    sed \
    wget \
    unzip \
    g++ \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app

RUN echo "[2/6] Copying project files..."
COPY src/apkbully-exec.cpp .
COPY src/apkbully-patterns.txt .

RUN echo "[3/6] Building C++ executable with C++20..." && \
    g++ -std=c++20 -O2 -o apkbully-exec apkbully-exec.cpp

RUN echo "[4/6] Downloading and installing jadx 1.5.2 this may take a while..." && \
    wget https://github.com/skylot/jadx/releases/download/v1.5.2/jadx-1.5.2.zip && \
    mkdir /opt/jadx-1.5.2 && \
    unzip jadx-1.5.2.zip -d /opt/jadx-1.5.2 && \
    rm jadx-1.5.2.zip && \
    ln -s /opt/jadx-1.5.2/bin/jadx /usr/local/bin/jadx && \
    chmod +x /opt/jadx-1.5.2/bin/jadx

RUN echo "[5/6] Setting executable permissions..." && \
    chmod +x ./apkbully-exec

RUN echo "[6/6] Build complete!"

ENTRYPOINT ["./apkbully-exec"]
