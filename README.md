# UDFTrace

user defined function tracer

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

    ./pin*/pin -t src/UDFTrace.so -- ./testprog/test1

### made by dinggul, blukat29
