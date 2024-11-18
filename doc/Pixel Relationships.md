## Neighbors
$N_4(x,y)$ => [(x+1, y), (x-1, y),(x, y+1), (x, y-1)]
$N_d(x,y)$ => [(x + 1, y + 1), (x + 1, y - 1), (x - 1, y + 1), (x - 1, y - 1)]
$N_8(x,y)$ => $N_d \cup N_4$
## Adjacency
Generally, parametrized by the neighbor type.

4-adjecency and 8-adjacency is self explanatory, p and q are adjacent if 
q is in $N_n$ of p. 

m-adjacency is if:
*(a)* q in in $N_4(p)$ or
*(b)* q is in $N_D(p)$ and the $N_4$ of p and q are disjoint within the image foreground (no common foreground pixel). 

A path is a sequence of adjacent pixels leading from $(x_0, y_0)$ to $(x_n, y_n)$.

A closed path is where $(x_0, y_0) = (x_n, y_n)$

A connected set is a set of pixels between which a path exists.

A region is a connected set within the image. $R_i$ and $R_j$ are adjacent if their union forms a connected set. 

## Connected Component Labeling
Algorithm to iterate the image, find all disjoint foreground regions, label them. 

```python
def is_foreground(img, x):
	# ...
	pass

def traverse_region(img, labels, x, connectivity, label):
	q = queue([x])
	labels[x] = label

	while (len(q) > 0):
		pixel = q.pop()		

		for neighbor in connectivity(x):
			if is_foreground(img, neighbor) and unlabeled(neighbor):
				labels[neighbor] = label
				q.push(neighbor)
		
		

def ccl(img, connectivity):
	labels = cv2.copy(img)
	current_label = random_color()
	
	for row in img:
		for pixel in row:
			if is_foreground(img, pixel) and labels[pixel] == [0,0,0]:
				traverse_region(img, labels, pixel, connectivity, current_label)
				current_label = random_color()                                                                                                                                                                                                                                                              
		
```




