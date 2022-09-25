#pragma once

#include <vector>

#include "HttpHeader.hpp"


namespace http::spec
{
    class HttpHeaders {
        private:
            std::vector<http::spec::HttpHeader> headers;

        public:
            void set(const std::string key, const std::string value);
            bool has(const std::string key);
            void remove(const std::string key);
            HttpHeader get(const std::string key);

            std::vector<http::spec::HttpHeader>::iterator begin();
            std::vector<http::spec::HttpHeader>::iterator end();
    };
}
