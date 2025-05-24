import numpy as np
from tkinter import filedialog
from utils.file_handlers import save_points, load_points
from utils.algorithms import de_casteljau

def on_click(app, event):
    if event.inaxes != app.ax:
        return
    click = np.array([event.xdata, event.ydata])
    if event.button == 1:
        for i, pt in enumerate(app.points):
            if np.linalg.norm(np.array(pt) - click) < 0.02:
                app.selected_point = i
                return
        app.points.append([event.xdata, event.ydata])
    elif event.button == 3:
        for i, pt in enumerate(app.points):
            if np.linalg.norm(np.array(pt) - click) < 0.02:
                app.points.pop(i)
                break
    app.draw()

def on_release(app, _):
    app.selected_point = None

def on_drag(app, event):
    if event.inaxes != app.ax or app.selected_point is None:
        return
    app.points[app.selected_point] = [event.xdata, event.ydata]
    app.draw()

def update_t(app, val):
    app.t = float(val)
    app.draw()

def clear(app):
    app.points = []
    app.draw()

def save(app):
    path = filedialog.asksaveasfilename(defaultextension='.json', filetypes=[('JSON files', '*.json')],
                                        initialfile='bezier-curve.json', initialdir='~/Downloads')
    if path:
        save_points(app.points, path)

def load(app):
    path = filedialog.askopenfilename(filetypes=[('JSON files', '*.json')], initialdir='~/Downloads')
    if path:
        app.points = load_points(path)
        app.draw()

def export_png(app):
    path = filedialog.asksaveasfilename(defaultextension='.png', initialfile='bezier-curve.png',
                                        initialdir='~/Pictures')
    if path:
        app.fig.savefig(path)
