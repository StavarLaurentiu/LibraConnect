// Description: This file contains the implementation of the functions that
// are used to compute the GET, POST and DELETE request.
#include "requests.h"

// Function that computes a GET request
string compute_get_request(string host, string url, string accept,
                           string auth_header, vector<string> cookies)
{
    // Declare the message
    string message = "";

    // Add the line containing the keyword, url and version
    message += "GET ";
    message += url;
    message += " HTTP/1.1";
    message += HEADER_END_LINE;

    // Add the line containing host ip
    message += "Host: ";
    message += host;
    message += HEADER_END_LINE;

    // Add the line for authorization if it is the case
    if (auth_header.size() != 0)
    {
        message += "Authorization: Bearer";
        message += auth_header;
        message += HEADER_END_LINE;
    }

    // Add Accept field
    message += "Accept: ";
    message += accept;
    message += HEADER_END_LINE;

    // Add the User-Agent field
    message += "User-Agent: CLI-Client";
    message += HEADER_END_LINE;

    // Add cookies if it's the case
    if (cookies.size() != 0)
    {
        message += "Cookie: ";

        for (string cookie : cookies)
        {
            message += cookie;
            message += "; ";
        }

        // Erase the "; " for the last cookie
        message.erase(message.size() - 2, 2);

        // Add the end line
        message += HEADER_END_LINE;
    }

    // Add an empty row to finish the message
    message += HEADER_END_LINE;
    return message;
}

// Function that computes a POST request
string compute_post_request(string host, string url, string content_type, json body_data,
                            string accept, string auth_header, vector<string> cookies)
{
    // Declare the message
    string message = "";

    // Add the line containing the keyword, params and version
    message += "POST ";
    message += url;
    message += " HTTP/1.1";
    message += HEADER_END_LINE;

    // Add the line containing host ip
    message += "Host: ";
    message += host;
    message += HEADER_END_LINE;

    // Add the line for content type
    message += "Content-Type: ";
    message += content_type;
    message += HEADER_END_LINE;

    // Add the line for content length
    message += "Content-Length: ";
    message += to_string(body_data.dump().size());
    message += HEADER_END_LINE;

    // Add the line for authorization if it is the case
    if (auth_header.size() != 0)
    {
        message += "Authorization: ";
        message += auth_header;
        message += HEADER_END_LINE;
    }

    // Add the line for accept if it's the case
    if (accept.size() != 0) {
        message += "Accept: ";
        message += accept;
        message += HEADER_END_LINE;
    }

    // Add the User-Agent field
    message += "User-Agent: CLI-Client";
    message += HEADER_END_LINE;

    // Add cookies if it's the case
    if (cookies.size() != 0)
    {
        message += "Cookie: ";

        for (string cookie : cookies)
        {
            message += cookie;
            message += "; ";
        }

        // Erase the "; " for the last cookie
        message.erase(message.size() - 2, 2);

        // Add the end line
        message += HEADER_END_LINE;
    }

    // Add an empty row to finish the message
    message += HEADER_END_LINE;

    // Add the body data
    message += body_data.dump();

    return message;
}