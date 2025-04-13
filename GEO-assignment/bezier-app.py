import customtkinter as ctk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

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
    self.create_widgets(root)


  # >>> CREATE WIDGETS
  def create_widgets(self, root):

    # matplot figure config
    self.fig, self.ax = plt.subplots(figsize=(9.0, 5.2))
    self.ax.set(xlim=(0, 1), xticks=np.arange(0, 1.1, 0.1),
                ylim=(0, 1), yticks=np.arange(0, 1.1, 0.1))
    self.ax.grid(color = '#EDEDED')
    self.canvas = FigureCanvasTkAgg(self.fig, root)
    self.canvas.get_tk_widget().pack(side=ctk.LEFT, fill=ctk.BOTH, expand=True)

    # control frame - buttons, checkboxes, slider
    control_frame = ctk.CTkFrame(root, fg_color='#FCFCFC')
    control_frame.pack(side=ctk.RIGHT, fill=ctk.Y)

    ctk.CTkLabel(control_frame, text="t érték:").pack(padx=10, anchor="w")
    self.t_slider = ctk.CTkSlider(control_frame, from_=0, to=1)
    self.t_slider.set(0.5)
    self.t_slider.pack(fill=ctk.X, pady=(0, 20))

    ctk.CTkButton(control_frame, text="Új görbe").pack(pady=5, anchor="c")
    ctk.CTkButton(control_frame, text="Mentés").pack(pady=5, anchor="c")
    ctk.CTkButton(control_frame, text="Betöltés").pack(pady=5, anchor="c")
    ctk.CTkButton(control_frame, text="Export PNG").pack(pady=(5, 20), anchor="c")

    ctk.CTkCheckBox(control_frame, text="Görbe", variable=self.show_curve, checkbox_width=20, checkbox_height=20).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control_frame, text="Lépések", variable=self.show_helpers, checkbox_width=20, checkbox_height=20).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control_frame, text="Tangens pontban", variable=self.show_tangent, checkbox_width=20, checkbox_height=20).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control_frame, text="Szakasz hosszak", variable=self.show_lengths, checkbox_width=20, checkbox_height=20).pack(padx=10, anchor="w")

    # event handlers
    # ...


class main():
  def __init__(self):
    root = ctk.CTk()
    BezierApp(root)
    root.mainloop()

main()