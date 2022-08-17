import cv2
import cv2.aruco as aruco
gridboard = aruco.GridBoard_create(
        markersX=3, 
        markersY=4, 
        markerLength=0.1, 
        markerSeparation=0.01, 
        dictionary=aruco.Dictionary_get(aruco.DICT_4X4_50))
img = gridboard.draw(outSize=(988, 1400))
cv2.imwrite("test_gridboard.jpg", img)
cv2.imshow('Gridboard', img)
cv2.waitKey(0)
cv2.destroyAllWindows()