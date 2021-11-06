import json
import boto3
from parameters import *

print('Loading function')

def lambda_handler(event, context):
    iot = boto3.client('iot-data', endpoint_url='https://'+iot_endpoint_id+'-ats.iot.'+region+'.amazonaws.com')
    output = {}
    try:
        print(event['Records'][0]['Sns']['Message'])     
        message = event['Records'][0]['Sns']['Message']
        message_json = json.loads(message)
        output['alarmName'] = message_json['AlarmName']
        output['NewStateValue'] = message_json['NewStateValue']
        iot.publish(topic=mqtt_topic,qos=0,payload=json.dumps(output, ensure_ascii=False))
    except:
        print("An exception occurred with SNS printing the event")
        print("Event: " + str(event))
        iot.publish(topic=mqtt_topic,qos=0,payload=json.dumps(event, ensure_ascii=False))