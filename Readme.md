# Otsus Threshoding 

Implementation for otsus thresholding written in C, heavily inspired by https://www.ipol.im/pub/art/2016/158/?utm_source=doi. 
This implementation is however not specialized for images and can deal with arbitrary `uint64_t` arrays, which is quite helpful when computing thresholds for microarchitectual attacks. 

# Usage
Include the `otsu.h` header and call `otsu_treshold(uint64_t * measures, int len)` if your measurement data is noisy it can help to compile with `-DFILTER_OUTLIER` or set the macro directly in the code to remove outliers. 

# Examples
The `example.c` file contains a small example showing how to use the `otsu.h` header. Build with `make`.