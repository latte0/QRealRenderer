from __future__ import print_function
import socket
from contextlib import closing

def main():
  host = '127.0.0.1'
  port = 1500
  bufsize = 4096

  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  with closing(sock):
    sock.bind((host, port))
    while True:
      print(sock.recv(bufsize))
  return

if __name__ == '__main__':
  main()
