#include "Tools/StringUtils.h"

#include <chrono>
#include <iomanip>
#include <typeindex>
#include <assert.h>
#include <iostream>
#include <filesystem>
#include <functional>
#include <cstring>
#include <time.h>
#include <stdio.h>
#include <regex>


namespace libapp {


/**
 * @brief Cria time_t com base no TIME ZONE GMT (Greenwich Mean Time).
 * 
 * @param value 
 * @return std::time_t 
 */
double make_time_gmt_mili(const std::string& value)
{
    std::tm     t {};

    std::istringstream ss( value );
    ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");

    std::time_t basetime = std::mktime( &t );
    std::time_t diff;

    t = *std::localtime( &basetime );
    t.tm_isdst = -1;
    diff = std::mktime( &t );

    t = *std::gmtime( &basetime );
    t.tm_isdst = -1;
    diff -= std::mktime( &t );

    double finaltime = basetime + diff;

    auto pos = value.find( "." );
    if( pos != std::string::npos ) {

        double tdec{};
        for( auto rit=value.rbegin() ; rit != value.rend() && value.rbegin()+ (value.size() - pos) > rit ; ++rit )
        {
            
            if ( *rit >= '0' && *rit <= '9' ) {
                tdec += *rit - '0';
                tdec /= 10;
            }
        }
        finaltime += tdec;
    }

    return finaltime;
}




/**
 * @brief Cria time_t com base no TIME ZONE GMT (Greenwich Mean Time).
 * 
 * @param value 
 * @return std::time_t 
 */
std::time_t make_time_gmt(const std::string& value)
{
    std::tm     t {};

    std::istringstream ss( value );
    ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");

    std::time_t basetime = std::mktime( &t );
    std::time_t diff;

    t = *std::localtime( &basetime );
    t.tm_isdst = -1;
    diff = std::mktime( &t );

    t = *std::gmtime( &basetime );
    t.tm_isdst = -1;
    diff -= std::mktime( &t );

    std::time_t finaltime = basetime + diff;

    return finaltime;
}

/**
 * @brief Valida se alguma posicao da Data/Hora possue apenas um unico digito.
 * 
 * @param value 
 * @return std::string 
 */
std::string check_format_time(std::string value) 
{
    const std::vector<std::pair<std::regex, std::string>> FORMATTERS = {
        {std::regex("^(\\d)([^\\d])"), "@$1$2"},
        {std::regex("([^\\d])(\\d)(?=[^\\d])"), "$1@$2$3"},
        {std::regex("([^\\d])(\\d)$"), "$1@$2"},
    };

    // Realiza a formatacao dos digitos sem zeros a esquerda para nao gerar erro de conversao
    for (const auto& fmt : FORMATTERS) {
        value = std::regex_replace(value, fmt.first, fmt.second);
        std::replace(value.begin(), value.end(), '@', '0');
    }

    return value;
}

/**
 * @brief Cria time_t com base no TIME ZONE local.
 * 
 * @param value
 * @return std::time_t 
 */
std::time_t make_time_local(const std::string& value, const std::string format)
{

    std::tm t = {};
    std::istringstream ss( value );
    ss >> std::get_time(&t, format.c_str());
    
    return  mktime(&t);
}


/**
 * @brief Realiza a conversao de <high_resolution_clock>   para string..
 *        Ex.:   to_string( std::chrono::high_resolution_clock::now() )
 * 
 * @param now 
 * @return std::string 
 */
std::string to_string(const std::chrono::high_resolution_clock::time_point & now )
{
    int msec = ( std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() ) % 1000000;

    time_t now_time_t = std::chrono::high_resolution_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&now_time_t), "%Y-%m-%dT%X.") 
        << std::right << std::setfill('0') << std::setw(6) << msec << "Z";

    return ss.str();
}

/**
 * @brief 
 *        Ex.:   to_string( std::chrono::system_clock::now(), "%Y-%m-%d" )
 * 
 * @param now 
 * @param format 
 * @return std::string 
 */
std::string to_string(const std::chrono::system_clock::time_point & now, const char* format, const bool gmt )
{
    time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    if (gmt)
        ss << std::put_time(std::gmtime(&now_time), format );
    else
        ss << std::put_time(std::localtime(&now_time), format );

    return ss.str();
}

/**
 * @brief 
 * 
 * @param dt 
 * @param format 
 * @param gmt 
 * @return std::string 
 */
std::string to_string(const time_t & dt, const char* format, const bool gmt )
{
    std::stringstream ss;
    if (gmt)
        ss << std::put_time(std::gmtime(&dt), format );
    else
        ss << std::put_time(std::localtime(&dt), format );

    return ss.str();
}


/**
 * @brief 
 * 
 * @param input 
 * @param delimiter 
 * @return std::vector<std::string> 
 */
std::vector<std::string> splitString(const std::string input, const char& delimiter)
{
    std::vector<std::string> elements;
    std::stringstream stream(input);
    std::string element;

    while (getline(stream, element, delimiter)) {
        elements.push_back(element);
    }

    return elements;
}

// std::string to_string(const double& num, const int& precision) {
//     std::stringstream ss("");
//     if ( precision <= 0 ) {
//         ss << std::fixed << std::setprecision(std::numeric_limits<double>::digits10 ) << num;
//     }
//     else {
//         ss << std::fixed << std::setprecision(precision) << num;
//     }

//     auto length = ss.str().size() -1;

//     if ( precision == 0 ) {
//         while ( ss.str().at( length  ) == '0' ) --length;
//         if ( ss.str().at( length  ) == '.' ) ++length;
//     }

//     return ss.str().substr( 0, length + 1 );
// }


/**
 * @brief 
 * 
 * @param input 
 * @param delimiter 
 * @return std::vector< std::string > 
 */
std::vector< std::string > split(const std::string input, const char& delimiter)
{
    return split(input, delimiter, 1);
}


/**
 * @brief 
 * 
 * @param input 
 * @param delimiter 
 * @param count 
 * @return std::vector< std::string > 
 */
std::vector< std::string > split(const std::string input, const char& delimiter, const std::size_t  count)
{
    assert( count > 0 );

    std::vector< std::string > elements;
    elements.reserve( count );

    char *itV1 { 0 };
    char* it { const_cast<char *>( input.data()) };

    while (*it)
    {
        if (*it == delimiter ) 
        {
            if( !itV1 )  elements.emplace_back( "" );
            else elements.emplace_back( std::string(itV1, it-itV1) );
            
            itV1 = 0;
        }

        else if (! itV1) itV1=it;

        ++it;
    }

    if( !itV1 )  elements.emplace_back( "" );
    else elements.emplace_back( std::string(itV1, it-itV1) );

    return elements;
}


typedef std::function< void(std::any const&, std::ostream&) >   function_impl;
extern const std::unordered_map<std::type_index, function_impl> any_visitor;

template<class TV, class FC >
inline std::pair<const std::type_index, function_impl >  mk_pair_any(FC const &f)
{
    return {
        std::type_index(typeid(TV)),
        [g = f](std::any const &a, std::ostream& os)
        {
            if constexpr (std::is_void_v<TV>)
                g(os);
            else
                g(std::any_cast<TV const&>(a), os);
        }
    };
}



/**
 * @brief 
 * 
 */
std::string to_string_json(const std::vector< std::pair<std::string, std::any> >& extras)
{
    std::stringstream ss("");

    ss << "{"; 
    to_string_json( ss, extras );
    ss << "}";

    return ss.str();
}



/**
 * @brief 
 * 
 */
std::string to_string_json(const std::map<std::string, std::string>& mp)
{
    std::stringstream ss("");


    int idx{0};

    std::stringstream _s;

    _s << "{";
    for (const auto& item : mp) {
        if (idx++) _s << ", ";

        _s << "\"" << item.first << "\": " << std::quoted(item.second);
    }
    _s << "}";

    return _s.str();

    return ss.str();
}

/**
 * @brief 
 * 
 */
std::string to_string_json(const std::vector<std::any>& vc) 
{
    int idx{0};

    std::stringstream _s;

    _s << "[";
    for (const auto& item : vc) {
        const auto& myType = std::type_index(item.type());
        const auto& itype = any_visitor.find( myType );

        if (itype == any_visitor.cend()) {
            std::stringstream ss("");
            ss << "<to_string_json> Tipo nao localizado para \"" << myType.name() << "\"";
            throw std::invalid_argument( ss.str() );
        }
        assert(itype != any_visitor.cend());            

        if (idx++) _s << ", ";

        itype->second(item, _s);
    }
    _s << "]";

    return _s.str();
};


/**
 * @brief 
 * 
 */
const std::unordered_map<std::type_index, function_impl> any_visitor {
    mk_pair_any<nullptr_t>([](nullptr_t, std::ostream& _s) { _s << "null"; }),
    mk_pair_any<int>([](int x, std::ostream& _s) { _s << x; }),
    mk_pair_any<uint64_t>([](uint64_t x, std::ostream& _s) { _s << x; }),
    mk_pair_any<unsigned>([](unsigned x, std::ostream& _s) { _s << x; }),
    mk_pair_any<double>([](double x, std::ostream& _s) { _s << libapp::to_string(x, 0); }),
    mk_pair_any<char const*>([](char const* s, std::ostream& _s) { _s << std::quoted(s); }),
    mk_pair_any<std::string const>([](std::string const s, std::ostream& _s) { _s << std::quoted(s); }),
    mk_pair_any<std::vector<std::any> const>([](std::vector<std::any> const& vc, std::ostream& _s) {  _s << to_string_json(vc); }),
    mk_pair_any<std::vector<std::pair<std::string, std::any> > const>([](std::vector<std::pair<std::string, std::any> > const& vc, std::ostream& _s) {  _s << to_string_json(vc); })
};

/**
 * @brief 
 * 
 * @param extras 
 * @param ss 
 */
void to_string_json(std::ostream& ss, const std::vector<std::pair<std::string, std::any> >& extras) 
{
    int i{0};

    for (const auto& it : extras) {
        const auto& typeField = std::type_index(it.second.type());

        const auto& itype = any_visitor.find( typeField );

        if (itype == any_visitor.cend()) {
            std::stringstream ss("");
            ss << "Tipo nao localizado para \"" << it.first << "\": " << it.second.type().name();
            throw std::invalid_argument( ss.str() );
        }
        assert(itype != any_visitor.cend());

        if (i++) ss << ", ";

        ss << "\"" << it.first << "\": ";
        itype->second(it.second, ss);
    }
}



uint64_t atoui64(std::string const& value) { 
    uint64_t result {};
    char* ptr = const_cast<char*>(value.data());

    while( *ptr != '\0' ) {
        result *= 10;
        result += ( *ptr - '0' );

        ++ptr;
    }
    return result;
}

std::string urlEncode(const std::string& str)
{
    static const char* dont_escape = "._-$,;~()";
    static const char* hex = "0123456789abcdef";

    std::string dst;
    size_t src_len = str.size();
    auto src = str.c_str();

    dst.clear();
    dst.reserve(src_len);

    for (; src_len > 0; src++, src_len--) {
        if (isalnum(*src) || strchr(dont_escape, *src) != NULL) {
            dst.push_back(*src);
        } else {
            dst.push_back('%');
            dst.push_back(hex[(*src) >> 4]);
            dst.push_back(hex[(*src) & 0xf]);
        }
    }

    return dst;
}


}
