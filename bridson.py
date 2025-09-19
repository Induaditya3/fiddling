'''
My implementation of the Bridson Poisson Disk Algorithm
    dimension is 2D (assumed)
    area is rectangular of length x breadth
'''
import math
import random
import matplotlib.pyplot as plt

def insert_point(grid: list[list[tuple[float,float]]], point: tuple[float,float], cell_size: float) -> None:
    """
    For inserting into 2d grid
    
    Parameters:
        grid (list[list[tuple[float,float]]]) : The grid where the point is inserted
        point (tuple[float,float]) : The point to be inserted
        cell_size (float) : previously calculated r/sqrt(N)
        
    Returns:
        None : It modifies the original grid argument
    """
    col : int = int(point[0] / cell_size)
    row : int = int(point[1] / cell_size)
    grid[row][col] = point

# calulate the distance between two points i.e. two tuples of size 2
distance = lambda p1, p2 : math.sqrt( (p1[0] - p2[0])**2 + (p1[1] - p2[1])**2 )

def is_valid_point(point: tuple[float,float], grid: list[list[tuple[float,float]]], length: int, breadth:int,
                   no_cells_horizontal : int, no_cells_vertical:int, cell_size: float, radius:float) -> bool:
    """
    For checking the point satisfies the contraint
        - point is within distance r of existing samples (using the background grid to only test nearby samples)

    Parameters:
        point (tuple[float,float]) : The point to check 
        grid (list[list[tuple[float,float]]]) : The grid for checking distance between point in the adjacent cells
        length (int): Length of the rectangle where points are sampled
        breadth (int): Breadth of the rectangle where points are sampled
        no_cells_horizontal (int): No. of cells horizontally
        no_cells_vertical (int): No. of cells vertically
        cell_size (float): Size of cell of the grid
        radius (float): Minimum distance between each samples

    Returns:
        bool : constraint is satisfied 
    """
    # check if the point is within the range of length and breadth
    if not (0 < point[0] < length and 0 < point[1] < breadth):
        return False
    
    # check with each of the adjacent cells
    # no of the cells to compare is 8 max - but could be in less in edge cases, literally ;)
    col :int = int(point[0] / cell_size) # these are indices of the cell of grid where point belongs
    row :int = int(point[1] / cell_size)

    start_j: int = max(col-1,0) # comparision should start from either 'col - 1' or 0, whichever index is valid
    end_j : int = min(col+1,no_cells_horizontal-1) # comparision ends with 'col +1' or 'no_cells_horizontal-1', again whichever is valid
    start_i: int = max(row-1,0)
    end_i : int = min(row+1,no_cells_vertical-1)

    # note: start* and end* indices are both inclusive
    for i in range(start_i,end_i+1):
        for j in range(start_j,end_j+1):
            # grid is not empty (meaning containg dummy value (-1,-1)) and minimum distance is violated 
            if grid[i][j] != (-1,-1) and distance(grid[i][j],point) < radius:
                return False 
    return True

log : int = 0
def poisson_disk_samples(radius:float,length: int, breadth:int,N:int = 2, k: int = 30) -> list[tuple[float,float]]:
    """
    For finding set of random points which are minimum distance apart from each other

    Parameters:
        radius (float): Minimum distance between each samples
        length (int): Length of the rectangle where points are sampled
        breadth (int): Breadth of the rectangle where points are sampled
        N (int): Dimension of sample, in our case it is by default 2D
        k (int): No. of times to try to generate points, according to Bridson k = 30

    Returns:
        list[tuple[float,float]]: Set of samples points
    """

    points: list[tuple[float,float]] = [] # list of points to return at the end
    active_points: list[tuple[float,float]] = [] # list of active points

    cell_size: float = radius / N**(0.5)
    
    # calculate the no of cells in the grid
    no_cells_horizontal : int = math.ceil(length/cell_size)   # I think it will be floor without any extra 1 added
    no_cells_vertical : int = math.ceil(breadth/cell_size)

    # initialize the grid with (-1,-1) - meaning it is a dummy point
    grid: list[list[tuple[float,float]]] = [[(-1,-1) for _ in range(no_cells_horizontal)] for _ in range(no_cells_vertical)]

    # generate a random 2d point - actually when generalised point should be n-tuple, accordingly dimension of the tuple needs to be modified
    random_point: tuple[float,float] = (random.uniform(0,length),random.uniform(0,breadth))

    insert_point(grid=grid,point=random_point,cell_size=cell_size)
    points.append(random_point)
    active_points.append(random_point)

    # generate a point which is requisite distance from the point in the active list
    while len(active_points) > 0:
        # choose any point from the active list
        any_active_point: tuple[float,float] = random.choice(active_points)

        # a satisfied variable to keep track if point generated satisfied above constraints
        # if satisfied keep the point and break out of the loop
        # else remove the point from the active_points list
        satisfied : bool = False

        # try k times to find the point that meets the constraint :
        for _ in range(k):
            # generate a point radial distance between r and 2r around 'any_active_point'
            angle: float = random.uniform(0,2*math.pi) # random angle between 0 to 2 pi
            random_r: float = random.uniform(radius, radius*2) # random distance between radius and 2*radius
            new_point: tuple[float,float] = (
                any_active_point[0]+ random_r*math.cos(angle),
                any_active_point[1]+ random_r*math.sin(angle)
                )
            global log
            print(log)
            print("\n\n")
            print(active_points)
            print("\n\n")
            print(points)
            print("\n\n")

            log+=1
            # check the constraint - within distance r of existing samples
            # if the constraint satisfied append the point to the lists, set the satisfied to true
            if is_valid_point(new_point, grid,length, breadth, no_cells_horizontal, no_cells_vertical, cell_size, radius):
                points.append(new_point)
                active_points.append(new_point)
                insert_point(grid,new_point,cell_size) # forget about this and made infinite loop
                satisfied = True
                break

        if not satisfied:
            active_points.remove(any_active_point)
        
    return points


