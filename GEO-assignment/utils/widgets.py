import customtkinter as ctk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import numpy as np
from utils import event_handlers as events

def create_widgets(app, root):
    app.fig, app.ax = plt.subplots(figsize=(9.0, 4.0))
    app.canvas = FigureCanvasTkAgg(app.fig, root)
    app.canvas.get_tk_widget().pack(side=ctk.LEFT, fill=ctk.BOTH, expand=True)

    control = ctk.CTkFrame(root, fg_color='#FCFCFC')
    control.pack(side=ctk.RIGHT, fill=ctk.Y)

    ctk.CTkLabel(control, text="t érték:").pack(padx=10, anchor="w")
    app.t_slider = ctk.CTkSlider(control, from_=0, to=1, command=lambda val: events.update_t(app, val))
    app.t_slider.set(0.5)
    app.t_slider.pack(fill=ctk.X, pady=(0, 20))

    ctk.CTkButton(control, text="Új görbe", command=lambda: events.clear(app)).pack(pady=5)
    ctk.CTkButton(control, text="Mentés", command=lambda: events.save(app)).pack(pady=5)
    ctk.CTkButton(control, text="Betöltés", command=lambda: events.load(app)).pack(pady=5)
    ctk.CTkButton(control, text="Export PNG", command=lambda: events.export_png(app)).pack(pady=(5, 20))

    ctk.CTkCheckBox(control, text="Görbe", variable=app.show_curve, command=app.draw).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control, text="Lépések", variable=app.show_helpers, command=app.draw).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control, text="Tangens pontban", variable=app.show_tangent, command=app.draw).pack(padx=10, anchor="w")
    ctk.CTkCheckBox(control, text="Szakasz hosszak", variable=app.show_lengths, command=app.draw).pack(padx=10, anchor="w")

    app.canvas.mpl_connect('button_press_event', lambda e: events.on_click(app, e))
    app.canvas.mpl_connect('button_release_event', lambda e: events.on_release(app, e))
    app.canvas.mpl_connect('motion_notify_event', lambda e: events.on_drag(app, e))
