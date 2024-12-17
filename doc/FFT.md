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

### FFT
$$
F(u) = F_{even}(u) + W_N^kF_{odd}(u)
$$

This can be used to do a divide and conquer algorithm for 1d Fourier Transform.

Conceptually, you split the array down to size 1, where F(u) = f(u). Then, use the above relationship to reassemble arrays of 2, 4, 8...

Actually, you can do this in place without recursion by rearranging the array. 
![[butterfly.png]]


### four1
### Inverse transform using forward fft
To get the inverse transform, we get the complex conjugates of all the pixels, and do a forward fft on that. Then we take a complex conjugate of the result. 

But if the input is real, the conjugate on the final fft output would have no effect one way or the other, because the imaginary terms would have to be near 0 either way.. right?

four1 takes a `isign` parameter, which gets multiplied into the exponent of $W$ twiddle factors. This controls the sign of the imaginary term in the result. This is a part of Progressive Twiddle Factor update, where `isign` is multiplied into $\Delta \theta$ (there's a section on Progressive Twiddle Factor Update below).

In this project, I ended up writing fft within a `twiddle_lookup` class, where twiddle factors are pre-computed. For the `ifft` I just negated the imaginary terms of the input, and took the real terms of the output after putting the conjugate $F$ through forward fft. Eventually, I got this to work. 

Setting `isign=-1` also worked pretty well when using the original `four1`. However, manually getting the conjugate of $F$ and then running it through forward `four1` resulted in a noisy image.. floating point errors?

##### Progressive Twiddle Factor Update
This is a trick that `four1` uses to get the twiddle factors with only 2 calls to `sin()`.

As we iterate through the first half of the samples we want to calculate:
$$
W_N^{k} = e^{-2 i \pi k / N }
$$
Note that at first, for $k=0$ , $W_n^{0} = (1, 0)$ 
In general:
$\theta = -2 \pi k / N$
$W_n^k = cos(\theta) + i sin(\theta)$

Note that as we iterate k in $[0, K)$ we are just adding $\Delta \theta$ to $\theta$:
$\Delta \theta = \frac{-2 \pi}{N}$
$$
\theta_t = \theta_{t-1} + (-2 \pi / N) = \theta_{t-1} + \Delta \theta
$$
So, say we have $W_N^{k}$ and we want $W_N^{k+1}$ . We actually want $e^{i(\theta + \Delta \theta)}$ or $e^{i \theta} e^{i \Delta \theta}$

We use trig identities to progressively get $W_N^{k+1}$ without many trig calls:
$cos(a + b) = cos(a)cos(b) - sin(a)sin(b)$
$cos(\theta + \Delta \theta) = cos(\theta)cos(\Delta \theta) - sin(\theta)sin(\Delta \theta)$
$cos(2 \theta) = 1 - 2 sin^2(\theta)$


$e^{i \Delta \theta} = cos(\Delta \theta) + i sin(\Delta \theta)$
$e^{i \theta + \Delta \theta} = e^{i \theta}(cos(\Delta \theta) + i sin(\Delta \theta))$
$cos(\Delta \theta) = 1 - 2 sin^2(0.5 \Delta \theta)$
$e^{i \theta + \Delta \theta} = e^{i \theta}([1 - 2 sin^2(0.5 \Delta \theta)] + i \cdot sin( \Delta \theta))$
$e^{i \theta + \Delta \theta} = (e^{i \theta})[1 - 2 sin^2(0.5 \Delta \theta)] + (e^{i \theta}) i \cdot sin(\Delta \theta)$
$e^{i \theta + \Delta \theta} = (e^{i \theta}) - (e^{i \theta}) 2 sin^2(0.5 \Delta \theta) + (e^{i \theta}) i \cdot sin(\Delta \theta)$
$e^{i \theta + \Delta \theta} = e^{i \theta} + e^{i \theta} \Big[ -2 sin^2(0.5 \Delta \theta) + i \cdot sin(\Delta \theta)  \Big]  )$

This is the derivation for the update of the twiddle factor, which in modern c++ looks like:
```c++
const double theta = -sign * (CV_2PI / fft_N);  
const double wtemp = std::sin(0.5 * theta);  
std::complex<double> wp{ -2.0 * wtemp * wtemp, std::sin(theta) };
...
w = w + w*wp
```

And in old four1 C code looks like
```c++
istep = mmax << 1; theta=isign*(6.28318530717959/mmax);
wtemp = sin(0.5*theta);
wpr = -2.0*wtemp*wtemp;
wpi = sin(theta);
...
wr = (wtemp=wr)*wpr - wi*wpi + wr;
wi = wi*wpr  +wtemp*wpi + wi;
```

#### Twiddle Table
When four1 was written (probably the 70s?) we might care about just having a huge lookup table W for any N and k. But we surely don't anymore, and we can just cache W values for any N and k up to some limit. For that, I wrote the `twiddle_table` class. 