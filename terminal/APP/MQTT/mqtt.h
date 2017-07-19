#ifndef ONENET_MQTT_H
#define ONENET_MQTT_H

#include <stdint.h>
#include <time.h>


extern int mqtt_connect_message(unsigned char *mqtt_message,char *client_id,char *username,char *password);
extern int mqtt_publish_message(unsigned char *mqtt_message, char * topic, char * message);
extern int mqtt_subscribe_message(unsigned char *mqtt_message,unsigned char *topic);
#endif // ONENET_MQTT_H
