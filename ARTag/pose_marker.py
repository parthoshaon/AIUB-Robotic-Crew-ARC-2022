import numpy as np
import cv2
import cv2.aruco as aruco
import os
import pickle
import math

calibration_path = './auto-travarsal/ar-tag/calibration/CameraCalibration.pckl'

# Check for camera calibration data
if not os.path.exists(calibration_path):
    print("You need to calibrate the camera you'll be using. See calibration project directory for details.")
    exit()

else:
    f = open(calibration_path, 'rb')
    (cameraMatrix, distCoeffs, _, _) = pickle.load(f,encoding='bytes')
    f.close()
    if cameraMatrix is None or distCoeffs is None:
        print("Calibration issue. Remove calibration/CameraCalibration.pckl and recalibrate your camera with calibration_ChAruco.py.")
        exit()

def isRotationMatrix(R) :
    Rt = np.transpose(R)
    shouldBeIdentity = np.dot(Rt, R)
    I = np.identity(3, dtype = R.dtype)
    n = np.linalg.norm(I - shouldBeIdentity)
    return n < 1e-6


def rotationMatrixToEulerAngles(R) :
    assert(isRotationMatrix(R))
    sy = math.sqrt(R[0,0] * R[0,0] +  R[1,0] * R[1,0])
    singular = sy < 1e-6
    if  not singular :
        x = math.atan2(R[2,1] , R[2,2])
        y = math.atan2(-R[2,0], sy)
        z = math.atan2(R[1,0], R[0,0])
    else :
        x = math.atan2(-R[1,2], R[1,1])
        y = math.atan2(-R[2,0], sy)
        z = 0
    return np.array([x, y, z])


cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_AUTOFOCUS, 0)
cap.set(3, 640)
cap.set(4, 480)
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
matrix_coefficients = cameraMatrix
distortion_coefficients = distCoeffs
marker = 0.20
###------------------ ARUCO TRACKER ---------------------------
while (True):
    ret, frame = cap.read()
    font = cv2.FONT_HERSHEY_SIMPLEX
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    cv2.aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_1000)
    parameters = cv2.aruco.DetectorParameters_create()


    corners, ids, rejected_img_points = cv2.aruco.detectMarkers(gray, cv2.aruco_dict,parameters=parameters,
        cameraMatrix=matrix_coefficients,
        distCoeff=distortion_coefficients)
    
    # If markers are detected
    if len(corners) > 0:
            for i in range(0, len(ids)):
                # show ids of the marker found
                strg = ''
                for i in range(0, ids.size):
                    strg += str(ids[i][0])+', '
                cv2.putText(frame, "Id: " + strg, (0,64), font, 1, (0,255,0),2,cv2.LINE_AA)
                # Estimate pose of each marker and return the values rvec and tvec---(different from those of camera coefficients)
                rvec, tvec, markerPoints = cv2.aruco.estimatePoseSingleMarkers(corners[i], marker, matrix_coefficients,
                                                                        distortion_coefficients)   
                rvec_flipped = rvec[0][0] * (-1)
                tvec_flipped = tvec[0][0] * (-1)
                rotationMatrix , jacobian = cv2.Rodrigues(rvec_flipped)
                realworld_tvec = np.dot(rotationMatrix,tvec_flipped)

                pitch,roll,yaw = rotationMatrixToEulerAngles(rotationMatrix)
                tvec_string = "x :%4.0fm  y :%4.0fm z :%4.0fm   theta :%4.0fdeg"%(realworld_tvec[0],abs(math.floor(realworld_tvec[1])), realworld_tvec[2],math.degrees(yaw))
                cv2.putText(frame,tvec_string,(20,460),font, 1, (0,255,0),2,cv2.LINE_AA)
                # Draw a square around the markers
                cv2.aruco.drawDetectedMarkers(frame, corners) 
                # Draw Axis
                cv2.aruco.drawAxis(frame, matrix_coefficients, distortion_coefficients, rvec, tvec, marker)  

                #Rover Controll
                y_dist = abs(math.floor(realworld_tvec[1]))
                x_dist = math.floor(realworld_tvec[1])
                deg = math.degrees(yaw)
    else:
        # 'No Ids' when no markers are found
        cv2.putText(frame, "No Ids", (0,64), font, 1, (0,255,0),2,cv2.LINE_AA)
    # display the resulting frame
    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()


