#pragma once

#include <string>

namespace http::spec
{
    typedef std::string HttpVersion;

    const HttpVersion VERSION_HTTP_10 = "HTTP/1.0";
    const HttpVersion VERSION_HTTP_11 = "HTTP/1.1";
}
