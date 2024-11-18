## Separability
2D Fourier transform can be computed like:
```cpp
Mat dft_fwd_2d_separable(const Mat& f) {
    const int M = f.rows;  
    const int N = f.cols;  
  
    Mat F(N, M, CV_32FC2);  
    merge({f, Mat::zeros(N, M, CV_32F)}, F);  
  
    Mat row_buffer(1, M, CV_32FC2);  
  
    for (int v = 0; v < F.rows; v++) {  
        F.row(v).copyTo(row_buffer);  
        dft_fwd_1d_complex(row_buffer).copyTo(F.row(v));  
    }  
  
    for (int u = 0; u < F.cols; u++) {  
        transpose(F.col(u), row_buffer);  
        transpose(dft_fwd_1d_complex(row_buffer), F.col(u));  
    }  
  
    return F;  
}
```
Because
$F(u,v) = \sum_{x=0}^{M-1} e^{-j 2\pi u x / M} \sum_{y=0}^{N-1} f(x,y) e^{-j 2\pi v y / N}$
$= \sum_{x=0}^{M-1} F(x,v) e^{-j 2\pi u x / M}$
$F(x,v)= \sum_{y=0}^{N-1} f(x,y) e^{-j 2\pi v y / N}$
