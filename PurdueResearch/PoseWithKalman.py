import os
import argparse
import csv
from datetime import datetime
from kalman2 import KF
import numpy as np

currentFolder = os.path.dirname(os.path.realpath(__file__))
log_path = os.path.join(currentFolder, 'log')
main_path = os.path.join(log_path, 'test_2024_11_19', 'AUV', 'x150', '0')

def compute_kf_pose(kf, idx, stamp, pose_data, acc_data):
    end_idx = idx
    prev_time = 0
    total_dt = 0
    dt = 0
    keys = list(acc_data.keys())
    for i in range(end_idx, len(keys)):
        acc_stamp = keys[i]
        dt = acc_stamp - prev_time
        total_dt += dt
        if end_idx < stamp:
            u = np.array([[acc_data[acc_stamp][0]],[acc_data[acc_stamp][1]]])
            if dt != 0:
                kf.predict(u, dt)
            prev_time = acc_stamp
        elif end_idx > stamp:
            break
    kf.update(np.array([[pose_data[0]],[pose_data[1]]]), total_dt)
    return kf.x, end_idx+1

def from_date_to_timestamp(date_str, date_format = None):
    date_format = "%Y-%m-%d %H:%M:%S" if date_format is None else date_format
    try:
        date_obj = datetime.strptime(date_str, date_format)
        stamp = date_obj.timestamp()
        return stamp
    except:
        return date_str

def main():
    parser = argparse.ArgumentParser(prog='Pose with kalman filter', description='A simple program to convert GPS to cartesian coordinates.')
    #parser.add_argument('-o', '--output-path', action='store', type=str, help='Output path for new CSV file.')
    parser.add_argument('-d', '--date', action='store', help='Provide the date.')

    args = parser.parse_args()
    #output_path = args.output_path
    date = args.date

    filename = 'kalman_%s.csv'%(date)
    filter_path = os.path.join(main_path, 'kalman')
    if not os.path.exists(filter_path):
        os.makedirs(filter_path)
    dest_path = os.path.join(filter_path, filename)

    acc_data = dict()
    ahrs_path = os.path.join(main_path, 'ahrs', 'x150_ahrs_%s.csv'%date)
    with open(ahrs_path, 'r') as ap:
        reader = csv.reader(ap)
        for row in reader:
            try:
                stamp = from_date_to_timestamp(row[0])
                acc_data[stamp] = [float(row[12]), float(row[13]), float(row[14])]
            except:
                pass

    pose_data = dict()
    shift_data = dict()
    calc_data = dict()
    est_path = os.path.join(main_path, 'est_pose', 'x150_est_pose_%s.csv'%date)
    with open(est_path, 'r') as ep:
        reader = csv.reader(ep)
        for row in reader:
            try:
                stamp = float(row[3])
                if row[4] == ' CalcPose':                
                    calc_data[stamp] = [row[4], float(row[9]), float(row[10]), float(row[11])]
                elif row[4] == ' ShiftPose':
                    shift_data[stamp] = [row[4], float(row[9]), float(row[10]), float(row[11])]
            except:
                pass

    #print(calc_data)
    #print(shift_data)

    start_x, start_y = 448.4517, 1370.53516

    R = np.array([[104.501, 14.17002],
                      [14.17002, 75.67645]])
    R2 = np.array([[10.32637, 0.340078],
                      [0.340078, 3.025482]])
    calc_kf = KF(0.1, 1.5, 20, 10, R2)
    calc_idx = 0
    calc_kf.x = np.array([[start_x], [start_y],[0.],[0.]])
    
    R = np.array([[88.8162, 14.17002],
                      [14.17002, 74.10239]])
    R2 = np.array([[3.366124, 0.340078],
                      [0.340078, 1.993272]])
    shift_kf = KF(0.1, 1.5, 20, 10, R2)
    shfit_idx = 0
    shift_kf.x = np.array([[start_x], [start_y],[0.],[0.]])

    with open(dest_path, 'w+') as of:
        writer = csv.writer(of, delimiter=',')
        writer.writerow(['stamp', 'est_type', 'x', 'y'])
        for key, data in calc_data.items():
            try:
                kf_pose = None
                est_type = data[0]
                kf_pose, calc_idx = compute_kf_pose(calc_kf, calc_idx, key, data[1:], acc_data)
                print(est_type, calc_idx, kf_pose)
                if kf_pose is not None:
                    writer.writerow([stamp, "%sKF"%est_type, kf_pose[0][0], kf_pose[1][0]])
                kf_pose, shfit_idx = compute_kf_pose(shift_kf, shfit_idx, key, shift_data[key][1:], acc_data) #data[1:]
                print("ShiftPose", shfit_idx, kf_pose)
                if kf_pose is not None:
                    writer.writerow([stamp, "ShiftPoseKF", kf_pose[0][0], kf_pose[1][0]])
            except Exception as e:
                print(e)

if __name__ == "__main__":
    main()

# PoseWithKalman.py -f 'log/AUV_X110/x150/0/gps/x150_gps_2024_10_29_16_15.csv' -o 'log/AUV_X110/x150/0