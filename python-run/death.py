#death.py

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
    return (c)

def is_cell_in_rect(centre, rect):
    """
    Simplified version of is_point_in_path which accepts a rect defined by two points
    """
    xy00, xy11 = rect
    xy10 = [xy11[0], xy00[1]]
    xy01 = [xy00[0], xy11[1]]
    polygon = [xy00, xy01, xy11, xy10]
    return is_point_in_path(centre[0], centre[1], polygon)

def kill_cell_in_rect(cell_pos, cell_id, rect, interface):
    """
    uses the interface to kill any cell that returns true when cell centre is within rect
    returns 1 if kill
    else 0
    """
    collision = is_cell_in_rect(cell_pos, rect)
    #print(collision)
    if collision:
        #print("Killing cell with id: ", cell_id)
        interface.killCell(cell_id)
        return 1
    return 0
   

def kill_cells_in_rect(centres, cell_ids, rect, interface):
    collisions = [] # note which IDs collide
    for (centre, cell_id) in zip(centres, cell_ids):
        coll = is_cell_in_rect(centre, rect)
        if coll:
            collisions.append(cell_id)
    if len(collisions) > 0:
        interface.killCells(collisions)
    
""" Add to end of loop
    for idx in index:
        #part_ids.append(sim.getParticle(idx))
        part = sim.getParticle(idx)
        print(part, "Killed")
        #death.kill_cell_in_rect(part, rect, interface)
"""
""" Add to bindings
    py::class_<Interface>(m, "interface")
        .def(py::init<Parameters>())
        .def("killCell", &Interface::killCell)
        .def("trackCell", &Interface::trackCell); 
"""