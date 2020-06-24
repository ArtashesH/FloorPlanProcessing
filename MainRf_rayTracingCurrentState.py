# -*- coding: utf-8 -*-
"""
Ray Tracing for wireless channel propogation

@author: Mohammed Hirzallah, 2019
"""

import xml.etree.ElementTree as ET
from xml.dom import minidom

import scipy.io as sio
import numpy as np
import math
import re
#import matplotlib.pyplot as plt



import matplotlib
matplotlib.use("TkAgg")
from matplotlib import pyplot as plt



from mpl_toolkits import mplot3d
from scipy.ndimage import gaussian_filter
from scipy.stats import norm


import scipy.misc
from skimage.draw import line_aa
from skimage.draw import polygon
from PIL import Image
import cv2

### Finding maximum value's index from givven array

def findMaximumValueIndexFromArray(inputArray):
    if len(inputArray) == 0:
        return -1
    indexOfMaximumEl = -1
    tmpMaximumEl = -90
    for i in range(0,len(inputArray)):
        if 	inputArray[i] >= tmpMaximumEl:
            tmpMaximumEl = inputArray[i]                            
            indexOfMaximumEl = i
    return indexOfMaximumEl


##Finding maximum value of given mat, the smallest value is -90 , considered as a smallest dbm value

def findMaximumElementOfMat(inputMat, countOfRows,countOfCols):
    maximumElement = -90
    fileArrayTxtFile = open("myfileNew.txt","w")	
    for i in range(countOfRows):
        for j in range(countOfCols):
            if maximumElement < inputMat[i,j]:
                maximumElement = inputMat[i,j]
                #print(
            fileArrayTxtFile.write(str(inputMat[i,j]))
            fileArrayTxtFile.write("\n")
    fileArrayTxtFile.close()
    return maximumElement
	
	
#For current point calculate SINR value#
	
def SINR_dbCalculationFunction(arrayOfDbsFromPoint):
#Find maximum value from given array
    maxDbmValueIndex = findMaximumValueIndexFromArray(arrayOfDbsFromPoint) 
    if maxDbmValueIndex == -1:
        return 0
    maxDbmValue = arrayOfDbsFromPoint[maxDbmValueIndex]
    currentMvValuesArray = []
   
    sumOfMvValues = 0
    for i in range(len(arrayOfDbsFromPoint)):
        tmpMvValue = math.pow(10,(arrayOfDbsFromPoint[i]/10))
    
        if i != maxDbmValueIndex:        
            sumOfMvValues += tmpMvValue
   
    noiseVal = math.pow(10,-90/10)
    sumOfMvValues += noiseVal
    
    bestMvValue =  math.pow(10,(arrayOfDbsFromPoint[maxDbmValueIndex]/10))

    sinrMv = bestMvValue / sumOfMvValues   

   
    sinrDbm = 10*(math.log10(sinrMv))	 
   # print(sinrMv)
    return sinrDbm
	
	
##Create colored (segmented )image for SINR values, with range by range	
	
def createColoredImageWithSINR( finalMapSINR,countOfRows,countOfCols):
    rfMapFull = np.ones((int(countOfRows),int(countOfCols)),np.int)
    imgRes = np.zeros( ( np.array(rfMapFull).shape[0], np.array(rfMapFull).shape[1] +150, 3 ) ) 
	
	## FinalMapSINR is the mat , where each pixel have sinr value in corresponding place.
    print("Before for sinr")
    print(countOfRows)
    print(countOfCols)
    for i in range(countOfRows):
        for  j in range(countOfCols):
            #Checking some value range for coloring with different colors
            if finalMapSINR[i,j] < - 20:
                imgRes[i,j,0] = 1
                imgRes[i,j,1 ] = 77
                imgRes[i,j,2] = 254
            elif finalMapSINR[i,j] >= -20 and finalMapSINR[i,j] < -5:
                imgRes[i,j,0] = 16
                imgRes[i,j,1] = 133
                imgRes[i,j,2] = 239		
            elif finalMapSINR[i,j] >=-5 and finalMapSINR[i,j] < 0:
                imgRes[i,j,0] = 39
                imgRes[i,j,1] = 181
                imgRes[i,j,2] = 216
            elif finalMapSINR[i,j] >=  0 and  finalMapSINR[i,j] < 5:
                imgRes[i,j,0] = 0
                imgRes[i,j,1] = 255
                imgRes[i,j,2] = 249
            elif finalMapSINR[i,j] >= 5 and finalMapSINR[i,j] < 20:
                imgRes[i,j,0] = 146
                imgRes[i,j,1] = 255
                imgRes[i,j,2] = 140
            elif finalMapSINR[i,j] >= 20 and finalMapSINR[i,j] < 50:
                imgRes[i,j,0] = 21
                imgRes[i,j,1] = 255
                imgRes[i,j,2] = 9
            
            elif finalMapSINR[i,j] >= 50 and finalMapSINR[i,j] < 80:
                imgRes[i,j,0] = 7
                imgRes[i,j,1] = 147
                imgRes[i,j,2]= 0
            
            elif finalMapSINR[i,j] >= 80 and finalMapSINR[i,j] < 120:
                imgRes[i,j,0] = 251
                imgRes[i,j,1] = 68
                imgRes[i,j,2] = 78
				
            elif finalMapSINR[i,j] >= 120 and finalMapSINR[i,j] < 150:
                imgRes[i,j,0] = 226
                imgRes[i,j,1] = 5
                imgRes[i,j,2] = 16
            elif finalMapSINR[i,j] >= 150 and finalMapSINR[i,j] < 180:
                imgRes[i,j,0] = 137
                imgRes[i,j,1] = 3
                imgRes[i,j,2] = 10
            else:
                imgRes[i,j,0] = 255
                imgRes[i,j,1] = 255
                imgRes[i,j,2] = 255 
				
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(countOfRows/35)),(countOfCols + 50, int(2*countOfRows / 35)),(1,77,254),-1)
    cv2.putText(imgRes,"< -20DB",(countOfCols + 5 ,int(3*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0),1)
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(4*countOfRows/35)),(countOfCols + 50, int(5*countOfRows / 35)),(16,133,239),-1)
    cv2.putText(imgRes,">= -20DB  < -5DB",(countOfCols + 5 ,int(6*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(7*countOfRows/35)),(countOfCols + 50, int(8*countOfRows / 35)),(39,181,216),-1)
    cv2.putText(imgRes,">= -5DB  < 0DB",(countOfCols + 5 ,int(9*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(10*countOfRows/35)),(countOfCols + 50, int(11*countOfRows / 35)),(0,255,249),-1)
    cv2.putText(imgRes,">= 0DB  < 5DB",(countOfCols + 5 ,int(12*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(13*countOfRows/35)),(countOfCols + 50, int(14*countOfRows / 35)),(146,255,140),-1)
    cv2.putText(imgRes,">= 5DB  < 20DB",(countOfCols + 5 ,int(15*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(16*countOfRows/35)),(countOfCols + 50, int(17*countOfRows / 35)),(21,255,9),-1)
    cv2.putText(imgRes,">= 20DB  < 50DB",(countOfCols + 5 ,int(18*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(19*countOfRows/35)),(countOfCols + 50, int(20*countOfRows / 35)),(7,147,0),-1)
    cv2.putText(imgRes,">= 50DB  < 80DB",(countOfCols + 5 ,int(21*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(22*countOfRows/35)),(countOfCols + 50, int(23*countOfRows / 35)),(251,68,78),-1)
    cv2.putText(imgRes,">= 80DB  < 120DB",(countOfCols + 5 ,int(24*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(25*countOfRows/35)),(countOfCols + 50, int(26*countOfRows / 35)),(226,5,16),-1)
    cv2.putText(imgRes,">= 120DB  < 150DB",(countOfCols + 5 ,int(27*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(28*countOfRows/35)),(countOfCols + 50, int(29*countOfRows / 35)),(137,3,10),-1)
    cv2.putText(imgRes,">= 150DB  < 180DB",(countOfCols + 5 ,int(30*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	
    imgRes = cv2.rectangle(imgRes,( countOfCols + 20 , int(31*countOfRows/35)),(countOfCols + 50, int(32*countOfRows / 35)),(255,255,255),-1)
    cv2.putText(imgRes,">= 180DB",(countOfCols + 5 ,int(33*countOfRows / 35)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0))
	    
    return imgRes				
 
##Create colored (segmented )image for DBm values, with range by range	
def coloringOfSignalAreas(finalRfMap, countOfRows, countOfCols):
    rfMapFull = np.ones((int(countOfRows),int(countOfCols)),np.int)
    imgRes = np.zeros( ( np.array(rfMapFull).shape[0], np.array(rfMapFull).shape[1], 3 ) )
    for i in range(countOfRows):
        for j in range(countOfCols):
            imgRes[i,j,0] = 223
            imgRes[i,j,1] = 156
            imgRes[i,j,2] = 150
    #imgRes = np.zeros( (countOfRows, countOfCols), dtype = int) 
    print("Start function color area detection ")
    print(countOfRows)
    print(countOfCols)
    for i in range(countOfRows):
        for j in range(countOfCols):
          #  print("Insode loop ")
            if finalRfMap[i,j] > -20: 
                imgRes[i,j,2] = 255
                imgRes[i,j,0] = 0
                imgRes[i,j,1] = 0
            if finalRfMap[i,j] <= -20 and finalRfMap[i,j] > -35:
              # imgRes[i,j,0] = 255
              #  imgRes[i,j,1] = 128
              #  imgRes[i,j,2] = 0
                imgRes[i,j,0] = 39
                imgRes[i,j,1] = 127
                imgRes[i,j,2] = 255	
            if finalRfMap[i,j] <= -35 and finalRfMap[i,j] > -47:
                imgRes[i,j,0] = 0
                imgRes[i,j,1] = 255
                imgRes[i,j,2] = 255
            if finalRfMap[i,j] <= -47 and finalRfMap[i,j] > -60:
                imgRes[i,j,0] = 0
                imgRes[i,j,1] = 255
                imgRes[i,j,2] = 128
            if finalRfMap[i,j] <= -60 and finalRfMap[i,j] > -73:
                imgRes[i,j,0] = 0
                imgRes[i,j,1] = 128
                imgRes[i,j,2] = 0
            if finalRfMap[i,j] <= -73 and finalRfMap[i,j] > -90:
                imgRes[i,j,0] = 255
                imgRes[i,j,1] = 0
                imgRes[i,j,2] = 0
                #imgRes[i,j,0] = 255
                #imgRes[i,j,1] = 128
                #imgRes[i,j,2] = 0
            
            #imgRes[i,j,1] = 0
            #imgRes[i,j,2] = 255
    print("Rectangle draw ")
   # imgRes = cv2.rectangle(imgRes,(countOfCols - 50 , countOfRows  - 50),(countOfCols - 20, countOfRows - 20),(0,255,0),-1)
    imgResFinal = np.ones( ( np.array(rfMapFull).shape[0], np.array(rfMapFull).shape[1]+150, 3 ) )
    for i in range(countOfRows):
        for j in range(countOfCols):
            imgResFinal[i,j,0] = imgRes[i,j,0]
            imgResFinal[i,j,1] = imgRes[i,j,1]
            imgResFinal[i,j,2] = imgRes[i,j,2]
    #for i in range(countOfCols):
	 #   i = i + countOfCols / 6 
      #  imgRes2 = cv2.rectangle(imgRes,(countOfCols - 50 , countOfRows  - 50),(countOfCols - 20, countOfRows - 20),(0,255,0),-1)
    imgResFinal = cv2.rectangle(imgResFinal,( countOfCols + 20 , int(countOfRows/20)),(countOfCols + 50, int(2*countOfRows / 20)),(0,0,255),-1)
    cv2.putText(imgResFinal,"> -20Dbm",(countOfCols + 5 ,int(3*countOfRows / 20)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0),1)
	
    imgResFinal = cv2.rectangle(imgResFinal,( countOfCols + 20 ,int(4*countOfRows / 20) ),(countOfCols + 50,  int(5*countOfRows / 20)),(39,127,255),-1)
    cv2.putText(imgResFinal,"<= -20Dbm > -35Dbm",(countOfCols + 5 ,int(6*countOfRows / 20)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0),1)
	
    imgResFinal = cv2.rectangle(imgResFinal,( countOfCols + 20 ,int(7*countOfRows / 20) ),(countOfCols + 50,  int(8*countOfRows / 20)),(0,255,255),-1)
    cv2.putText(imgResFinal,"<= -35Dbm > -47Dbm",(countOfCols + 5 ,int(9*countOfRows / 20)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0),1)
	
    imgResFinal = cv2.rectangle(imgResFinal,( countOfCols + 20 , int(10*countOfRows / 20)),(countOfCols + 50,  int(11*countOfRows / 20)),(0,255,128),-1)
    cv2.putText(imgResFinal,"<= -47Dbm > -60Dbm",(countOfCols + 5 ,int(12*countOfRows / 20)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0),1)
	
    imgResFinal = cv2.rectangle(imgResFinal,( countOfCols + 20 , int(13*countOfRows / 20)),(countOfCols + 50,  int(14*countOfRows / 20)),(0,128,0),-1)
    cv2.putText(imgResFinal,"<= -60Dbm > -73Dbm",(countOfCols + 5 ,int(15*countOfRows / 20)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0),1)
	
    
    imgResFinal = cv2.rectangle(imgResFinal,( countOfCols + 20 , int(16*countOfRows / 20)),(countOfCols + 50,  int(17*countOfRows / 20)),(255,0,0),-1)
    cv2.putText(imgResFinal,"<= -73Dbm > -90Dbm",(countOfCols + 5 ,int(18*countOfRows / 20)),cv2.FONT_HERSHEY_COMPLEX,0.3,(0,255,0),1)  
        
    print("coloring finished ")
    return imgResFinal


#### Find reflection angle for current wave
def FindReflectionAngle(x,y,theta,rayPowerStopThresholddB,rfMap,layout,nTrace,nPixelsX,nPixelsY,poly):
   
    c= 3e8 # speed of light
       
   


    r1 = np.array([int(nPixelsX*0.1),int(nPixelsX*0.9)])
    c1 = np.array([int(nPixelsY*0.1),int(nPixelsY*0.1)])
    rr1,cc1 = polygon(poly[:,0], poly[:,1], rfMap.shape)

    val = 1

#print(layout)


# Find reflection angle

    #case 1 vertical wall North-South
    if layout[x,y-1] == 1 and layout[x,y+1]:
        return np.pi - theta
    
    #case 2 horizontal wall West-East
    if layout[x-1,y] == 1 and layout[x+1,y]:
        return 2*np.pi - theta
    
    
    #case 3 inclined wall north-east
    if layout[x-1,y-1] == 1 and layout[x+1,y+1]:
        return 1/2*np.pi - theta   
    
    #case 3 inclined wall north-west
    if layout[x-1,y+1] == 1 and layout[x+1,y-1]:
        return 3/2*np.pi - theta
    else:
        return np.pi + theta
        


#Find building entry loss

def FindEntryLoss(freq):
    buildingType = 'traditional'
    theta = 125 # Elevation angle of the path at building facade
    if buildingType == 'traditional':
        r = 12.64
        s = 3.72
        t = 0.96
        u = 9.6
        v = 2.0
        w = 9.1
        x = -3.0
        y = 4.5
        z = -2.0
    else:  # thermal efficient      
        r = 28.19
        s = -3.0
        t = 8.48
        u = 13.9
        v = 3.8
        w = 27.8
        x = -2.9
        y = 9.4
        z = -2.1
        
        
    Le = 0.212*abs(theta)
    Lh = r + s*np.log10(freq) + t*(np.log10(freq))**2
    mu1 = Lh + Le
    mu2 = w + x*np.log10(freq)
    sigma1 = u + v*np.log10(freq)
    sigma2 = y + z*np.log10(freq)
    p = 0.95
    inverse_F  = norm.ppf(p)
    A_p = mu1 + sigma1*inverse_F
    B_p = mu2 + sigma2*inverse_F
    C = -3.0
    Loss = 10*np.log10(10**(0.1*A_p) + 10**(0.1*B_p) + 10**(0.1*C))
    return Loss
        

# Find pathloss
def FindPathLoss(d, freq):
    path_loss = 43.3*np.log10(d) + 11.5 + 20 * np.log10(freq)
    #path_loss = 20*np.log10(c/(freq*1e9)/(4*3.14*dist))
    return path_loss
    

# Update power level at (x,y)
def UpdatePowerLevel(x,y, pr,rfMap,layout,nTrace):
    orignalPowerdB = rfMap[x,y]
    avgPower = (10**(orignalPowerdB/10)*nTrace[x,y] + 10**(pr/10))/(nTrace[x,y] +1)
    return 10*np.log10(avgPower)
    

# Launch a new ray    
def GenerateRay(ray,recursionIndex,x0,y0,txPrdB,theta,gain,rayPowerStopThresholddB,maxRecursionsPerRay,deltaX,deltaY,angle,xMin,xMax, yMin,yMax, freq,nPixelsX, nPixelsY,poly, rfMap,layout,nTrace):
    # Check if the maximum number of reflections/refractions has been reached
    if recursionIndex < maxRecursionsPerRay: 
        stop = False # Conntinue ray
    else:
        stop = True # Terminate ray
        
    deltaDist = np.sqrt(deltaX**2 + deltaY**2)*deltaX # ray update resolution
    dist = deltaDist

    recursionIndex = recursionIndex + 1 # update the recursion index
    PtdB = txPrdB # transmit power for this ray
    while(not stop):
        #print('dist ', dist)
        xNew = x0 + dist*np.cos(theta) # update ray position        
        yNew = y0 + dist*np.sin(theta) # update ray position
        
        # If the new ray position exceeds simulation area, then terminate this ray
        if xNew > xMax or xNew < xMin or yNew > yMax or yNew < yMin :
            stop = True
            continue
                
        # Find power level at the updated position        
        PL = FindPathLoss(dist,freq)
        PrdB = PtdB - PL        
        
        # If power level is below noise level, then terminate this ray
        if PrdB < rayPowerStopThresholddB:
            stop = True
            continue
        
        # Find the index (pixel) for the new updated ray location
        xIndex = int(max(min(np.ceil(xNew/deltaX),nPixelsX-1),0))
        yIndex = int(max(min(np.ceil(yNew/deltaY), nPixelsY-1),0))
        if layout[xIndex,yIndex] == 0: # There is no wall/obstacle
            # Update rss value
            if nTrace[xIndex,yIndex] == 0:
                rfMap[xIndex,yIndex] = PrdB
                nTrace[xIndex,yIndex] = 1 + nTrace[xIndex,yIndex]
            else:
                rfMap[xIndex,yIndex] = UpdatePowerLevel(xIndex,yIndex,PrdB,rfMap,layout,nTrace)                                
                nTrace[xIndex,yIndex] = 1 + nTrace[xIndex,yIndex]                
                dist = dist + deltaDist # update ray length
                
        else: # There is wall
            stop = True # terminate ray
         #   print('Wall!')
            
            # Find the reflection angle (we should check on wall geometry by checking adjacent pixels)
            reflectionAngle = FindReflectionAngle(xIndex,yIndex,angle,rayPowerStopThresholddB,rfMap,layout,nTrace,nPixelsX,nPixelsY,poly)
         #   print('Reflection angle', reflectionAngle)
        
            
            # Ray reflection
            reflectionLossdB = 10
            GenerateRay(ray,recursionIndex, xNew,yNew,PrdB-reflectionLossdB, reflectionAngle,0,rayPowerStopThresholddB,maxRecursionsPerRay,deltaX,deltaY,angle,xMin,xMax, yMin, yMax, freq,nPixelsX, nPixelsY, poly,rfMap,layout,nTrace)
            
            # Ray insertion
            insertionLossdB = FindEntryLoss(freq) 
            GenerateRay(ray,recursionIndex, xNew,yNew,PrdB-insertionLossdB, angle,0,rayPowerStopThresholddB,maxRecursionsPerRay,deltaX,deltaY,angle,xMin,xMax, yMin, yMax, freq,nPixelsX, nPixelsY,poly, rfMap,layout,nTrace)
####



'''
Main routine
'''           

	
##Main function for height map calculation 

def finalFunctionHeightMap():
    fileArrayTxtFile = open("myfile.txt","w")	
   # testArray = [-90,-70,-80,-60]
   # print(testArray)
    #resSINR = SINR_dbCalculationFunction(testArray)
   # print(resSINR)
   # return 0
   # rayPowerStopThresholddB  = -90 # dBm
    print("Call python function start")
    lineImageSize = ''
    splitImageSizeArray = []
    
	### Reading image size from txt file and saving into some variables.
    with open('imageSize.txt') as fp:
        lineImageSize = fp.readline()
        splitImageSize = re.findall(r'\S+', lineImageSize)
        splitImageSizeArray.append(splitImageSize)
        while lineImageSize:
            lineImageSize = fp.readline()
            splitImageSize = re.findall(r'\S+', lineImageSize)
            splitImageSizeArray.append(splitImageSize)
            print(lineImageSize)
    
    splitImageSize = re.findall(r'\S+', lineImageSize)    
    print(splitImageSize)
    ##Reading data for selected or detected walls
    tree = ET.parse('testHeightMapData.xml')
    root = tree.getroot()
    if len(root) == 0:
        print("Please select an image and points")
        return 0
		
    #Automate generated data parse 
    treeAut = ET.parse('testHeightMapDataAutoGenerated.xml')
    rootAut = treeAut.getroot()
	
	
	
    arrayFinal = []
    arrayFinalAut = []
    print("retrurn")
	##Seting some parameters connected with wave frequency, power and etc.
    xMin = 0 # 
    xMax =  int(splitImageSizeArray[0][1]) #
    rayPowerStopThresholddB  = -90
    yMin = 0
    yMax = int(splitImageSizeArray[0][0])
    freq =  4.6#1.18 # channel frequency in GHz
    nPixelsX =    int(splitImageSizeArray[0][1])#imageHeigth;#(xMax - xMin)/deltaX;
    nPixelsY =   int(splitImageSizeArray[0][0]) #imageWidth#(yMax - yMin)/deltaY;
    maxRecursionsPerRay = 1
    nRays = 3000
    deltaX = 1 #* distPerPixel # Resolution in x-axis
    deltaY = 1 #* distPerPixel # Resolution in y-axis 
   
     #Creating empty layout mat, on which we will set 1 if that is wall, and 0 if that is not wall
    layout	= np.zeros((int(nPixelsX),int(nPixelsY))) # Roof layout  	
    nTrace = np.zeros((int(nPixelsX),int(nPixelsY))) # Number of times rss computed per pixel
     ## We are observing automated detected walls and user selected walls
    for elem in rootAut:
        array1 = []
        for subelem in elem:
            array1.append(int(subelem.text))
        arrayFinalAut.append(array1)
    #Set layout corresponding pixel 1 from automaticlly detected walls
	for tmpArray in arrayFinalAut:
        layout[tmpArray[1],tmpArray[0]] = 1
		
	
	#Set layout from user selected wall
    for elem in root:
  
   # print(elem.text)
        array1 = []
        for subelem in elem:
        #array1 = []
            array1.append(int(subelem.text))
	    #array1.append(12)
      #  print(subelem.text)
        arrayFinal.append(array1)
    for tmpArray in arrayFinal:
        maxValX = tmpArray[0]
        minValX = tmpArray[2]
        maxValY = tmpArray[1]
        minValY = tmpArray[3]
        if tmpArray[1] < tmpArray[3]:
            maxValY = tmpArray[3]
            minValY = tmpArray[1]
        if tmpArray[2] > tmpArray[0]:
            maxValX = tmpArray[2]
            minValX = tmpArray[0]
        if minValX == maxValX and minValY != maxValY:
            layout[minValY:maxValY,minValX] = 1
        if minValX != maxValX and minValY == maxValY:
            layout[minValY,minValX:maxValX] = 1
        else :			
            layout[minValY:maxValY,minValX:maxValX] = 1
    
# Generate rays
	

    poly = np.array((
        (100, 100),
        (100, 200),
        (300, 300)
  #  (220, 590),
    
    ))

    #rfMap is mat which is containing  dbm values

    print("RANGE OF INDEX ")
    print((len(splitImageSizeArray)))
    resultImagesArray = []
    indexOfCurrentWroteImage = 1;
    rfMapFull = rayPowerStopThresholddB*np.ones((int(nPixelsX),int(nPixelsY)),np.int)
    imgFullSINR = rayPowerStopThresholddB*np.ones((int(nPixelsX),int(nPixelsY)),np.float)
    imgFullColored = np.zeros( ( np.array(rfMapFull).shape[0], np.array(rfMapFull).shape[1], 3 ) )
    maskImgHeight = np.zeros( ( np.array(rfMapFull).shape[0], np.array(rfMapFull).shape[1], 1 ) )
    tmpImage = np.zeros( ( np.array(rfMapFull).shape[0], np.array(rfMapFull).shape[1], 3 ) )
	
	
	 #Start wave generation and dbm mat calculation
    for indexOfArray in range(len(splitImageSizeArray)-1):
        rfMap = rayPowerStopThresholddB*np.ones((int(nPixelsX),int(nPixelsY)),np.int)
        rfMap = rayPowerStopThresholddB*np.ones((int(nPixelsX),int(nPixelsY)),np.int) # RF MAp   
        img2 = np.zeros( ( np.array(rfMap).shape[0], np.array(rfMap).shape[1], 3 ) )
        print("VERY INITIAL STATE")
        print("Inside loop ")
        
        txLocationX = int(splitImageSizeArray[indexOfArray][3])*deltaX# 1*xMax/6
        txLocationY =  int(splitImageSizeArray[indexOfArray][2])*deltaY# 2*yMax/4
        xStart = txLocationX
        yStart =  txLocationY
        txPower = 73 # dBm
        
        for rayId in range(nRays):
            angle = 2*np.pi/nRays*rayId
            gaindB = 0
            recursion = 0
            GenerateRay(rayId,recursion,xStart,yStart,txPower,angle,gaindB,rayPowerStopThresholddB,maxRecursionsPerRay,deltaX ,deltaY ,angle,xMin, xMax,yMin,yMax, freq, nPixelsX, nPixelsY,poly, rfMap,layout,nTrace)
                    		
        print("After generate ray function ")
       
    # Apply Guassian filter to rfMap
        rfMap = gaussian_filter(rfMap,sigma=1)
        rfMapFull = gaussian_filter(rfMapFull,sigma=1)
        np.savetxt('textRfMap.txt',rfMap,fmt='%.2f')
        resultImagesArray.append(rfMap)
    ##print('rfMap', rfMap)    
        tmpImage[:,:,0] = 100
        tmpImage[:,:,1] = rfMap  + np.abs(np.min(rfMap))
        tmpImage[:,:,2]  = 0


        for tmpArray in arrayFinal:   
            tmpImage = cv2.line(tmpImage, (tmpArray[0],tmpArray[1]), (tmpArray[2],tmpArray[3]), (0,0,255), 3)
            img2 = cv2.line(img2, (tmpArray[0],tmpArray[1]), (tmpArray[2],tmpArray[3]), (0,0,255), 3)
			
        for tmpArray in arrayFinalAut:   

            img2 = cv2.circle(img2,(tmpArray[0],tmpArray[1]),3,(0,0,255),-1)
            tmpImage = cv2.circle(tmpImage,(tmpArray[0],tmpArray[1]),3,(0,0,255),-1)			
			
        finalImagePath = "testfileRes" + str(indexOfCurrentWroteImage) + ".png"
        cv2.imwrite(finalImagePath,tmpImage)
        indexOfCurrentWroteImage = indexOfCurrentWroteImage + 1


    colorArray = []
    print("Before call function coloring areas")

    for j in range(len(resultImagesArray)):
        color = list(np.random.choice(range(256), size=3))
        colorArray.append(color)
        print("CURRENT COLOR")
    
    maxElSINR = -100000
    minElSINR = 100000
    maxDBMValue = -90	
    countOfPositiveValues = 0    	
    print("After new function ")
    file2write=open("rfMapFull.txt",'w')
    fileSINRData = open("sinrData.txt",'w')
#file2write.write("here goes the data")

    for i in range(nPixelsX):
        for j in range(nPixelsY):
        
             
            if rfMap[i,j] > 0 :
                countOfPositiveValues = countOfPositiveValues +1           
            arrayOfCompValues=[]
            for k in range(len(resultImagesArray)):
                arrayOfCompValues.append(resultImagesArray[k][i,j])
              
			
            maxValueColorIndex  =  findMaximumValueIndexFromArray(arrayOfCompValues)
            rfMapFull[i,j] = resultImagesArray[maxValueColorIndex][i,j]
            if rfMapFull[i,j] >= -70:
                maskImgHeight[i,j] = 255
            file2write.write(str(rfMapFull[i,j]))
            file2write.write(" ")
     
            sinrDbmValue = SINR_dbCalculationFunction(arrayOfCompValues)
            fileSINRData.write(str(sinrDbmValue))
            fileSINRData.write(" ")
           # print("after SINR function call")
            if sinrDbmValue < minElSINR:
                minElSINR = sinrDbmValue
            if sinrDbmValue > maxElSINR:
                maxElSINR = sinrDbmValue
            imgFullSINR[i,j] = sinrDbmValue
          #  fileArrayTxtFile.write(str(imgFullSINR[i,j]))
            fileArrayTxtFile.write(" ")
       
          
            if maxValueColorIndex == -1:
                color = (0,0,0)
            else:			
                color = colorArray[maxValueColorIndex]
   
            imgFullColored[i,j,0] = color[0]
            imgFullColored[i,j,1] = color[1]
            imgFullColored[i,j,2] = color[2]
        file2write.write("\n")
        fileSINRData.write("\n")
       # fileArrayTxtFile.write("\n")
    coloredAreasImage = coloringOfSignalAreas(rfMapFull, nPixelsX, nPixelsY)
    file2write.close()
    fileSINRData.close()
   # sio.savemat("resHeightMap.mat", {"proj": rfMapFull})
    #save('resHeightMap.mat', 'rfMapFull')
    print("Count of positive values ")
    print(countOfPositiveValues)
    maxDBMValue =  findMaximumElementOfMat(rfMapFull,nPixelsX,nPixelsY)
    print(maxDBMValue)
    img2[:,:,0] = 100
    img2[:,:,1] = rfMapFull  + np.abs(np.min(rfMapFull))
    img2[:,:,2] = 0	   
    for tmpArray in arrayFinal:   
        img2 = cv2.line(img2, (tmpArray[0],tmpArray[1]), (tmpArray[2],tmpArray[3]), (0,0,255), 3)
		
		
    for tmpArray in arrayFinalAut:   
        img2 = cv2.circle(img2,(tmpArray[0],tmpArray[1]),3,(0,0,255),-1)
        			
			
    print("After full SINR work")
    fileArrayTxtFile.write("Max El DBM \n")
    print("after writing first string SINR\n")
    print(maxDBMValue)

    print("After  max SINR element writing into file")   
    fileArrayTxtFile.close()
############Drawing of axis points on result images#############
    print("Before sinr function coloring call ")
    imgResSINR = createColoredImageWithSINR(imgFullSINR,nPixelsX,nPixelsY)
    print("After sinr function coloring call")
    for indexOfArray in range(len(splitImageSizeArray)-1):        
        txLocationX = int(splitImageSizeArray[indexOfArray][3])*deltaX# 1*xMax/6
        txLocationY =  int(splitImageSizeArray[indexOfArray][2])*deltaY
        print("Circle point coordinate")
        print(txLocationX)
        print(txLocationY)		
        cv2.circle(coloredAreasImage,(int(txLocationY)*4,int(txLocationX)*4),15,(0,255,0),-1)        		
        cv2.circle(imgFullColored,(int(txLocationY)*4,int(txLocationX)*4),15,(0,255,0),-1)
        currentAxisPointText = "AP" + str(indexOfArray)
        if  txLocationX*4 -20  > 0:
            print("Inside if ")		
            cv2.putText(imgFullColored,currentAxisPointText,(int(txLocationY)*4 ,int(txLocationX)*4  -20),cv2.FONT_HERSHEY_SIMPLEX,3.5,(0,255,0),cv2.LINE_AA)		
    cv2.imwrite("resultImageFull.png",img2)
    cv2.imwrite("coloredAreasImage.png",coloredAreasImage)
    cv2.imwrite("resultSINRImage.png",imgResSINR)
    cv2.imwrite("maskImgHeight.png",maskImgHeight)
    print("Finall array size with rfMap")
    print(len(resultImagesArray))
    y = np.linspace(xMin,xMax,nPixelsX)
    x = np.linspace(yMin,yMax,nPixelsY)
    cv2.imwrite("ColoredImage.png",imgFullColored)
    X,Y = np.meshgrid(x,y)

   
    return img2	
	
	
#if __name__== "__main__":
 #   print("Hello World")
 #   finalFunctionHeightMap(800,600)
	
	
	
	


