/*
base on
v2.0 2016/4/19
 */
#include "mqtt.h"
#include "../JSON/cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include "SIM900.h"

///////////////////////////////////////
//////////////////////////////////////
  

int mqtt_connect_message(unsigned char *mqtt_message,char *client_id,char *username,char *password)  
{   
    char i = 0;
    int baseIndex;      
    char client_id_length = strlen(client_id);    
    int username_length = strlen(username);    
    int password_length = strlen(password);    
    int packetLen = 12 + 2 + client_id_length + 2 + username_length + 2 + password_length;    
        
    mqtt_message[0] = 16;                      // MQTT Message Type CONNECT    
    mqtt_message[1] = packetLen%256;    
    baseIndex = 2;    
    if( packetLen >127 ){//    
        mqtt_message[2] = 1;//packetLen/127;    
        baseIndex = 3;    
    }    
        
        
    mqtt_message[baseIndex] = 0;                       // Protocol Name Length MSB    
    mqtt_message[baseIndex+1] = 6;                       // Protocol Name Length LSB    
    mqtt_message[baseIndex+2] = 77;                      // ASCII Code for M    
    mqtt_message[baseIndex+3] = 81;                      // ASCII Code for Q    
    mqtt_message[baseIndex+4] = 73;                      // ASCII Code for I    
    mqtt_message[baseIndex+5] = 115;                     // ASCII Code for s    
    mqtt_message[baseIndex+6] = 100;                     // ASCII Code for d    
    mqtt_message[baseIndex+7] = 112;                     // ASCII Code for p    
    mqtt_message[baseIndex+8] = 3;                      // MQTT Protocol version = 3    
    mqtt_message[baseIndex+9] = 94;//194;                   // conn flags    
    mqtt_message[baseIndex+10] = 0;                      // Keep-alive Time Length MSB    
    mqtt_message[baseIndex+11] = 60;                     // Keep-alive Time Length LSB    
        
    mqtt_message[baseIndex+12] = 0;                      // Client ID length MSB    
    mqtt_message[baseIndex+13] = client_id_length;       // Client ID length LSB    
        
    baseIndex += 14;    
    // Client ID    
    for(i = 0; i < client_id_length; i++){    
        mqtt_message[baseIndex + i] = client_id[i];    
    }    
        
    baseIndex = baseIndex+client_id_length;    
        
    //username    
    mqtt_message[baseIndex] = 0;                      //username length MSB    
    mqtt_message[baseIndex+1] = username_length;       //username length LSB    
    baseIndex = baseIndex+2;    
    for(i = 0; i < username_length ; i++){    
        mqtt_message[baseIndex + i] = username[i];    
    }    
        
    baseIndex = baseIndex + username_length;    
        
    //password    
    mqtt_message[baseIndex] = 0;                      //password length MSB    
    mqtt_message[baseIndex+1] = password_length;       //password length LSB    
    baseIndex = baseIndex + 2;    
    for(i = 0; i < password_length ; i++){    
        mqtt_message[baseIndex + i] = password[i];    
    }    
        
    baseIndex += password_length;    
    
    return baseIndex;    
}  
  
  
int mqtt_publish_message(unsigned char *mqtt_message, char * topic, char * message) 
{    
        
    //mqtt_message = 0;    
        
    unsigned char i = 0;    
    unsigned char topic_length = strlen(topic);    
    unsigned char message_length = strlen(message);    
        
    mqtt_message[0] = 48;                                  // MQTT Message Type CONNECT    
    mqtt_message[1] = 2 + topic_length + message_length;   // Remaining length    
    mqtt_message[2] = 0;                                   // MQTT Message Type CONNECT    
    mqtt_message[3] = topic_length;                        // MQTT Message Type CONNECT    
        
    // Topic    
    for(i = 0; i < topic_length; i++){    
        mqtt_message[4 + i] = topic[i];    
    }    
        
    // Message    
    for(i = 0; i < message_length; i++){    
        mqtt_message[4 + topic_length + i] = message[i];    
    }    
        
    return 4 + topic_length + message_length;    
}    
  
  
int subindex = 0;  
int mqtt_subscribe_message(unsigned char *mqtt_message,unsigned char *topic)    
{    
    unsigned char topic_len = strlen((char*)topic);    
    mqtt_message[0] = 130;    
    mqtt_message[1] = topic_len+5;    
    mqtt_message[2] = 0;    
    mqtt_message[3] = 1;    
    mqtt_message[4] = 0;    
    mqtt_message[5] = topic_len;    
  
    for (subindex=0; subindex<topic_len; subindex++) {    
        mqtt_message[6+subindex] = topic[subindex];    
    }    
    mqtt_message[topic_len+6+1] = 0;    
    return topic_len+7;    
}
