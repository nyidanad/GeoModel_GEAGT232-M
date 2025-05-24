import json

def save_points(points, path):
    with open(path, 'w') as f:
        json.dump(points, f)

def load_points(path):
    with open(path, 'r') as f:
        return json.load(f)
