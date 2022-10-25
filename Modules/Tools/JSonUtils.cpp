#include "Tools/JSonUtils.h"


#include <rapidjson/document.h>
#include <spdlog/spdlog.h>
#include "Tools/Logger.h"

namespace libapp {

void printJSon(const rapidjson::Value::Array& a, const std::string spaceIterator) 
{
    size_t idx{};

    for (auto& itsub : a) {
        if (itsub.IsString()) {
            _log->debug("printJSon | {}   - [{}] ==>   value string: \"{}\"", spaceIterator, idx++, itsub.GetString());
        } else if (itsub.IsInt()) {
            _log->debug("printJSon | {}   - [{}] ==>   value int: {}", spaceIterator, idx++, itsub.GetInt());
        } else if (itsub.IsNumber()) {
            _log->debug("printJSon | {}   - [{}] ==>   value double: {}", spaceIterator, idx++, itsub.GetDouble());
        } else if (itsub.IsBool()) {
            _log->debug("printJSon | {}   - [{}] ==>   value bool: {}", spaceIterator, idx++, itsub.GetBool());
        } else if (itsub.IsArray()) {
            printJSon( itsub.GetArray(), fmt::format("{}  [{}]", spaceIterator, idx++) );
        }
    }
}

void printJSon(const rapidjson::Value::Object& config, const std::string spaceIterator) 
{
    for (auto& cfgContext : config) {
        if (cfgContext.value.IsString()) {
            _log->debug("printJSon | {} - item \"{}\", value string: \"{}\"", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetString());
        } else if (cfgContext.value.IsInt()) {
            _log->debug("printJSon | {} - item \"{}\", value int: {}", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetInt());
        } else if (cfgContext.value.IsNumber()) {
            _log->debug("printJSon | {} - item \"{}\", value double: {}", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetDouble());
        } else if (cfgContext.value.IsBool()) {
            _log->debug("printJSon | {} - item \"{}\", value bool: {}", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetBool());
        } else if (cfgContext.value.IsArray()) {
            _log->debug("printJSon | {} - array name: \"{}\" ", spaceIterator, cfgContext.name.GetString());
            printJSon( cfgContext.value.GetArray() );
        } else if (cfgContext.value.IsObject()) {
            _log->debug("printJSon | {} - object name: \"{}\" ", spaceIterator, cfgContext.name.GetString());
            printJSon(cfgContext.value.GetObject(), spaceIterator + "  ");
        } else {
            const size_t size = (cfgContext.value.IsNull() ? 0 : cfgContext.value.MemberCount());
            _log->debug("printJSon | {}  ==> \"{}\" : type \"{}\", size: {}", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetType(), size);
        }
    }
}

}


