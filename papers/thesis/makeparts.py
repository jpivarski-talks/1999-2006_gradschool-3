from pyx import *
text.defaulttexrunner = text.texrunner(mode="latex", lfs="12pt", errordebug=1)

c = canvas.canvas()
c.text(0, 0, r"\boldmath $f_B\sqrt{M_B}$", [text.halign.boxcenter])
c.writeEPSfile("plots/part_fbmb.eps")

c = canvas.canvas()
c.text(0, 0, r"\boldmath $\sigma(\Upsilon \to e^+e^-)$", [text.halign.boxcenter])
c.writeEPSfile("plots/part_sigmaupsee.eps")

