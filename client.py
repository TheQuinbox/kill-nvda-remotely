import configparser
import socket


def main():
    config = configparser.ConfigParser()
    config.read("config.ini")
    address = config["server"]["address"]
    port = int(config["server"]["port"])
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((address, port))
    message = "kill"
    s.sendall(message.encode())
    s.close()


if __name__ == "__main__":
    main()
