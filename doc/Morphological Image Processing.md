
## As Set Theory
This area can be seen as set theory, where we work with sets of pixels.   
$s : \mathbb{Z}^2$ 
   
```c++  
struct set {  
    float xy[2];};  
```  
  
When dealing with actual color data, the sets are of x + <num_channels>   
values:  
  $s : \mathbb{Z}^{2+c}$
  
```c++  
struct set {  
    float xy[2];    
    uint8_t rgb[3];};  
```  
  
We have two types of sets of pixels: objects and Structuring Elements.   
Objects are sets of foreground pixels. SEs can have background pixels.   
  
SEs sometimes have 'dont care' elements, denoted by $\times$.   

### Set Operations

The reflection of a set is the negation of all its points:
$\hat{B} = \{w | w = -b : b \in B \}$

Translation:
$(B)_z = \{c | c = b + z : b \in B \}$

## Erosion 
Erosion of A by B:
$A \ominus B = \{ z | (B)_z  \subseteq A \}$
So, some translation of each point in B where the output is in A. 
A is a set of foreground pixels
B is a Structuring Element

The input and output are images with foreground pixels embedded in background pixels:
$I \ominus B = \{ z | (B)_z \subseteq A \ and\  A \subseteq I \} \cup \{A^c | A^c \subseteq I \}$
## Dilation  
Reflect B about origin. Then for all translations, paint pixels where b intersects A by at least a point. 
$A \oplus B = \{ z | (\hat{B})_z \cup A \neq \emptyset \}$$
or
$A \oplus B = \{ z\ |\ [(\hat{B})_z \cup A] \subseteq A \}$

Dilation is useful for bridging gaps. So if we have a SE that's just a white square, it would fill in anywhere on the image where it intersects two disjoint parts of foreground. 
## Opening  
Tends to smooth an image. Open A by B:
$A \circ B = (A \ominus B) \oplus B$ 
So, erode A by B, then dilate result by B

The opening of A by B is the union of all the translations of B so that B fits entirely in A

Can also be written as
$A \circ B = \bigcup \{ (B)_z\ |\ (B)_z \subseteq A \}$

The difference between this and plain erosion is that this is a "swept shape" , where all of B
is filled in, as opposed to just the origins of B (which is the case for erosion). 
## Closing
Close A by B:
$A \cdot B = ( A \oplus B) \ominus B$
So, dilate A by B, then erode result by B. 




