"""
find border of cell wound
method used - alpha shape
requirements - scipy, numpy
source
https://stackoverflow.com/questions/23073170/calculate-bounding-polygon-of-alpha-shape-from-the-delaunay-triangulation
"""

from scipy.spatial import Delaunay
import numpy as np


def alpha_shape(points, alpha, only_outer=True):
    """
    compute rhe alpha shape of a set of points
    inputs:
        - points :: np array of (n,2) points
        - alpha :: float alpha value 
        - only_outer :: a bool to determin whether the internal edges are drawn
    """
    
    def add_edge(edges, i, j): # <- function within the function to abuse the abilities of python sets
        """
        Add a line between the i-th and j-th points,
        if not in the list already
        """
        if (i, j) in edges or (j, i) in edges:
            # already added
            assert (j, i) in edges, "Can't go twice over same directed edge right?"
            if only_outer:
                # if both neighboring triangles are in shape, it's not a boundary edge
                edges.remove((j, i))
            return
        edges.add((i, j))

    tri = Delaunay(points) # <- creates a list of points that form triangles 
    edges = set()
    
    for ia, ib, ic in tri.vertices: # <- separates the tris into the three points
        pa = np.array(points[ia])
        pb = np.array(points[ib])
        pc = np.array(points[ic])
        
        # Computes the radius of the triangles circumcircle
        a = np.linalg.norm(pa-pb)
        b = np.linalg.norm(pb-pc)
        c = np.linalg.norm(pc-pa)
        s = 0.5 * (a + b + c)
        area = np.sqrt(s * (s - a) * (s - b) * (s - c))
        radius = a * b * c / (4 * area)
        if radius < alpha:
            add_edge(edges, ia, ib)
            add_edge(edges, ib, ic)
            add_edge(edges, ic, ia)
    return edges 
    
if __name__ == "__main__":        
    import matplotlib.pyplot as plt

    # Constructing the input point data
    np.random.seed(0)
    x = 3.0 * np.random.rand(2000)
    y = 2.0 * np.random.rand(2000) - 1.0
    inside = ((x ** 2 + y ** 2 > 1.0) & ((x - 3) ** 2 + y ** 2 > 1.0) & ((x - 1.5) ** 2 + y ** 2 > 0.09))
    points = np.vstack([x[inside], y[inside]]).T

    # Computing the alpha shape
    edges = alpha_shape(points, alpha=0.25, only_outer=True)

    # Plotting the output
    plt.figure()
    plt.axis('equal')
    plt.plot(points[:, 0], points[:, 1], '.')
    for i, j in edges:
        plt.plot(points[[i, j], 0], points[[i, j], 1])
    plt.show()