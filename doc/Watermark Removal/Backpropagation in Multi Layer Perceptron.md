Squared difference Loss:
$$ \huge {
C_0 = \sum_{j=0}^{n-1} (a_j^L - y_j)^2
}$$
Loss of jth node in layer L:
$$
\huge {
C_0^j = (a_j^L - y_j)^2
}
$$
$z_j^l$ the input into j'th row of layer l from weighted activations of l-1:
$$
\huge{
	z_j^l = \sum_{k=0}^{n-1} w_{jk}^l a_k^{l-1}
}
$$

$a_j^l$ the activation in node j of layer l:
$$
\huge {
a_j^l = g^l (z_j^l)
}
$$

## $C_0$ as Composition of Functions
$$
\huge {
C_0^j = (a_j^L - y_j)^2
}
$$
But if we look at y as a parameter that helps define the function. 