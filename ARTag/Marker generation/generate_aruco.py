import numpy as np
import cv2
import cv2.aruco as aruco
aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_50)
img = aruco.drawMarker(aruco_dict, 1, 200)
cv2.imwrite("test_marker.jpg", img)
cv2.imshow('frame', img)
cv2.waitKey(0)
cv2.destroyAllWindows()