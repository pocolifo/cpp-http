#pragma once

#include <string>


namespace http::spec
{
    typedef struct HttpHeader {
        std::string key;
        std::string value;
    } HttpHeader;
}