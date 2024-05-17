#include "utils.h"
#include "requests.h"

void handle_register(Client &client)
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

        return;
    }

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
    cout << response << "\n";

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

// Handle any command that the client sends to the server
void handle_command(Client &client, string command)
{
    if (command == "register") {
        handle_register(client);
    } else if (command == "login") {
        cout << "Login command\n";
    } else if (command == "enter_library") {
        cout << "Enter library command\n";
    } else if (command == "get_books") {
        cout << "Get books command\n";
    } else if (command == "get_book") {
        cout << "Get book command\n";
    } else if (command == "add_book") {
        cout << "Add book command\n";
    } else if (command == "delete_book") {
        cout << "Delete book command\n";
    } else if (command == "logout") {
        cout << "Logout command\n";
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