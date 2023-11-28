import ftplib
import os

class FTPserver:
    file_info = []

    def __init__(self):
        pass

    def list_ftp_directory(self, ftp_server, ftp_username, ftp_password, path):
        ftp = ftplib.FTP(ftp_server)
        ftp.login(user=ftp_username, passwd=ftp_password)

        def process_ftp_line(line):
            file_info = line.split()
            name = file_info[-1]
            full_path = path + '/' + name

            try:
                if line.startswith('d'):
                    folder_name = name
                    self.list_ftp_directory(ftp_server, ftp_username, ftp_password, path + '/' + folder_name)
                else:
                    file_size = int(file_info[4])
                    extension = os.path.splitext(name)[1][1:]

                    new_entry = [full_path, file_size]

                    if all(entry[0] != full_path for entry in self.file_info):
                        self.file_info.append(new_entry)
            except Exception as e:
                print(f"Error processing file/directory {full_path}: {str(e)}")

        def process_ftp_listing(line):
            if line.startswith('d'):
                folder_name = line.split()[-1]
                try:
                    self.list_ftp_directory(ftp_server, ftp_username, ftp_password, path + '/' + folder_name)
                except Exception as e:
                    print(f"Error listing directory {path}/{folder_name}: {str(e)}")
            else:
                process_ftp_line(line)

        try:
            ftp.cwd(path)
            ftp.retrlines('LIST', process_ftp_listing)
        except Exception as e:
            print(f"Error retrieving file/directory list in {path}: {str(e)}")
        finally:
            ftp.quit()

    def print_file_info(self):
        total_size = sum([file[1] for file in self.file_info])
        file_sizes = {}
        for file in self.file_info:
            extension = os.path.splitext(file[0])[1][1:]
            file_sizes[extension] = file_sizes.get(extension, 0) + file[1]

        result = []
        for file in self.file_info:
            result.append(file)

        result.append(("Total file size", total_size))

        for ext, size in file_sizes.items():
            result.append((f"Total size of files with extension {ext}", size))

        return result