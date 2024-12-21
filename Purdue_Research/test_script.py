import argparse

import rospy

from backseat_control.msg import VehicleState

class Roughie:
    def __init__(self) -> None:
        self.tank = 0
        self.lin_mass = 0
        self.servo = 0

class Test:
    def __init__(self) -> None:
        self.roughie = Roughie()
        self.vs = VehicleState()
        rospy.Subscriber("/VehicleState", VehicleState, self.__vs_cbk)

    def __vs_cbk(self, data):
        self.vs = data

    def create_log(self):
        from datetime import datetime
        import os
        timestamp = datetime.now().isoformat('_', timespec='seconds')
        filename = f"{timestamp}_.csv"
        with open(filename) as f:
           f.write("Timestamp, Current Tank Position, Expected Tank Position Current Linear Mass Position, Expected Linear Mass Position, Expected Servo Position")
           f.close()
        return filename

    def write_entry(self, timestamp, filename): #, curr_tank, exp_tank, curr_lin_mass, exp_lin_mass, exp_servo):
        with open(filename) as f:
            scalar = 6.5
            tank_pos_cm = self.vs.tankPos/scalar
            lin_mass_pos_cm = self.vs.linPos/scalar
            f.write(f"{timestamp},{tank_pos_cm},{self.roughie.tank},{lin_mass_pos_cm},{self.roughie.lin_mass},{self.roughie.servo}\n")
            
if __name__=="__main__":
    parser = argparse.ArgumentParser(description="ROUGHIE stuff")
    parser.add_argument('-i', '--identification', help='Simple text here', nargs=3)
    args = parser.parse_args()
    #print(args.identification)
    print(f"Tank: {args.identification[0]}, Linear Mass: {args.identification[1]}, Servo: {args.identification[2]}" )