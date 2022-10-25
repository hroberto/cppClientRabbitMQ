
#ifndef __LIB_MISC_UTILS_H__
#define __LIB_MISC_UTILS_H__

#include <chrono>
#include <iomanip>
#include <any>
#include <unordered_map>
#include <vector>
#include <map>
#include <iomanip>
#include <cmath>


namespace libapp {


std::string check_format_time(std::string value);
double make_time_gmt_mili(const std::string& value);
std::time_t make_time_gmt(const std::string& value);
std::time_t make_time_local(const std::string& value, const std::string format = "%Y-%m-%dT%H:%M:%S");

std::string to_string(const std::chrono::system_clock::time_point & now, const char* format, const bool gmt = false );
std::string to_string(const time_t & dt, const char* format = "%Y-%m-%dT%H:%M:%S", const bool gmt = false );
std::string to_string(const std::chrono::high_resolution_clock::time_point& now);
// std::string to_string(const double& num, const int& precision);


template < class T >
#if (__cplusplus > 201703L)
    requires std::is_floating_point_v<T>
#endif
std::string to_string(const T& num, const int& precision) {
    std::stringstream ss("");

    if ( precision <= 0 ) {
        const T num_abs = std::abs( num );
        const int ndigits = ( num_abs >= 1 ? std::log10( num_abs ) : 1 );
        
        const int LIMIT =  std::numeric_limits<T>::digits10 - ndigits - 1;
        ss << std::fixed << std::setprecision( LIMIT ) << num;
    }
    else {
        ss << std::fixed << std::setprecision(precision) << num;
    }
    auto length = ss.str().size() -1;

    if ( precision == 0 ) {
        while ( ss.str().at( length  ) == '0' ) --length;
        if ( ss.str().at( length  ) == '.' ) ++length;
    }

    return ss.str().substr( 0, length + 1 );
}


std::vector<std::string> splitString(const std::string input, const char& delimiter);
std::vector<std::string> split(const std::string input, const char& delimiter);
std::vector<std::string> split(const std::string input, const char& delimiter, const std::size_t count);

std::string to_string_json(const std::vector<std::pair<std::string, std::any> >&);
std::string to_string_json(const std::map<std::string, std::string> &);
std::string to_string_json(const std::vector<std::any>&);
void to_string_json(std::ostream& ss, const std::vector<std::pair<std::string, std::any> >&);


std::string urlEncode(const std::string& str);

template<class T>
void encode_header(std::vector<uint8_t>& msg, const T& number, const int tam = 0) noexcept
{
    const size_t SIZE_NUMBER = ( tam == 0 ? sizeof( number ) : tam );
    constexpr uint SKIP_OFFSET { 8 };
    uint offset { 0 };

    msg.resize( SIZE_NUMBER );
    std::size_t value = number;

    for( int pos = SIZE_NUMBER - 1; pos >= 0 ; --pos ) {
        msg[ pos ] = (uint8_t) ((value >> offset) & 0xFF);
        offset += SKIP_OFFSET;
    }
}

/**
 * @brief 
 * 
 *      const std::vector<uint8_t> result_ ar;
 *      std::uint32_t number_;
 *      decode_header<std::uint32_t>( number_, result_ );
 * 
 * @tparam T 
 * @tparam V 
 * @param number 
 * @param msg 
 */
template<typename T, typename V>
void decode_header(T& number, const V& msg, const size_t tam = 0)  noexcept
{
    const size_t SIZE_NUMBER = ( tam == 0 ? sizeof( number ) : tam );
    constexpr uint8_t SKIP_OFFSET { 8 };

    number = 0;    

    for (unsigned n = 0; n < SIZE_NUMBER; ++n) {
        (number <<= SKIP_OFFSET ) |= static_cast<uint8_t>( msg[ n ] );
    }
}

uint64_t atoui64(std::string const& value);


}

#endif // __LIB_MISC_UTILS_H__
