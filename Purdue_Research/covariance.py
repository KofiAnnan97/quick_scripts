import numpy as np
import os
import argparse
import csv

class Covariance:
    def __init__(self):
        pass

    def strip_time_data(self, data):
        new_data = []
        for elem in data:
            new_data.append(elem[1:])
        return np.array(new_data)

    def get_data_from_file(self, filepath, time_idx, x_idx, y_idx, group_by_idx=-1, group_by=None):
        data = []
        with open(filepath, 'r') as fp:
            reader = csv.reader(fp)
            headers = next(reader)
            if group_by is not None:
                data = dict()
                try:
                    for row in reader:
                        group_name = row[group_by_idx]
                        if group_name not in data.keys():
                            data[group_name] = list()
                        data[group_name].append([float(row[time_idx]), float(row[x_idx]), float(row[y_idx])])
                except Exception as e:
                    print(e)
            else:
                try:
                    for row in reader:
                        data.append([float(row[time_idx]), float(row[x_idx]), float(row[y_idx])])
                except Exception as e:
                    print(e)
        return data

    def get_closest_stamp_idx(self, stamp, data):
        idx = -1
        for i in range(len(data)):
            if stamp == data[i][0]:
                idx = i
                break
            elif stamp < data[i][0]:
                idx = i-1
                break
        return idx

    def compute_err(self, ground_truth, test_data):
        err = []
        trunc_ground_truth = []
        for elem in test_data:
            stamp = elem[0]
            gt_idx = self.get_closest_stamp_idx(stamp, ground_truth)
            gt_elem = ground_truth[gt_idx]
        
            err_x = 100*(elem[1] - gt_elem[1])/gt_elem[1]
            err_y = 100*(elem[2] - gt_elem[2])/gt_elem[2]

            trunc_ground_truth.append(gt_elem)
            err.append([err_x, err_y])
        return np.array(trunc_ground_truth), np.array(err)

    def compute_covariance_with_error(self, data, errors):
        """
        Computes the covariance matrix, taking into account measurement errors.

        Args:
            data (numpy.ndarray): A 2D array where each row represents a variable and each column represents an observation.
            errors (numpy.ndarray): A 2D array with the same shape as data, containing the standard errors of each measurement.

        Returns:
            numpy.ndarray: The covariance matrix.
        """

        # Check if the input arrays have the same shape
        if data.shape != errors.shape:
            raise ValueError("Data and errors arrays must have the same shape.")

        # Calculate the covariance matrix without considering errors
        cov_matrix = np.cov(data, rowvar=False, bias=True)

        # Calculate the error covariance matrix
        error_cov_matrix = np.diag(np.square(np.transpose(errors)).mean(axis=1))

        # Add the error covariance matrix to the original covariance matrix
        return cov_matrix + error_cov_matrix

def main():
    parser = argparse.ArgumentParser(prog='Compute covariance', description='A simple program to compute covariance.')
    parser.add_argument('-d', '--date', action='store', type=str, help='Date format: YYYY_mm_DD_hh_MM_SS')
    parser.add_argument('-g', '--group-by', action='store', type=str, help='Group data by column vaue.')

    args = parser.parse_args()
    group_by = args.group_by if args.group_by is not None else None
    date_str = args.date if args.date is not None else ""

    log_path = "./fake_logs/AUV/"
    est_pose_filepath = os.path.join(log_path, "x150/0/est_pose/", "x150_est_pose_%s.csv"%date_str) 
    ground_truth_filepath = os.path.join(log_path, "gps_cartesian", "gps_cartesian_%s.csv"%date_str)
    cov = Covariance()
    
    ground_truth_data = cov.get_data_from_file(ground_truth_filepath, time_idx=0, x_idx=2, y_idx=3, group_by=None)
    test_data = cov.get_data_from_file(est_pose_filepath, time_idx=3, x_idx=9, y_idx=10, group_by_idx=4, group_by=group_by)
    if isinstance(test_data, dict):
        for key, data in test_data.items():
            print(key)
            gt_data, err_data = cov.compute_err(ground_truth_data, data) 
            #print(err_data)
            cov_matrix = cov.compute_covariance_with_error(cov.strip_time_data(gt_data), err_data)
            print(cov_matrix)

if __name__ == "__main__":
    main()

"""
Usage example:
python3 covariance.py -d "2024_11_19_15_27" -g "est_type"
"""