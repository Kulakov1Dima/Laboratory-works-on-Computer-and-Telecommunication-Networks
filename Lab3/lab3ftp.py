
import socket

from readFtp import FTPserver

def ftp_check(ftp_address):
    host = ftp_address
    port = 21

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((host, port))
        except Exception as e:
            print(f"Ошибка соединения: {e}")
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
            print(f"Ошибка соединения: {e}")
            return False

        welcome_message = s.recv(1024).decode('utf-8')
        print(welcome_message)

        s.sendall(f'USER {login}\r\n'.encode('utf-8'))
        user_response = s.recv(1024).decode('utf-8')
        print(user_response)

        if user_response.startswith('331'):
            s.sendall(f'PASS {password}\r\n'.encode('utf-8'))
            pass_response = s.recv(1024).decode('utf-8')
            print(pass_response)

            if pass_response.startswith('230'):
                s.sendall(b'QUIT\r\n')
                return True
            else:
                print("Неверный логин или пароль")
                s.sendall(b'QUIT\r\n')
                return False
        else:
            print("Неверный логин или пароль")
            s.sendall(b'QUIT\r\n')
            return False

        
def list_files_ftp(ftp_address, login, password):
    ftp_server = FTPserver()
    ftp_server.list_ftp_directory(ftp_address, login, password, '')
    file_info = ftp_server.print_file_info()
    return [(file[0], file[1]) for file in file_info]