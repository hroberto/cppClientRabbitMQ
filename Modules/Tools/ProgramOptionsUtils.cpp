#include "Tools/ProgramOptionsUtils.h"


#include <boost/program_options.hpp>
#include <stdexcept>


namespace libapp {


/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @param optDesc 
 * @return const auto 
 */
boost::program_options::variables_map parseOptions(int argc, char const *argv[], boost::program_options::options_description& optDesc) {
    boost::program_options::variables_map vm;

    boost::program_options::store( 
        boost::program_options::command_line_parser (argc, argv)
        .options(optDesc).run (), vm);

    boost::program_options::notify (vm);

    return vm;
}


void conflicting_options(const boost::program_options::variables_map& vm, const char* opt1, const char* opt2) 
{
    if (vm.count(opt1) && !vm[opt1].defaulted() 
        && vm.count(opt2) && !vm[opt2].defaulted())
        throw std::logic_error(std::string("Conflicting options '") + opt1 + "' and '" + opt2 + "'.");
}


void option_dependency(const boost::program_options::variables_map& vm, const char* for_what, const char* required_option) 
{
    if (vm.count(for_what) && !vm[for_what].defaulted())
        if (vm.count(required_option) == 0 || vm[required_option].defaulted())
            throw std::logic_error(std::string("Option '") + for_what  + "' requires option '" + required_option + "'.");
}

}
