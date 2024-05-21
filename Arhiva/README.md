Stavar Laurentiu-Cristian
Grupa 322CC
Homework 4 - PCOM

1. Total implementation time: ~12h

2. Tests passed: ALL

3. Implemented in c++

4. JSON library

    I've used the "nlohmann" library for JSON file manipulation. Used 
only the class "json" to store data in JSON format and "parse()" method
to parse a string and translate it into a json object.

5. Client explanation

    I have a "Client" structure that stores all the relevant data about
the client. The fields are:

    int socket; -- The socket on which the client speaks with the server
    struct sockaddr_in server; -- The addres of the server
    string username -- Username
    string password -- Password
    bool logged_in -- True if the client has logged in
    string JWT_token -- The JWT token used for authorization
    vector<string> cookies -- All the cookies from the current session

    The implementation of the client starts with setting up the Client
structure and the server address.
    After that there is a main loop that reads commands from the CLI
until the command is "exit". If the command is different from exit the
function handle_command(Client, command) is called.
    In this function we check which of the commands was typed in, and
call the specific function for every command.
    The rest of the functions implement the solution for every command
and are using the three helper functions, with sugestive names,
from requests.cpp:

    string compute_get_request(string host, string url, 
                            string accept, string auth_header, 
                            vector<string> cookies);

    string compute_post_request(string host, string url, 
                            string content_type, json body_data, 
                            string accept, string auth_header, 
                            vector<string> cookies);

    string compute_delete_request(string host, string url,
                                string accept, string auth_header,
                                vector<string> cookies);

    Before making any requests to the server I check that the input
is correct so that I make as few requests as possible. This leads to
the server running better, because it has fewer requests. If an input
is invalid the error message is printed without sending a bad request.

Thank you for your time!

╭━┳━╭━╭━╮╮
┃┈┈┈┣▅╋▅┫┃
┃┈┃┈╰━╰━━━━━━╮
╰┳╯┈┈┈┈┈┈┈┈┈◢▉◣
╲┃┈┈┈┈┈┈┈┈┈┈▉▉▉
╲┃┈┈┈┈┈┈┈┈┈┈◥▉◤
╲┃┈┈┈┈╭━┳━━━━╯
╲┣━━━━━━┫