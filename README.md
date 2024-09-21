# 0xVec
In-memory vector database implemented in C++ with Python bindings using `pybind11`. It supports basic CRUD operations on vectors and collections, as well as similarity queries using different metrics:

- **Euclidean Distance**
- **Cosine Similarity**
- **Dot Product Similarity**

The database is designed to be numerically stable by using double-precision floating-point numbers (`double`) for all computations. The Python bindings allow you to use the database seamlessly within Python applications, leveraging the performance of C++.

## Benchmark

### 145 time faster query than Pure Python
![0xVec](https://github.com/user-attachments/assets/8fc6a340-bb55-4515-8876-77f623523a3d)

```
For 1000 vectors, OxVec is 137.64 times faster than Pure Python
For 2000 vectors, OxVec is 167.61 times faster than Pure Python
For 3000 vectors, OxVec is 139.40 times faster than Pure Python
For 4000 vectors, OxVec is 136.29 times faster than Pure Python
For 5000 vectors, OxVec is 136.93 times faster than Pure Python
For 6000 vectors, OxVec is 153.92 times faster than Pure Python
For 7000 vectors, OxVec is 155.10 times faster than Pure Python
For 8000 vectors, OxVec is 135.87 times faster than Pure Python
For 9000 vectors, OxVec is 137.07 times faster than Pure Python
For 10000 vectors, OxVec is 151.53 times faster than Pure Python
```

## Features

- **In-Memory Storage**: Fast read/write operations suitable for real-time applications.
- **Multiple Collections**: Manage multiple vector collections within the database.
- **CRUD Operations**: Create, read, update, and delete vectors and collections.
- **Similarity Queries**: Find similar vectors using various similarity metrics.
- **Python Bindings**: Use the database in Python with easy-to-use bindings.
- **Numerical Stability**: Uses double-precision floats to ensure accurate computations.
- **Extensibility**: Designed to allow for additional similarity metrics and features.

## Prerequisites

- **C++ Compiler**: Supports C++11 or higher (e.g., GCC, Clang, MSVC).
- **Python**: Version 3.6 or higher.
- **Python Packages**:
  - `pybind11`: For creating Python bindings.
  - `setuptools`: For building the extension module.
- **Build Tools**:
  - Make sure you have the necessary build tools installed (e.g., `build-essential` on Linux).

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/ToyMath/0xVec.git
cd 0xVec
```

### 2. Install Python Dependencies

```bash
pip install pybind11
```

### 3. Build the Python Module

Run the following command in the project directory:

```bash
python setup.py build_ext --inplace
```

This command compiles the C++ code and builds the `oxvec` Python module.

## Usage

### Python Usage

#### Import the Module

```python
from oxvec import OxVec, CosineSimilarity, EuclideanDistance, DotProductSimilarity
```

#### Initialize the Database and Collection

```python
oxvec = OxVec()
oxvec.create_collection("my_vectors")
collection = oxvec.get_collection("my_vectors")
```

#### Add Vectors

```python
collection.add_vector("vec1", [1.0, 2.0, 3.0])
collection.add_vector("vec2", [4.0, 5.0, 6.0])
```

#### Update a Vector

```python
collection.update_vector("vec1", [1.1, 2.2, 3.3])
```

#### Delete a Vector

```python
collection.delete_vector("vec2")
```

#### Query Similar Vectors

```python
metric = CosineSimilarity()
results = collection.query([1.0, 2.0, 3.0], metric, 5)
for entry in results:
    print(f"Vector ID: {entry.get_id()}, Vector: {entry.get_vector()}")
```

#### Accessing Vectors

```python
if "vec1" in collection.vectors:
    print("vec1 exists in the collection.")

vector_entry = collection.get_vector("vec1")
print(f"Vector ID: {vector_entry.get_id()}, Vector: {vector_entry.get_vector()}")
```

### C++ Usage

#### Include Headers

```cpp
#include "oxvec.hpp"
```

#### Initialize the Database and Collection

```cpp
OxVec oxvec;
oxvec.createCollection("my_vectors");
auto collection = vectorDB.getCollection("my_vectors");
```

#### Add Vectors

```cpp
collection->addVector("vec1", {1.0, 2.0, 3.0});
collection->addVector("vec2", {4.0, 5.0, 6.0});
```

#### Update a Vector

```cpp
collection->updateVector("vec1", {1.1, 2.2, 3.3});
```

#### Delete a Vector

```cpp
collection->deleteVector("vec2");
```

#### Query Similar Vectors

```cpp
CosineSimilarity cosineSimilarity;
auto results = collection->query({1.0, 2.0, 3.0}, cosineSimilarity, 5);
for (const auto& entry : results) {
    std::cout << "Vector ID: " << entry->getId() << ", Vector: ";
    printVector(entry->getVector());
    std::cout << std::endl;
}
```

## Examples

### Python Example Script

```python
from oxvec import OxVec, CosineSimilarity

# Initialize Database and Collection
oxvec = OxVec()
oxvec.create_collection("example_collection")
collection = oxvec.get_collection("example_collection")

# Add Vectors
collection.add_vector("vector_a", [1.0, 0.0, 0.0])
collection.add_vector("vector_b", [0.0, 1.0, 0.0])
collection.add_vector("vector_c", [0.0, 0.0, 1.0])

# Query Similar Vectors
metric = CosineSimilarity()
results = collection.query([1.0, 0.0, 0.0], metric, 2)

# Print Results
print("Top 2 Similar Vectors:")
for entry in results:
    print(f"Vector ID: {entry.get_id()}, Vector: {entry.get_vector()}")
```

## Testing

### Running Tests

A test script `test_vector_db.py` is provided to verify the functionality of the vector database.

#### Run the Test Script

```bash
python test_vector_db.py
```

#### Expected Output

```
Cosine Similarity Results:
Vector ID: vec1, Vector: [1.1, 0.0, 0.0]
Vector ID: vec4, Vector: [1.0, 1.0, 0.0]
Vector ID: vec3, Vector: [0.0, 0.0, 1.0]

All tests passed successfully.
```

### Test Cases Include

- Adding, updating, and deleting vectors.
- Querying using different similarity metrics.
- Accessing and verifying vectors in the collection.
- Ensuring numerical stability in computations.

## Project Structure

```
0xVec/
├── oxvec.hpp
├── oxvec.cpp
├── bindings.cpp
├── setup.py
├── test_oxvec.py
├── README.md
```

## Notes on Numerical Stability

- **Double Precision**: All computations use `double` to ensure higher numerical precision.
- **Consistency**: Using `double` aligns with Python's `float`, which is also double-precision.
- **Reduced Errors**: Higher precision reduces rounding errors in similarity computations.
- **Best Practices**: The implementation follows numerical stability best practices, making it suitable for applications where accuracy is critical.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss improvements, features, or bug fixes.

## Troubleshooting

### Common Issues

- **Module Not Found**: Ensure that the `vector_db` module is built and located in the same directory as your script.
- **Compilation Errors**: Verify that all prerequisites are installed and that your compiler supports C++11 or higher.
- **Python Version Compatibility**: Make sure you're using a compatible Python version (3.6 or higher).

## Future Work

- **Persistence**: Implement functionality to save and load collections from disk.
- **Advanced Indexing**: Incorporate more efficient indexing structures for faster queries on large datasets.
- **Additional Metrics**: Add support for more similarity metrics.
- **Concurrency**: Ensure thread-safe operations for concurrent access.
- **Performance Optimization**: Optimize the code for better performance with large-scale data.
