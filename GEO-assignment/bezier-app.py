import customtkinter as ctk
import numpy as np

from utils.draw import draw
from utils.widgets import create_widgets

class BezierApp:
    def __init__(self, root):
        self.points = []
        self.selected_point = None
        self.t = 0.5
        self.show_curve = ctk.BooleanVar(value=True)
        self.show_helpers = ctk.BooleanVar(value=False)
        self.show_tangent = ctk.BooleanVar(value=False)
        self.show_lengths = ctk.BooleanVar(value=False)

        root.title('Bézier Curve Drawer')
        root.geometry('1080x720')
        ctk.set_appearance_mode('light')
        ctk.set_default_color_theme('dark-blue')

        self.draw = lambda: draw(self)
        create_widgets(self, root)
        self.draw()

if __name__ == "__main__":
    root = ctk.CTk()
    app = BezierApp(root)
    root.mainloop()
