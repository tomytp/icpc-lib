def svg(P, o="d.svg"):
    if not P: return
    X, Y = zip(*P)
    x0, x1, y0, y1 = min(X), max(X), min(Y), max(Y)
    p = max(x1-x0, y1-y0) * 0.1 or 10
    head = f'<svg viewBox="{x0-p} {y0-p} {x1-x0+2*p} {y1-y0+2*p}" xmlns="http://www.w3.org/2000/svg" style="background:#eee">'
    poly = f'<polygon points="{" ".join(f"{x},{y}" for x,y in P)}" fill="#f003" stroke="red" stroke-width="{p/20}"/>'
    text = "".join([f'<text x="{x}" y="{y}" font-size="{p/3}">{i}</text>' for i,(x,y) in enumerate(P)])
    with open(o, 'w') as f: f.write(head + poly + text + '</svg>')
