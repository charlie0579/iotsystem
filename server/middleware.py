import asyncio
from hbmqtt.client import MQTTClient
from hbmqtt.mqtt.constants import QOS_0, QOS_1, QOS_2
import json


TOPICS = {
    'connect': '/connect',
    'msg_to_system': '/msg_to_system',
    'msg_to_terminal': '/msg_to_terminal',
    'alarm': '/alarm',
}

terminals = set()
out_queue = asyncio.Queue(maxsize=10000)
in_queue = asyncio.Queue(maxsize=10000)


def add_terminal(deviceid):
    global terminals
    terminals.add(deviceid)

def del_terminal(deviceid):
    global terminals
    if deviceid in terminals:
        terminals.remove(deviceid)

async def init_mqtt_client():
    client = MQTTClient()
    await client.connect('mqtt://127.0.0.1')
    await client.subscribe([
        (TOPICS['connect'], QOS_2),
        (TOPICS['msg_to_system'], QOS_0),
        (TOPICS['alarm'], QOS_2),
    ])
    return client 
    
    
def convert_msg(message):
    msg = None
    try:
        msg = json.loads(message)
    except Exception as e:
        print(e)
        pass
    
    return msg
        
    
async def handle_msg(topic, message, msg_queue):
    global terminals
    message = convert_msg(message)
    if message is None:
        return False
        
    if topic == TOPICS['connect']:
        if 'type' not in message or 'deviceid' not in message:
            return False
            
        if message['type'] == "in":
            add_terminal(message['deviceid'])
            print("A terminal is in. (%s)"%terminals)
            await msg_queue.put(message['deviceid'])
        elif message['type'] == "out":
            del_terminal(message['deviceid'])
            print("A terminal is out. (%s)"%terminals)
            await msg_queue.put(message['deviceid'])
        else:
            print("Invalid:(%s)%s"%(topic, message))
    elif topic == TOPICS['msg_to_system']:
        print('msg:%s'%message['content'])
        await msg_queue.put(message['content'])
    elif topic == TOPICS['alarm']:
        print('alarm:%s'%message['content'])
        await msg_queue.put(message['content'])
    else:
        print("Invalid topic:(%s)%s"%(topic, message))
    
    
async def system_recv_handler(client, msg_queue):
    while True:
        try:
            message = await client.deliver_message()
            packet = message.publish_packet
            topic = packet.variable_header.topic_name
            await handle_msg(topic, packet.payload.data.decode(), msg_queue)
        except Exception as e:
            print(e)
            raise e
            
            
async def system_send_handler(client, msg_queue):
    while True:
        try:
            message = await msg_queue.get()
            await client.publish(TOPICS['msg_to_terminal'], message.encode(), qos=QOS_0)
        except Exception as e:
            print(e)
            raise e
           

async def loop_test(in_queue, out_queue):
    while True:
        try:
            message = await in_queue.get()
            await out_queue.put(message)
        except Exception as e:
            print(e)
            raise e


async def send_msg(message):
    global out_queue
    await out_queue.put(message)


async def recv_msg():
    global in_queue
    message = await in_queue.get()
            
 
def get_mqtt_tasks(client):
    global in_queue
    global out_queue
    system_recv_task = asyncio.ensure_future(system_recv_handler(client, in_queue))
    system_send_task = asyncio.ensure_future(system_send_handler(client, out_queue))
    mqtt_tasks = asyncio.wait([system_recv_task, system_send_task], return_when=asyncio.FIRST_EXCEPTION)   
    return mqtt_tasks

    
if __name__ == '__main__':
    print('running...')
    loop = asyncio.get_event_loop()
    client = loop.run_until_complete(init_mqtt_client())

    system_recv_task = asyncio.ensure_future(system_recv_handler(client, in_queue))
    system_send_task = asyncio.ensure_future(system_send_handler(client, out_queue))
    loop_task = asyncio.ensure_future(loop_test(in_queue, out_queue))
    tasks = asyncio.wait([system_recv_task, system_send_task, loop_task], return_when=asyncio.FIRST_EXCEPTION)                         

    done, pending = loop.run_until_complete(tasks)
    for task in pending:
        task.cancel()
    loop.close()
        