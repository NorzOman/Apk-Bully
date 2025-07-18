


/**
 * @brief ApkBully Secret Scanner Tool
 * 
 * @section COMMAND_FORMAT Command Usage
 * ./main --src file.apk --out output
 * 
 * @section PARAMETERS Parameters
 * - --src : Source APK file path
 * - --out : Output directory for decompilation
 * 
 * @section DOCKER_REQUIREMENTS Docker Dependencies
 * - Java Runtime Environment
 * - JADX Decompiler
 * 
 * @section INSTALLATION Installation Steps
 * 1. Install Java
 * 2. Install JADX
 * 
 * @section LIMITATIONS Script Limitations
 * - Assumes successful decompilation
 * - Continues scanning without comprehensive error checking
 * 
 * @warning Requires manual verification of decompilation process
 * 
 * @author : omannorz@gmail.com
 */


//!--- importing all the required libraries ---
#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <cstdlib>
#include <unordered_set>
#include <sstream>
#include <regex>
#include <vector>
#include <iomanip>



//!--- constants declaration ---
std::string jadx_path = "/usr/local/bin/jadx";
std::vector<std::string> files_to_scan;
std::vector<std::pair<std::regex, std::string>> patterns;


//! --- function declarattions ---
void setup(std::string apk_path);
std::string run(std::string, bool show = false);
void scan(std::vector<std::string> files_to_scan, std::string output_file);
void loader();
std::string escape_json(const std::string& input);  

//< --- Main Entry Point function ---
int main(int argc , char * argv[]){

	std::string apk_path;
	std::string output_file;

	if(argc == 5 && std::string(argv[1]) == "--src" && std::string(argv[3]) == "--out"){
		apk_path = std::string(argv[2]);
		output_file = std::string(argv[4]);
		if(!output_file.ends_with(".json")) output_file  += ".json";
		if(!apk_path.ends_with(".apk"))	return 0;
	}
	else{
		std::cout << "[ error ] Weird number of paramters were passed instead of standard" << std::endl;
		std::cout << "[ debug ] Follow command pattern : ./main --src path_to_apkfile.apk --out output_file.json " << std::endl;
		return 0;
	}


	std::cout << "[ debug ] Got the apk file path as : " << apk_path << std::endl;
	std::cout << "[ debug ] Got the output file json path as : " << output_file << std::endl;

	
    // Quick check if JADX is available
    std::string jadx_check = run("which jadx", false);
    if (jadx_check.empty()) {
        std::cout << " [ error ]  JADX not found in PATH" << std::endl;
        return 0;
    }

    std::cout << "[ debug ] JADX found at: " << jadx_check << std::endl;

    // Now firstly decompile the apk
    std::cout << "\n\n[ debug ] Now setting up in .." ;
    loader();
    setup(apk_path);


    std::cout << "\n\n[ debug ] Now loading files in ..";
    loader();
	// Now command to filter out the noise and get the actual list of files to scan
	std::string files = run("find sandbox/sources/ sandbox/resources/ -type f "
	    "\\( -name \"*.java\" -o -name \"*.xml\" -o -name \"*.smali\" \\) "
	    "! -path \"sandbox/sources/android/*\" "
	    "! -path \"sandbox/sources/androidx/*\" "
	    "! -path \"sandbox/sources/com/google/*\" "
	    "! -path \"sandbox/sources/com/facebook/*\" "
	    "! -path \"sandbox/sources/org/apache/*\" "
	    "! -path \"sandbox/sources/junit/*\" "
	    "! -path \"sandbox/sources/javax/*\" "
	    "! -path \"sandbox/sources/kotlin/*\" "
	    "! -path \"sandbox/sources/com/squareup/*\" "
	    "! -path \"sandbox/sources/com/bumptech/*\" "
	    "! -path \"sandbox/sources/com/fasterxml/*\" "
	    "| sort | uniq");


	std::stringstream ss(files);
	std::string file;

	while (std::getline(ss, file)) {
		std::cout << "		[ debug ] loaded file " << file << std::endl;
	    files_to_scan.push_back(file);
	}


	// Load the patterns into the map

	std::cout << "\n\n[ debug ] Now loading regex for the scanner in .."; 
	loader();

	std::ifstream infile("apkbully-patterns.txt");
	std::string line;
	int line_count = 0;

	while(getline(infile,line)){
		++line_count;
		int pos = line.find("::");

		if(pos == std::string::npos){
			continue;
		}

		std::string regex_str = line.substr(0,pos);
		std::string label = line.substr(pos+2);

		try{
			patterns.push_back(std::make_pair(std::regex(regex_str) , label));
			std::cout <<  "		[ debug ] Loaded regex for: " << label << std::endl;
		}
		catch (const std::regex_error& e){
        	std::cout << "		[ error ] Invalid regex: " << regex_str << " (" << e.what() << ")" << std::endl;
        }
	}



	std::cout << "\n\n[ debug ] Now initiating scan in ..";
	loader();
	if(!files_to_scan.empty()){
		scan(files_to_scan , output_file);
	}
	else{
		std::cout << "The list of files to scan seems to be empty , means a soft error occured in the process . exiting" << std::endl;
		return 0;
	}

	std::cout << "[ debug ] scannig completed" << std::endl;

}


std::string run(std::string command , bool show){

	std::array<char , 128> buffer;
	std::string output;

	FILE* pipe = popen(command.c_str(),"r");

	if(!pipe) return "Failed to run command :: " + command;

	while(fgets(buffer.data() , buffer.size() , pipe)){
		if(show){
			std::cout << buffer.data();
		}
		output += buffer.data();
	}

	pclose(pipe);

    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    return output;
}



void setup(std::string apk_path){
		// preimlinary check to see if the apk path actuall exists 
	if(!std::filesystem::exists(apk_path)){
		std::cout << "[ error ] the apk path provided doesnt exists , if you think it does make sure you enter full path to the file" << std::endl;
		exit(1);
	}

	// Create a folder called as sandbox
	std::cout << "[ debug ] removing any preivously created sandboxes \n";
	run("rm -rf sandbox/");

	std::cout << "[ debug ] creating a new sandbox \n";	
	run("mkdir sandbox");

	std::string decompile_command = jadx_path + " -d sandbox " + apk_path;
	std::cout << "[ debug ] decompiling the apk using the jadx using the command : " << decompile_command  << "\n";

	std::string output = run(decompile_command);
    std::cout << "[ debug ] Decompilation output: " << output << "\n" << std::endl;
    std::cout << "[ debug ] assuming decompilation was successful if u see error in above output then ctrl+c" << std::endl;
}


void scan(std::vector<std::string> files_to_scan, std::string output_file) {
    std::ofstream outfile(output_file);
    if (!outfile.is_open()) {
        std::cerr << "[ error ] Failed to open file for writing: " << output_file << std::endl;
        return;
    }

    int total_detection = 0;
    int total_lines = 0;
    int total_files = 0;
    bool first = true;

    outfile << "[\n";

    for (const std::string& filepath : files_to_scan) {
        ++total_files;
        std::cout << "\n[ debug +] Now scanning " << filepath << std::endl;
        std::ifstream infile(filepath);

        if (infile) {
            std::string line;
            int line_count = 0;

            while (getline(infile, line)) {
                ++line_count;
                ++total_lines;

                for (const auto& [re, label] : patterns) {
                    std::smatch match;
                    if (std::regex_search(line, match, re)) {
                        ++total_detection;

                        std::string escaped_line = escape_json(line);

                        std::string json_obj = "  {\n"
                            "    \"file\": \"" + filepath + "\",\n"
                            "    \"line\": " + std::to_string(line_count) + ",\n"
                            "    \"label\": \"" + label + "\",\n"
                            "    \"match\": \"" + escaped_line + "\"\n"
                            "  }";

                        if (!first) outfile << ",\n";
                        outfile << json_obj;
                        first = false;

                        std::cout << "[ debug ] " << json_obj << "\n";
                    }
                }
            }
        } else {
            std::cerr << "[ error ] Failed to read file: " << filepath << std::endl;
        }

        std::cout << "[ debug -] Scan completed " << filepath << "\n";
    }

    std::string summary = "  {\n"
        "    \"summary\": {\n"
        "      \"total_detections\": " + std::to_string(total_detection) + ",\n"
        "      \"total_files\": " + std::to_string(total_files) + ",\n"
        "      \"total_lines\": " + std::to_string(total_lines) + "\n"
        "    }\n"
        "  }\n";

    if (!first) outfile << ",\n";
    outfile << summary;
    outfile << "]\n";

    outfile.close();
    std::cout << "[ debug ] scanning completed\n";
}

std::string escape_json(const std::string& input) {
    std::ostringstream ss;
    for (char c : input) {
        switch (c) {
            case '"': ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default:
                if (c >= 0 && c < 0x20)
                    ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                else
                    ss << c;
        }
    }
    return ss.str();
}


void loader(){
    for(int i = 3 ; i > 0 ; i--){
        std::cout << i << ".." << std::flush;
        sleep(1);
    }
    std::cout << "\n";
}
