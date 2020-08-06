# -*- coding: utf-8 -*-
"""
Created on Fri Jul 31 16:09:13 2020

@author: Owner
"""
import math

def is_line_intersect_circle(line, circle):
    """Determine a line crosses the circumference of a circle.

    Args:
      Line -- a list of 2 points 
      circle -- a list containing the centre point and the radius of the circle
                  ((x,y), r)

    Returns:
      True if the centre is within the rect.
    """
    p1, p2 = line
    ax, ay = p1
    bx, by = p2
    centre, r = circle
    cx, cy = centre
    ax -= cx
    ay -= cy
    bx -= cx
    by -= cy
    
    a = (bx - ax)^2 + (by - ay)^2
    b = 2*(ax*(bx - ax) + ay*(by - ay))
    c = ax^2 + ay^2 - r^2
    disc = b^2 - 4*a*c
    if(disc <= 0 or a == 0): #dunno about a = 0 got to check on the cause of that
        return False
    sqrtdisc = math.sqrt(disc)
    t1 = (-b + sqrtdisc)/(2*a)
    t2 = (-b - sqrtdisc)/(2*a)
    if((0 < t1 and t1 < 1) or (0 < t2 and t2 < 1)):
        return True
    return False

def is_line_intersect_circle_2(line, circle):
    ((Ax,Ay),(Bx,By)) = line
    ((Cx,Cy),R) = circle
    # compute the euclidean distance between A and B
    LAB = math.sqrt( (Bx-Ax)**2+(By-Ay)**2 )
    
    # compute the direction vector D from A to B
    Dx = (Bx-Ax)/LAB
    Dy = (By-Ay)/LAB
    
    # the equation of the line AB is x = Dx*t + Ax, y = Dy*t + Ay with 0 <= t <= LAB.
    
    # compute the distance between the points A and E, where
    # E is the point of AB closest the circle center (Cx, Cy)
    t = Dx*(Cx-Ax) + Dy*(Cy-Ay)    
    
    # compute the coordinates of the point E
    Ex = t*Dx+Ax
    Ey = t*Dy+Ay
    
    # compute the euclidean distance between E and C
    LEC = math.sqrt((Ex-Cx)**2+(Ey-Cy)**2)
    
    # test if the line intersects the circle
    if( LEC <= R ) and 0 <= t and t <= LAB: return True
    return False
        
        
        
def is_point_in_path(x, y, poly) -> bool:
    """Determine if the point is in the path.
    Raytracing method to see if point is within a polygon

    Args:
      x -- The x coordinates of point.
      y -- The y coordinates of point.
      poly -- a list of tuples [(x, y), (x, y), ...]

    Returns:
      True if the point is in the path.
    """
    num = len(poly)
    i = 0
    j = num - 1
    c = False
    for i in range(num):
        if ((poly[i][1] > y) != (poly[j][1] > y)) and \
                (x < poly[i][0] + (poly[j][0] - poly[i][0]) * (y - poly[i][1]) /
                                  (poly[j][1] - poly[i][1])):
            c = not c
        j = i
    return (c, 5)

def circle_intersection(circle, polygon):
    """ determines whether a circle is interseting with a polygon
    first checks if the centre of the circle is within the polygon
    then loops through the polygons sides to check if the lines intersect
    
    Args: 
        circle -- ((x,y), r) a list of the centre coord and the radius
        polygon -- a list of x,y coordinates
        
    returns:
        True if collision
    """
    centre = circle[0]
    intersect, col = is_point_in_path(centre[0], centre[1], polygon)
    if intersect: return (intersect, col)
    num = len(polygon)
    i = 0
    
    for i in range(num):
        j = i - 1
        line = (list(polygon[i]), list(polygon[j]))
        #print(line)
        intersect = is_line_intersect_circle_2(line, circle)
        if intersect: return (intersect,i)
    return (intersect, None)
        
def circle_intersect_simple_rect(rect, circ):
    """ determines whether a simple rect (one describe by 2 coords)
        intersects a circle
        
    Args:
        rect -- [p1, p2] 
        circle -- ((x, y), r)
        
    returns:
        True if collision
    """
    xy00, xy11 = rect
    xy10 = [xy11[0], xy00[1]]
    xy01 = [xy00[0], xy11[1]]
    polygon = [xy00, xy01, xy11, xy10]
    #print(polygon)
    return circle_intersection(circ, polygon)
    
    