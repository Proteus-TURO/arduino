import roslibpy

if __name__ == '__main__':
    client = roslibpy.Ros(host='localhost', port=9090)

    client.run()

    led = roslibpy.Topic(client, '/serial/led', 'std_msgs/UInt8')
    while True:
        number = int(input('Enter a number to send to the Arduino: '))
        led.publish(roslibpy.Message({'data': number}))


