# STL-VL_Vector

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Version](https://img.shields.io/badge/version-1.0.0-blue)

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

## Introduction
Welcome to the STL-VL_Vector project! This repository contains an efficient implementation of the well-known vector (STL) in C++. The project aims to provide a robust and high-performance vector class with additional functionalities.

![VL_Vector](https://www.richardhnguyen.com/static/8b8014a377520dede83b7bfd3a317c8d/cf1a2/what-is-vector.webp)

## Features
- Efficient memory management
- Dynamic resizing
- Support for various data types
- Iterators for easy traversal

## Installation
To use the VL_Vector class in your project, follow these steps:

1. **Clone the repository**:
    ```sh
    git clone https://github.com/shalevbarda/STL-VL_Vector.git
    cd STL-VL_Vector
    ```

2. **Include the header file** in your C++ project:
    ```cpp
    #include "vl_vector.hpp"
    ```

## Usage
Here's a simple example of how to use the VL_Vector class:

```cpp
#include "vl_vector.hpp"
#include <iostream>

int main() {
    vl_vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }

    return 0;
}
