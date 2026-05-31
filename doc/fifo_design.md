# FIFO design

We start out simple with a First In First Out (FIFO) buffer. For this solution, I assume a single thread to write data into the buffer (also called the publisher), and a single thread which reads from the buffer (called the consumer).

When I talk about threads, I include interrupts as commonly used in microcontrollers.

## Needs

* Exchange data between interrupt context and a different interrupt context (or main).

  For a single producer/single consumer buffer, this can be achieved by implementing the data buffer as a circular buffer. The publisher only writes new data to the queue, modifying the (atomic) write pointer. On the other hand, the consumer reads data and thereby only (atomically) modifies the read pointer.

  A clear **disadvantage** of this solution is data old data can be overwritten, this will depend on the logic of the publisher.

* Able to store primative data types and user defined objects.

  In case of objects, we need to manage object lifetimes.

## API

The basic API is based on generic sequence containers as defined in the C++ STL.

* `capacity` and `size` to return the maximum and current number of elements stored;
* `clear` to reset the buffer;
* `emplace` to create a new element in-place;
* `empty` to indicate if there is data in the queue;
* ~`front` to access the first element;~
* `full` to indicate the buffer is full and old entries will be overwritten;
* `pop` to remove the oldest element from the buffer;
* `push` to add a new element to the buffer;

### Producer

The following API calls are meant for the producer:

* `emplace` to create a new element in-place;
* `full` to indicate the buffer is full and old entries will be overwritten;
* `push` to add a new element to the buffer;

```
@startuml
autoactivate on

participant Producer as p
database    FIFO     as fifo
participant Consumer as c

alt FIFO has space available
  p -> fifo: full()
  return False
  p -> fifo: push(data)
  return True
else FIFO is full
  p -> fifo: full
  return True
end
@enduml
```

### Consumer

The following API calls are meant for the consumer:

* ~`front` to access the first element;~
* `empty` to indicate if there is data in the queue;
* `pop` to remove the oldest element from the buffer;

```
@startuml
autoactivate on

participant Producer as p
database    FIFO     as fifo
participant Consumer as c

alt FIFO has data available
  loop While not empty
    c -> fifo: empty()
    return False
    c -> fifo: front()
    return data
    c -> fifo: pop()
    return
  end
else FIFO is empty
  c -> fifo: empty()
  return True
end

@enduml
```

## Details

### Aligned storage

To store objects of arbitrary size, a separate storage type is introduced. This type ensured there is enough memory allocated, and using the correct alignment, for the object to store. The buffer itself is an array of this storage type.

### Distinguish between empty and full

The internal buffer is sized one element larger then required, to allow the buffer to differentiate between empty and full.

## References

1. [Why is std::aligned_storage to be deprecated in C++23 and what to use instead?](https://stackoverflow.com/questions/71828288/why-is-stdaligned-storage-to-be-deprecated-in-c23-and-what-to-use-instead)

1. [What's the best way to have aligned storage for an object so you can do placement new (and do explicit d'tor calls later)?](https://www.reddit.com/r/cpp/comments/1dree7m/whats_the_best_way_to_have_aligned_storage_for_an/)


# TODO

- cache line size for cortex-m
https://en.wikipedia.org/wiki/ARM_Cortex-M#Silicon_customization
  -> optional for M7, M35P, M52, M55, M85
  - size: 32bytes