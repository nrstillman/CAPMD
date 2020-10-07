import cv2
import numpy as np
import collision

events = [i for i in dir(cv2) if 'EVENT' in i]
# Default values for drawing in window
drawing = False # true when mouse down
mode = True # may change this frome T/F to an item from a list depends what is needed
ix,iy = -1,-1 # initial mouse down coord for rects 

def animPlot(positions, radii, part_ids, params, res = (1280, 780), rect = None):
    animFrame = np.ones([res[0], int(res[0]*params.Lx/params.Ly), 3])
    #print(animFrame)
    kill_list = []
    if rect is not None:
        cv2.rectangle(animFrame, (rect[0,0],rect[0,1]), (rect[1,0],rect[1,1]), (255,255,0), -1)
    for (position, radius, ids) in zip(positions, radii, part_ids): 
        coll = 0
        pixel = convertPoint2Pixel(position, (params.Lx, params.Ly), res)
        #print(radius)
        radius = int(20*radius)
        if rect is not None:
            (coll,_) = collision.circle_intersect_simple_rect(rect, (pixel,radius))
        #radius, _ = convertPoint2Pixel((radius,0), (params.Lx, params.Ly), res)
        cv2.circle(animFrame, pixel, radius, (0,coll,1))
        cv2.putText(animFrame, str(ids), pixel, cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0,0,0))
        if coll:
            kill_list.append(ids)
        
    cv2.imshow('Simulation', animFrame)
    cv2.namedWindow('Simulation', cv2.WINDOW_AUTOSIZE)
    cv2.setWindowProperty('Simulation', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    return kill_list
    
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

def testIntersection():
    
    size = 1000 
    circs = 500
    animFrame = np.ones([size, size, 3])
    rect = np.random.randint(0,size,(2,2))
    cv2.rectangle(animFrame, (rect[0,0],rect[0,1]), (rect[1,0],rect[1,1]), (255,255,0), -1)
    for _ in range(circs):
        pxls = np.random.randint(5,size-5,2)
        pxls = (pxls[0], pxls[1])
        r = np.random.randint(5,25)
        (coll,var) = collision.circle_intersect_simple_rect(rect, (pxls,r))
        if var is None:
            color = (0,0,1)
        else:
            color = (1,0,1)
        """
        elif var == 0:
            color = (1,0,0) # g
        elif var == 1:
            color = (0,1,0) # b
        elif var == 2:
            color = (1,0,1) # y
        elif var == 3:
            color = (1,1,1) # b
        elif var == 5:
            color = (0,0,0) # w
        """
                
        cv2.circle(animFrame, pxls, r, color, 5)
    cv2.imshow('Test_Simulation', animFrame)
    cv2.namedWindow('Test_Simulation', cv2.WINDOW_AUTOSIZE)
    cv2.setWindowProperty('Test_Simulation', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    
#testIntersection()