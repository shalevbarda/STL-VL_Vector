//
// Created by Shalev Michael Barda on 10/04/2024.
//
//<-----------------Description Section----------------------->
// This is a header file that contains the implementation of a vector-like
// class called vl_vector. The class is a template class that can hold
// elements of any type. The class has a static capacity that is used
// to allocate memory on the stack. If the number of elements exceeds
// the static capacity,the class will allocate memory on the heap.
// The class provides a similar interface to the std::vector class
// in the C++ Standard Library, including constructors, destructors,
// operators, element access functions, iterators, and other member functions.
// The class also provides a set of global functions and operators
// to support the vector-like functionality.

//--------Static Capacity-----------//
// defining a static capacity allows users to specify the initial
// capacity of the vector. If not specified, it defaults to 16.
// This provides flexibility for users who may want to customize
// the initial capacity based on their needs.

//--------Exception Safety-----------//
// noexcept, indicating that they do not throw exceptions.
// allows users to rely on the noexcept guarantee when using
// these functions in exception-sensitive contexts.

//--------Iterator Support-----------//
// defined iterator types (iterator, const_iterator, reverse_iterator,
// const_reverse_iterator) and provided member functions
// (begin, end, rbegin, rend, crbegin, crend)
// to support iteration over the elements of the vector.
// This allows users to use range-based for loops and other
// iterator-based algorithms with our vector class.

//--------Const implementation-----------//
// The reason for having two versions of the functions is to allow
// the user to access the elements of the vector without modifying them.

//--------Operator Overloading-----------//
// overloading operators like operator[], op=, op==, and op!=
// provides a more natural and expressive way to work with vectors,
// making the code easier to understand and maintain.

//--------Expand Capacity Function-----------//
// The expand_capacity function is a helper function used internally
// to increase the capacity of the vector when needed.
// It calculates the new capacity based on the current size and
// the number of elements to add. for more details,
// see the implementation of the expand_capacity function.

//--------Time Complexity-----------//
// Many operations of the vl_vector class, such as accessing elements
// (operator[], at), adding elements (push_back), and removing elements
// (pop_back), have amortized constant time complexity (O(1)).
// This means that, on average, these operations take a constant
// amount of time to execute, regardless of the size of the vector.
// that make our vector class more efficient from the original vector class.

//<-----------------------HEADER----------------------->
//--------Header Guards-----------//

// Header guards are used to prevent multiple inclusions of the header file
// in the same translation unit, which can lead to compilation errors.
// The #ifndef, #define, and #endif directives ensure that the contents
// of the header file are only included once.

#ifndef _VL_VECTOR_HPP_
#define _VL_VECTOR_HPP_

//<------------------DEFINE & INCLUDES--------------------->
#define STATIC_CAPACITY 16 // for not using magic numbers
//<-----------------------IMPLEMENTATION----------------------->
template<typename T, size_t static_capacity = STATIC_CAPACITY>
class vl_vector
{
 public:

  //<--------Constructors and Destructor---------->

  /**  * Default constructor, new empty vector.
         not using allocation memory on the heap.
         Runtime complexity: O(1).
   */

  vl_vector ()
  {
    v_size = 0;
    v_capacity = static_capacity;
    is_on_heap = false; // Start with stack memory
  }

/**  * Copy constructor.
       Assumes that the other vector is with the same
       static capacity.
       Runtime complexity: O(n) - number of elements.
 */

  vl_vector (const vl_vector<T,static_capacity> &other) // cannot modify the other vector.
  {
    v_size = other.v_size;
    v_capacity = other.v_capacity;
    is_on_heap = other.is_on_heap;
    if (is_on_heap)
    {
      v_heap_data = new T[v_capacity];
      std::copy (other.v_heap_data, other.v_heap_data + v_size, v_heap_data);
    }
    else
    {
      std::copy (other.v_stack_data,
                 other.v_stack_data + v_size, v_stack_data);
    }
  }

/**  * Sequence based constructor.
       Runtime complexity: O(n)- num of elements in the range [first, last).
 */
  template<class ForwardIterator>
  vl_vector (const ForwardIterator &first, const ForwardIterator &last)
  {
    v_capacity = static_capacity;
    v_size = 0;
    is_on_heap = false;
    size_t range = std::distance (first, last);
    if (v_size + range > static_capacity)
    {
      expand_capacity (range);
    }
    for (ForwardIterator it = first; it != last; ++it)
    {
      push_back (*it);
    }
  }

/**  * Single-value initialized constructor.
       The choice not to mark it as explicit is
       to enhance code readability and ease of use.
       Not leading to unexpected behavior, as it offers a unique signature
       not found in other constructors.
       Runtime complexity: O(count) - number of elements with value v.
 */
  vl_vector (size_t count, const T &v)
  {
    v_capacity = static_capacity;
    v_size = 0;
    is_on_heap = false;
    for (size_t i = 0; i < count; ++i)
    {
      push_back (v);
    }
  }

/**  * initializer_list Constructor.
       convenient way to initialize the vector with a known set of values.
       Runtime complexity: O(n) - number of elements in in_l.
 */
  vl_vector (std::initializer_list<T> in_l)
  {
    v_capacity = static_capacity;
    v_size = 0;
    is_on_heap = false;
    for (const T &value: in_l)  // iterates over elements in "in_l".
    {
      push_back (value);
    }
  }

/**  * Destructor.
       Runtime complexity: O(1).
 */
  ~vl_vector ()
  {
    if (is_on_heap)
    {
      delete[] v_heap_data;// Memory erase - an array of elements.
    }
  }
//  //<--------Iterators---------->

/**
 * iterator - Random access iterator for the vector.
 */
  class iterator
  {
    public:
    using iterator_category = std::random_access_iterator_tag; //category
    using value_type = T;
    using difference_type = std::ptrdiff_t; //difference between two pointers
    using pointer = T *;
    using reference = T &;

    /**
       Default constructor. initializes the iterator with a null pointer.
     */
    iterator() : m_ptr(nullptr) {}

    /**
       Default constructor initializes the iterator with a pointer.
     */
    iterator (pointer ptr) : m_ptr (ptr) {}

    /**
       operator* - Dereference operator.
     */
    reference operator* () const { return *m_ptr; }

    /**
       operator-> - Member access operator.
     */
    pointer operator-> () { return m_ptr; }

    /**
       operator++ - Prefix increment operator.
     */
    iterator &operator++ ()
    {
      m_ptr++;
      return *this;
    }

    /**
        operator++ - Postfix increment operator.
     */
    iterator operator++ (int)
    {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
        operator-- - Prefix decrement operator.
     */
    iterator &operator-- ()
    {
      m_ptr--;
      return *this;
    }

    /**
        operator-- - Postfix decrement operator.
     */
    iterator operator-- (int)
    {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }

    /**
        operator+ - Addition operator.
     */

    iterator operator+ (difference_type n) const
    {
      return iterator (m_ptr + n);
    }

    /**
        operator- - Subtraction operator.
     */

    iterator operator- (difference_type n) const
    {
      return iterator (m_ptr - n);
    }

    /**
       operator- - Subtraction operator.
    */
    difference_type operator- (const iterator &rhs) const
    {
      return m_ptr - rhs.m_ptr;
    }

    /**
        operator+= - Addition assignment operator.
     */
    iterator &operator+= (difference_type n)
    {
      m_ptr += n;
      return *this;
    }


    /**
        operator-= - Subtraction assignment operator.
     */
    iterator &operator-= (difference_type n)
    {
      m_ptr -= n;
      return *this;
    }

    /**
        operator== - Equality operator.
     */
    friend bool operator== (const iterator &lhs, const iterator &rhs)
    {
      return lhs.m_ptr == rhs.m_ptr;
    }

    /**
        operator!= - Inequality operator.
     */
    friend bool operator!= (const iterator &lhs, const iterator &rhs)
    {
      return !(lhs == rhs);
    }
   private:
    pointer m_ptr;

  };

  /**
     begin() - Returns an iterator to the beginning of the vector.
     Runtime complexity: O(1).
     */
  iterator begin () noexcept
  {
    return iterator (data ());
  }


  /**
      end() - Returns an iterator to the end of the vector.
      Runtime complexity: O(1).
     */
  iterator end () noexcept
  {
    return iterator (data () + v_size);
  }


  /**
       const_iterator - Constant random access iterator for the vector.
     */
  class const_iterator
  {
    public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    /**
       Default constructor. initializes the iterator with a null pointer.
     */
    const_iterator() : m_ptr(nullptr) {}

    /**
       initializes the iterator with a pointer.
     */
    const_iterator (pointer ptr) : m_ptr (ptr) {}

    /**
       reference operator* () - Dereference operator.
     */
    reference operator* () const { return *m_ptr; }

    /**
       pointer operator-> () - Member access operator.
     */
    pointer operator-> () const { return m_ptr; }

    /**
       operator++ - Prefix increment operator.
     */
    const_iterator &operator++ ()
    {
      m_ptr++;
      return *this;
    }

    /**
       operator++ - Postfix increment operator.
     */
    const_iterator operator++ (int)
    {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
        operator-- - Prefix decrement operator.
     */
    const_iterator &operator-- ()
    {
      m_ptr--;
      return *this;
    }

    /**
        operator-- - Postfix decrement operator.
     */
    const_iterator operator-- (int)
    {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }

    /**
        operator+ - Addition operator.
     */
    const_iterator operator+ (difference_type n) const
    {
      return const_iterator (m_ptr + n);
    }

    /**
        operator- - Subtraction operator.
     */
    const_iterator operator- (difference_type n) const
    {
      return const_iterator (m_ptr - n);
    }

    /**
       operator- - Subtraction operator.
    */
    difference_type operator- (const iterator &rhs) const
    {
      return m_ptr - rhs.m_ptr;
    }

    /**
        operator+= - Addition assignment operator.
     */
    const_iterator &operator+= (difference_type n)
    {
      m_ptr += n;
      return *this;
    }

    /**
        operator-= - Subtraction assignment operator.
     */
    const_iterator &operator-= (difference_type n)
    {
      m_ptr -= n;
      return *this;
    }

    /**
        operator== - Equality operator.
     */
    friend bool operator== (const const_iterator &lhs, const const_iterator &rhs)
    {
      return lhs.m_ptr == rhs.m_ptr;
    }

    /**
        operator!= - Inequality operator.
     */
    friend bool operator!= (const const_iterator &lhs, const const_iterator &rhs)
    {
      return !(lhs == rhs);
    }
   private:
    pointer m_ptr;
  };

    /**
       cbegin() - Returns a const iterator to the beginning of the vector.
       Runtime complexity: O(1).
     */
    const_iterator begin () const noexcept
    {
      return const_iterator (data ());
    }

    /**
        cend() - Returns a const iterator to the end of the vector.
        Runtime complexity: O(1).
     */
    const_iterator end () const noexcept
    {
      return const_iterator (data () + v_size);
    }
    //reverse_iterator - reverse iterator.
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /**
     rbegin() - Returns a reverse iterator to the beginning
     of the vector.
     Runtime complexity: O(1).
   */
    reverse_iterator rbegin () noexcept
    {
      return reverse_iterator (end ());
    }

    /**
       rend() - Returns a reverse iterator to the end of the vector.
       Runtime complexity: O(1).
     */
    reverse_iterator rend () noexcept
    {
      return reverse_iterator (begin ());
    }

    /**
        crbegin() - Returns a const reverse iterator
        to the beginning of the vector.
        Runtime complexity: O(1).
     */
    const_reverse_iterator crbegin () const noexcept
    {
      return const_reverse_iterator (end ());
    }

    /**
       crend() - Returns a const reverse iterator to the end of
       the vector.
       Runtime complexity: O(1).
     */
    const_reverse_iterator crend () const noexcept
    {
      return const_reverse_iterator (begin ());
    }

//<--------Operators---------->

/**  * size() - Returns the number of elements in the vector.
       Runtime complexity: O(1).
 */
  size_t size () const noexcept
  {
    return v_size;
  }

/**  * capacity() - Returns the number of elements that the vector can hold.
       Runtime complexity: O(1).
 */
  size_t capacity () const noexcept
  {
    return v_capacity;
  }

/**  * empty() - Returns whether the vector is empty.
       Runtime complexity: O(1).
  */
  bool empty () const noexcept
  {
    return v_size == 0;
  }

//<--------Element Access and operations---------->

/** * at() - Accesses the element at the specified index with bounds check.
      exception if the index out of range.
      myVector.at(3) = 10; // set the value at index 3 to 10
      Runtime complexity: O(1).
  */
  T &at (size_t index) noexcept (false)
  {
    if (index >= v_size)
    {
      throw std::out_of_range ("Index out of range");
    }
    if (is_on_heap)
    {
      return v_heap_data[index];
    }
    else
    {
      return v_stack_data[index];
    }
  }

/** * at() - const version of the at() function.
      Runtime complexity: O(1).
  */
  const T &
  at (size_t index) const noexcept (false)
  {
    if (index >= v_size)
    {
      throw std::out_of_range ("Index out of range");
    }
    if (is_on_heap)
    {
      return v_heap_data[index];
    }
    else
    {
      return v_stack_data[index];
    }
  }

/** * push_back() - Adds an element to the end.
      Runtime complexity: O(1) amortized (Amortized analysis).
 */

  void push_back (const T &value) noexcept
  {
    if (v_size == v_capacity)
    {
      expand_capacity (1);
    }
    if (is_on_heap)
    {
      v_heap_data[v_size] = value;
    }
    else
    {
      v_stack_data[v_size] = value;
    }
    ++v_size;
  }

/** * An operation that receives an iterator and position
      and add to the left of the position.
      the func return iterator to the new member.
      Runtime complexity: O(n) - number of elements (size).
  */
  iterator insert (iterator position, const T &value) noexcept
  {
    if (v_size == v_capacity)
    {
      size_t shift = std::distance (begin (), position);
      expand_capacity (1);
      position = begin () + shift; // update the position after reallocation
    }
    iterator it = end ();
    while (it != position)
    {
      *it = *(it - 1);
      --it;
    }
    *position = value;
    ++v_size;
    return position;
  }

/** * An operation that receives an iterator and a range of elements and add
      to the left of the position. return an iterator which points to
      the first member From the sequence of the new elements in the new vec.
      Runtime complexity: O(n)- number of elements (size) +
      number of elements in the range [first, last).
  */
  template<class ForwardIterator>
  iterator
  insert (iterator position, ForwardIterator first, ForwardIterator last)
  noexcept
  {
    size_t range = std::distance (first, last);
    if (v_size + range > v_capacity)
    {
      size_t shift = std::distance (begin (), position);
      expand_capacity (range);
      position = begin () + shift; // update the position after reallocation
    }
    iterator end_i = end();
    while (end_i != position)
    {
      *end_i = *(end_i - range);
      --end_i;
    }
    while (first != last)
    {
      *position = *first;
      ++position;
      ++first;
      ++v_size;
    }
    return position;
  }

/** * pop_back() - Removes the last element from the end of the vector.
      If the vector is using dynamic memory allocation and the size is
      back within the static capacity, the elements are copied back to
      the stack memory and the dynamic memory is deleted.
      In the case where the vector is using dynamic memory allocation
      (i.e., when its capacity exceeds the static capacity),
      there's no need to explicitly delete the memory when removing
      elements via pop_back().
      Runtime complexity: O(1) amortized (Amortized analysis).
  */
  void pop_back () noexcept
  {
    if (v_size > 0)
    { // Check if the vector is not empty
      --v_size; // Decrement the size to remove the last element

      // Check if the vector is currently using dynamic memory allocation
      if (is_on_heap)
      {
        // Check if the size is back within the static capacity
        if (v_size <= static_capacity)
        {
          T temp_data[static_capacity]; // Create temporary array on stack
          std::copy (v_heap_data, v_heap_data
                                  + v_size, temp_data); //dynamic to stack
          delete[] v_heap_data; // deleted the dynamically allocated memory
          std::copy (temp_data, temp_data
                                + v_size, v_stack_data); // temp to stack
          v_capacity = static_capacity; // Reset capacity to static capacity
          is_on_heap = false; // Update the flag to indicate stack memory
        }
      }
    }
  }

/** * erase() - Removes the element at the specified position.
      return the right of the position.
      Runtime complexity: O(n)- number of elements (size).
  */
  iterator erase (iterator position) noexcept
  {
    iterator it = position;
    while (it != end () - 1)
    {
      *it = *(it + 1); // shift left
      ++it;
    }
    --v_size; // decrement the size of the vector.
    return position;
  }

/** * erase() - Removes the elements in the range [first, last).
      return an iterator to the organ to the right of the organs Removed.
      Runtime complexity: O(n)- number of elements (size).
  */
  template<class ForwardIterator>
  ForwardIterator
  erase (ForwardIterator first, ForwardIterator last) noexcept
  {
    size_t range = std::distance (first, last);
    ForwardIterator it = first;
    while (it != end () - range)
    {
      *it = *(it + range); // shift left
      ++it;
    }
    v_size -= range; // decrement the size of the vector.
    return first;
  }

/** * clear() - Removes all elements from the vector.
      Runtime complexity: O(n) - number of elements.
  */
  void clear () noexcept
  {
    if (is_on_heap)
    {
      delete[] v_heap_data; // deleted the dynamic memory
      v_heap_data = nullptr;
      is_on_heap = false; // Update the flag to indicate stack memory
      v_capacity = static_capacity; // Reset the capacity to static capacity
    }
    v_size = 0; // Reset the size to zero
  }

/** * data() - Returns a direct pointer to the memory array
      used by the vector now. (Stack or Heap)
      Runtime complexity: O(1).
  */
  T *data () noexcept
  {
    if (is_on_heap)
    {
      return v_heap_data;
    }
    else
    {
      return v_stack_data;
    }
  }

  // By providing both versions (data) , we allow non-const access only when
  // explicitly requested (via the non-const version of data()).
  // We ensure that const objects remain immutable by providing
  // a const-correct interface.
  // This improves the safety and clarity of our code,
  // as it prevents accidental modifications to const objects and enables
  // users to understand the intended usage of our class more easily.

/** * data() - const version of the data() function.
      Runtime complexity: O(1).
  */
  const T *data () const noexcept
  {
    if (is_on_heap)
    {
      return v_heap_data;
    }
    else
    {
      return v_stack_data;
    }
  }


//<--------Operators---------->
/** * operator= - Copy assignment operator.
      Runtime complexity: O(n).
  */
  vl_vector &operator= (const vl_vector &other) noexcept
  {
    if (this != &other) // Check for self-assignment
    {
      if (is_on_heap)
      {
        delete[] v_heap_data; // Deallocate the old heap memory
      }
      v_size = other.v_size;
      v_capacity = other.v_capacity;
      is_on_heap = other.is_on_heap;
      if (is_on_heap) // Check if the other vector is using heap memory
      {
        v_heap_data = new T[v_capacity];
        std::copy (other.v_heap_data,
                   other.v_heap_data + v_size, v_heap_data);
      }
      else
      {
        std::copy (other.v_stack_data,
                   other.v_stack_data + v_size, v_stack_data);
      }
    }
    return *this;
  }

/** * operator[] - Accesses the element at the specified index.
      Runtime complexity: O(1).
  */
  T &operator[] (size_t index) noexcept
  {
    if (is_on_heap)
    {
      return v_heap_data[index];
    }
    else
    {
      return v_stack_data[index];
    }
  }

/** * operator[] - const version of the operator[].
      Runtime complexity: O(1).
  */
  const T &operator[] (size_t index) const
  {
    if (is_on_heap)
    {
      return v_heap_data[index];
    }
    else
    {
      return v_stack_data[index];
    }
  }

/** * operator== - Compares two vectors for equality.
      Runtime complexity: O(n).
  */
  bool operator== (const vl_vector &other) const
  {
    if (v_size != other.v_size)
    {
      return false;
    }
    for (size_t i = 0; i < (size_t) v_size; ++i)
    {
      if (at (i) != other.at (i))
      {
        return false;
      }
    }
    return true;
  }

/** * operator!= - Compares two vectors for inequality.
      Runtime complexity: O(n).
  */
  bool operator!= (const vl_vector &other) const
  {
    return !(*this == other);
  }

 protected:
  size_t v_size;
  size_t v_capacity;
  T v_stack_data[static_capacity]; // Fixed-size array stored on the stack
  T *v_heap_data = nullptr; // Pointer to dynamic memory
  bool is_on_heap; // Flag to indicate whether the vec using dynamic memory.


/** * expand_capacity() - Expands the capacity of the vector.
      Runtime complexity: O(n).
  */

  void expand_capacity (size_t k)
  {
    size_t new_capacity = cap_c (v_size, k, v_capacity);
    T *new_data = new T[new_capacity];
    if (is_on_heap)
    {
      std::copy (v_heap_data, v_heap_data + v_size, new_data);
      delete[] v_heap_data;
      v_heap_data = nullptr;
    }
    else
    {
      std::copy (v_stack_data, v_stack_data + v_size, new_data);
    }
    v_capacity = new_capacity;
    is_on_heap = true;
    v_heap_data = new_data;
  }
/** * cap_c() - Calculates the new capacity of the vector based on
      the current size and the number of elements to add.
      The function uses a formula that approximates
      the optimal capacity for the vector.
      Runtime complexity: O(1).
  */
  size_t cap_c (size_t size, size_t k, size_t C)
  {
    if (size + k <= C)
    {
      // If sum of current size and k is <= to the static capacity C,
      // return C indicating we still in static memory.
      return C;
    }
    else
    {
      // If size + k exceeds C, calculate the dynamic memory capacity with
      // a formula that approximates optimal: (size + k) * 3 / 2.
      return ((size + k) * 3) / 2;
    }
  }
};

#endif //_VL_VECTOR_HPP_
