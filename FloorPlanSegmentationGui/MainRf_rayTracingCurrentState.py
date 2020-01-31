# -*- coding: utf-8 -*-
"""
Ray Tracing for wireless channel propogation

@author: Mohammed Hirzallah, 2019
"""




import numpy as np
import math
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
from scipy.ndimage import gaussian_filter
from scipy.stats import norm


import scipy.misc
from skimage.draw import line_aa
from skimage.draw import polygon
from PIL import Image
import cv2


def FindReflectionAngle(x,y,theta,rayPowerStopThresholddB,rfMap,layout,nTrace,nPixelsX,nPixelsY,poly):
   
    c= 3e8 # speed of light
       
   
    #rayPowerStopThresholddB  = -90 # dBm
 

   



   # img = np.zeros((int(nPixelsX),int(nPixelsY)))


# fill polygon
#    poly = np.array((  (100, 100),    (100, 200),    (300, 300)))
 #   rr, cc = polygon(poly[:,0], poly[:,1], img.shape)
#img[rr,cc] = 1
#plt.imshow(img)
#plt.show()



#rr, cc, val = line_aa(int(nPixelsX*0.1),int(nPixelsY*0.1), int(nPixelsX*0.9),  int(nPixelsY*0.1))
    r1 = np.array([int(nPixelsX*0.1),int(nPixelsX*0.9)])
    c1 = np.array([int(nPixelsY*0.1),int(nPixelsY*0.1)])


#poly = np.array((
#    (10, 10),
#    (20, 20),
#	(30,30)
#))

    rr1,cc1 = polygon(poly[:,0], poly[:,1], rfMap.shape)

    val = 1






##################OUR ADDED 



#ax.vlines(x=20, ymin=200, ymax=350, linewidth=2, color='r')
#ax.hlines(y=350, xmin=20, xmax=100, linewidth=2, color='r')
#ax.vlines(x=100, ymin=350, ymax=550, linewidth=2, color='r')
#ax.hlines(y=550, xmin=100, xmax=500, linewidth=2, color='r')
#ax.vlines(x=500, ymin=400, ymax=550, linewidth=2, color='r')



















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
        
    deltaDist = np.sqrt(deltaX**2 + deltaY**2)*0.25 # ray update resolution
    dist = deltaDist
    print('ray ', ray)
    print('txPower', txPrdB)
    print('angle ', angle*180/np.pi)
    print('x0 ', x0)
    print('y0 ', y0)
    
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
            print('Wall!')
            
            # Find the reflection angle (we should check on wall geometry by checking adjacent pixels)
            reflectionAngle = FindReflectionAngle(xIndex,yIndex,angle,rayPowerStopThresholddB,rfMap,layout,nTrace,nPixelsX,nPixelsY,poly)
            print('Reflection angle', reflectionAngle)
        
            
            # Ray reflection
            reflectionLossdB = 10
            GenerateRay(ray,recursionIndex, xNew,yNew,PrdB-reflectionLossdB, reflectionAngle,0,rayPowerStopThresholddB,maxRecursionsPerRay,deltaX,deltaY,angle,xMin,xMax, yMin, yMax, freq,nPixelsX, nPixelsY, poly,rfMap,layout,nTrace)
            
            # Ray insertion
            insertionLossdB = FindEntryLoss(freq) 
            GenerateRay(ray,recursionIndex, xNew,yNew,PrdB-insertionLossdB, angle,0,rayPowerStopThresholddB,maxRecursionsPerRay,deltaX,deltaY,angle,xMin,xMax, yMin, yMax, freq,nPixelsX, nPixelsY,poly, rfMap,layout,nTrace)




'''
Main routine
'''           
            
def finalFunctionHeightMap():			
   # rayPowerStopThresholddB  = -90 # dBm
 
# Generate rays
    xMin = 0 # 
    xMax = 150 #
    rayPowerStopThresholddB  = -150
    yMin = 0
    yMax = 200
    freq = 1.18 # channel frequency in GHz
    nPixelsX = 600;#(xMax - xMin)/deltaX;
    nPixelsY =  800#(yMax - yMin)/deltaY;
    maxRecursionsPerRay = 3
    nRays = 360
    deltaX = .25 # Resolution in x-axis
    deltaY = .25 # Resolution in y-axis 
 
 
			
    rfMap = rayPowerStopThresholddB*np.ones((int(nPixelsX),int(nPixelsY)),np.int) # RF MAp   
    layout	= np.zeros((int(nPixelsX),int(nPixelsY))) # Roof layout
    nTrace = np.zeros((int(nPixelsX),int(nPixelsY))) # Number of times rss computed per pixel	

    img2 = np.zeros( ( np.array(rfMap).shape[0], np.array(rfMap).shape[1], 3 ) )	

    poly = np.array((
        (100, 100),
        (100, 200),
        (300, 300)
  #  (220, 590),
    
    ))
    rr, cc = polygon(poly[:,0], poly[:,1], layout.shape)
    r1 = np.array([int(nPixelsX*0.1),int(nPixelsX*0.9)])
    c1 = np.array([int(nPixelsY*0.1),int(nPixelsY*0.1)])


    rr1,cc1 = polygon(poly[:,0], poly[:,1], rfMap.shape)


  #  layout[rr1,cc1] = 1

    layout[50,100:300] = 1
    layout[50,400:600] = 1
    layout[50:300,100] = 1




    layout[50:80,300] = 1
    layout[120:180,300] = 1
    layout[220:300,300] = 1
    layout[380:400,300] = 1
    layout[300,100:180] = 1
    layout[300,220:300] = 1




    layout[200,550:600] = 1
    layout[400,470:610] = 1
    layout[400,300:320] = 1
    layout[200,20:100] = 1




    layout[200:350,20] = 1
    layout[350,20:100] = 1
    layout[350:550,100] = 1
    layout[550,100:500] = 1
    layout[400:550,500] = 1
    layout[50:400,600] = 1



    txLocationX = 2*xMax/3
    txLocationY = 1*yMax/4
    xStart = txLocationX
    yStart =  txLocationY
    txPower = 73 # dBm
    for rayId in range(nRays):
        angle = 2*np.pi/nRays*rayId
        gaindB = 0
        recursion = 0
        GenerateRay(rayId,recursion,xStart,yStart,txPower,angle,gaindB,rayPowerStopThresholddB,maxRecursionsPerRay,deltaX ,deltaY ,angle,xMin, xMax,yMin,yMax, freq, nPixelsX, nPixelsY,poly, rfMap,layout,nTrace)            


    # Apply Guassian filter to rfMap
    rfMap = gaussian_filter(rfMap,sigma=1)
  
    print('rfMap', rfMap)    



#'''
#Ploting results
#'''


 ###   plt.figure(1)
#plt.imshow(layout, cmap='viridis')
#plt.colorbar()
  ###  plt.savefig('layout.png')


   ##### fig, ax = plt.subplots()
#ax.plot(x, y)
  ####  ax.hlines(y=50, xmin=100, xmax=300, linewidth=2, color='r')
  ###  ax.hlines(y=50, xmin=400, xmax=600, linewidth=2, color='r')
  ####  ax.vlines(x=600, ymin=50, ymax=400, linewidth=2, color='r')
  ####  ax.vlines(x=100, ymin=50, ymax=300, linewidth=2, color='r')

  ####  ax.vlines(x=300, ymin=50, ymax=80, linewidth=2, color='r')
  #####  ax.vlines(x=300, ymin=120, ymax=180, linewidth=2, color='r')
#####    ax.vlines(x=300, ymin=220, ymax=300, linewidth=2, color='r')
  #####  ax.vlines(x=300, ymin=380, ymax=400, linewidth=2, color='r')
  ####  ax.hlines(y=300, xmin=100, xmax=180, linewidth=2, color='r')
  #######  ax.hlines(y=300, xmin=220, xmax=300, linewidth=2, color='r')


  #########  ax.hlines(y=200, xmin=550, xmax=600, linewidth=2, color='r')

  #######  ax.hlines(y=400, xmin=470, xmax=600, linewidth=2, color='r')
  ######  ax.hlines(y=400, xmin=300, xmax=320, linewidth=2, color='r')


  #######  ax.hlines(y=200, xmin=20, xmax=100, linewidth=2, color='r')
  ####  ax.vlines(x=20, ymin=200, ymax=350, linewidth=2, color='r')
   ########3 ax.hlines(y=350, xmin=20, xmax=100, linewidth=2, color='r')


  #####  ax.vlines(x=100, ymin=350, ymax=550, linewidth=2, color='r')
 #####   ax.hlines(y=550, xmin=100, xmax=500, linewidth=2, color='r')
  #########  ax.vlines(x=500, ymin=400, ymax=550, linewidth=2, color='r')






 ###   plt.figure(2)
 ###   plt.imshow(nTrace, cmap='viridis')
###    plt.colorbar()


    
#plt.figure(3)
#rfMap[rr,cc] = val
   # rfMap[rr1,cc1] = 1
    print('Last Value')
    #print(rfMap[100])
    print(int(txLocationX))
    print(int(txLocationY))    	
  #####  plt.imshow(rfMap, cmap='viridis')
 #   plt.colorbar()
  ####  plt.savefig('RfMap.png')
    img2[:,:,0] = 255
    img2[:,:,1] = rfMap + 255
    img2[:,:,2] = 0
   # img2[int(txLocationX),int(txLocationY),0] = 0
   # img2[int(txLocationX),int(txLocationY),0] = 255
   # img2[int(txLocationX),int(txLocationY),0] = 0
   # vis2 = cv2.CreateMat(int(nPixelsX),int(nPixelsY), cv.CV_32FC3)
   # vis0 = cv2.fromarray(rfMap)
   # vis2 = cv2.cvtColor(rfMap, cv2.COLOR_GRAY2BGR)
	
   # img2[50,100:300,0] = 0
   # img2[50,100:300,1] = 0
   # img2[50,100:300,2] = 255
 #   cv2.circle(img2,(200,300), 5, (0,255,0), -1)   
  #  img2 = cv2.line(img2, (100,50), (300,50), (0,0,255), 3) 
   # img2[:,:,4] = rfMap + 255	
   # rfMap = cv.convertScaleAbs(rfMap, alpha=(255.0))
   # image = cv2.cvtColor(rfMap,cv2.COLOR_GRAY2BGR)
    #img = cv2.convertScaleAbs(rfMap, alpha=(255.0))
    #outputImg8U = cv2.convertScaleAbs(rfMap, alpha=(255.0/65535.0))
    cv2.imwrite('testfileRes.png',img2)


  ####  fig4 = plt.figure(4)
    y = np.linspace(xMin,xMax,nPixelsX)
    x = np.linspace(yMin,yMax,nPixelsY)
    X,Y = np.meshgrid(x,y)
  ###  ax = plt.axes(projection='3d')
 ###   ax.set_xlabel('x')
 ###   ax.set_ylabel('y')
  ###  ax.set_zlabel('RF power (dBm)')
  ###  ax.plot_surface(Y,X,rfMap, cmap='viridis')
  #########  ax.view_init(60,30)
    return img2	
	
	
#if __name__== "__main__":
##    print("Hello World")
  #  finalFunctionHeightMap()
	
	
	
	


