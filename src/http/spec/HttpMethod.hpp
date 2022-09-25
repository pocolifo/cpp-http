#pragma once

#include <string>

namespace http::spec
{
    typedef std::string HttpMethod;

    const HttpMethod METHOD_GET = "GET";
    const HttpMethod METHOD_POST = "POST";
}
