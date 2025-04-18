import customtkinter as ctk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
from tkinter import filedialog
import json

class BezierApp:
  def __init__(self, root):

    # initial variables
    self.points = []
    self.selected_point = None
    self.t = 0.5
    self.show_curve = ctk.BooleanVar(value=True)
    self.show_helpers = ctk.BooleanVar(value=False)
    self.show_tangent = ctk.BooleanVar(value=False)
    self.show_lengths = ctk.BooleanVar(value=False)

    # canvas config
    root.title('Bézier Curve Drawer')
    root.geometry('1080x720')
    ctk.set_appearance_mode('light')
    ctk.set_default_color_theme('dark-blue')

    # canvas details
    self.create_widgets()
    self.draw()


  # >>> CREATE WIDGETS
  def create_widgets(self):

    # matplot figure config
    self.fig, self.ax = plt.subplots(figsize=(9.0, 4.0))
    self.canvas = FigureCanvasTkAgg(self.fig, root)
    self.canvas.get_tk_widget().pack(side=ctk.LEFT, fill=ctk.BOTH, expand=True)

    # control frame - buttons, checkboxes, slider
    control_frame = ctk.CTkFrame(root, fg_color='#FCFCFC')
    control_frame.pack(side=ctk.RIGHT, fill=ctk.Y)

    ctk.CTkLabel(control_frame, text="t érték:").pack(padx=10, anchor="w")
    self.t_slider = ctk.CTkSlider(control_frame, from_=0, to=1, command=self.update_t)
    self.t_slider.set(0.5)
    self.t_slider.pack(fill=ctk.X, pady=(0, 20))

    ctk.CTkButton(control_frame, command=self.clear, text="Új görbe").pack(pady=5, anchor="c")
    ctk.CTkButton(control_frame, command=self.save, text="Mentés").pack(pady=5, anchor="c")
    ctk.CTkButton(control_frame, command=self.load, text="Betöltés").pack(pady=5, anchor="c")
    ctk.CTkButton(control_frame, command=self.export_png, text="Export PNG").pack(pady=(5, 20), anchor="c")

    ctk.CTkCheckBox(control_frame, text="Görbe", variable=self.show_curve, command=self.draw, checkbox_width=20, checkbox_height=20).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control_frame, text="Lépések", variable=self.show_helpers, command=self.draw, checkbox_width=20, checkbox_height=20).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control_frame, text="Tangens pontban", variable=self.show_tangent, command=self.draw, checkbox_width=20, checkbox_height=20).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control_frame, text="Szakasz hosszak", variable=self.show_lengths, command=self.draw, checkbox_width=20, checkbox_height=20).pack(padx=10, anchor="w")

    # event handlers
    self.canvas.mpl_connect('button_press_event', self.on_click)
    self.canvas.mpl_connect('button_release_event', self.on_release)
    self.canvas.mpl_connect('motion_notify_event', self.on_drag)


  # >>> EVENT HANDLER FUNCTIONS
  # mouse event handelers
  def on_click(self, event):
    if (event.inaxes != self.ax):
      return
    
    click = np.array([event.xdata, event.ydata])

    # left click - add control point OR select it
    if event.button == 1:
      for i, pt in enumerate(self.points):
        if np.linalg.norm(np.array(pt) - click) < 0.02:
          self.selected_point = i
          return
      self.points.append([event.xdata, event.ydata])

    # right click - remove control point
    elif event.button == 3:
      for pt in enumerate(self.points):
        if np.linalg.norm(np.array(pt[1]) - click) < 0.02:
          self.points.pop(pt[0])

    self.draw()
  
  # deselecting selected point
  def on_release(self, _):
    self.selected_point = None

  # move selected point
  def on_drag(self, event):
    if event.inaxes != self.ax or self.selected_point is None:
      return
    self.points[self.selected_point] = [event.xdata, event.ydata]
    self.draw()

  # t update depending on slider
  def update_t(self, val):
    self.t = float(val)
    self.draw()

  # >>> BUTTON HANDLER FUNCTIONS
  # clear canvas
  def clear(self):
    self.points = []
    self.draw()

  # save canvas points into .json
  def save(self):
    path = filedialog.asksaveasfilename(
      defaultextension='.json', 
      filetypes=[('JSON files', '*.json')],
      initialfile='bezier-curve.json', 
      initialdir='~/Downloads',
      title='SaveAs JSON file'
    )
    if path:
      with open(path, 'w') as f:
        json.dump(self.points, f)

  # load previously saved canvas
  def load(self):
    path = filedialog.askopenfilename(
      filetypes=[('JSON files', '*.json')],
      initialdir='~/Downloads',
      title='Open JSON file'
    )
    if path:
      with open(path, 'r') as f:
        self.points = json.load(f)
      self.draw()

  # save canvas into .png
  def export_png(self):
    path = filedialog.asksaveasfilename(
      defaultextension='.png',
      initialfile='bezier-curve.png',
      initialdir='~/Pictures',
      title='SaveAS PNG file'
    )
    if path:
      self.fig.savefig(path)


  # >>> DE CASTELJAU ALGORITHM
  def de_casteljau(self, points, t):
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
  

  # >>> CALCULATE TANGENT
  def calc_tangent(self, t):
    n = len(self.points) - 1
    if n < 1:
      return None
      
    diff_points = []
    for i in range(n):
      diff_points.append(np.subtract(self.points[i+1], self.points[i]))
    tangent = self.de_casteljau(diff_points, t)[0]
    return n * tangent


  # >>> (RE)DRAW CANVAS
  def draw(self):
    self.ax.clear()
    self.ax.set(xlim=(0, 1), xticks=np.arange(0, 1.1, 0.1),
                ylim=(0, 1), yticks=np.arange(0, 1.1, 0.1))
    self.ax.grid(color='#EDEDED')

    # cheking if there's any existing control point
    if self.points == []:
      self.ax.set_title('Kattintson a pontok hozzáadásához')
      self.canvas.draw()
      return
    
    # draw control points
    xs, ys = zip(*self.points)
    self.ax.plot(xs, ys, 'o--', color='gray', label='Kontrollpontok')

    # draw length between points
    if self.show_lengths.get():
      for i in range(len(self.points) - 1):
        p1 = np.array(self.points[i])
        p2 = np.array(self.points[i+1])
        length = np.linalg.norm(p2 - p1)
        mid = (p1 + p2) / 2
        self.ax.text(*mid, f"{length:.2f}", fontsize=10, color='purple')

    # draw curve
    if self.show_curve.get() and len(self.points) >= 2:

      # curve
      ts = np.linspace(0, 1, 200)
      curve = [self.de_casteljau(self.points, t)[0] for t in ts]
      xs, ys = zip(*curve)
      self.ax.plot(xs, ys, 'b', label='Bézier görbe')

      # t point
      t = self.t_slider.get()
      pt = self.de_casteljau(self.points, t)[0]
      self.ax.plot(pt[0], pt[1], 'ro', label=f'P(t={t:.2f})')

    # draw helpers
    if self.show_helpers.get():
      t = self.t_slider.get()
      levels = self.de_casteljau(self.points, t)[1]

      for level in levels[1:]:
        if len(level) < 2:
          break
        xs, ys = zip(*level)
        self.ax.plot(xs, ys, 'o--')

    # draw tangent
    if self.show_tangent.get() and len(self.points) >= 2:
      t = self.t_slider.get()
      tangent = self.calc_tangent(t)

      if tangent is not None:
        pt = self.de_casteljau(self.points, t)[0]
        norm = tangent / np.linalg.norm(tangent) * 0.07
        self.ax.arrow(pt[0], pt[1], norm[0], norm[1], color='green', head_width=0.01, label='Tangens')

    self.ax.legend()
    self.canvas.draw()


# >>> ENTRY POINT
if __name__ == "__main__":
  root = ctk.CTk()
  app = BezierApp(root)
  root.mainloop()