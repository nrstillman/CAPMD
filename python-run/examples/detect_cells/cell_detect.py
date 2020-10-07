#cell_detect.py
# FIND CELLS USING MASK IN MARKER FRAMES
# 2 methods
# - take original frame and mask frames
# - locate all the cells in each frame and record th XYZs using 

# OLD AIMS
# Step 1 Detect location of cells
# - Step 1 Import Images
# - Step 2 Create Mask
#   - Step 1 use gaussian blur to make more stead threshold
#   - Step 2 use Otsu to find threshold value of mask (or 45 is still a good value for orig dataset)
#   - Step 3 use mask to create labels to identify independant cells
# - Step 3 
# Step 2 Detect location between frames to allow tracking

import skimage.io as skio
import matplotlib.pyplot as plt
import skimage.filters as skfilt
import skimage.measure as skmeas
#import skimage.viewer as skview
#import time
frame = 10
imstack_napari = skio.imread("./test_napari.tiff", plugin="tifffile")
print(imstack_napari.shape)


imstack_marker = skio.imread("cell_movies/marker.tif", plugin="tifffile")
print(imstack_marker.shape)
exit()
imstack_gray   = skio.imread("cell_movies/phase.tif", plugin="tifffile")
#viewer = skview.ImageViewer(imstack_marker[0] > 100)
#viewer.show()
"""
# Overlaying orignal frame and marker
plt.imshow(imstack_gray[frame], cmap="gray")
plt.imshow(imstack_marker[frame], alpha = 0.6)
plt.show()
"""
# Overlaying binary mask made by markers 
#mask = imstack_marker[frame] > 45 # <- manual input of mask
blurred_marker = skfilt.gaussian(imstack_marker[frame])#, sigma = (9,9))
threshold_value = skfilt.threshold_otsu(blurred_marker)
mask = blurred_marker > threshold_value
"""
# mask created by phase image # <- not useful to take the mean of a phase
blurred_marker = skfilt.gaussian(imstack_gray[frame])#, sigma = (9,9))
threshold_value = skfilt.threshold_otsu(blurred_marker)
mask_gray = blurred_marker > threshold_value
"""

object_labels = skmeas.label(mask)
object_properties = skmeas.regionprops(object_labels)
centroids = [p.centroids for p in object_properties]

plt.figure()
plt.imshow(imstack_gray[frame], cmap="gray")
plt.imshow(mask, alpha = 0.6)
plt.show()
plt.figure()
plt.imshow(imstack_gray[frame], cmap="gray")
plt.imshow(mask_gray, alpha = 0.6)
plt.show()
"""
plt.figure()
plt.scatter(areas, eccentricity)
plt.show()
"""

