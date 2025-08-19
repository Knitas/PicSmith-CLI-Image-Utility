#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cctype>

std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

std::string help() {
        return R"(
    PicSmith - Image Conversion & Compression Utility
    -------------------------------------------------

    Usage:
      PicSmith -i INPUT [OPTIONS]

    Arguments:
      -i INPUT
        |__ Path to the input image

    Options:
      -f FORMAT
        |__ Output format (jpg, png, webp, bmp, tiff, exr, jp2)

      -c [VALUE]
        |-- Compression quality
        |   |-- JPG/WEBP: 1-100  (default: JPG=85, WEBP=80)
        |   |__ PNG: 1-9  (1 = max compression, 9 = min compression, default=6)
        |__ Only supported for jpg, jpeg, png, webp

      -o OUTPUT
        |__ Output directory

      -r RESOLUTION
        |-- Rescale image to a predefined resolution:
        |   |-- SD, SD_640x480, SD_800x600
        |   |-- HD, HD_widescreen, HD+
        |   |-- FHD
        |   |-- 2K, QHD
        |   |-- 4K
        |   |__ 8K
        |__ Example: -r HD

      -h, --help
        |__ Show this help message
    )";
}

int main(int argc, char* argv[]) {

    std::string format; 
    std::string input_image_path;
    std::string output;
    int compression_value;
    int target_width;
    int target_height;

    std::map<std::string, cv::Size> predefined_resolutions = {
        {"SD", {640, 480}},
        {"SD_640x480", {640, 480}},
        {"SD_800x600", {800, 600}},

        {"HD", {1280, 720}},
        {"HD_widescreen", {1366, 768}},
        {"HD+", {1600, 900}},

        {"FHD", {1920, 1080}},

        {"2K", {2560, 1440}},
        {"QHD", {2560, 1440}},

        {"4K", {3840, 2160}},

        {"8K", {7680, 4320}}
    };

    bool arg_c = false;
    bool arg_f = false;
    bool arg_o = false;
    bool arg_r = false;

    if(argc == 1) {
        std::cout << help() << '\n';
        return 0;
    }

    for(int i = 1; i <argc; i++) {

        std::string arg = argv[i];

        if(arg == "-h" || arg == "--help") {
            std::cout << help() << '\n';
            return 0;
        }

        else if(arg == "-i") {
            if(i + 1 < argc) {
                input_image_path = argv[++i];
                if(!std::filesystem::exists(input_image_path)) {
                    std::cerr << "[!]Error: input file does not exist\n";
                    return 1;
                }
                if (!std::filesystem::is_regular_file(input_image_path)) {
                    std::cerr << "[!]Error: input path is not a file\n";
                    return 1;
                }
            }
            else {
                std::cerr << "[!]Error: missing argument for -i\n";
                return 1;
            }
        }

        else if(arg == "-f") {
            arg_f = true;
            if(i + 1 < argc) {
                format = argv[++i];
                format = toLower(format);
                if(format != "jpg" && format != "jpeg" && format != "png" && format != "webp" && format != "bmp" && format != "tiff" && format != "tif" && format != "ppm" && format != "pbm" && format != "pgm" && format != "ras" && format != "sr" && format != "hdr" && format != "exr" && format != "jp2") {
                    std::cerr << "[!]Error: unsupported format: " << format  << '\n';
                    return 1;
                }
                std::filesystem::path input_path(input_image_path);
                std::string input_ext = input_path.extension().string();
                if(!input_ext.empty() && input_ext[0] == '.') {
                    input_ext.erase(0, 1);
                }
                input_ext = toLower(input_ext);
                if(format == input_ext) {
                    std::cerr << "[!] Error: Output format cannot be the same as input format\n";                    
                    return 1;
                }
            }
            else {
                std::cerr << "[!]Error: missing argument for -f\n";
                return 1;
            } 
        }

        else if(arg == "-c") {
            arg_c = true;
            if(!arg_f) {
                std::filesystem::path input_path(input_image_path);
                format = input_path.extension().string();
                if(!format.empty() && format[0] == '.') {
                    format.erase(0, 1);
                }
                format = toLower(format);
            }

            if(format == "webp") {
                if(i + 1 < argc) {
                    std::string value = argv[++i];
                    for(char c : value) {
                        if(!isdigit(c)) {
                            std::cerr << "[!]Error: compression value must a number\n";
                            return 1;
                        }
                    }
                    compression_value = std::stoi(value);   
                    if(compression_value < 1 || compression_value > 100) {
                        std::cerr << "[!]Error: compression value must be between 1-100\n";
                        return 1;
                    }                                     
                }
                else {
                    compression_value = 80;
                }
            }
            else if(format == "png") {
                if(i + 1 < argc) {
                    std::string value = argv[++i];
                    for(char c : value) {
                        if(!isdigit(c)) {
                            std::cerr << "[!]Error: compression value must a number\n";
                            return 1;
                        }
                    }
                    compression_value = std::stoi(value);
                    compression_value = 10 - compression_value;
                    if(compression_value < 1 || compression_value > 9) {
                        std::cerr << "[!]Error: compression value must be between 1-9\n";
                        return 1;
                    }   
                                    
                }
                else {
                    compression_value = 6;
                }
            }
            else if(format == "jpeg" or format == "jpg") {
                if(i + 1 < argc) {
                    std::string value = argv[++i];
                    for(char c : value) {
                        if(!isdigit(c)) {
                            std::cerr << "[!]Error: compression value must a number\n";
                            return 1;
                        }
                    }
                    compression_value = std::stoi(value);
                    if(compression_value < 1 || compression_value > 100) {
                        std::cerr << "[!]Error: compression value must be between 1-100\n";
                        return 1;
                    }
                }
                else {
                    compression_value = 85;
                }
            }
            else {
                std::cerr << "[!]Error: compression is not supported for this format\n";
                return 1;
            }
        }

        else if(arg == "-o") {
            arg_o = true;
            if(i + 1 < argc) {
                output = argv[++i];
                if (!std::filesystem::exists(output)) {
                    std::cerr << "[!]Error: Output directory does not exist\n";
                    return 1;
                }
                if (!std::filesystem::is_directory(output)) {
                   std::cerr << "[!]Error: Output path is not a directory\n";
                    return 1;
                }
            }
            else {
                std::cerr << "[!]Error: output path must be provided\n";
                return 1;
            }
        }

        else if(arg == "-r") {
            arg_r = true;
            if(!arg_f) {
                std::filesystem::path input_path(input_image_path);
                format = input_path.extension().string();
                if(!format.empty() && format[0] == '.') {
                       format.erase(0, 1);
                }
               format = toLower(format);
            }
           if(i + 1 < argc) {
             std::string res_key = argv[++i];
                res_key = toLower(res_key);
                bool found = false;
                for(const auto& kv : predefined_resolutions) {
                    std::string key_lower = toLower(kv.first);
                    if(key_lower == res_key) {
                    target_width = kv.second.width;
                    target_height = kv.second.height;
                    found = true;
                    break;
                    }
                }
                if(!found) {
                    std::cerr << "[!]Error: invalid resolution key '" << res_key << "'\n";
                    std::cerr << "Valid options: SD, SD_640x480, SD_800x600, HD, HD_widescreen, HD+, FHD, 2K, QHD, 4K, 8K\n";
                    return 1;
                }
            } 
            else {
                std::cerr << "[!]Error: missing argument for -r\n";
                return 1;
            }
        }

        else {
            std::cerr << "[!]Error: unknown argument " << arg << "\n";
            return 1;
        }

    }

    if (input_image_path.empty()) {
        std::cerr << "[!]Error: No input file specified (use -i <file>)\n";
        return 1;
    }

    if (format.empty() && !arg_c && !arg_r) {
        std::cerr << "[!] Error: No operation specified\n"
                  << "You must choose at least one of these options:\n"
                  << "  - Change format: -f <format> (jpg, png, webp...)\n"
                  << "  - Apply compression: -c [value]\n"
                  << "  - Resize image: -r WxH\n";
        return 1;
    }


    cv::Mat image = cv::imread(input_image_path, cv::IMREAD_UNCHANGED);
    if(image.empty()) {
        std::cerr << "[!]Error: failed to load image\n";
        return 1;        
    }

    std::filesystem::path inpath(input_image_path);
    std::string outpath;
    std::filesystem::path output_image_path(output);
    if(arg_o == false) {
       outpath = (inpath.parent_path() / (inpath.stem().string() + "." + format)).string(); 
    }
    else {
        outpath = (output_image_path / (inpath.stem().string() + "." + format)).string();
    }
    if(std::filesystem::exists(outpath)) {
        std::cout << "[!]Warning: file already exists. do you want to overwrite it?(Y/n) : ";
        std::string confirm_overwrite;
        std::cin >> confirm_overwrite;
        if(confirm_overwrite == "N" || confirm_overwrite == "n") {
            return 1;
        }
        else if(confirm_overwrite != "Y" && confirm_overwrite != "y") {
            std::cerr << "[!]Error: input is invalid!\n";
            return 1;
        }
    }

    std::vector<int> compression_params;    
    if(arg_c == true) {
        if(format == "jpeg" || format == "jpg") {
            compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
            compression_params.push_back(compression_value);
        }
        else if(format == "webp") {
            compression_params.push_back(cv::IMWRITE_WEBP_QUALITY);
            compression_params.push_back(compression_value);
        }
        else if(format == "png") {
            compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
            compression_params.push_back(compression_value);
        }
    }

    if(arg_c == false) {
        if(format == "jpeg" || format == "jpg") {
            compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
            compression_params.push_back(100);
        }
        else if(format == "webp") {
            compression_params.push_back(cv::IMWRITE_WEBP_QUALITY);
            compression_params.push_back(100);
        }
        else if(format == "png") {
            compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
            compression_params.push_back(0);
        }
    }

    cv::Mat resized_image;
    if(arg_r == true) {
        cv::Size target_resolution(target_width, target_height);
        cv::resize(image, resized_image, target_resolution, 0, 0, cv::INTER_AREA);
    }

    cv::Mat image_to_save = arg_r ? resized_image : image;
    bool success;
    if (!compression_params.empty()) {
        success = cv::imwrite(outpath, image_to_save, compression_params);
    } 
    else {
        success = cv::imwrite(outpath, image_to_save);
    }

    if(arg_f == true && arg_c == false) {
        if(arg_r == false) {
            std::cout << "[+]Converted: " << input_image_path << " -> " << outpath << "\n"; 
        }
        else {
            std::cout << "[+]Converted, Rescaled: " << input_image_path << " -> " << outpath << "\n"; 
        }
    }
    else if(arg_f == false && arg_c == true) {
        if(arg_r == false) {
            std::cout << "[+]Compressed: " << input_image_path << " -> " << outpath << "\n";
        } 
        else {
            std::cout << "[+]Compressed, Rescaled: " << input_image_path << " -> " << outpath << "\n";
        }
    }
    else if (arg_r == true && arg_f == false && arg_c == false) {
        std::cout << "[+]Rescaled: " << input_image_path << " -> " << outpath << "\n";
    }
    else if(arg_f == true && arg_r == true && arg_c == true) {
        std::cout << "[+]Converted, Compressed, Rescaled: " << input_image_path << " -> " << outpath << "\n"; 
    }
    else {
        std::cout << "[+]Converted, Compressed: " << input_image_path << " -> " << outpath << "\n"; 
    }
    
    return 0;
}