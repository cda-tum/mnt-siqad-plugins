Plugin Source Code
==================

This folder contains the source code for all plugins distributed via this repository.

To add a new plugin, simply create a new sub-folder and add a single `.cpp` file containing the plugin source code as
well as a single `.physeng` configuration file that contains the plugin metadata and arguments.

---
**NOTE**

The folder name, source file name and configuration file name must be identical!

For instance if you want to create a plugin called `myplugin`, you must create a folder called `myplugin` and add a file
called `myplugin.cpp` and `myplugin.physeng`.
---