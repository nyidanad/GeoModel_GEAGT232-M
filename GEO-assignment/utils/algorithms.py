import numpy as np

def de_casteljau(points, t):
    levels = [points]
    while len(points) > 1:
        new_points = []
        for i in range(len(points) - 1):
            b0 = np.array(points[i])
            b1 = np.array(points[i + 1])
            interp = (1 - t) * b0 + t * b1
            new_points.append(interp)
        levels.append(new_points)
        points = new_points
    return points[0], levels

def calc_tangent(points, t):
    n = len(points) - 1
    if n < 1:
        return None
    diff_points = [np.subtract(points[i+1], points[i]) for i in range(n)]
    tangent, _ = de_casteljau(diff_points, t)
    return n * tangent
