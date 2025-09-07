# Wiener_filter
Implementation of Wiener filter for image noise removal.

The Wiener filter is a statistical noise reduction and deblurring filter in digital image processing that minimizes the mean square error between the estimated and original noiseless image.

The Wiener filter tailors itself to the local image variance.
In the high detailed regions, where the variance is large, the filter performs little smoothing.
In the low detailed regions, Where the variance is small, the filter performs more smoothing.


References:

- "Two-Dimensional Signal and Image Processing", Jae S. Lim, pages 536-40

- https://en.wikipedia.org/wiki/Wiener_filter
