import smtplib
from email import encoders
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email.mime.multipart import MIMEMultipart
from email.header import Header
import os

def send_email(host, port, email, password, to_addr, subject, body_text, file_path=None):
    try:
        message = MIMEMultipart()
        message["From"] = email
        message["To"] = to_addr
        message["Subject"] = subject
        message.attach(MIMEText(body_text, "plain"))

        if file_path:
            filename1 = file_path.split("\\")
            filename = filename1[len(filename1)-1]
            with open(file_path, "rb") as attachment:
                part = MIMEBase("application", "octet-stream")
                part.set_payload(attachment.read())
            encoders.encode_base64(part)

            # Явное кодирование имени файла
            part.add_header(
                "Content-Disposition",
                "attachment",
                filename=Header(filename, 'utf-8').encode()
            )
            message.attach(part)

        server = smtplib.SMTP(host, port)
        server.starttls()
        server.login(email, password)
        server.sendmail(email, to_addr, message.as_string())
        return "True"
    except smtplib.SMTPException as err:
        print(f"Произошла ошибка: {err}")
        return "False"
    finally:
        server.quit()