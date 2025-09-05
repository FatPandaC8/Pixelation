## Steps I Have Done

- Load images
- Reduce resolution with block averaging
- Quantize (palette reduction)
- Dither using Floyd-Steinberg

## Next:
- Upscale
- Thread pool fix
- Optimize all the algorithms for faster compiling

## TODO: More dithering algorithm:
- Jarvis, Judice, and Ninke (JJN) – higher quality, bigger kernel
- Stucki dithering – better edges, less noisy
- Burkes dithering – good compromise between speed & quality
- Ordered dithering (Bayer matrix) – faster, uses threshold matrices
- Blue-noise dithering – produces very clean results, used in modern graphics

## TODO: More color quantization:
- Median-cut algorithm (used in GIF compression)
- Octree quantization – fast and memory-efficient
- Principal Component Analysis (PCA) – reduces RGB space intelligently
- k-means++ initialization – speeds up your clustering

## TODO: More processing + streaming:
- Stream images on-the-fly instead of keeping everything in RAM.
- Use producer-consumer threads:
    - One thread loads images.
    - Another processes them.
    - Another saves results.
- Allow video input and process frames in real-time.

## TODO: Add CLI & config support

## TODO: Add unit testing

## TODO: Logging and benchmarking
