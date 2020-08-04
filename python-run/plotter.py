import cv2
import numpy as np

events = [i for i in dir(cv2) if 'EVENT' in i]
# Default values for drawing in window
drawing = False # true when mouse down
mode = True # may change this frome T/F to an item from a list depends what is needed
ix,iy = -1,-1 # initial mouse down coord for rects 

def animPlot(positions, radii, params, res = (1280, 780)):
    animFrame = np.ones([res[0], int(res[0]*params.Lx/params.Ly), 3])
    #print(animFrame)
    for (position, radius) in zip(positions, radii): 
        pixel = convertPoint2Pixel(position, (params.Lx, params.Ly), res)
        #print(radius)
        radius = int(20*radius)
        #radius, _ = convertPoint2Pixel((radius,0), (params.Lx, params.Ly), res)
        cv2.circle(animFrame, pixel, radius, (0,0,1))
    
    cv2.imshow('Simulation', animFrame)
    cv2.namedWindow('Simulation', cv2.WINDOW_AUTOSIZE)
    cv2.setWindowProperty('Simulation', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    return None
    
def convertPoint2Pixel(pos, L, res):
    Lx,Ly = L
    px = pos[0]*res[0]/(Lx) + res[0]/2
    py = pos[1]*res[0]*Ly/(Lx*Lx) + res[0]*Ly/(2*Lx)
    return (int(px), int(py))

def checkPause(pauseKey):
    key = cv2.waitKey(1) & 0xFF
    if key == ord(pauseKey):
        cv2.waitKey(0)
        
def cleanExit(exitKey):
    key = cv2.waitKey(1) & 0xFF
    if key == ord(exitKey):
        return True
    return False
        
def endAnimation():
    cv2.destroyAllWindows()
    
def drawInWindow(event, x, y, flags, params):
    global ix, iy, drawing, mode, img
    if event == cv2.EVENT_LBUTTONDOWN:
        drawing = True
        ix, iy = x, y
    elif event == cv2.EVENT_MOUSEMOVE:
        if drawing == True:
            pass
            #shapes(mode, img, (ix, iy), (x, y))
    elif event == cv2.EVENT_LBUTTONUP:
        drawing = False
        shapes(mode, img, (ix, iy), (x, y))
    
def shapes(mode, img, ixy, xy):
    ix, iy = ixy
    x, y = xy
    if mode == True:
        cv2.rectangle(img, (ix,iy), (x,y), (0,255,0), -1)
    else:
        dx, dy = (ix - x), (iy - y)
        r = int(np.sqrt(dx*dx + dy*dy))
        cv2.circle(img, (ix,iy), r, (0,0,255), -1)
        
def testDrawWindow(windowName):
    global mode, img
    img = np.zeros((512,512,3), np.uint8) 
    cv2.namedWindow(windowName)
    cv2.setMouseCallback(windowName, drawInWindow)
    print("Entered")
    while(1):
        cv2.imshow(windowName, img)
        key = cv2.waitKey(1) & 0xFF
        if key == ord('m'):
            mode = not mode
        elif key == 32:
            print("exit")
            break