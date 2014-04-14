#Name: Colin Ward
import cv
import time
import Image
#WARNING: Almost all of this code was from a combination Mr. Luca Amore (lines 18 to 57) and the opencv #library below (all other portion of code)
#http://www.lucaamore.com/?p=638
#http://docs.opencv.org/trunk/doc/py_tutorials/py_objdetect/py_face_detection/py_face_detection.html
#All I added was a right eye detector (to Mr. Luca Amore's code) which I found on the opencv library
#and please notice that on the screen it
#looks like the eye on the left but since the image is flipped its actually my right eye
#If there are any questions feel free to contact me and/or remove points 
#Capture from the default camera
capture = cv.CaptureFromCAM(0)
#Load necessary functions
faceCascade = cv.Load("haarcascade_frontalface_alt.xml")
eyecascade= cv.Load("haarcascade_eye.xml")
while (cv.WaitKey(15)==-1):
    #Allocate variables
    image = cv.QueryFrame(capture)     
    min_size = (20,20)
    image_scale = 3
    haar_scale = 1.1
    min_neighbors = 3
    haar_flags = 0
 
    #Create temporary images and allocate memory for the smaller images
    grayscale = cv.CreateImage((image.width, image.height), 8, 1)
    smallImage = cv.CreateImage(
            (
                cv.Round(image.width / image_scale),
                cv.Round(image.height / image_scale)
            ), 8 ,1)
 
    #Convert BGR to Gray so the information can be used by the HaarDetect objects
    cv.CvtColor(image, grayscale, cv.CV_BGR2GRAY)
 
    #Make the image smaller so the calculations are not as long
    cv.Resize(grayscale, smallImage, cv.CV_INTER_LINEAR)
 
    #Equalize the histogram
    cv.EqualizeHist(smallImage, smallImage)
 
    #Look for faces
    faces = cv.HaarDetectObjects(
            smallImage, faceCascade, cv.CreateMemStorage(0),
            haar_scale, min_neighbors, haar_flags, min_size
        )
 
    #If there is a detected face go though and put a red rectangle around them
    #then once we know where the faces are we begin detection of the right eye
    if faces:
        for ((x, y, w, h), n) in faces:
            pt1 = (int(x * image_scale), int(y * image_scale))
            pt2 = (int((x + w) * image_scale), int((y + h) * image_scale))
            cv.Rectangle(image, pt1, pt2, cv.RGB(255, 0, 0), 5, 8, 0)
            eyes = cv.HaarDetectObjects(
            smallImage, eyecascade, cv.CreateMemStorage(0),
            haar_scale, min_neighbors, haar_flags, min_size)
            for((xe,ye,we,he),i) in eyes:
		 #This excludes any eyes detected on the left side of the face
	   	 if (xe+we/2)<(x+w/2):
			#Create a rectangle around the eye
			pt1 = (int(xe * image_scale), int(ye * image_scale))
			pt2 = (int((xe + we) * image_scale), int((ye + he) * image_scale))
			cv.Rectangle(image, pt1, pt2, cv.RGB(255, 0, 0), 5, 8, 0)
    cv.ShowImage("face detection test", image)
 
cv.ReleaseCapture(capture)
