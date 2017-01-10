# -*- coding:utf-8 -*-
import socket
import thread

BLOCK_SIZE = 1024
host = '127.0.0.1'
port = 12344


def send(s, filename):
    print('Before Sending...')
    with open(filename, 'rb') as f:
        block = f.read(BLOCK_SIZE)
        while block:
            print('Sending...')
            s.send(block)
            block = f.read(BLOCK_SIZE)
    print s.recv(1024)
    print('Done Sending...')



if __name__ == '__main__':
    num = 1
    print('Start')
    while num <= 10:
        s = socket.socket()
        s.connect((host, port))
        filename = 'sent/' + str(num).zfill(2) + '.jpg'
        send(s, filename)
        s.close()
        num += 1
    print('Finished')


    # send(s, 'sent/01.jpg')
    # s.sendall('0')
    # while True:
    #     data = s.recv(1024)
    #     print(data.decode('utf-8'))

    # f = open('sent/18.jpg', 'rb')
    # print('Sending...')
    # l = f.read(1024)
    # while (l):
    #     print('Sending...')
    #     s.send(l)
    #     l = f.read(1024)
    # f.close()
    # print('Done Sending')
    # s.shutdown(socket.SHUT_WR)