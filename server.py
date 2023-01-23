import configparser
import socket
import subprocess


def run_nvda():
    subprocess.Popen(
        ["cmd", "/C", r"C:\Program Files (x86)\NVDA\nvda.exe", "-r"])


def main():
    config = configparser.ConfigParser()
    config.read("config.ini")
    address = config["server"]["address"]
    port = int(config["server"]["port"])
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((address, port))
    s.listen(1)
    while True:
        conn, addr = s.accept()
        while True:
            data = conn.recv(1024).decode()
            if not data:
                break
            elif data.strip() == "kill":
                run_nvda()
                break
        conn.close()


if __name__ == "__main__":
    main()
