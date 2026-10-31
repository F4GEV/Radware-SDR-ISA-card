The examples in this directory show how to use the pre-defined
file I/O messages to open, read, and write files with the scope
debugger.



FCREATE.LD: This algorithm opens EXAMPLE3.DAT for
writing, fills it with integer data (0-10000), and then
closes it. This demonstrates how to open, close, and fwrite
to a file using the file I/O messages.



FCOPY.LD:   This algorithm opens EXAMPLE3.DAT for reading and
opens EXCOPY3.DAT for writing. It reads from EXAMPLE3.DAT and
writes to EXCOPY3.DAT. The end result is a file (EXCOPY3.DAT)
that is a exact copy of EXAMPLE3.DAT. This demonstrates how to
open, close, fwrite, fread from two different files.

NOTE:   Since this algorithm reads from EXAMPLE3.DAT you should
run FCREATE first.
