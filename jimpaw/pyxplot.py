import os, cPickle as pickle, re, time
from pyx import *

### User configuration options ##########################

tmp_directory = "/tmp"

# Your TeX setup (see PyX documentation for details)
def reboot_tex():
  global texrunner
  texrunner = text.texrunner(mode="latex", lfs="12pt", errordebug=1)
  texrunner.preamble(r"""
\setlength{\parskip}{0.2 cm}
""")
reboot_tex()

#########################################################

pyxplotset = set()

class PyxPlot_command:
  def __init__(self, item, command="insert", attrs=[]):
    self.item = item
    if command == "insert":   self.command = 0
    elif command == "stroke": self.command = 1
    elif command == "fill":   self.command = 2
    elif command == "draw":   self.command = 3
    else:
      raise ValueError, "Command must be 'insert', 'stroke', 'fill', or 'draw'."
    self.attrs = attrs

  def apply(self, canvas):
    if self.command == 0:   canvas.insert(self.item, self.attrs)
    elif self.command == 1: canvas.stroke(self.item, self.attrs)
    elif self.command == 2: canvas.fill(self.item, self.attrs)
    elif self.command == 3: canvas.draw(self.item, self.attrs)

  def __repr__(self):
    if self.command == 0:   return "canvas.insert("+repr(self.item)+", "+repr(self.attrs)+")"
    elif self.command == 1: return "canvas.stroke("+repr(self.item)+", "+repr(self.attrs)+")"
    elif self.command == 2: return "canvas.fill("+repr(self.item)+", "+repr(self.attrs)+")"
    elif self.command == 3: return "canvas.draw("+repr(self.item)+", "+repr(self.attrs)+")"

###############################################################

class PyxLinkedAxis_pointer:
  def __init__(self, which, pointer):
    self.which = which
    self.pointer = pointer

  def apply(self):
    return graph.axis.linkedaxis(self.pointer.graph.axes[self.which])

class PyxGraph_command:
  def __init__(self, *ds, **args):
    self.ds = list(ds)
    self.args = args

  def modify(self, **args):
    self.args.update(args)

  def apply(self, **args):
    a = self.args.copy()
    a.update(args)
    for kw, val in a.items():
      if isinstance(val, PyxLinkedAxis_pointer):
        a[kw] = val.apply()
    self.graph = graph.graphxy(**a)
    for d, s in self.ds: self.graph.plot(d, s)
    return self.graph

  def copy(self, **args):
    a = self.args.copy()
    a.update(args)
    return PyxGraph_command(*self.ds, **a)

  def __repr__(self):
    output = "pyx.graph.graphxy("
    keys = self.args.keys()
    order = {"xpos":0, "ypos":1, "width":2, "height":3, "ratio":4, "key":5, "backgroundattrs":6, "axesdist":7, "axisat":8, "yaxisat":9}
    keys.sort(lambda a1, a2: cmp(order.get(a1,10), order.get(a2,10)))
    for k in keys: output += k+"="+repr(self.args[k])+", "
    output += ")\n"
    for d, s in self.ds:
      output += "plot("+repr(d)+", styles="+repr(s)+")\n"
    return output

###############################################################

class PyxPlot:
  def __init__(self, name="pyxplot", logname=None, width=15, height=15, display=None, logpaperformat=document.paperformat.Letter):

    # Give this pyxplot a unique (human-grokable) name
    assigned_name = name[:]
    counter = 1
    while assigned_name in pyxplotset:
      counter += 1
      assigned_name = name+"<"+str(counter)+">"
    pyxplotset.add(assigned_name)
    self.name = assigned_name

    self.instance = id(self)
    self.session_name = "pyxplot-"+str(self.instance)
    self.file_name = tmp_directory+"/pyxplot-"+str(self.instance)+".pdf"

    self.width = width
    self.height = height
    self.xgap = 1.5
    self.ygap = 1.5
    self.x2gap = 0.5
    self.y2gap = 0.5
    self.lining("visible")
    self.graphs = []
    self.make_canvas().writePDFfile(self.file_name)

    # Note: this is not a safe OS call: what if self.name contained
    # something dastardly like "; rm -rf *"?  It only affects the
    # user's own area.
    dodisplay = ""
    if display: dodisplay = "-display "+display
    os.system("xpdf -z width -t '"+self.name+"' "+dodisplay+" -remote "+self.session_name+" "+self.file_name+" &")

    # Set up the paper trail
    self.log = []
    if logname == None: logname = name
    self.logfile_name = logname+".pdf"
    self.paperformat = logpaperformat
    self.plog = {}
    self.plog_page = []
    self.plogfile_name = logname+".p"

  def __del__(self):
    pyxplotset.remove(self.name)
    os.system("xpdf -remote "+self.session_name+" -quit")
    os.remove(self.file_name)

###############################################################

  def make_canvas(self):
    commands = self.commands_lining + map(lambda g: PyxPlot_command(g.apply()), self.graphs)

    c = canvas.canvas(texrunner=texrunner)
    for i in commands: i.apply(c)
    return c

  def redraw(self):
    self.make_canvas().writePDFfile(self.file_name)
    os.system("xpdf -remote "+self.session_name+" -reload")

  def raise_window(self):
    os.system("xpdf -remote "+self.session_name+" -raise")

###############################################################

  def append_log(self, message=r"\mbox{ }", objects={}):
    # Add these objects to the pickled dictionary with a unique name
    # and remember what page they're associated with.
    page_objects = []

    objects_newnames = {}
    for k, o in objects.items():
      assigned_name = k[:]
      counter = 1
      while assigned_name in self.plog:
        counter += 1
        assigned_name = k+"_"+str(counter)

      self.plog[assigned_name] = o
      page_objects.append(assigned_name)

    self.plog_page.append(page_objects)
        
    # Add useful information to the message
    underscore = re.compile("_")
    message += "\n\n\\mbox{ }\n\n\\hline\n"

    if len(page_objects) > 0:
      message += "\\begin{itemize}\\setlength{\\itemsep}{-0.2 cm}\n"
      for assigned_name in page_objects:
        message += "    \\item "+underscore.sub("\\_", assigned_name)+"\n"
      message += "\\end{itemize}\n"

    name_with_underscores = os.path.abspath(self.logfile_name)
    name_with_underscores = underscore.sub("\\_", name_with_underscores)
    message += "{\\it "+time.asctime(time.localtime(time.time()))+" \\hfill "+name_with_underscores+"}"

    # Write the objects out to the pickle file
    pickle.dump(self.plog, open(self.plogfile_name, "w"))

    # Create the textbox and write out the pdf
    textbox = texrunner.text(0, -0.5, message, [text.parbox(15), text.halign.left, text.halign.flushleft, text.valign.top])
    c = self.make_canvas()
    c.insert(textbox)
    self.log.append(document.page(c, paperformat=self.paperformat, fittosize=1))
    document.document(self.log).writetofile(self.logfile_name)

  def view_log(self, page):
    if type(page) != int and type(page) != long:
      raise TypeError, "Only one page, please."
    self.log[page].canvas.writePDFfile(self.file_name)
    os.system("xpdf -remote "+self.session_name+" -reload")

  def del_log(self, page):
    if type(page) != int and type(page) != long:
      raise TypeError, "Only one page, please."
    for name in self.plog_page[page]:
      del self.plog[name]
    del self.log[page]
    del self.plog_page[page]

    # Update the pickle and pdf files
    pickle.dump(self.plog, open(self.plogfile_name, "w"))
    document.document(self.log).writetofile(self.logfile_name)

###############################################################

  def lining(self, style="visible"):
    if style == "invisible":
      self.commands_lining = [PyxPlot_command(path.rect(0, 0, 15, 15), "stroke", [color.grey.white])]
    elif style == "visible":
      self.commands_lining = [PyxPlot_command(path.rect(0, 0, 15, 15), "stroke", [color.grey.black])]
    elif style == "none":
      self.commands_lining = []
    else:
      self.commands_lining = [PyxPlot_command(path.rect(0, 0, 15, 15), "stroke", style)]

  def plot(self, *args, **kwds):
    defaults = {"xpos": self.xgap, "ypos": self.ygap, "width": self.width-self.xgap-self.x2gap, "height": self.height-self.ygap-self.y2gap, "x": graph.axis.linear(min=0, max=1), "y": graph.axis.linear(min=0, max=1)}
    if len(args) > 0:
      defaults["x"] = graph.axis.linear()
      defaults["y"] = graph.axis.linear()
    defaults.update(kwds)

    self.graphs = [PyxGraph_command(*args, **defaults)]
    self.redraw()
    return self.graphs[0]

  def noplot(self, *args, **kwds):
    defaults = {"xpos": self.xgap, "ypos": self.ygap, "width": self.width-self.xgap-self.x2gap, "height": self.height-self.ygap-self.y2gap, "x": graph.axis.linear(min=0, max=1), "y": graph.axis.linear(min=0, max=1)}
    if len(args) > 0:
      defaults["x"] = graph.axis.linear()
      defaults["y"] = graph.axis.linear()
    defaults.update(kwds)

    return PyxGraph_command(*args, **defaults)

  def table(self, graphs):
    output = []
    reversed_graphs = graphs[:]
    reversed_graphs.reverse()

    each_height = float(self.height) / float(len(graphs)) - self.ygap - self.y2gap
    ypos = self.ygap
    for row in reversed_graphs:
      each_width = float(self.width) / float(len(row)) - self.xgap - self.x2gap
      xpos = self.xgap
      for g in row:
        output.append(g.copy(xpos=xpos, ypos=ypos, width=each_width, height=each_height))
        xpos += each_width + self.xgap + self.x2gap
      ypos += each_height + self.ygap + self.y2gap

    self.graphs = output
    self.redraw()

  def table_transpose(self, graphs):
    output = []

    each_width = float(self.width) / float(len(graphs)) - self.xgap - self.x2gap
    xpos = self.xgap
    for col in graphs:
      col.reverse()

      each_height = float(self.height) / float(len(col)) - self.ygap - self.y2gap
      ypos = self.ygap
      for g in col:
        output.append(g.copy(xpos=xpos, ypos=ypos, width=each_width, height=each_height))
        ypos += each_height + self.ygap + self.y2gap
      xpos += each_width + self.xgap + self.x2gap

    self.graphs = output
    self.redraw()

  def array(self, graphs):
    output = []
    reversed_graphs = graphs[:]
    reversed_graphs.reverse()

    each_height = float(self.height-self.ygap-self.y2gap) / float(len(graphs))
    ypos = self.ygap
    firstrow = True
    for row in reversed_graphs:
      each_width = float(self.width-self.xgap-self.x2gap) / float(len(row))
      xpos = self.xgap
      firstcol = True
      if firstrow: xmaster = []
      i = 0
      for g in row:
        if firstrow and firstcol:  # (first row from the bottom up)
          output.append(g.copy(xpos=xpos, ypos=ypos, width=each_width, height=each_height))
        elif firstrow and not firstcol:
          output.append(g.copy(xpos=xpos, ypos=ypos, width=each_width, height=each_height, y=PyxLinkedAxis_pointer("y", ymaster)))
        elif not firstrow and firstcol:
          output.append(g.copy(xpos=xpos, ypos=ypos, width=each_width, height=each_height, x=PyxLinkedAxis_pointer("x", xmaster[i])))
        else:
          output.append(g.copy(xpos=xpos, ypos=ypos, width=each_width, height=each_height, x=PyxLinkedAxis_pointer("x", xmaster[i]), y=PyxLinkedAxis_pointer("y", ymaster)))
        if firstrow: xmaster.append(output[-1])
        if firstcol: ymaster = output[-1]
        xpos += each_width
        firstcol = False
        i += 1
      ypos += each_height
      firstrow = False

    self.graphs = output
    self.redraw()

#   def array_transpose(self, graphs):
#     output = []

#     each_width = float(self.width-self.xgap-self.x2gap) / float(len(graphs))
#     xpos = self.xgap
#     for col in graphs:
#       col.reverse()

#       each_height = float(self.height-self.ygap-self.y2gap) / float(len(col))
#       ypos = self.ygap
#       for g in col:
#         output.append(g.copy(xpos=xpos, ypos=ypos, width=each_width, height=each_height))
#         ypos += each_height
#       xpos += each_width

#     self.graphs = output
#     self.redraw()

###############################################################

class data:
  def __init__(self, x, y, dy=None, dx=None):
    self.x = x
    self.y = y
    self.dx = dx
    self.dy = dy

  def translate_symbolstyle(self, symbol, thecolor, fillcolor, size):
    if type(thecolor) == str:
      if thecolor in translate_colors:
        colorattr = translate_colors[thecolor]
      else:
        raise ValueError, "Unrecognized color \""+thecolor+"\"."
    else:
      colorattr = thecolor

    if type(fillcolor) == str:
      if fillcolor in translate_colors:
        fillcolorattr = translate_colors[fillcolor]
      elif fillcolor == "same":
        fillcolorattr = colorattr
      else:
        raise ValueError, "Unrecognized fillcolor \""+fillcolor+"\"."
    else:
      fillcolorattr = fillcolor

    if type(symbol) == str:
      if symbol in translate_symbols:
        symbolattr = translate_symbols[symbol]
    else:
      raise ValueError, "Unrecognized symbol \""+symbol+"\"."
        
    if fillcolor == None:
      return graph.style.symbol(symbolattr, size=size, symbolattrs=[deco.stroked([colorattr])])
    else:
      return graph.style.symbol(symbolattr, size=size, symbolattrs=[deco.filled([fillcolorattr]), deco.stroked([colorattr])])

  def translate_errorbarstyle(self, thecolor, size):
    if type(thecolor) == str:
      if thecolor in translate_colors:
        colorattr = translate_colors[thecolor]
      else:
        raise ValueError, "Unrecognized color \""+thecolor+"\"."
    else:
      colorattr = thecolor

    return graph.style.errorbar(size=size, errorbarattrs=[colorattr])

  def points(self, title="", symbol="circle", color="black", fillcolor="same", size=0.13*unit.v_cm):
    if len(self.x) != len(self.y):
      raise ValueError, "Lists x and y need to be the same length."
    d = graph.data.list(zip(self.x, self.y), title=title, x=1, y=2)
    s = [self.translate_symbolstyle(symbol, color, fillcolor, size)]
    return d, s

  def errorbars(self, title="", symbol="circle", color="black", fillcolor="same", size=0.13*unit.v_cm, capsize=0.13*unit.v_cm):
    if self.dy == None or len(self.x) != len(self.y) or len(self.y) != len(self.dy):
      raise ValueError, "Lists x, y, and dy need to be the same length."
    d = graph.data.list(zip(self.x, self.y, self.dy), title=title, x=1, y=2, dy=3)
    s = [self.translate_errorbarstyle(color, capsize), self.translate_symbolstyle(symbol, color, fillcolor, size)]
    return d, s

###############################################################



p = PyxPlot()

p.xgap = 1.7
p.x2gap = 0.3

p.ygap = 1.5
p.y2gap = 0.5

p.table([[p.noplot(x=graph.axis.lin(title="x", min=0, max=1), y=graph.axis.lin(title="y", min=0, max=1)), p.noplot(x=graph.axis.lin(title="x", min=0, max=1), y=graph.axis.lin(title="y", min=0, max=1))], [p.noplot(x=graph.axis.lin(title="x", min=0, max=1), y=graph.axis.lin(title="y", min=0, max=1)), p.noplot(x=graph.axis.lin(title="x", min=0, max=1), y=graph.axis.lin(title="y", min=0, max=1))]])

p.array([[p.noplot(y=graph.axis.lin(title="y1", min=0, max=1)), p.noplot()], [p.noplot(x=graph.axis.lin(title="x1", min=0, max=1), y=graph.axis.lin(title="y2", min=0, max=1)), p.noplot(x=graph.axis.lin(title="x2", min=0, max=1))]])




d = data([0.2, 0.4, 0.8], [0.2, 0.6, 0.4], [0.1, 0.1, 0.1])
d2 = data([0.2, 0.4, 0.8], [0.2+0.2, 0.6+0.2, 0.4+0.2], [0.1, 0.1, 0.1])
g = p.plot(d.errorbars(color="blue"), d2.points(color="red"))
g.modify(x=graph.axis.log())
p.redraw()



g2 = PyxGraph_command([d.errorbars(), d2.points(color="red")], xpos=1.5, ypos=1.5, width=13, height=13, x=graph.axis.linear(), y=graph.axis.linear())
g2 = PyxGraph_command([], xpos=1.5, ypos=1.5, width=13, height=13, x=graph.axis.linear(min=0, max=1), y=graph.axis.linear(min=0, max=1))
g = g2.apply()
g.writePDFfile(p.file_name)
os.system("xpdf -remote "+p.session_name+" -reload")




# These should be in another module
# data class that has x, y, ey, ey can be None, y can be a function, generates the list
# hist:data that can read ntuples
# prof:data that can read ntuples
# cloud generates hist

# data generates a graph.data object, possibly with title

# PyxPlot has a show([X,Y]) function for quickly looking at superimposed graph.data objects

# ?????????????????

# freestyle class that creates objects that can be put on a canvas
# plot:freestyle class that has the superposition and generates a graph.graphxy

# PyxPlot keeps a [[], []] of ???? objects as the standard ones for show() (this will replace the redraw() method)





translate_colors = {"black": color.grey.black, \
                    "white": color.grey.white, \
                    "grey10": color.grey(0.10), \
                    "grey20": color.grey(0.20), \
                    "grey30": color.grey(0.30), \
                    "grey40": color.grey(0.40), \
                    "grey50": color.grey(0.50), \
                    "grey60": color.grey(0.60), \
                    "grey70": color.grey(0.70), \
                    "grey80": color.grey(0.80), \
                    "grey90": color.grey(0.90), \
                    "red": color.rgb.red, \
                    "green": color.rgb.green, \
                    "blue": color.rgb.blue, \
                    "GreenYellow": color.cmyk.GreenYellow, \
                    "Yellow": color.cmyk.Yellow, \
                    "Goldenrod": color.cmyk.Goldenrod, \
                    "Dandelion": color.cmyk.Dandelion, \
                    "Apricot": color.cmyk.Apricot, \
                    "Peach": color.cmyk.Peach, \
                    "Melon": color.cmyk.Melon, \
                    "YellowOrange": color.cmyk.YellowOrange, \
                    "Orange": color.cmyk.Orange, \
                    "BurntOrange": color.cmyk.BurntOrange, \
                    "Bittersweet": color.cmyk.Bittersweet, \
                    "RedOrange": color.cmyk.RedOrange, \
                    "Mahogany": color.cmyk.Mahogany, \
                    "Maroon": color.cmyk.Maroon, \
                    "BrickRed": color.cmyk.BrickRed, \
                    "Red": color.cmyk.Red, \
                    "OrangeRed": color.cmyk.OrangeRed, \
                    "RubineRed": color.cmyk.RubineRed, \
                    "WildStrawberry": color.cmyk.WildStrawberry, \
                    "Salmon": color.cmyk.Salmon, \
                    "CarnationPink": color.cmyk.CarnationPink, \
                    "Magenta": color.cmyk.Magenta, \
                    "VioletRed": color.cmyk.VioletRed, \
                    "Rhodamine": color.cmyk.Rhodamine, \
                    "Mulberry": color.cmyk.Mulberry, \
                    "RedViolet": color.cmyk.RedViolet, \
                    "Fuchsia": color.cmyk.Fuchsia, \
                    "Lavender": color.cmyk.Lavender, \
                    "Thistle": color.cmyk.Thistle, \
                    "Orchid": color.cmyk.Orchid, \
                    "DarkOrchid": color.cmyk.DarkOrchid, \
                    "Purple": color.cmyk.Purple, \
                    "Plum": color.cmyk.Plum, \
                    "Violet": color.cmyk.Violet, \
                    "RoyalPurple": color.cmyk.RoyalPurple, \
                    "BlueViolet": color.cmyk.BlueViolet, \
                    "Periwinkle": color.cmyk.Periwinkle, \
                    "CadetBlue": color.cmyk.CadetBlue, \
                    "CornflowerBlue": color.cmyk.CornflowerBlue, \
                    "MidnightBlue": color.cmyk.MidnightBlue, \
                    "NavyBlue": color.cmyk.NavyBlue, \
                    "RoyalBlue": color.cmyk.RoyalBlue, \
                    "Blue": color.cmyk.Blue, \
                    "Cerulean": color.cmyk.Cerulean, \
                    "Cyan": color.cmyk.Cyan, \
                    "ProcessBlue": color.cmyk.ProcessBlue, \
                    "SkyBlue": color.cmyk.SkyBlue, \
                    "Turquoise": color.cmyk.Turquoise, \
                    "TealBlue": color.cmyk.TealBlue, \
                    "Aquamarine": color.cmyk.Aquamarine, \
                    "BlueGreen": color.cmyk.BlueGreen, \
                    "Emerald": color.cmyk.Emerald, \
                    "JungleGreen": color.cmyk.JungleGreen, \
                    "SeaGreen": color.cmyk.SeaGreen, \
                    "Green": color.cmyk.Green, \
                    "ForestGreen": color.cmyk.ForestGreen, \
                    "PineGreen": color.cmyk.PineGreen, \
                    "LimeGreen": color.cmyk.LimeGreen, \
                    "YellowGreen": color.cmyk.YellowGreen, \
                    "SpringGreen": color.cmyk.SpringGreen, \
                    "OliveGreen": color.cmyk.OliveGreen, \
                    "RawSienna": color.cmyk.RawSienna, \
                    "Sepia": color.cmyk.Sepia, \
                    "Brown": color.cmyk.Brown, \
                    "Tan": color.cmyk.Tan}

translate_symbols = {"cross": graph.style.symbol.cross, \
                     "plus": graph.style.symbol.plus, \
                     "square": graph.style.symbol.square, \
                     "triangle": graph.style.symbol.triangle, \
                     "circle": graph.style.symbol.circle, \
                     "diamond": graph.style.symbol.diamond}
