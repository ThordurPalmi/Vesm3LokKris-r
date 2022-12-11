"""
Simple example of using the RF24 class.
"""

import time
import struct
import board
from digitalio import DigitalInOut
from random import normalvariate
import requests
import threading
from flask import Flask, request

ip = "192.168.1.140"
global flag
flag = 0

app = Flask(__name__)

@app.route('/', methods=['POST'])
def handle_post_request():
    global flag

    length = int(request.headers['Content-Length'])

    body = request.get_data().decode()


    if body == "refresh":
        flag = 1

    return 'Received POST request'


def run_flask():
    app.run(host=ip, port=5050)

thread = threading.Thread(target=run_flask)  # notast við threads til að geta keyrt flask serverinn i bakgrunni
thread.start()


from circuitpython_nrf24l01.rf24 import RF24
import spidev

SPI_BUS, CSN_PIN, CE_PIN = (None, None, None)

 
SPI_BUS = spidev.SpiDev() 
CSN_PIN = 0  
CE_PIN = DigitalInOut(board.D22) 
SPI_BUS = board.SPI() 
CE_PIN = DigitalInOut(board.D4)
CSN_PIN = DigitalInOut(board.D5)


nrf = RF24(SPI_BUS, CSN_PIN, CE_PIN)


nrf.pa_level = -12

# pípurnar í c++ eru þær [0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL]
address = [b'\xe1\xf0\xf0\xf0\xf0', b'\xd2\xf0\xf0\xf0\xf0']



# skrifar í pípu 2
nrf.open_tx_pipe(address[1])  

# hlustar á pípu 1
nrf.open_rx_pipe(1, address[0])  # using pipe 1




def sendastreng(strengur):  # count = 5 will only transmit 5 packets
    nrf.listen = False  # ensures the nRF24L01 is in TX mode

    nrf.send(strengur)



# 300 sec = 5 min
delay = 300

last_send_time = 0


while True:
    if time.perf_counter() - last_send_time >= delay or flag == 1:  # non block delay
        sendastreng(b"senddata00000000")
        print("sent")
        flag = 0
        last_send_time = time.perf_counter()

    nrf.listen = True

    if nrf.available():

        payload_size, pipe_number = (nrf.any(), nrf.pipe)

        buffer = nrf.read()

        message = buffer.decode('utf-8')
        print(message)
                                   #         {rak tem}{rak tem}{  }{  }
        if message != "":           #          {dht22 }{AM2301}{VH}{re}
        #         hiti            raki         123123  123123  99 11
        #                                      012345  678911  23 45
            dht22 = [message[3:6], message[0:3]] # 123 123
            Am2301 = [message[9:12],message[6:9]] # 123 123
            Vatnsh = [message[12:14]]
            relaypos = [message[14], message[15]] # 1 1
            print(message)
            print(dht22)
            print(Am2301)
            print(Vatnsh)
            print(relaypos)
            json_data = {
                "dht22" : ":".join(dht22),
                "Am2301" : ":".join(Am2301),
                "Vatnsh" : ":".join(Vatnsh),
                "relaypos" : ":".join(relaypos),
            }

            headers = {
                'Content-Type': 'application/json'}



            try:
                response = requests.post('http://'+ip+':5000/api', json=json_data, headers=headers)

            except requests.HTTPError as e:
                print("Http error")

            except requests.RequestException as e:
                print("Http error")