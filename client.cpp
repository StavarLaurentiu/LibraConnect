#include "utils.h"
#include "requests.h"

// Function that checks if a string is a number
bool isStrictNumber(const std::string& str) {
    if (str.empty()) return false; // Check if the string is empty

    // Check for leading zero in cases where the string length is more than 1
    if (str[0] == '0' && str.length() > 1) return false;

    // Check each character to ensure it's a digit
    for (char ch : str) {
        if (!isdigit(ch)) return false;
    }

    return true;
}

// Read the username and password from the client
int get_credentials(Client &client)
{
    // Read the username and password
    cin.ignore();
    cout << "username=";
    getline(cin, client.username);
    cout << "password=";
    getline(cin, client.password);

    // Verify if the username and password have spaces
    if (client.username.find(" ") != NOT_FIND || client.password.find(" ") != NOT_FIND || 
            client.username.empty() || client.password.empty()) {
        cout << "ERROR - Username and password must not contain spaces or be empty\n";

        // Reset the client credentials
        client.username = "";
        client.password = "";

        return -1;
    }

    return 0;
}

// Handle the register command
void handle_register(Client &client)
{
    // Read the username and password
    int ret = get_credentials(client);
    if (ret == -1) return;

    // Create the JSON body
    json body_data;
    body_data["username"] = client.username;
    body_data["password"] = client.password;

    // Create the message
    string message = compute_post_request(IP_PORT, "/api/v1/tema/auth/register", "application/json", body_data, "application/json", "", client.cookies);

    // Open the connection
    client.socket = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    // Send the message to the server
    send_to_server(client.socket, message);

    // Receive the response from the server
    string response = receive_from_server(client.socket);

    // Parse the response body
    int response_body_index = response.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    string response_body = response.substr(response_body_index);

    // Check if the response is successful
    if (response_body == "ok") {
        cout << "SUCCES - Successfully registered\n";
    } else {
        json response_json = json::parse(response_body);
        cout << "ERROR - " << response_json["error"] << "\n";
    }

    // Close the connection
    close_connection(client.socket);
}

// Handle the login command
void handle_login(Client &client)
{
    // Read the username and password
    int ret = get_credentials(client);
    if (ret == -1) return;

    // Create the JSON body
    json body_data;
    body_data["username"] = client.username;
    body_data["password"] = client.password;

    // Create the message
    string message = compute_post_request(IP_PORT, "/api/v1/tema/auth/login", "application/json", body_data, "application/json", "", client.cookies);
    
    // Open the connection
    client.socket = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    // Send the message to the server
    send_to_server(client.socket, message);

    // Receive the response from the server
    string response = receive_from_server(client.socket);

    // Parse the response body
    int response_body_index = response.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    string response_body = response.substr(response_body_index);

    // Check if the response is successful
    if (response_body == "ok") {
        // Mark the client as logged in
        client.logged_in = true;

        // Get the cookie
        int cookie_index = response.find("Set-Cookie: ") + 12;
        string cookie = response.substr(cookie_index, response.find(";", cookie_index) - cookie_index);

        // Add the cookie to the client
        client.cookies.push_back(cookie);

        cout << "SUCCES - Successfully logged in\n";
    } else {
        json response_json = json::parse(response_body);
        cout << "ERROR - " << response_json["error"] << "\n";
    }

    // Close the connection
    close_connection(client.socket);
}

// Handle the enter_library command
void handle_enter_library(Client &client) {
    // Check if the client is logged in
    if (!client.logged_in) {
        cout << "ERROR - You must be logged in to enter the library\n";
        return;
    }

    // Create the message
    string message = compute_get_request(IP_PORT, "/api/v1/tema/library/access", "application/json", "", client.cookies);

    // Open the connection
    client.socket = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    // Send the message to the server
    send_to_server(client.socket, message);

    // Receive the response from the server
    string response = receive_from_server(client.socket);

    // Parse the response body
    int response_body_index = response.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    string response_body = response.substr(response_body_index);

    // Get the response code
    int response_code = stoi(response.substr(9, 3));

    // Check if the response is successful
    if (response_code == 200) {
        // Get the JWT token
        client.JWT_token = response_body.substr(10, response_body.size() - 12);

        cout << "SUCCES - Successfully entered the library\n";
    } else {
        json response_json = json::parse(response_body);
        cout << "ERROR - " << response_json["error"] << "\n";
    }

    // Close the connection
    close_connection(client.socket);
}

// Handle the get_books command
void handle_get_books(Client &client)
{
    // Check if the client is logged in
    if (!client.logged_in) {
        cout << "ERROR - You must be logged in to get the books\n";
        return;
    }

    // Check if the client has entered the library
    if (client.JWT_token.empty()) {
        cout << "ERROR - You must enter the library to get the books\n";
        return;
    }

    // Create the message
    string message = compute_get_request(IP_PORT, "/api/v1/tema/library/books", "application/json", client.JWT_token, client.cookies);

    // Open the connection
    client.socket = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    // Send the message to the server
    send_to_server(client.socket, message);

    // Receive the response from the server
    string response = receive_from_server(client.socket);

    // Parse the response body
    int response_body_index = response.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    string response_body = response.substr(response_body_index);

    // Get the response code
    int response_code = stoi(response.substr(9, 3));

    // Check if the response is successful
    if (response_code == 200) {
        // Print the books in JSON format
        cout << response_body << "\n";
    } else {
        json response_json = json::parse(response_body);
        cout << "ERROR - " << response_json["error"] << "\n";
    }

    // Close the connection
    close_connection(client.socket);
}

// Handle the get_book command
void handle_get_book(Client &client)
{
    // Check if the client is logged in
    if (!client.logged_in) {
        cout << "ERROR - You must be logged in to get a book\n";
        return;
    }

    // Check if the client has entered the library
    if (client.JWT_token.empty()) {
        cout << "ERROR - You must enter the library to get a book\n";
        return;
    }

    // Read the book id
    int book_id;
    cout << "id=";
    cin >> book_id;

    // Create the message
    string message = compute_get_request(IP_PORT, "/api/v1/tema/library/books/" + to_string(book_id), "application/json", client.JWT_token, client.cookies);

    // Open the connection
    client.socket = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    // Send the message to the server
    send_to_server(client.socket, message);

    // Receive the response from the server
    string response = receive_from_server(client.socket);

    // Parse the response body
    int response_body_index = response.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    string response_body = response.substr(response_body_index);

    // Get the response code
    int response_code = stoi(response.substr(9, 3));

    // Check if the response is successful
    if (response_code == 200) {
        // Print the book in JSON format
        cout << response_body << "\n";
    } else {
        json response_json = json::parse(response_body);
        cout << "ERROR - " << response_json["error"] << "\n";
    }

    // Close the connection
    close_connection(client.socket);
}

// Handle the add_book command
void handle_add_book(Client &client)
{
    // Check if the client is logged in
    if (!client.logged_in) {
        cout << "ERROR - You must be logged in to add a book\n";
        return;
    }

    // Check if the client has entered the library
    if (client.JWT_token.empty()) {
        cout << "ERROR - You must enter the library to add a book\n";
        return;
    }

    // Read the book details
    string title, author, genre, publisher, page_count;
    cout << "title=";
    cin.ignore();
    getline(cin, title);
    cout << "author=";
    getline(cin, author);
    cout << "genre=";
    getline(cin, genre);
    cout << "publisher=";
    getline(cin, publisher);
    cout << "page_count=";
    getline(cin, page_count);

    // Verify if the page count is a number
    if (!isStrictNumber(page_count)) {
        cout << "ERROR - Page count must be a number\n";
        return;
    }

    // Create the JSON body
    json body_data;
    body_data["title"] = title;
    body_data["author"] = author;
    body_data["genre"] = genre;
    body_data["publisher"] = publisher;
    body_data["page_count"] = stoi(page_count);

    // Create the message
    string message = compute_post_request(IP_PORT, "/api/v1/tema/library/books", "application/json", body_data, "application/json", client.JWT_token, client.cookies);

    // Open the connection
    client.socket = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    // Send the message to the server
    send_to_server(client.socket, message);

    // Receive the response from the server
    string response = receive_from_server(client.socket);

    // Parse the response body
    int response_body_index = response.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    string response_body = response.substr(response_body_index);

    // Get the response code
    int response_code = stoi(response.substr(9, 3));

    // Check if the response is successful
    if (response_code == 200) {
        cout << "SUCCES - Successfully added the book\n";
    } else {
        json response_json = json::parse(response_body);
        cout << "ERROR - " << response_json["error"] << "\n";
    }

    // Close the connection
    close_connection(client.socket);
}

// Handle the logout command
void handle_logout(Client &client)
{
    // Check if the client is logged in
    if (!client.logged_in) {
        cout << "ERROR - You must be logged in to logout\n";
        return;
    }

    // Create the message
    string message = compute_get_request(IP_PORT, "/api/v1/tema/auth/logout", "application/json", client.JWT_token, client.cookies);

    // Open the connection
    client.socket = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    // Send the message to the server
    send_to_server(client.socket, message);

    // Receive the response from the server
    string response = receive_from_server(client.socket);

    // Parse the response body
    int response_body_index = response.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    string response_body = response.substr(response_body_index);

    // Get the response code
    int response_code = stoi(response.substr(9, 3));

    // Check if the response is successful
    if (response_code == 200) {
        // Reset the client details
        client.logged_in = false;
        client.JWT_token = "";
        client.cookies.clear();

        cout << "SUCCES - Successfully logged out\n";
    } else {
        json response_json = json::parse(response_body);
        cout << "ERROR - " << response_json["error"] << "\n";
    }

    // Close the connection
    close_connection(client.socket);
}

// Handle the delete_book command
void handle_delete_book(Client &client) {
    // Check if the client is logged in
    if (!client.logged_in) {
        cout << "ERROR - You must be logged in to delete a book\n";
        return;
    }

    // Check if the client has entered the library
    if (client.JWT_token.empty()) {
        cout << "ERROR - You must enter the library to delete a book\n";
        return;
    }

    // Read the book id
    int book_id;
    cout << "id=";
    cin >> book_id;

    // Create the message
    string message = compute_delete_request(IP_PORT, "/api/v1/tema/library/books/" + to_string(book_id), "application/json", client.JWT_token, client.cookies);

    // Open the connection
    client.socket = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    // Send the message to the server
    send_to_server(client.socket, message);

    // Receive the response from the server
    string response = receive_from_server(client.socket);

    // Parse the response body
    int response_body_index = response.find(HEADER_TERMINATOR) + HEADER_TERMINATOR_SIZE;
    string response_body = response.substr(response_body_index);

    // Get the response code
    int response_code = stoi(response.substr(9, 3));

    // Check if the response is successful
    if (response_code == 200) {
        cout << "SUCCES - Successfully deleted the book\n";
    } else {
        json response_json = json::parse(response_body);
        cout << "ERROR - " << response_json["error"] << "\n";
    }

    // Close the connection
    close_connection(client.socket);
}

// Handle any command that the client sends to the server
void handle_command(Client &client, string command)
{
    if (command == "register") {
        handle_register(client);
    } else if (command == "login") {
        handle_login(client);
    } else if (command == "enter_library") {
        handle_enter_library(client);
    } else if (command == "get_books") {
        handle_get_books(client);
    } else if (command == "get_book") {
        handle_get_book(client);
    } else if (command == "add_book") {
        handle_add_book(client);
    } else if (command == "delete_book") {
        handle_delete_book(client);
    } else if (command == "logout") {
        handle_logout(client);
    } else {
        cout << "Invalid command\n";
    }
}

int main(int argc, char *argv[])
{
    // Create a client
    Client client;

    // Set the server details
    client.server.sin_family = AF_INET;
    client.server.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_IP, &client.server.sin_addr);

    // Main loop
    string command;
    while(1) {
        // Read the command
        cin >> command;

        // Check if the command is "exit"
        if (command == "exit") break;

        // Handle the command
        handle_command(client, command);
    }

    return 0;
}