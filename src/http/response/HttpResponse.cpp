#include "HttpResponse.hpp"

#define BUFFER_SIZE 1024

using namespace http::response;


enum Expecting {
    HTTP_VERSION,
    HTTP_STATUS_CODE,
    HTTP_STATUS_TEXT,

    HEADER_NAME,
    HEADER_SPACE,
    HEADER_VALUE,

    HEADER_BODY_SEPARATOR,
    BODY
};

HttpResponse http::response::recv(int(*next_bytes_func)(void* buf, size_t buffer_length)) {
    char inBuffer[BUFFER_SIZE];
    
    std::string buffer;
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
    Expecting expecting = Expecting::HTTP_VERSION;

    std::string headerName;

    int length;

    do {
        // read
        length = next_bytes_func(&inBuffer, BUFFER_SIZE);

        // parse
        for (int i = 0; length > i; i++) {
            char c = inBuffer[i];
            
            switch (expecting) {
                case Expecting::HTTP_VERSION:
                    if (c == ' ') {
                        response.version = buffer;
                        buffer.clear();
                        expecting = Expecting::HTTP_STATUS_CODE;
                        continue;
                    }

                    buffer.push_back(c);
                    break;
                
                case Expecting::HTTP_STATUS_CODE:
                    if (c == ' ') {
                        response.statusCode = std::stoi(buffer);
                        buffer.clear();
                        expecting = Expecting::HTTP_STATUS_TEXT;
                        continue;
                    }

                    buffer.push_back(c);
                    break;
                
                case Expecting::HTTP_STATUS_TEXT:
                    if (c == '\r') continue;
                    if (c == '\n') {
                        response.statusText = buffer;
                        buffer.clear();
                        expecting = Expecting::HEADER_NAME;
                        continue;
                    }

                    buffer.push_back(c);
                    break;
                
                case Expecting::HEADER_NAME:
                    if (c == '\r') {
                        expecting = Expecting::HEADER_BODY_SEPARATOR;
                        continue;
                    }

                    if (c == ':') {
                        headerName = buffer;
                        buffer.clear();
                        expecting = Expecting::HEADER_SPACE;
                        continue;
                    }

                    buffer.push_back(c);
                    break;

                case Expecting::HEADER_SPACE:
                    if (c == ' ') {
                        expecting = Expecting::HEADER_VALUE;
                    }

                    break;

                case Expecting::HEADER_VALUE:
                    if (c == '\r') continue;
                    if (c == '\n') {
                        response.headers.set(headerName, buffer);
                        buffer.clear();
                        expecting = Expecting::HEADER_NAME;
                        continue;
                    }

                    buffer.push_back(c);
                    break;
                
                case Expecting::HEADER_BODY_SEPARATOR:
                    if (c == '\n') {
                        expecting = Expecting::BODY;
                    }

                    break;

                case Expecting::BODY:
                    response.body.push_back(c);
                    break;
            }
        }
    } while (length > 0);

    return response;
}

std::string HttpResponse::get() {
    std::string str;

    str.append(this->version).append(" ").append(std::to_string(this->statusCode)).append(" ").append(this->statusText).append("\r\n");

    for (HttpHeader header : this->headers) {
        str.append(header.key).append(": ").append(header.value).append("\r\n");
    }

    str.append("\r\n").append(this->body);

    return str;
}