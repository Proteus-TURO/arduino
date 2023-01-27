import serial
import time
import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class ArduinoConnection(Node):
    def __init__(self):
        super().__init__('arduino_connection')
        self.serial_connection = serial.Serial(port='/dev/ttyUSB0', baudrate=9600)
        self.subscription = self.create_subscription(String, 'controller/led', self.callback, 10)

    def callback(self, msg):
        self.serial_connection.write(msg.data.encode('utf-8'))


def main(args=None):
    rclpy.init(args=args)
    arduino_connection = ArduinoConnection()
    rclpy.spin(arduino_connection)
    arduino_connection.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

