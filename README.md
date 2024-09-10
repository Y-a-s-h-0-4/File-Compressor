# File Compression Project

## Overview

This project implements a file compression and decompression utility using the Huffman coding algorithm. It's written in C++ and provides an efficient way to compress text files, potentially reducing file sizes significantly depending on the input data.

## Features

- Text file compression using Huffman coding
- File decompression to recover original content
- Calculation and display of compression ratio
- Performance metrics (compression/decompression time)
- Error handling for robust operation

## Requirements

- C++11 compatible compiler
- Standard C++ libraries

## Installation

1. Clone this repository or download the source code.
2. Compile the code using a C++11 compatible compiler. For example:
   ```
   g++ -std=c++11 file_compressor.cpp -o file_compressor
   ```
3. Run the compiled executable:
   ```
   ./file_compressor
   ```

## Usage

1. Run the program.
2. Choose between compression (1) or decompression (2).
3. Enter the input filename (the file to be compressed/decompressed).
4. Enter the output filename (where the result will be saved).
5. The program will process the file and display statistics.

Example:
```
File Compression Utility
------------------------
1. Compress
2. Decompress
Enter your choice (1/2): 1
Enter input filename: example.txt
Enter output filename: example_compressed.bin
```

## How It Works

### Compression
1. The program reads the input file and counts the frequency of each character.
2. It builds a Huffman tree based on these frequencies.
3. It generates Huffman codes for each character.
4. It encodes the input file using these codes and writes the compressed data to the output file.
5. The Huffman codes are also stored in the compressed file for later decompression.

### Decompression
1. The program reads the Huffman codes from the compressed file.
2. It then reads the compressed data, decodes it using the Huffman codes, and writes the original text to the output file.

## Performance

The compression ratio and execution time are displayed after each operation. The effectiveness of compression depends on the input data characteristics.

## Limitations

- This implementation is designed for text files. Binary files may not compress effectively.
- Very large files might require significant memory during compression/decompression.

## Future Improvements

- Implement adaptive Huffman coding for one-pass compression.
- Add support for compressing multiple files into a single archive.
- Optimize memory usage for very large files.

## Contributing

Contributions to improve the project are welcome. Please feel free to submit issues and pull requests.

## Contact

For any queries or suggestions, please open an issue in the project repository.
