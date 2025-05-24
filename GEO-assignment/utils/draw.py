import numpy as np
from utils.algorithms import de_casteljau, calc_tangent

def draw(app):
    ax = app.ax
    ax.clear()
    ax.set(xlim=(0, 1), xticks=np.arange(0, 1.1, 0.1),
           ylim=(0, 1), yticks=np.arange(0, 1.1, 0.1))
    ax.grid(color='#EDEDED')

    if not app.points:
        ax.set_title('Kattintson a pontok hozzáadásához')
        app.canvas.draw()
        return

    xs, ys = zip(*app.points)
    ax.plot(xs, ys, 'o--', color='gray', label='Kontrollpontok')

    if app.show_lengths.get():
        for i in range(len(app.points) - 1):
            p1, p2 = np.array(app.points[i]), np.array(app.points[i+1])
            length = np.linalg.norm(p2 - p1)
            mid = (p1 + p2) / 2
            ax.text(*mid, f"{length:.2f}", fontsize=10, color='purple')

    if app.show_curve.get() and len(app.points) >= 2:
        ts = np.linspace(0, 1, 200)
        curve = [de_casteljau(app.points, t)[0] for t in ts]
        xs, ys = zip(*curve)
        ax.plot(xs, ys, 'b', label='Bézier görbe')
        pt = de_casteljau(app.points, app.t_slider.get())[0]
        ax.plot(pt[0], pt[1], 'ro', label=f'P(t={app.t_slider.get():.2f})')

    if app.show_helpers.get():
        _, levels = de_casteljau(app.points, app.t_slider.get())
        for level in levels[1:]:
            if len(level) < 2:
                break
            xs, ys = zip(*level)
            ax.plot(xs, ys, 'o--')

    if app.show_tangent.get() and len(app.points) >= 2:
        tangent = calc_tangent(app.points, app.t_slider.get())
        if tangent is not None:
            pt = de_casteljau(app.points, app.t_slider.get())[0]
            norm = tangent / np.linalg.norm(tangent) * 0.07
            ax.arrow(pt[0], pt[1], norm[0], norm[1], color='green', head_width=0.01, label='Tangens')

    ax.legend()
    app.canvas.draw()
