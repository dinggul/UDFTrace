# UDFTrace

User defined function tracer

Currently for x86-64.

### Prerequisites

- Python2
- PyYAML
- libyaml-cpp-dev (0.5.2)
- libboost-dev
- Pin

    sudo pip2 install pyyaml colorlog
    sudo apt install libboost-dev libyaml-cpp-dev
    ./install_pin.sh

### Test

    ./pin*/pin -t src/obj-intel64/UDFTrace.so -- ./test/test1

### Intermediate trace format

```c
struct type {
    uint8_t kind;       // argument type.
    uint8_t location;   // argument location (stack or register).
    uint16_t stack_ofs; // used for stack arguments.
    uint32_t mem_size;  // used for memory arguments.
};

struct func {
    uint64_t addr;
    uint32_t nargs;
    struct type ret_ty;
    struct type arg_ty[0]; // 'nargs' elements.
};
```

### Credits

made by dinggul, blukat29
