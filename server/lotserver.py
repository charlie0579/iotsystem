#!/usr/bin/env python
# -*- coding: utf-8 -*- 
import asyncio
import websockets
import json
import os,sys
from middleware import get_mqtt_tasks, init_mqtt_client, send_msg, recv_msg

class phone_info(object):
    phone_info = {
        'id': id,
        'status': status,
    }
    def __init__(self, id, status):
        self.id = id
        self.status = status
    
    def store_phone_info(id, status=None):
        phone_info['id'] = id
        phone_info['status'] = status
  
    def get_phone_info(id):
        pass
    
    def update_phone_info(id, status=None):
        phone_info['id'] = id
        phone_info['status'] = status

def phone_to_terminal(message):
    msg_to_terminal = {
        'msg_type': 'system',
        'id': 1,
        'object': 'terminal',
        'status': 'connected',
    }
    if message["object"] == 'light':
        #TODO: call interface from mqtt
        pass
    elif message["object"] == 'temperature':
        #TODO: call interface from mqtt
        pass
    else:
        pass
def handle_phone_msg(message):
    ack_to_phone = {
        'msg_type': 'ack',
        'object': 'server',
        'status': 'succeed',
    }
    #phone_info.update_phone_info(message['id'])                               #store id
    message = None
    ack_to_phone['request_msg'] = message['request_msg'] 
    if message['action'] == 'connect':
        # TODO: How to decide connect succeed or failed things need to do when connected.
        phone_info.store_phone_info(message['id'], status='connected')                               #store id
        message = json.dumps(ack_to_phone)
    elif message['action'] == 'disconnect':
        #TODO: things need to do when disconnect
        phone_info.store_phone_info(message['id'], status='disconnected')                               #store id
        message = json.dumps(ack_to_phone)
    elif message['action'] == 'getinfo':
        #TODO: call class to get info of terminals    
        message = json.dumps(ack_to_phone)
    return message

def consumer(message):
    msg_body = json.loads(message)
    print (msg_body)
    if msg_body["msg_type"] == 'system' and msg_body["object"] == 'phone':
         message = handle_phone_msg(msg_body)
    elif msg_body["msg_type"] == 'operate':
         phone_to_terminal(msg_body)      # or call mqtt interface directly if there is no need to change msg content,  return?????????
    return message

def producer():
    msg_to_phone = {
        'msg_type': 'system',
        'id': 'id',
        'object': 'terminal',
        'action': 'connect',
    }
    message = json.dumps(msg_to_phone)
    return message
#consumer_handler
async def recv_handler(websocket):
    while True:
        try:
            message = await websocket.recv()
            print("< {}".format(message))
            message = consumer(message)
            await websocket.send(message)
        except Exception as e:
            print(e)
            raise e

#producer_handler
async def send_handler(websocket):
   # while True:
        try:
            message = producer()
            await websocket.send(message)
            print("> {}".format(message))
        except Exception as e:
            print(e)
            raise e

async def handler(websocket, path):
    await websocket.send('Connect success.')
    recv_task = asyncio.ensure_future(recv_handler(websocket))
    send_task = asyncio.ensure_future(send_handler(websocket))
    done, pending = await asyncio.wait(
         [recv_task, send_task],
         return_when=asyncio.FIRST_EXCEPTION,
    )
    for task in pending:
        task.cancel()

start_server = websockets.serve(handler, '0.0.0.0 ', 8765)

loop = asyncio.get_event_loop()
client = loop.run_until_complete(init_mqtt_client())
mqtt_tasks = get_mqtt_tasks(client)
tasks = asyncio.gather(start_server, mqtt_tasks)
loop.run_until_complete(tasks)
loop.run_forever()
