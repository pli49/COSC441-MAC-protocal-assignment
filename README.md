# Framework for MAC Assignment


In the following it is assumed that you have created a project
'COSC441-MAC' or similar in the OMNeT++ Eclipse IDE, that has a folder
structure that includes the following two sub-directories:

- src: contains all C++ source files, all .msg files and all .ned files

- simulations: contains omnetpp.ini file and a sub-directory 'results'
  where all the output .sca files are stored
  
  
The contents of the given framework are accordingly split up over two
sub-directories:

- src: contains all the files that will go into the 'src' folder of
  your project
  
- simulations: contains a skeleton 'omnetpp.ini' file, needs to go
  into the 'simulations' folder of your project
  
  
If you have chosen to set up your OMNeT++ project with another folder
structure, you will need to put the provided files into appropriate
places.



Your main tasks are:

- To extend the file 'CsmaMac.h' as needed and create an
  implementation in 'CsmaMac.cc'

- Similarly for 'AlohaMac.h' and 'AlohaMac.cc'

- Develop NED network files for the four experiments

- Fill in the configuration sections for these four experiments in 'omnetpp.ini'
