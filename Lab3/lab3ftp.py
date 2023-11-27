import socket

def ftp_check(ftp_address):
    host = ftp_address
    port = 21

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((host, port))
        except Exception as e:
            print(f"Connection error: {e}")
            return False

        welcome_message = s.recv(1024).decode('utf-8')
        print(welcome_message)

        s.sendall(b'QUIT\r\n')

    return True

def connect_ftp(ftp_address, login, password):
    host = ftp_address
    port = 21

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((host, port))
        except Exception as e:
            print(f"Connection error: {e}")
            return False

        welcome_message = s.recv(1024).decode('utf-8')
        print(welcome_message)

        s.sendall(b'QUIT\r\n')

    return True