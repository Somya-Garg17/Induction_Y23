import socket

def send_command(sock, command):
    sock.sendall(command.encode())
    response = sock.recv(1024).decode().strip()
    print(response)
    return response

def main():
    server_address = ('localhost', 1234)
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(server_address)
    print("Connected to FTP server.")

    welcome_message = client_socket.recv(1024).decode().strip()
    print(welcome_message)

    if send_command(client_socket, "USER admin") != "Please enter your password":
        return
    if send_command(client_socket, "PASS adminpassword") != "Admin authentication successful. Welcome!":
        return
    
    send_command(client_socket,"ADDUSER user3 password3")
    send_command(client_socket,"DELUSER user2")
    send_command(client_socket,"BAN user1")
    send_command(client_socket,"UNBAN user4")
    client_socket.close()

if __name__ == "__main__":
    main()
