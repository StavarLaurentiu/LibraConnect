#include "utils.h"

string compute_get_request(string host, string url, string accept,
                           string auth_header, vector<string> cookies);

string compute_post_request(string host, string url, string content_type, json body_data,
                            string accept, string auth_header, vector<string> cookies);

string compute_delete_request(string host, string url, string accept,
                              string auth_header, vector<string> cookies);