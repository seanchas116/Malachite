# coding: UTF-8

import sys, os, glob, shutil

def argerror():
	
	# show usage and exit
	print u"Usage: python malachite-install.py [builddir] [installdir]"
	sys.exit()

def makedirectory(dst):
	
	# remove existing file / directory
	
	if os.path.islink(dst) or os.path.isfile(dst):
		os.remove(dst)
	
	if os.path.isdir(dst):
		pass
	
	else:
		os.makedirs(dst)


if __name__ == "__main__":
	
	# get paths
	
	try:
		builddir = sys.argv[1].decode("UTF-8")
	except:
		argerror()
	
	try:
		installdir = sys.argv[2].decode("UTF-8")
	except:
		argerror()
	
	builddir = os.path.abspath(builddir)
	installdir = os.path.abspath(installdir)
	
	if not os.path.isdir(installdir) and os.path.isdir(builddir):
		argerror()
	
	projectdir = os.path.dirname(os.path.abspath(__file__));
	
	# get headers & libs list to install
	
	headerlist = glob.glob(os.path.join(projectdir, u"ml*.h"))
	headerlist.append(os.path.join(projectdir, u"malachite.h"))
	
	liblist = glob.glob(os.path.join(builddir, u"*.dylib"))
	
	# create directories
	
	includepath = os.path.join(installdir, u"include/Malachite")
	libpath = os.path.join(installdir, u"lib")
	
	if not os.path.isdir(includepath):
		makedirectory(includepath)
	
	if not os.path.isdir(libpath):
		makedirectory(libpath)
	
	# install files
	
	for file in headerlist:
		shutil.copy(file, includepath)
	
	for file in liblist:
		shutil.copy(file, libpath)
	
	
	
