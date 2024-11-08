import sys

import ROOT
ROOT.gInterpreter.ProcessLine('#include "Core/include/PythiaEvent.h"')
ROOT.gInterpreter.AddIncludePath("Core/include")
ROOT.gROOT.LoadMacro("Core/src/PythiaEvent.cxx")


file = ROOT.TFile("~/JETStream/tmp.root")
tree = file.Get("tree")

