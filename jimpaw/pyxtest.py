import Numeric
from pyx import *

## c = canvas.canvas()
## circle = path.circle(0, 0, 2)
## line = path.line(-3, 1, 3, 2)
## c.stroke(circle)
## c.stroke(line)
## isects_circle, isects_line = circle.intersect(line)
## isectx, isecty = circle.at(isects_circle[0])
## c.stroke(path.line(0, 0, isectx, isecty))
## arc1, arc2 = circle.split(isects_circle)
## isects_line.sort()
## line1, line2, line3 = line.split(isects_line)
## segment = line2 << arc2
## c.stroke(segment, [deco.filled([color.grey(0.8)])])
## c.text(0, -3, r"""This is where I will explain a lot ($10^{55}$) of things. \par $\underbrace{x \not= \longrightarrow {\sqrt{y} \over 3}}$ and $\Upsilon$ $\to$ hadrons""", [text.parbox(10), text.halign.boxcenter, text.halign.flushleft])
## c.writeEPSfile("tmp.eps")

g = graph.graphxy(width=15, x=graph.axis.linear(min=-5, max=10), y=graph.axis.linear(min=1, max=110), ratio=1, key=graph.key.key(pos="tl"))
g.plot(graph.data.list(map(lambda x: (x, 2+x**2), Numeric.arange(-5, 11, 1.0)), x=1, y=2, title=r"zonkers! $x^{5^5}!$"), [graph.style.histogram(steps=1)])
g.plot(graph.data.function("y(x) = f(x)", context={"f": lambda x: 2+x**2}, title=r"$2 x^2$"), [graph.style.line()])
t1 = text.text(14.5, 14.5, r"100", [text.halign.boxright])
t2 = text.text(14.5-t1.width, 14.5, r"mean", [text.halign.boxright])
g.insert(t1)
g.insert(t2)
g.writeEPSfile("tmp.eps")



p = document.page(g, paperformat=document.paperformat.Letter, fittosize=0)
document.document([p]).writetofile("log.pdf")
