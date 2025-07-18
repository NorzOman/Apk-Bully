#!/bin/bash
# filepath: /home/oman/Documents/GitHub/Apk-Bully/apkbully-run.sh

container_name="apkbully"
input_apk=""
output_name=""

# Parse CLI args
while [[ $# -gt 0 ]]; do
    case "$1" in
        --container-name)
            container_name="$2"
            shift 2
            ;;
        --input)
            input_apk="$2"
            shift 2
            ;;
        --out)
            output_name="$2"
            shift 2
            ;;
        *)
            shift
            ;;
    esac
done

# Validate required inputs
if [[ -z "$input_apk" || -z "$output_name" ]]; then
    echo "[ error ] Missing required arguments."
    echo "Usage: ./apkbully-run.sh --input <apk_path> --out <output_filename.json> [--container-name name]"
    exit 1
fi

if [[ ! -f "$input_apk" ]]; then
    echo "[ error ] APK not found at: $input_apk"
    exit 1
fi

output_dir="$(pwd)/output"
if [[ ! -d "$output_dir" ]]; then
    mkdir -p "$output_dir"
    echo "[ debug ] Created output/ directory"
fi

# Compose Docker command
command="docker run --rm -v \"$input_apk\":/app/input.apk -v \"$output_dir\":/app/output $container_name --src /app/input.apk --out /app/output/$output_name"

echo "[ debug ] Running:"
echo "$command"
echo

eval $command