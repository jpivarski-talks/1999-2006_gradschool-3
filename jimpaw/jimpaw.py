def sigfigs_internal(num, err1, err2, err3, mult=1., errs=None, stringwide=10):
  """Return strings that present num, err1, err2, err3 in two significant digits of the smallest error.

  This version can't handle err1=None or err2=None or err3=None (or zero)."""
  if errs == None: errs = [err1, err2, err3]
  smallest = min(errs)
  if 10. <= smallest < 100.:
    if mult > 1.:
      f = "%"+str(stringwide)+"d"
      return f % (round(num)*mult), f % (round(err1)*mult), f % (round(err2)*mult), f % (round(err3)*mult)
    else:
      import math
      f = "%"+str(stringwide)+"."+str(-int(math.log10(mult)))+"f"
      return f % (round(num)*mult), f % (round(err1)*mult), f % (round(err2)*mult), f % (round(err3)*mult)
  elif smallest > 50.:
    errs2 = []
    for e in errs: errs2.append(e/10.)
    return sigfigs_internal(num/10., err1/10., err2/10., err3/10., mult*10., errs2, stringwide)
  else:
    errs2 = []
    for e in errs: errs2.append(e*10.)
    return sigfigs_internal(num*10., err1*10., err2*10., err3*10., mult/10., errs2, stringwide)

def sigfigs(num, err1, err2=None, err3=None, stringwide=10, minerror=1e-14):
  """Return strings that present num, err1, err2, err3 in two significant digits of the smallest error."""
  err1none, err2none, err3none = False, False, False
  errs = []

  if err1 == None or abs(err1) < minerror:
    err1none = True
    err1 = 1.
  else:
    errs.append(abs(err1))

  if err2 == None or abs(err2) < minerror:
    err2none = True
    err2 = 1.
  else:
    errs.append(abs(err2))

  if err3 == None or abs(err3) < minerror:
    err3none = True
    err3 = 1.
  else:
    errs.append(abs(err3))

  numstr, err1str, err2str, err3str = sigfigs_internal(num, err1, err2, err3, 1., errs, stringwide)

  return numstr, err1str, err2str, err3str
