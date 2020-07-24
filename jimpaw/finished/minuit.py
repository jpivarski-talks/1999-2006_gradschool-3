# import _minuit.py

class _Minuit:
  def __init__(self, fcn, npar, names): pass

  def npar(self):
    return 3
  def names(self):
    return ('x', 'y', 'z')
  def value(self, name):
    return 0.
  def error(self, name):
    return 0.1
  def errlow(self, name):
    return 0.1
  def errhigh(self, name):
    return 0.1
  def ncalls(self):
    return 100

  def set_value(self, name, value):
    return True
  def set_error(self, name, value):
    return True

class Minuit:
  class ParBlock: pass
  class Parameter:
    def __init__(self, name, index, value, error, errlow, errhigh):
      self.name = name
      self.index = index
      self.value = value
      self.error = error
      self.errlow = errlow
      self.errhigh = errhigh
    def __repr__(self):
      import jimpaw
      output = "    %4d %10s" % (self.index, self.name)
      if self.value != None:
        valstr, errstr, lowstr, highstr = jimpaw.sigfigs(self.value, self.error, self.errlow, self.errhigh, stringwide=10)
        output += " %10s" % (valstr)
        if errstr != None:
          output += " +/- %10s" % (errstr)
        else:
          output += "     %10s" % ("")
        if lowstr != None:
          output += " - %10s" % (lowstr)
        else:
          output += "   %10s" % ("")
        if highstr != None:
          output += " + %10s" % (highstr)
        else:
          output += "   %10s" % ("")
      else:
        output += "       none"
      return output
          
  def __init__(self, fcn, start = {}, step = {}):
    import inspect
    names = inspect.getargspec(fcn)[0]
    self.f = fcn
    self.m = _Minuit(fcn, len(names), tuple(names))

    self.p = Minuit.ParBlock
    self.pi = []
    self.pn = {}
    for index, name in enumerate(names):
      value, error = None, None
      if name in start:
        value = start[name]
        self.m.set_value(value)
      if name in step:
        error = step[name]
        self.m.set_error(error)
      exec("self.p."+name+" = Minuit.Parameter(\""+name+"\", "+str(index)+", "+str(value)+", "+str(error)+", None, None)")
      self.pi.append(eval("self.p."+name))
      self.pn[name] = eval("self.p."+name)

    self.log = []

  def __repr__(self):
    output = ""
    if self.log == []:
      output += "MINUIT has not been invoked.\n"
    else:
      for l in self.log: output += l
    output += "\n"+str(self.m.ncalls())+" function calls\n\n"
    output += "      id       name      value          error              MINOS errors\n"
    output += "-----------------------------------------------------------------------\n"
    for i in self.pi: output += str(i)+"\n"
    return output

  def sync_p(self):
    self.p = Minuit.ParBlock
    names = self.m.names()
    for index, name in enumerate(names):
      value = self.m.value(name)
      error = self.m.error(name)
      errlow = self.m.errlow(name)
      errhigh = self.m.errhigh(name)
      exec("self.p."+name+" = Minuit.Parameter(\""+name+"\", "+str(index)+", "+str(value)+", "+str(error)+", "+str(errlow)+", "+str(errhigh)+")")
      self.pi.append(eval("self.p."+name))
      self.pn[name] = eval("self.p."+name)



