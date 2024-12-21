import numpy as np

class KF:
    def __init__(self, dt, std_acc, x_std_meas, y_std_meas, R):
        # Time step
        self.dt = dt
        # Initial state estimate (x, y, vx, vy)
        self.x = np.array([[0.], [0.], [0.], [0.]])  
        # Initial state covariance matrix
        self.P = np.eye(4)  
        # State transiition matrix
        self.A = np.array([[1., 0., self.dt, 0.],
                           [0., 1., 0., self.dt],
                           [0., 0., 1., 0.],
                           [0., 0., 0., 1.]]) 
        # Control input matrix (acceleration)
        self.B = np.array([[0.5 * self.dt**2, 0],
                           [0, 0.5 * self.dt**2],
                           [self.dt, 0],
                           [0, self.dt]])       
        # Measurement matrix
        self.H = np.array([[1, 0, 0, 0],
                           [0, 1, 0, 0]])  
        # Process noise covariance matrix
        self.Q = np.array([[std_acc**2 * dt**4 / 4, 0, std_acc**2 * dt**3 / 2, 0],
                           [0, std_acc**2 * dt**4 / 4, 0, std_acc**2 * dt**3 / 2],
                           [std_acc**2 * dt**3 / 2, 0, std_acc**2 * dt**2, 0],
                           [0, std_acc**2 * dt**3 / 2, 0, std_acc**2 * dt**2]])  
        # Measurement noise covariance matrix
        self.R = R #np.array([[x_std_meas**2, 0],
                   #        [0, y_std_meas**2]])  

    def predict(self, u, dt):
        self.dt = dt
        self.x = np.dot(self.A, self.x) + np.dot(self.B, u)
        self.P = np.dot(np.dot(self.A, self.P), self.A.T) + self.Q

    def update(self, z, dt):
        self.dt = dt
        S = np.dot(np.dot(self.H, self.P), self.H.T) + self.R
        K = np.dot(np.dot(self.P, self.H.T), np.linalg.inv(S))
        y = z - np.dot(self.H, self.x)
        self.x = self.x + np.dot(K, y)
        self.P = np.dot((np.eye(4) - np.dot(K, self.H)), self.P)

def kf_test():
    dt = 0.1
    R = np.array([[10.326, 0.34],
                  [0.34, 3.025]])
    std_acc = 0.7 #np.std(acc_data)
    x_std_meas = 1.5
    y_std_meas = 1.5

    kf = KF(dt, std_acc, x_std_meas, y_std_meas, R)

    # Simulate measurements
    measurements = np.array([[10, 20], [2, -3], [-3, 14]])

    import random
    for z in measurements:
        u_x = random.uniform(1,10)
        u_y = random.uniform(1,10)
        u = np.array([[u_x],[u_y]])
        kf.predict(u, dt)
        kf.update(np.array([z]), dt)
    print(kf.x)

if __name__ == "__main__":
    kf_test()

"""

Explanation:

    Initialization:
    We create a Kalman filter object with dim_x=6 (3 positions and 3 velocities) and dim_z=3 (measuring 3 positions).
    Matrices:
    We define the state transition matrix (F), measurement matrix (H), process noise covariance (Q), and measurement noise covariance (R).
    Prediction:
    The kf.predict() step predicts the next state based on the current state and the motion model.
    Update:
    The kf.update() step updates the state estimate based on the new measurement. 

Important considerations:

    Tuning Q and R:
    The process and measurement noise covariance matrices need to be tuned based on the characteristics of your system and sensors.
    Motion model:
    The example uses a constant velocity model, but you might need to adjust it based on the dynamics of your system.
"""