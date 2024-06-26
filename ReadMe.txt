Introduction

-->This program implements a file system simulation with functionalities for creating, managing, and manipulating files and directories. It supports operations such as file creation, deletion, reading, directory creation, deletion, listing, metadata updates, searching, size calculation, and traversal.


Technologies Used

-->C++
-->Standard Template Library (STL)
-->Huffman Coding Algorithm for compression
-->Encryption and decryption for file content



Features

-->1. File Operations
Create File: Creates a new file with specified content and path.
Delete File: Deletes an existing file.
Read File: Reads and displays the content of a file.
Update File Metadata: Updates permissions or modification date of a file.

-->2. Directory Operations
Create Directory: Creates a new directory at the specified path.
Delete Directory: Deletes an existing directory.
List Directory: Lists all files and subdirectories within a directory.

-->3. Search Operations
Search File: Searches for a file by name within the file system.
Search Directory: Searches for a directory by name within the file system.

-->4. Miscellaneous Operations
Calculate Directory Size: Calculates the total size of a directory and its contents.
Traverse Directory (DFS): Performs Depth-First Search traversal of a directory.
Traverse Directory (BFS): Performs Breadth-First Search traversal of a directory.



Menu: 

--> Follow the on-screen menu to perform desired operations by entering corresponding option numbers and required inputs.


Notes

-->File content is compressed using Huffman Coding before storage.
-->File content is encrypted before storage and decrypted when read.
-->Metadata such as permissions and modification dates can be updated for files and directories.
