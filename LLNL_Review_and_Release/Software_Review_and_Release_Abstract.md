# Software Review and Release Abstract


* **Identification**:  paraDIS_lib

* **Developer Name(s) and Affiliation**:  Richard D. Cook, LLNL

* **Software Completion Date**  03-30-2016

* **Brief Description**:  The ParaDIS_lib software is a project that is funded by the DOE ASC Program. Its purpose is to provide visualization and analysis capabilities for the existing ParaDIS parallel dislocation dynamics simulation code. 

* **Method of Solution**:  Software creates a graph of the nodes and edges in a paraDIS dump file.  It then traces out important features using DFS of the graph and data values provided for each node and edge in the graph.  It produces VTK files and provides a C++ interface for use in tools such as VisIt for visualization.  It also outputs statistics found while traversing the graph. 

* **Computer(s) for which software is written** Linux machines such as those installed in the Livermore Computing clusters. 

* **Operating System**. Red Hat Enterprise Linux 6 or similar flavors of Linux.  

* **Programming Language(s) Used** C/C++

* **Software Limitations** Software is serial and in-core.  Very large datasets could require more than the available memory in a machine.

* **Unique Features of the Software** Software produces unique statistics resulting from a post-processing graph analysis which cannot be performed without searching the entire graph.  

* **Related and Auxiliary Software**. Works on data produced by the ParaDIS parallel dislocation dynamics simulation code. 

* **Other Programming or Operating Information or Restrictions** none

* **Hardware Requirements** Uses ordinary off the shelf linux compatible hardware.

* **Time Requirements** None

* **References**. Documentation is included with the software.  Contact the authors for more information about publications.  
