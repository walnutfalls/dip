## Variables
Image $f(x,y)$ is an M by N image, where $L$ is that max intensity value.

For discrete (integer) textures:
$$
r_k \text{ in } [0, L-1] \text{The list of all discrete values}
$$
Histogram:
$$
h(r_k) = n_k \text{ for } k \text{ in } [0,L-1]
$$
$n_k$ is number of pixels with intensity $r_k$

Normalized histogram:
$$
p(r_k)=\frac{h(r_k)}{MN} = \frac{n_k}{MN}
$$

So, fraction over total number of pixels of pixels at an intensity $r_k$
$s = T(r)$
$s_k = T(r_k)$

Choose T(R) as the cumulative probability function multiplied by L-1:
$$
s = T(r) = (L-1)\int_{a}^{b}p_r(w)dw
$$

So that's out transform function. If you're just dealing with a discrete array, you sum the probabilities for levels 0...r, multiply by L-1, and that gives you $s_k$. The rest of the cleverness is there to show that $s_k$ has uniformly  distributed PDF. 

We can say that 
$$
p_s(s) = p_r(r)|\frac{dr}{ds}|
$$
Because calculus. T transforms r -> s. p(s) then depends on p(r). 
$$
\frac{ds}{dr} = \frac{dT(r)}{dr}
$$
$$
=(L-1)\frac{d}{dr} \int_{0}^{r} p_r(w)dw
$$

So, notice that this is just deriving over dx a dx integral, because x here is "r"...?
$$
\frac{ds}{dr} = (L-1)p_r(r) 
$$
$$
p_s(s)=p_r(r)|\frac{dr}{ds} = \frac{1}{L-1}
$$
## Histogram Matching
Suppose you have another image z, with $p_z(z)$. Calculate the T(z) for that just like histogram equalization, except store a value->key map. 

Equalize the input image as normal. Then, assign colors like
$$
z = G^{-1}(s)
$$
