#ifndef __LIB_MISC_SYSTEMUTILS_H__
#define __LIB_MISC_SYSTEMUTILS_H__

#include <string>
#include <system_error>
#include <fstream>
#include <vector>


namespace libapp {


std::vector<std::string> find_files_directory(const std::string& pathDir, const std::string& extension);

void compress_files(const std::vector<std::string>& files);
void compress_files(const std::string& pathDir, const std::string& extension);
bool rename_files(const std::vector<std::string>& files, const std::string& extension, const char* formatDate);
bool rename_and_compress_files(const std::vector<std::string>& files, const std::string& extension, const char* formatDate);



void check_path_directory(const std::string& pathDir, std::error_code& ec);
bool  openStreamFile(const std::string& strFile, std::ofstream& outfs, const bool append = true );

}


#endif