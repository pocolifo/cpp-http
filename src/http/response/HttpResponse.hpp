#pragma once

#include <string>

#include "../spec/HttpMethod.hpp"
#include "../spec/HttpVersion.hpp"
#include "../spec/HttpHeaders.hpp"

using namespace http::spec;


namespace http::response
{
    class HttpResponse {
        public:
            short statusCode;
            std::string version;
            std::string statusText;
            HttpHeaders headers;
            char body[];

            HttpResponse(HttpVersion version, short statusCode, std::string statusText);

            std::string get();
    };

    HttpResponse recv(void(*next_bytes_func)(void* buf, size_t buffer_length));
}
