FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

# Install Java + essential tools
RUN apt-get update && apt-get install -y \
    default-jre-headless \
    bash \
    coreutils \
    findutils \
    grep \
    sed \
    wget \
    unzip \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY apkbully-exec .
COPY apkbully-patterns.txt .

# Download and install jadx 1.5.2
RUN wget -q https://github.com/skylot/jadx/releases/download/v1.5.2/jadx-1.5.2.zip \
    && mkdir /opt/jadx-1.5.2 \
    && unzip jadx-1.5.2.zip -d /opt/jadx-1.5.2 \
    && rm jadx-1.5.2.zip \
    && ln -s /opt/jadx-1.5.2/bin/jadx /usr/local/bin/jadx \
    && chmod +x /opt/jadx-1.5.2/bin/jadx

# Make main script executable (if it's a shell or Python script)
RUN chmod +x ./apkbully-exec

# Run the main script when the container starts
ENTRYPOINT ["./apkbully-exec"]
