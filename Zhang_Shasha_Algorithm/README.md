# Zhang-Shasha Algorithm - Tree Edit Distance (TED)

This project implements the Zhang-Shasha algorithm to solve the Tree Edit Distance problem. The algorithm calculates the minimum edit distance between two trees using insertion, deletion, and relabeling operations.

## Prerequisites

To compile and run this project, you need:

- **GCC Compiler (g++)** compatible with C++17
- **Windows PowerShell** or **Command Prompt**
- Windows, Linux, or macOS operating system

### Windows Installation

1. **MinGW-w64** or **Visual Studio with C++ tools**
2. **PowerShell** (pre-installed on Windows 10/11)

### Verifying Prerequisites

```powershell
# Check if g++ is installed
g++ --version

# Check PowerShell version
$PSVersionTable.PSVersion
```

## How to Compile and Run

### Option 1: Using PowerShell (Recommended)

1. Open **PowerShell** in the project directory
2. Run the following command:

```powershell
g++ -o programa .\main.cpp .\Tree.cpp .\Tree_Editing.cpp; .\programa.exe
```

This command will:
- Compile all `.cpp` files with the `g++` compiler
- Create an executable named `programa.exe`
- Automatically run the program after compilation

### Option 2: Step-by-Step Compilation

If you prefer to compile and run separately:

```powershell
# Compile the project
g++ -o programa .\main.cpp .\Tree.cpp .\Tree_Editing.cpp

# Run the program
.\programa.exe
```

### Option 3: Manual Compilation with Flags

For development with additional compiler flags:

```powershell
g++ -std=c++17 -Wall -Wextra -g -o programa .\main.cpp .\Tree.cpp .\Tree_Editing.cpp
.\programa.exe
```

## Alternative Compilation Commands

### Using Command Prompt (cmd)

```cmd
g++ -o programa main.cpp Tree.cpp Tree_Editing.cpp && programa.exe
```

### Using Git Bash

```bash
g++ -o programa main.cpp Tree.cpp Tree_Editing.cpp && ./programa.exe
```

### Linux/macOS

```bash
g++ -o programa main.cpp Tree.cpp Tree_Editing.cpp
./programa
```

## Project Structure

```
Zhang_Shasha_Algorithm/
├── main.cpp              # Main program with performance tests
├── Tree.h                # Tree class declaration
├── Tree.cpp              # Tree class implementation
├── Tree_Editing.h        # TreeEditing class declaration
├── Tree_Editing.cpp      # TreeEditing class implementation
├── README.md             # This file
└── complexity_results.csv # Generated performance results (after running)
```

## Program Features

The program includes:

1. **Performance Analysis**: Tests tree edit distance calculation for trees of different sizes (10, 100, 1000, 10000 nodes)
2. **Automatic Tree Generation**: Creates random trees for testing
3. **CSV Export**: Saves performance results to `complexity_results.csv`
4. **Debug Mode**: Optional detailed output for algorithm steps

### Sample Output

```
========================================
  PERFORMANCE ANALYSIS - ZHANG-SHASHA ALGORITHM
       Tree Edit Distance (TED)
========================================

Running tests for different tree sizes...
Sizes tested: 10, 100, 1000, 10000 nodes
------------------------------------------------------------
Testing trees of size 10... Completed in 15 ms
  Tree 1 size: 10 (Root Keys length: 8)
  Tree 2 size: 10 (Root Keys length: 7)
  Algorithm time: 2.34 ms
  Edit distance (TED): 6

...

Results saved to: complexity_results.csv
========================================
           TESTS COMPLETED
========================================
```

## Compiler Flags Used

The project supports the following compiler flags:

- **`-std=c++17`**: Uses C++17 standard
- **`-Wall`**: Enables common warnings
- **`-Wextra`**: Enables additional warnings
- **`-g`**: Includes debug information

## Troubleshooting

### Error: "g++: command not found"
- **Windows**: Install MinGW-w64, Visual Studio with C++ tools, or use WSL
- **Linux**: `sudo apt-get install g++` (Ubuntu/Debian)
- **macOS**: Install Xcode Command Line Tools: `xcode-select --install`

### Error: "Permission denied"
```powershell
# If execution policy prevents running, try:
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### Compilation Errors
1. Ensure all `.cpp` and `.h` files are present in the directory
2. Check that your compiler supports C++17
3. Verify file paths are correct (use `.\` prefix in PowerShell)

### Performance Issues
- For very large trees (>10000 nodes), the algorithm may take considerable time
- Monitor memory usage for extremely large test cases
- Consider reducing test sizes if needed

## About the Zhang-Shasha Algorithm

The **Zhang-Shasha Algorithm** efficiently calculates the edit distance between two ordered trees. The allowed operations are:

1. **Node Insertion**: Add a new node to the tree
2. **Node Deletion**: Remove an existing node from the tree
3. **Node Relabeling**: Change the label of an existing node

### Key Features:
- **Time Complexity**: O(|T1| × |T2| × depth(T1) × depth(T2))
- **Space Complexity**: O(|T1| × |T2|)
- **Optimal Solution**: Finds the minimum cost sequence of operations

### Algorithm Components:
- **Post-order Traversal**: For node indexing
- **Keyroot Identification**: Critical nodes for decomposition
- **Dynamic Programming**: Matrix-based distance calculation
- **Forest Distance**: Intermediate calculations for subtrees

## Output Files

After running the program:
- **`complexity_results.csv`**: Contains performance data for analysis
- **`programa.exe`**: The compiled executable (can be deleted after use)

## Advanced Usage

### Custom Tree Testing

The code includes commented examples for manual tree testing:

```cpp
// Example trees can be created and tested individually
// See commented code in main.cpp for manual testing examples
```

### Debug Mode

For detailed algorithm output, modify the debug flags in the source code or use the `DebugTreeEditing` class.

## License

This project is developed for educational and research purposes as part of academic coursework on tree algorithms and computational complexity analysis.

---

**Note**: This implementation is optimized for clarity and educational purposes. For production use, consider additional optimizations and error handling.
