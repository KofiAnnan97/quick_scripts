#!/usr/bin/env python

import rospy
from datetime import datetime

from seatrac_pkg.msg import bcn_remote_gps, bcn_pose, bcn_status, head, bcn_pose_array, bcn_status_array
GPS_TOPIC = "/beacn_gps_data"
HEADING_TOPIC = "/heading"
POSE_TOPIC = "/beacon_pose"
STATUS_TOPIC = "/beacon_status"

class CommLogging:
    def __init__(self):
        self.gps = bcn_remote_gps()
        self.poses = bcn_pose_array()
        self.status = bcn_status_array()
        self.head = head()

    def init_app(self):
        self.rate = rospy.Rate(10)

        # Subscribers
        rospy.Subscriber(GPS_TOPIC, bcn_remote_gps, self.__gps_cbk)
        rospy.Subscriber(HEADING_TOPIC, head, self.__heading_cbk)
        rospy.Subscriber(POSE_TOPIC, bcn_pose_array, self.__pose_cbk)
        rospy.Subscriber(STATUS_TOPIC, bcn_status_array, self.__status_cbk)

    def __heading_cbk(self, data):
        self.heading = data

    def __gps_cbk(self, data):
        self.gps = data

    def __pose_cbk(self, data):
        if data is not None and len(data.pose) > 0:
            self.poses = data

    def __status_cbk(self, data):
        if data is not None and len(data.status) > 0:
            self.status= data

    def create_csv(self):
        import os
        parent_dir = os.path.dirname(os.path.realpath(__file__))
        log_dir = os.path.join(parent_dir, "log")
        comm_dir = os.path.join(log_dir, "comm")
        if not os.path.exists(comm_dir):
            os.mkdir(comm_dir)

        timestamp = datetime.now().isoformat()
        import re
        timestamp = re.sub(':', '_', timestamp)

        paths = []

        filename = "{}_gps.csv".format(timestamp)
        filepath = os.path.join(comm_dir, filename)
        paths.append(filepath)
        with open(filepath, "w") as f:
           f.write("timestamp,heading,lat,long\n")

        filename = "{}_heading.csv".format(timestamp)
        filepath = os.path.join(comm_dir, filename)
        paths.append(filepath)
        with open(filepath, "w") as f:
           f.write("timestamp,heading,xacc,yacc,zacc,roll,pitch\n")

        filename = "{}_pose.csv".format(timestamp)
        filepath = os.path.join(comm_dir, filename)
        paths.append(filepath)
        with open(filepath, "w") as f:
           f.write("timestamp,bid,roll,pitch,yaw,x,y,z\n")

        filename = "{}_status.csv".format(timestamp)
        filepath = os.path.join(comm_dir, filename)
        paths.append(filepath)
        with open(filepath, "w") as f:
           f.write("timestamp,bid,supply_voltage,temperature,pressure,vel_of_sound,comp_acc_x,comp_acc_y,comp_acc_z, comp_mag_x,comp_mag_y,comp_mag_z,comp_gyro_x,comp_gyro_y,comp_gyro_z,raw_acc_x,raw_acc_y,raw_acc_z, raw_mag_x,raw_mag_y,raw_mag_z,raw_gyro_x,raw_gyro_y,raw_gyro_z\n")

        return paths

    def write_csv_entry(self, paths):
        
        gps_log = paths[0]
        head_log = paths[1]
        pose_log = paths[2]
        status_log = paths[3]

        with open(gps_log, 'a') as f:
            t_stamp = datetime.now().isoformat()
            heading = self.gps.heading
            lat = self.gps.lat
            long = self.gps.long
            f.write("{0},{1},{2},{3}\n".format(t_stamp,heading,lat,long))

        with open(head_log, 'a') as f:
            t_stamp = datetime.now().isoformat()
            heading = self.head.heading
            xacc = self.head.xacc
            yacc = self.head.yacc
            zacc = self.head.zacc
            roll = self.head.roll
            pitch = self.head.pitch
            f.write("{0},{1},{2},{3},{4},{5},{6}\n".format(t_stamp,heading,xacc,yacc,zacc,roll,pitch))

        with open(pose_log, 'a') as f:
            for p in self.poses.pose:
                t_stamp = datetime.now().isoformat()
                bid = p.bid
                roll = p.roll
                pitch = p.pitch
                yaw = p.yaw
                x = p.x
                y = p.y
                z = p.z
                f.write("{0},{1},{2},{3},{4},{5},{6},{7}\n".format(t_stamp,bid,roll,pitch,yaw,x,y,z))

        with open(status_log, 'a') as f:
            for status in self.status.status:
                t_stamp = datetime.now().isoformat()
                bid = status.bid
                supply_voltage = status.supply_voltage
                temperature = status.temperature
                pressure = status.pressure
                vel_of_sound = status.vel_of_sound 
                f.write("{0},{1},{2},{3},{4},{5}\n".format(t_stamp,bid,supply_voltage,temperature,pressure,vel_of_sound))

    def run(self):
        log_paths = self.create_csv()
        while not rospy.is_shutdown():
            self.write_csv_entry(log_paths)
            self.rate.sleep()

if __name__ == "__main__":
    rospy.init_node("comm_logging_node", anonymous=True)
    cl = CommLogging()
    cl.init_app()
    try:
        cl.run()
    except rospy.ROSInterruptException:
        pass