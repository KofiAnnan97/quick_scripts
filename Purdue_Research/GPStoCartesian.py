import os
import argparse
import csv
import utm
from datetime import datetime

currentFolder = os.path.dirname(os.path.realpath(__file__))
log_path = os.path.join(currentFolder, 'log')

def trunc_vals(xy):
    tmp = []
    for val in xy:
        int_max = 1000000000
        if (val * 100000) < int_max:
            tmp.append(int(val * 100000))
        else:
            tmp.append(int((val * 100000) % int_max))
        tmp[-1] /= 100000.0
    return tmp

def from_gps_to_cartesian(lat, long):
    data = utm.from_latlon(latitude=float(lat), longitude=float(long))
    return data[0:2]

def from_date_to_timestamp(date_str, date_format = None):
    date_format = "%Y-%m-%d %H:%M:%S" if date_format is None else date_format
    try:
        date_obj = datetime.strptime(date_str, date_format)
        stamp = date_obj.timestamp()
        return stamp
    except:
        return date_str

def main():
    parser = argparse.ArgumentParser(prog='GPS to Cartesian', description='A simple program to convert GPS to cartesian coordinates.')
    parser.add_argument('-f', '--file-path', action='store', type=str, help='Path to desired CSV file.')
    parser.add_argument('-o', '--output-path', action='store', type=str, help='Output path for new CSV file.')
    parser.add_argument('-t', '--truncate-values', action='store_true', help='Truncate values.')
    parser.add_argument('-df' '--date-format', action='store', help='Provide the format of the date for convert. Default is %Y-%m-%d %H:%M:%S.')

    args = parser.parse_args()
    filepath = args.file_path
    output_path = args.output_path
    trunc = args.truncate_values

    data = list()
    with open(filepath, 'r') as f:
        reader = csv.reader(f)
        for line in reader:
            data.append(line)

    import re
    date_info = re.search('[\d+_]*.csv', filepath).group(0)
    filename = 'gps_cartesian%s'%(date_info)

    cartesian_path = os.path.join(output_path, 'gps_cartesian')
    if not os.path.exists(cartesian_path):
        os.makedirs(cartesian_path)

    dest_path = os.path.join(cartesian_path, filename)

    with open(dest_path, 'w+') as o:
        writer = csv.writer(o, delimiter=',')
        writer.writerow(['stamp', 'ms', 'x', 'y'])
        for line in data:
            try:
                xy = from_gps_to_cartesian(line[2], line[3])
                stamp = from_date_to_timestamp(line[0])
                if trunc == True:
                    xy = trunc_vals(xy)
                    writer.writerow([stamp, line[1], xy[0], xy[1]])
                else:
                    writer.writerow([stamp, line[1], xy[0], xy[1]])
            except:
                pass

if __name__ == "__main__":
    main()

#python3 GPStoCartesian.py -t -f 'log/AUV_X110/x150/0/gps/x150_gps_2024_10_29_16_15.csv' -o 'log/AUV_X110/x150/0'