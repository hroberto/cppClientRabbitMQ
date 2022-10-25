#ifndef __LIB_MISC_JSONUTILS_H__
#define __LIB_MISC_JSONUTILS_H__

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>

#include "Tools/Logger.h"


namespace libapp {



void printJSon(const rapidjson::Value::Array& a, const std::string spaceIterator = "");
void printJSon(const rapidjson::Value::Object& config, const std::string spaceIterator = "");

inline size_t loadConfigFromJson(rapidjson::Document* ptrConfig, const std::string& FILE_NAME) {
    FILE* fp = fopen(FILE_NAME.c_str(), "rb");
    if (!fp)
        return 0;

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    size_t length =  strlen(readBuffer);

    if (ptrConfig->ParseStream(is).HasParseError()) {
        const std::string str = rapidjson::GetParseError_En(ptrConfig->GetParseError() );
        _log->error("Error Parser Json ( offset {} ): \"{}\" ", ptrConfig->GetErrorOffset(), str);
        length = 0;
    }
    fclose(fp);

    return length;
}

template<typename Type, typename V>
constexpr inline bool HAS_ERROR_CONTENT_BOOL(Type& d, const V& s1)  { 
    return d.HasMember(s1) && !d[s1].IsNull() && !d[s1].IsBool(); 
}

template<typename Type, typename V>
constexpr inline bool HAS_ERROR_CONTENT_STRING(Type& d, const V& s1)  { 
    return d.HasMember(s1) && !d[s1].IsNull() && !d[s1].IsString(); 
}

template<typename Type, typename V>
constexpr inline bool HAS_ERROR_CONTENT_ARRAY(Type& d, const V& s1)  { 
    return d.HasMember(s1) && !d[s1].IsNull() && !d[s1].IsArray(); 
}



template<typename Type>
constexpr inline bool HAS_CONFIG(Type& c, const char* s1)   { return (c.HasMember(s1)); }

template<typename Type>
constexpr inline bool HAS_CONFIG(Type& c, const char* s1, const char* s2)   { return (c.HasMember(s1) && c[s1].HasMember(s2)); }

template<typename Type>
constexpr inline bool HAS_CONFIG(Type& c, const char* s1, const char* s2, const char* s3)   { return (c.HasMember(s1) && c[s1].HasMember(s2)  && c[s1][s2].HasMember(s3)); }



template<typename Type>
constexpr auto GET_CONFIG_BOOL(Type& c, const char* s1, const bool& d)   { return (c.HasMember(s1) && c[s1].IsBool() ? c[s1].GetBool() : d); }

template<typename Type>
constexpr auto GET_CONFIG_BOOL(Type& c, const char* s1, const char* s2, const bool& d)   { return (HAS_CONFIG(c, s1, s2) && c[s1][s2].IsBool() ? c[s1][s2].GetBool() : d); }

template<typename Type>
constexpr auto GET_CONFIG_BOOL(Type& c, const char* s1, const char* s2, const char* s3, const bool& d)   { return (HAS_CONFIG(c, s1, s2, s3) && c[s1][s2][s3].IsBool() ? c[s1][s2][s3].GetBool() : d); }



template<typename Type>
constexpr auto GET_CONFIG_STRING(Type& c, const char* s1, const char* d)   { return (c.HasMember(s1) && c[s1].IsString() ? c[s1].GetString() : d); }

template<typename Type>
constexpr auto GET_CONFIG_STRING(Type& c, const char* s1, const char* s2, const char* d)   { return (HAS_CONFIG(c, s1, s2) && c[s1][s2].IsString() ? c[s1][s2].GetString() : d); }

template<typename Type>
constexpr auto GET_CONFIG_STRING(Type& c, const char* s1, const char* s2, const char* s3, const char* d)   { return (HAS_CONFIG(c, s1, s2, s3) && c[s1][s2][s3].IsString() ? c[s1][s2][s3].GetString() : d); }



template<typename Type>
constexpr auto GET_CONFIG_STRING(Type& c, const char* s1, const std::string& d)   { return (c.HasMember(s1) && c[s1].IsString() ? c[s1].GetString() : d); }

template<typename Type>
constexpr auto GET_CONFIG_STRING(Type& c, const char* s1, const char* s2, const std::string& d)   { return (HAS_CONFIG(c, s1, s2) && c[s1][s2].IsString() ? c[s1][s2].GetString() : d); }

template<typename Type>
constexpr auto GET_CONFIG_STRING(Type& c, const char* s1, const char* s2, const char* s3, const std::string& d)   { return (HAS_CONFIG(c, s1, s2, s3) && c[s1][s2][s3].IsString() ? c[s1][s2][s3].GetString() : d); }



template<typename Type>
constexpr auto GET_CONFIG_INT(Type& c, const char* s1, const int& d)   { return (HAS_CONFIG(c, s1) && c[s1].IsInt() ? c[s1].GetInt() : d); }

template<typename Type>
constexpr auto GET_CONFIG_INT(Type& c, const char* s1, const char* s2, const int& d)   { return (HAS_CONFIG(c, s1, s2) && c[s1][s2].IsInt() ? c[s1][s2].GetInt() : d); }

template<typename Type>
constexpr auto GET_CONFIG_INT(Type& c, const char* s1, const char* s2, const char* s3, const int& d)   { return (HAS_CONFIG(c, s1, s2, s3) && c[s1][s2][s3].IsInt() ? c[s1][s2][s3].GetInt() : d); }



template<typename Type>
constexpr auto GET_CONFIG_DOUBLE(Type& c, const char* s1, const double& d)   { return (HAS_CONFIG(c, s1) && c[s1].IsNumber() ? c[s1].GetDouble() : d); }

template<typename Type>
constexpr auto GET_CONFIG_DOUBLE(Type& c, const char* s1, const char* s2, const double& d)   { return (HAS_CONFIG(c, s1, s2) && c[s1][s2].IsNumber() ? c[s1][s2].GetDouble() : d); }

template<typename Type>
constexpr auto GET_CONFIG_DOUBLE(Type& c, const char* s1, const char* s2, const char* s3, const double& d)   { return (HAS_CONFIG(c, s1, s2, s3) && c[s1][s2][s3].IsNumber() ? c[s1][s2][s3].GetDouble() : d); }



template<typename Type>
constexpr auto GET_CONFIG_FLOAT(Type& c, const char* s1, const float& d)   { return (HAS_CONFIG(c, s1) && c[s1].IsNumber() ? c[s1].GetFloat() : d); }

template<typename Type>
constexpr auto GET_CONFIG_FLOAT(Type& c, const char* s1, const char* s2, const float& d)   { return (HAS_CONFIG(c, s1, s2) && c[s1][s2].IsNumber() ? c[s1][s2].GetFloat() : d); }

template<typename Type>
constexpr auto GET_CONFIG_FLOAT(Type& c, const char* s1, const char* s2, const char* s3, const float& d)   { return (HAS_CONFIG(c, s1, s2, s3) && c[s1][s2][s3].IsNumber() ? c[s1][s2][s3].GetFloat() : d); }



template<typename Type>
constexpr auto GET_CONFIG_ARRAY(Type& c, const char* s1)   { assert(HAS_CONFIG(c, s1)); assert(c[s1].IsArray()); return c[s1].GetArray(); }


}

#endif //__LIB_MISC_LOGGER_H__