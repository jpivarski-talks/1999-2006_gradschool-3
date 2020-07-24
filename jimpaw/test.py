# import _minuit
# m = _minuit.Minuit(lambda x, y, z: x**2+y**2+z**2, 3, 1, ('x', 'y', 'z'))

import rootntuple
ntuple = rootntuple.RootDir("/cdat/dafe/mccann/synthesis/root/db_122986.root").get("UpsilonGammaEEAnalysisProc").get("nt1")
hist = rootntuple.RootDir("/cdat/dafe/mccann/synthesis/root/db_122986.root").get("UpsilonGammaEEAnalysisProc").get("h150")
