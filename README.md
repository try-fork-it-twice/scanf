# scanf

FreeRTOS profile grabber

## Update FreeRTOS-Kernel submodule

To fetch the latest FreeRTOS-Kernel source code execute the following command:
```bash
git submodule update --init --recursive
```

## Build and run example

To build and run the example task on FreeRTOS execute the following commands:

```bash
mkdir -p build
cd build
cmake ..
make
./example
```
