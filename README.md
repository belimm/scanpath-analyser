# scanpath-analyser
Practice with graph data structure, and basic graph operations.




<h2>Overview</h2>

Eye tracking does not only allow us to understand which areas of interest (AOIs) of visual stimuli take the most attention, but also allows us to understand in which order these AOIs are used. Figure 1 shows an example of an eye movement sequence (i.e. scanpath) of a particular person on the home page of the Babylon website. The rectangles show the AOIs of the web page, and the circles show where the person looked at. The radius of each circle is directly proportional to the duration of its corresponding fixation, and the numbers in the circles show the sequence of the fixations.

<img width="855" alt="image" src="https://user-images.githubusercontent.com/76854271/156815475-cda215d2-2334-4c88-b910-c18a4ffbea13.png">


<h2>Functional Requirements</h2>

1) Create Vertices: according the file name read the string representation of scanpaths and create graph data structure with different vertex for each scanpath.

2) Create Edges: computes the similarity between each pair of the scanpaths of the graphs taking from createVertices function.

<img width="491" alt="image" src="https://user-images.githubusercontent.com/76854271/156815519-617ca5fe-8629-4d3a-8ed5-07463bdcd732.png">


