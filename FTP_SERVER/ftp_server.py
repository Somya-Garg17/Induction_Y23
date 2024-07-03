import socket
import os

def handle_client(client_socket):
    client_socket.sendall(b"Welcome to the FTP server\r\n")

    users = {
        "user1": "password1",
        "user2": "password2",
    }

    banned_users = {
        "user4" : True
    }

    # Admin credentials
    admin_username = "admin"
    admin_password = "adminpassword"


    authenticated = False
    is_admin = False
    username = None  
    while not authenticated:
        request = client_socket.recv(1024).decode().strip()
        if not request:
            break

        print(f"Received request: {request}")

        if request.startswith("USER "):
            username = request.split(" ")[1]
            if username in users or username==admin_username:
                client_socket.sendall(b"Please enter your password\r\n")
            else:
                client_socket.sendall(b"Invalid username\r\n")

        elif request.startswith("PASS "):
            password = request.split(" ")[1]
            if username in users:
                if password == users[username]:
                    client_socket.sendall(b"Authentication successful. Welcome!\r\n")
                    authenticated = True
                else:
                    client_socket.sendall(b"Authentication failed. Please try again.\r\n")
            elif username == admin_username and password == admin_password:
                client_socket.sendall(b"Admin authentication successful. Welcome!\r\n")
                authenticated = True
                is_admin = True
            else:
                client_socket.sendall(b"Invalid username\r\n")

        else:
            client_socket.sendall(b"Command not recognized. Please enter a valid command.\r\n")

    if authenticated:
        while True:
            request = client_socket.recv(1024).decode().strip()
            if not request:
                break

            print(f"Received request: {request}")

            if request == "LIST":
                file_list = "\n".join(os.listdir())
                client_socket.sendall(f"List of files:\n{file_list}\r\n".encode())

            elif request.startswith("RETR "):
                filename = request.split(" ")[1]
                if os.path.isfile(filename):
                    with open(filename, "rb") as file:
                        file_data = file.read()
                    client_socket.sendall(b"\r\n")
                    client_socket.sendall(file_data)
                    client_socket.sendall(b"END_OF_FILE\r\n")
                else:
                    client_socket.sendall(b"File not found\r\n")

            elif request.startswith("STOR "):
                filename = request.split(" ")[1]
                client_socket.sendall(b"Please send the file data.\r\n")
                file_data = client_socket.recv(1024)
                with open(filename, "wb") as file:
                    file.write(file_data)
                client_socket.sendall(b"File stored successfully.\r\n")

            elif is_admin and request.startswith("ADDUSER "):

                parts = request.split(" ")
                if len(parts) != 3:
                    client_socket.sendall(b"Invalid ADDUSER command. Usage: ADDUSER <username> <password>\r\n")
                else:
                    if parts[1] in banned_users:
                        client_socket.sendall(f"User {parts[1]} is banned from this server.\r\n".encode())
                    else :
                        new_username = parts[1]
                        new_password = parts[2]
                        users[new_username] = new_password
                        client_socket.sendall(f"User {new_username} added successfully.\r\n".encode())
                        print("Updated users dictionary:", users) 
            
            elif is_admin and request.startswith("DELUSER "):
                parts = request.split(" ")
                if len(parts) != 2:
                    client_socket.sendall(b"Invalid DELUSER command. Usage: DELUSER <username>\r\n")
                else:
                    username_to_delete = parts[1]
                    if username_to_delete in users:
                        del users[username_to_delete]
                        client_socket.sendall(f"User {username_to_delete} deleted successfully.\r\n".encode())
                        print("Updated users dictionary:", users) 
                    else:
                        client_socket.sendall(f"User {username_to_delete} not found.\r\n".encode())

            elif is_admin and request.startswith("BAN "):
                parts = request.split(" ")
                if len(parts) != 2:
                    client_socket.sendall(b"Invalid BAN command. Usage: BAN <username>\r\n")
                else :
                    if parts[1] in users:
                        del users[parts[1]]
                        banned_users[parts[1]]=True
                        client_socket.sendall(f"User {parts[1]} banned succesfully.\r\n".encode())
                    else:
                        banned_users[parts[1]]=True
                        client_socket.sendall(f"User {parts[1]} banned succesfully.\r\n".encode())
                    print("Updated users dictionary:", users)
                    print("Updated banned dictionary: ",banned_users)

            elif is_admin and request.startswith("UNBAN "):
                parts = request.split(" ")
                if len(parts) != 2:
                    client_socket.sendall(b"Invalid UNBAN command. Usage: UNBAN <username>\r\n")
                else :
                    if parts[1] in banned_users:
                        del banned_users[parts[1]]
                        client_socket.sendall(f"User {parts[1]} unbanned succesfully.\r\n".encode())
                    else:
                        client_socket.sendall(f"User {parts[1]} not found.\r\n".encode())
                    print("Updated users dictionary:", users) 
                    print("Updated banned dictionary: ",banned_users)

            elif request == "QUIT":
                client_socket.sendall(b"Goodbye! Disconnecting from the server.\r\n")
                break

            else:
                client_socket.sendall(b"Command not recognized. Please enter a valid command.\r\n")

    client_socket.close()

def start_ftp_server(host, port):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((host, port))
    server.listen(5)
    print(f"FTP server is listening on {host}:{port}")

    while True:
        client_socket, client_address = server.accept()
        print(f"Connection established with {client_address}")

        handle_client(client_socket)

if __name__ == "__main__":
    start_ftp_server("localhost", 1234)
