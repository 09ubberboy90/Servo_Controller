# -*- coding: utf-8 -*-
"""
Copyright(c) 2019 Florent Audonnet: audoflo63@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

"""

import serial
import sys
import time

class ServoController(object):
    """
    Create a controller for the robot arm through serial.


    Args:

        port (str): Name of the serial port to which the arm is connected to or name of the arm.

        nb_servo (int): Number of servo connected to the PIC

        baudrate (int, optional): Speed of communication. Defaults to 9600.
    """

    def __init__(self, port: str, nb_servo: int, baudrate: int = 9600):
        self.port = port
        self.nb_servo = nb_servo
        # default value of init for the servo in the PIC
        # +1 since index 0 is unused and the first servo is servo 1
        self.angles = [90]*(nb_servo+1)       
        try:
            self.ser = serial.Serial(self.port, baudrate, timeout=1)
        except serial.SerialException:
            print("SerialException : Invalid Serial name. Please input a valid name")
            sys.exit()
        
        self.send("H")
        print(self.ser.readline()) # Acknowledge of the pic name
        self.send("S") #Disable message from the PIC since it can cause error

    def send(self, mess):
        """
        send a message to the PIC one letter at a time

        Args:

            mess (string): the message you want to send
        """

        for i in mess:
            self.ser.write(i.encode())
    
    def enable_servo(self):
        """
        Enable the servos
        """
        self.send("D")
        self.send("E")


    def disable_servo(self):
        """
        Disable the servos
        """
        self.send("D")
        self.send("V")


    def set_angle(self, servo_id: int, angle: int, step: int, wait_time: int):
        """
        Set the angle smoothly from the actual angle to a new one
        based on the step and the timeout in between

        Args:

            servo_id (int): ID of the servo needed to change angle. From 1 to nb_servo.

            angle (int): New angle the servo should be in.

            step (int): How much should the angle increase each loop. Must be smaller than.

            wait_time (int): How much time to wait in between command sending
        """
        actual_angle = self.angles[servo_id]
        interval = abs(actual_angle-angle)
        if actual_angle > angle:
            sign = -1
        else:
            sign = 1

        for new_angle in range(0, interval, step):
            value = actual_angle+sign*new_angle
            print("The servo is at angle: {:d}".format(value))
            self.send("R{:02d}{:03d}".format(servo_id, value))
            self.angles[servo_id] = new_angle
            time.sleep(wait_time)

    def set_angles(self, angles: list):
        """
        Set the angle for all the servo at the same time
        
        Args:
            angles (list): List of length nb_servo where elements are list of len 4:
            [[servo_id, angle, step, wait_time],....]
        """
        
        if len(angles) < self.nb_servo:
            raise "Error too few element in the list"
        for angle in angles:
            if len(angle) < 4:
                raise "Error too few element in the sub array"
            if self.angles[angle[0]] != angle[1]:
                self.set_angle(angle[0], angle[1], angle[2], angle[3])