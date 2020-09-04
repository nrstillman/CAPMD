#cell_detect2.py
import json
import numpy as np
from tqdm import tqdm
import skimage.io as skio
import matplotlib.pyplot as plt
import skimage.filters as skfilt
import skimage.measure as skmeas
#from btrack.dataio import ObjectFactory

#import raw video to detect cells
imstack_marker = skio.imread("cell_movies/marker.tif", plugin="tifffile")
#imstack_gray   = skio.imread("cell_movies/phase.tif", plugin="tifffile")

#all_cells = np.zeros((4,0))
cell_id = 0
cell_dict = {}
num_frames = imstack_marker.shape[0]
# cycle through all frames
for i in tqdm(range(num_frames)):
    # create a mask using the marker layer
    blurred_marker = skfilt.gaussian(imstack_marker[i])#, sigma = (9,9))
    threshold_value = skfilt.threshold_otsu(blurred_marker)
    mask = blurred_marker > threshold_value
    
    # Use mask to separate objects (i.e. cells) to find x,y centroids
    object_labels = skmeas.label(mask)
    object_properties = skmeas.regionprops(object_labels)
    centroids = [p.centroid for p in object_properties]
    #print(np.array(centroids))
    # Format in t x y z format ready for ObjectFactory
    num_cells = len(centroids)
    coords = np.zeros((num_cells,4))
    coords[:,0] = i
    coords[:,1:3] = np.array(centroids)
    
    # get max and min value of area
    # only take the top 95% biggest as the chance is that the small centroids are mistakes from noise
    areas = [p.area for p in object_properties]
    max_area = max(areas)
    min_area = min(areas)
    area_range = max_area - min_area
    area95 = min_area + area_range*0.05
    
    # Create a dictionary ready in json format 
    for cell in range(coords.shape[0]):
        if object_properties[cell].area > area95:
            string_name = "Object_{}".format(cell_id)
            cell_dict[string_name] = {}
            cell_dict[string_name]['t'] = i
            cell_dict[string_name]['x'] = coords[cell,1]
            cell_dict[string_name]['y'] = coords[cell,2]
            cell_dict[string_name]['z'] = coords[cell,3]
            cell_dict[string_name]['states'] = 2
            cell_dict[string_name]['label'] = 0
            
            cell_id += 1
    #objects_to_track = [ObjectFactory.get(txyz) for txyz in coords]
    # add coords to the final array
    #all_cells = np.vstack((all_cells, coords))

# Return final dict as a json file for future use
out_file = open("cell_track_image.json", "w")
json.dump(cell_dict, out_file)
out_file.close()