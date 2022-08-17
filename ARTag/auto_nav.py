import numpy as np
import math
import cv2
import cv2.aruco as aruco
import os
import pickle
import rover_nav
import time

debug = False
show_cam = True
marker_length = .18
font = cv2.FONT_HERSHEY_SIMPLEX
#load camera calibration
if not os.path.exists('./calibration/CameraCalibration.pckl'):
    print("Claibrate file not found")
    exit()
else:
    f = open('./calibration/CameraCalibration.pckl', 'rb')
    (mtx, dist, _, _) = pickle.load(f,encoding='bytes')
    f.close()
    if mtx is None or dist is None:
        print("Calibration issue")
        exit()
aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_250)
parameters =  aruco.DetectorParameters_create()
def getMarkerLocations(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    corners, ids, rejectedImgPoints = aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
    if ids is None:
        ids = np.array([])   
    frame_markers = aruco.drawDetectedMarkers(frame.copy(), corners, ids)
    rvecs, tvecs, _ = aruco.estimatePoseSingleMarkers(corners, marker_length, mtx, dist)   
    if rvecs is None:
        rvecs = np.array([])
    if corners:
        frame_markers = cv2.circle(frame_markers, tuple(corners[0][0][0]), 10, (255, 0, 255), 2)
    if tvecs is None:
        tvecs = np.array([])
    
    if show_cam:
        #drow box corner and id on marker
        frame_markers = aruco.drawDetectedMarkers(frame.copy(), corners, ids)
        #visualize axes
        axis_length = marker_length/2
        if rvecs is not None and tvecs is not None:
            for i in range(len(rvecs)):
                aruco.drawAxis(frame_markers, mtx, dist, rvecs[i], tvecs[i],axis_length)
        cv2.imshow("img", frame_markers)
        
    markers = np.array([])
    marker_count = len(ids)
    if len(rvecs) == marker_count and len(tvecs) == marker_count:
        markers = np.array([{"id":ids[i][0], "tvect":tvecs[i][0], "rvect":rvecs[i][0]} for i in range(len(ids))])
    else:
        print("Invalid count of translation or rotation vectors, should be", marker_count, "tvecs:", len(tvecs), "rvecs:", len(rvecs))

    if debug:
        print(markers)

    return corners, markers 

def coordinateTransform(tvect, rvect):
    r_matrix, _ = cv2.Rodrigues(rvect)
    r_matrix_inv = np.linalg.inv(r_matrix)
    cam_tvect = np.matmul(r_matrix_inv, -np.transpose(tvect))
    cam_rvect = -rvect
    return np.transpose(cam_tvect), cam_rvect

def QuaternionToDegrees(rvecs):
    R, _ = cv2.Rodrigues(rvecs)
    yaw = 180*math.asin(R[2][0])/math.pi
    pitch = 180*math.atan2(-R[2][1], R[2][2])/math.pi
    roll = 180*math.atan2(-R[1][0], R[0][0])/math.pi
    return pitch, roll, yaw


if __name__ == '__main__':
    rover_nav.rover_speed('1')
    video = cv2.VideoCapture(2)
    video.set(cv2.CAP_PROP_AUTOFOCUS, 0)
    video.set(3, 640)
    video.set(4, 480)
    halt_flag = 0
    right_flag = 0
    forward_flag = 0
    found_flag = 0
    while True:
        ret, frame = video.read()
        if ret:
            corners, markers  = getMarkerLocations(frame)
            if len(markers):
                if found_flag == 0:
                    rover_nav.rover_halt()
                    time.sleep(.5)
                    found_flag = 1
                    right_flag = 0

                m = markers[0]
                tcam, rcam = coordinateTransform(m["tvect"], m["rvect"])
                degrees = QuaternionToDegrees(rcam)
                cam_pos_str = (tcam[0], tcam[1], tcam[2], degrees[0], degrees[1], degrees[2])
                distance = math.floor(tcam[2])

                if len(markers) > 0 :
                    if distance > 2 and found_flag == 1 and forward_flag == 0:
                        rover_nav.rover_forward()
                        forward_flag = 1
                        halt_flag = 0

                    if distance <= 1  and halt_flag == 0 and forward_flag == 1:
                        rover_nav.rover_halt()
                        halt_flag = 1
                        forward_flag = 0           
            else:
                if right_flag == 0:
                    rover_nav.rover_right_rotation()
                    right_flag = 1
                    halt_flag = 0
                    forward_flag = 0
                    found_flag = 0


        if cv2.waitKey(1) == ord('q'):
            video.release()
            break

cv2.destroyAllWindows()





