#ifndef __PROGRAMOPTIONSUTILS_H__
#define __PROGRAMOPTIONSUTILS_H__


#include <boost/program_options.hpp>
#include <stdexcept>


namespace libapp {


boost::program_options::variables_map parseOptions(int argc, char const *argv[], boost::program_options::options_description& optDesc);
void conflicting_options(const boost::program_options::variables_map& vm, const char* opt1, const char* opt2);
void option_dependency(const boost::program_options::variables_map& vm, const char* for_what, const char* required_option);

}

#endif // __PROGRAMOPTIONSUTILS_H__
