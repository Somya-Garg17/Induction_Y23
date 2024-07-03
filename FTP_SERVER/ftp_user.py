import socket

def send_command(sock, command):
    sock.sendall(command.encode())
    response = sock.recv(1024).decode().strip()
    print(response)
    return response

def main():
    # Connect to the FTP server
    server_address = ('localhost', 1234)
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(server_address)
    print("Connected to FTP server.")

        # Receive the server's welcome message
    welcome_message = client_socket.recv(1024).decode().strip()
    print(welcome_message)

        # Authenticate with username
    if send_command(client_socket, "USER user1") != "Please enter your password":
        return

        # Authenticate with password
    if send_command(client_socket, "PASS password1") != "Authentication successful. Welcome!":
        return

        # After successful authentication, you can send other commands
    send_command(client_socket, "LIST")
        # Retrieve a file
    response = send_command(client_socket, "RETR test.txt")
    if response == "File not found":
        print("File not found on the server.")
    else:
    # File exists, receive the file data
        file_data = b""
        while True:
            chunk = client_socket.recv(1024)
            if not chunk:
                break
            if chunk.endswith(b"END_OF_FILE\r\n"):
            # Remove the "END_OF_FILE" marker from the last chunk
                chunk = chunk[:-len(b"END_OF_FILE\r\n")]
                file_data += chunk
                break
            file_data += chunk
    # Save the received file data to a file
        with open("received_file.txt", "wb") as file:
            file.write(file_data)
        print("File retrieved successfully.")


        # Store a file
    send_command(client_socket, "STOR test_upload.txt")
    with open("test1.txt", "rb") as file:
        file_data1 = file.read()
    client_socket.sendall(file_data1)
    response = client_socket.recv(1024).decode().strip()
    print(response)

        # Quit
    response = send_command(client_socket, "QUIT")
    if response == "Goodbye! Disconnecting from the server.":
        print("Server closed the connection.")
        client_socket.close()
    else:
        print("dumb")
            


if __name__ == "__main__":
    main()
