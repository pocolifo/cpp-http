#include <sstream>
#include <iostream> // temp

#include "HttpResponse.hpp"

#define BUFFER_SIZE 1024

using namespace http::response;


HttpResponse http::response::recv(int(*next_bytes_func)(void* buf, size_t buffer_length)) {
    char inBuffer[BUFFER_SIZE];
    
    std::stringstream buffer;
    HttpResponse response;

    /**
     * 0x10 = http version
     * 0x11 = http status code
     * 0x12 = http status text
     * 
     * 0x20 = header name || header-body separator
     * 0x21 = header space
     * 0x22 = header value
     * 
     * 0x30 = new line for body start
     * 0x31 = body
     */
    char expecting = 0x10;

    std::string headerName;

    int length;

    do {
        // read
        length = next_bytes_func(&inBuffer, BUFFER_SIZE);

        // parse
        for (int i = 0; length > i; i++) {
            char c = inBuffer[i];
            
            switch (expecting) {
                case 0x10:
                    if (c == ' ') {
                        response.version = buffer.str();
                        buffer.str("");
                        expecting = 0x11;
                        continue;
                    }

                    buffer << c;
                    break;
                
                case 0x11:
                    if (c == ' ') {
                        response.statusCode = std::stoi(buffer.str());
                        buffer.str("");
                        expecting = 0x12;
                        continue;
                    }

                    buffer << c;
                    break;
                
                case 0x12:
                    if (c == '\r') continue;
                    if (c == '\n') {
                        response.statusText = buffer.str();
                        buffer.str("");
                        expecting = 0x20;
                        continue;
                    }

                    buffer << c;
                    break;
                
                case 0x20:
                    if (c == '\r') {
                        expecting = 0x30;
                        continue;
                    }

                    if (c == ':') {
                        headerName = buffer.str();
                        buffer.str("");
                        expecting = 0x21;
                        continue;
                    }

                    buffer << c;
                    break;

                case 0x21:
                    if (c == ' ') {
                        expecting = 0x22;
                    }

                    break;

                case 0x22:
                    if (c == '\r') continue;
                    if (c == '\n') {
                        response.headers.set(headerName, buffer.str());
                        buffer.str("");
                        expecting = 0x20;
                        continue;
                    }

                    buffer << c;
                    break;
                
                case 0x30:
                    if (c == '\n') {
                        expecting = 0x31;
                    }

                    break;

                case 0x31:
                    response.body.push_back(c);
                    break;
            }
        }
    } while (length > 0);

    return response;
}

std::string HttpResponse::get() {
    std::stringstream buf;

    buf << this->version << " " << this->statusCode << " " << this->statusText << "\r\n";

    for (HttpHeader header : this->headers) {
        buf << header.key << ": " << header.value << "\r\n";
    }

    buf << "\r\n" << this->body;

    return buf.str();
}