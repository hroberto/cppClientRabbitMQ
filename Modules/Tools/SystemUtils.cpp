#include "Tools/SystemUtils.h"

// #include <chrono>
// #include <iomanip>
// #include <typeindex>
#include <assert.h>
// #include <iostream>
#include <filesystem>
// #include <functional>
#include <cstring>
// #include <time.h>
// #include <stdio.h>
// #include <regex>
#include <vector>


namespace libapp {


std::vector<std::string> find_files_directory(const std::string& pathDir, const std::string& extension)
{
    std::vector<std::string> files_out;

    files_out.clear();
    const auto fd = std::filesystem::directory_iterator(pathDir);

    for (const auto& entry : fd ) {
        const std::string& name = entry.path().string();

        if (name.size() <= extension.size()) continue;

        if (extension == name.substr(name.size() - extension.size(), extension.size())) {
            files_out.emplace_back(name);
        }
    }
    return files_out;
}

/**
 * @brief 
 * 
 * @param files         
 * @param extension    "log"
 * @param formatDate   "%Y%m%d-%H%M%S"
 * 
 * @ref find_files_directory
 */
void compress_files(const std::vector<std::string>& files)
{
    if ( files.empty() ) {
        return;
    }

    std::stringstream ssCmd("");
    ssCmd << "xz -fz ";

    int  idx{ 0 };

    for (const auto& file_name : files) {
        if (!std::filesystem::is_regular_file(file_name)) continue;
        if (std::filesystem::is_symlink(file_name)) continue;

        ++idx;
        ssCmd << " " << file_name;
    }

    if (idx) {
        ssCmd << " &";
        system( ssCmd.str().c_str() );
    }
}

void compress_files(const std::string& pathDir, const std::string& extension)
{
    const std::vector<std::string>& files = find_files_directory(pathDir, extension);
    compress_files(files);
}


/**
 * @brief 
 * 
 * @param files         
 * @param extension    "log"
 * @param formatDate   "%Y%m%d-%H%M%S"
 * 
 * @ref rename_files
 */
bool rename_files(const std::vector<std::string>& files, const std::string& extension, const char* formatDate)
{
    if ( files.empty() ) {
        return true;
    }

    std::stringstream ss("");
    std::stringstream ssCmd("");

    bool sucess = true;

    for (const auto& fl : files) {
        if (!std::filesystem::is_regular_file(fl)) continue;
        if (std::filesystem::is_symlink(fl)) continue;

        ssCmd.str("");

        std::string name_file = fl;

        if (std::strlen(formatDate) > 0 ) {
            const auto fstime = std::filesystem::last_write_time(fl);
            const auto ftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(fstime));            

            ss.str("");
            ss << "." << std::put_time( std::localtime(&ftime), formatDate );

            name_file = fl.substr(0, fl.size() - extension.size()) + ss.str() + extension;

            sucess &= (rename(fl.c_str(), name_file.c_str()) == 0);
        }
    }

    return sucess;
}


bool rename_and_compress_files(const std::vector<std::string>& files, const std::string& extension, const char* formatDate)
{
    if ( files.empty() ) {
        return true;
    }

    std::stringstream ss("");
    std::stringstream ssCmd("");

    bool sucess = true;
    std::vector<std::string> files_renamed;

    for (const auto& fl : files) {
        if (!std::filesystem::is_regular_file(fl)) continue;
        if (std::filesystem::is_symlink(fl)) continue;

        ssCmd.str("");

        std::string name_file = fl;

        if (std::strlen(formatDate) > 0 ) {
            const auto fstime = std::filesystem::last_write_time(fl);
            const auto ftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(fstime));            

            ss.str("");
            ss << "." << std::put_time( std::localtime(&ftime), formatDate );

            name_file = fl.substr(0, fl.size() - extension.size()) + ss.str() + extension;

            if (rename(fl.c_str(), name_file.c_str()) == 0) {
                files_renamed.emplace_back(name_file);
            } 
            else {
                sucess = false;
            }
        }
    }
    compress_files(files_renamed);

    return sucess;
}

}

