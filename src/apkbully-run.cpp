#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::string container_name = "apkbully";
    std::string input_apk;
    std::string output_name;

    // Parse CLI args
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--container-name" && i + 1 < argc) {
            container_name = argv[++i];
        } else if (arg == "--input" && i + 1 < argc) {
            input_apk = argv[++i];
        } else if (arg == "--out" && i + 1 < argc) {
            output_name = argv[++i];
        }
    }

    // Validate required inputs
    if (input_apk.empty() || output_name.empty()) {
        std::cerr << "[ error ] Missing required arguments.\n";
        std::cerr << "Usage: ./apkbully-run --input <apk_path> --out <output_filename.json> [--container-name name]\n";
        return 1;
    }

    if (!std::filesystem::exists(input_apk)) {
        std::cerr << "[ error ] APK not found at: " << input_apk << "\n";
        return 1;
    }

    std::filesystem::path output_dir = std::filesystem::current_path() / "output";
    if (!std::filesystem::exists(output_dir)) {
        std::filesystem::create_directory(output_dir);
        std::cout << "[ debug ] Created output/ directory\n";
    }

    // Compose Docker command
    std::string command =
        "docker run --rm "
        "-v \"" + input_apk + "\":/app/input.apk "
        "-v \"" + output_dir.string() + "\":/app/output "
        + container_name + " "
        "--src /app/input.apk --out /app/output/" + output_name;

    std::cout << "[ debug ] Running:\n" << command << "\n\n";
    return std::system(command.c_str());
}
