#pragma once

#include <string>

#include "../spec/HttpMethod.hpp"
#include "../spec/HttpVersion.hpp"
#include "../spec/HttpHeaders.hpp"

using namespace http::spec;


namespace http::request
{
    class HttpRequest {
        public:
            std::string method;
            std::string version;
            std::string resource;
            HttpHeaders headers;
            std::string body;

            HttpRequest(HttpMethod method, HttpVersion version, std::string resource);

            std::string get();
    };
}
