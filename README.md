# gaussian blur opencl
C++ implementation of Gaussian Blur on PNG images with OpenCL on GPU.


### SIGMA: 1 | FILTER SIZE: 5x5

```python
Image size: 6678966 B [6.36 MB]
Image dimensions: 3000 x 1453 

[NFO] Time spent transfering data : 0.008
[NFO] Time spent on preparation : 1.393
[NFO] Time spent on blurring : 0.011
[NFO] Time spent on cleanup : 0.163
[NFO] Full time spent by OpenCL part: 1.573
```

```python
Image size: 126207384 B [120 MB]
Image dimensions: 9000 x 3500 

[NFO] Time spent transfering data : 0.026
[NFO] Time spent on preparation : 1.413
[NFO] Time spent on blurring : 0.064
[NFO] Time spent on cleanup : 0.163
[NFO] Full time spent by OpenCL part: 1.646
[NFO] Time spent on conversion and saving: 44.368
[NFO] Total time spent: 50.348

```

### SIGMA 1 | FILTER SIZE: 20x20

```python
Image size: 6678966 B [6.36 MB]
Image dimensions: 3000 x 1453 

[NFO] Time spent transfering data : 0.008
[NFO] Time spent on preparation : 1.137
[NFO] Time spent on blurring : 0.068
[NFO] Time spent on cleanup : 0.184
[NFO] Full time spent by OpenCL part: 1.393
[NFO] Time spent on conversion and saving: 9.847
[NFO] Total time spent: 14.138
```

```
Image size: 126207384 B [120 MB]
Image dimensions: 9000 x 3500 

[NFO] Time spent transfering data : 0.026
[NFO] Time spent on preparation : 1.405
[NFO] Time spent on blurring : 0.551
[NFO] Time spent on cleanup : 0.182
[NFO] Full time spent by OpenCL part: 2.143
[NFO] Time spent on conversion and saving: 45.035
[NFO] Total time spent: 51.512

```
