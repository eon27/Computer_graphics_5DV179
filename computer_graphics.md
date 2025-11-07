Project:
    C++ 11
    Any external libs
    Must compile on CS linux computers

    Oral presentation:
        Done in groups of 5 students
        Reserve slot online
        Demonstate the software, functionality, system design (and implementation)

# Lecture 1:
## Rasterazation:
rasterazation is the converstion from lines and triangles into screen pixels

We look into the scene from a single point and compute the color for each visable pixel

We use shading to approximate illumination but no raytracing to calculate the acctual lightning.

### Raytracing and Pathtracing

### The 3d scene:
- Objects
- Camera (viewer)
- Light source(s)

### Object specification:
Building blocks:
- vertex -> Edges -> Face/Polygon -> Mesh
Optional:
- Vertex normals (for face normals)
- Texture coordinates

Material properties:
- Defines how objects reflect light
- ex. Opaque, transparen, translucent
        
### Synthetic camera model:
Pinhole camera where the view originates from a single point
Use trigonometry to find projection at point (x,y,z)
    
### Light source:
Point light: light bulb
Spotlight: Light cone
Directional light: Sun
Area light: Larger lamp with a surface

Has a color (intensity/temprature) and often a position
        
Light color + object material = pixel color

### Global vs local illumination
In local illumination we compute color or shade of each object independently
        
### Scene graphs:
If we can represent all the elements of a scene, we can represent the complete scene by a tree called a scene graph

Often some sort of tree: octtree, BSP tree, directed acyclic graph
We could write a generic traversal to render the scene:
- Find what is visible -> fast rendering

## Mesh storage formats
What do we want to achieve:
- Small storage cost
- Fast access to vertex position
- Fast traversal
- Minimal vertex processing
- Fast clipping and filling of polygons
        
### Data structures for meshes
Storage cost
Efficiency
(In c++ use std::vector)

Examples:
- Triangle list
- Triangle strip
- Indexed triangle mesh

### Triangle list
Vertex data is stored for each triangle. Then we have a vector where the first three elements are the 3 vertices of the first triangle

Simplest data structure but duplicated vertices

### Triangle strip
The first three vertices are for the first triangle then the last vertex is also used for the next triangle

No duplication and efficient but complicated preprocessing to represent objects with strips

### Indexed triangle mesh
Seperate vertex data and polygon index

Vertex data is only stored once but we need an index list

`Vertices = {v0,v1,v2,v3,v4,...}`

`Index list = {0,1,2,2,3,4,...}`

We construc triangles from the index list by referencing vertices by index.  

Low storage cost, easy to implement, used by modern graphics api

# Lecture 1,5:
## OpenGL
A computer graphics standard API. Version 3.3 is required for the project but you may use up to 4.4

### Portable
- Consistent visual display regardless of hardware, OS, and windows systems
- Callable from C,C++,Python, Perl, Java, etc
- Runs on all major OS and windows systems

It is well documented with the latest version being 4.6

[Reference cards]()


### GLFW
OpenGL is window independent so we need another system for handeling windows, keybord input and events.

GLFW is open source multiplatform library for OpenGL

## Using OpenGL

### OpenGL building blocks
- Shader processors
- Shader program
- OpenGL graphical context
- OpenGL Objects
- Buffer Object data: Data normally in single percisition floats
- Primitives
- OpenGL buffers

### Shaders
Programmable shaders that are highly customizable, enables GPGPU programming.

Shaders:
- Vertex shader
- Fragment shader
- Tessellation shader
- Geometry shader
- Compute shader

Pipeline:
    Take vertices and do the tansformation on them (to their geometric position). Then assemble the meshes with edges from the information. Then rasterize to find the pixles along the edges and mesh then find the color for the pixels.

    Vertex Data -> Vertex shader -> Tessellation control shader -> Tessellator -> Tessellation evaluation shader -> Geometry shader -> Primitive assembly -> Rasterizer -> Fragment shader -> Depth & Stencil -> Color buffer blend -> Dither -> Frame buffer

### Vertex processor
- Object coordiantes
- World scene coordiantes
- Camera coordinates
- Normal device coordinates
- Screen coordinates

Every Change of coordinates is equivalent to a matrix transformation.

Vertex colors may be computed

### Clipper and primitive assembler
Center of Perspective projects to Back clipping plane. Infront of the view plane that is the screen is a front clipping plane. The things that are rendered are in between the back clipping and front clipping planes.

### Rasterizer
If an object is inside the clipping volume then the razterizer produces a set of fragments for the object. Fragments are potential viewable pixels.

### Fragment processor
Fragments are processed to determined the color of the corresponding pixel in the frame buffer. Then we project everything down to a flat screen.

### Shader languages
First introduced by nvidia geforce 3
Running on the GPU

Specialized programming languages
- OpenGL Shader Language (GLSL)
- High level shader language (HLSL), DirectX
- Metal Unified Graphics and Compute Language
- Cg (deppricated), Nvidia
- etc (Game engines often have a dialect of these)

### GLSL
C-style language. Shader files are linked and compiled by the application

Files usually end with .glsl (or .vert or .frag)

The main() function is the entry point

Input and OUtput variables must be declared globally (before main())

Arrays stored in column-major order

Documentation found in the OpenGL reference pages

### GLSL Built-in funcitons
- Trigonometric functions
- Normalize
- Cross and dot product
- Reflect, refract
- Distance- Clamp, max, min
- etc.

### GLSL datatypes
- Primatives: float, bool, int
- Vectors: vec and ivec
- Matrices: mat

(vec and mat are floating point)

### Vector and Scalar components
- {x,y,z,w} Positions and vectors
- {r,g,b,a} Colors
- {s,t,p,q} Texture coordinates

x = r = s, but they are used to denote what the vector represents.

### GLSL qualifiers
- In, out: Specify in and out parameters to a shader program
- const: Compile time read
- uniform: Global variable for the shader program. They are uniform and wont change during runtime.

### Tessellation shaders
Creates meshes from parametric surfaces. This can control the level of detail (LOD) with subdivision of surfaces and displacement mapping.

### Geometric shaders
- Per-primitive processing
- Create vertices and meshes
- Particle systems
- Surface textures

### Fragment shaders
- Computing colors and texture coordinates per fragment
- Texture application
- Fog computation

### Vertex shader 
A vertex shader must atleast set the variable `gl_Position`. It usually transformas the vertex with the model-view and projection matrices.

The vertex shader
```
in vec4 Vertex;
uniform mat4 ProjectionMatrix;
out vec4 color;

void main() {
    color = vec4(1.0, 1.0, 0.0, 0.5);
   gl_Position = ProjectionMatrix * Vertex;
}
```

In the fragment shader
```
in vec4 color;
out vec4 fColor;

void main() {
    fColor = vec4(color.rg, 0.0, 1.0);
}
```

Accessing the shader attributes: to get the id pointer to a shader variable we use: `glGetAttribLocation()`

### Shader Program
An application can have several shader programs. Each program has its own set of shader files.

We select which program to use with `glUseProgram()`

### OpenGL context
We need to have an active context to render to screen, contexts are local to each application. Contexts are seperated from each other but can share objects between them. One application can have multiple contexts.

### OpenGL objects
Major OpenGL objects:
- Vertex Array Objects: A VAO is used to encapsulate a set of buffer objects
- Buffer objects: Store the data. Bound to a specific target eg, GL_ARRAY_BUFFER
- Index buffer

### OpenGL primitive types
Polygons, triangles, render primitives

# Lecture 2
Linear algebra stuff, check slides.

## Projective geometry  
## Homogenous coordinates
for the line L => ax+by+c=0 we can write the line with a 3-vector as (a,b,c)

and a point P is written with (x,y,1)

Any point on a line fullfills the equation L^T^P = 0

Scaling a line in a homogenous coordinate system doesnt change the line because multiplying both sides of a line equation is the same line. 

If we do the same for a point we dont change its 2d coordinate but we can see it as we are moving the point along a line in 3d (see lecture 2 slide 58). Therefore we can represent a point (u,v,w) with (u/w,v/w)

### Point at infinity
A point with the homogenous coordinate (u,v,0), u and v become infinitly large so it becomes a direction that is defined by the ratio between u and v. 

### Intersection

To find the intersection of two points in the homogenous coordinate system we find the point P that fulfills:

L^T^P = 0

L2^T^P = 0

This is done by taking the cross product between the lines. Because we get a vector pointing orthogonal to both lines which is a line that points out or inwards in the 3d space which looks like a point in 2d.

### Intersection of parallell lines
If we do the same for two parallell lines we get a point at infinity

(0,1,-4) x (0,1,-1) we get the vector (3,0,0)

## Frames
lecture 2 slide 68
### The canonical frame
Consider the canonical frame 

    (1,0,0)
F = (0,1,0)
    
    (0,0,1)

It is the identity matrix with the directions x,y and the point (0,0)

### Geometric transformations
Using homogenous coordinates all important transformations may be written as matrix-vector multiplications

## Carteisian VS Homogenous
summary found on page 73

## Transformations
An object is a set of points.

In 2d the transformation matrix is 3x3

In 3d the transformation matrix is 4x4

### Translation in 2d
Moving points in a uniform direction

Vector v and Point p

the transformaed point q is q = T(v)p

We put the transformation vector v in the last column of a standard matrix and multiply it by the points then the points gets shifted by the vector.

### Scaling and reflection
Isotropic scaling: The same in all directions
anisotropic: One size of scaling in each direction

Isotropic we put the same value in ((a,0,0),(0,a,0),(0,0,1))

anisotropic ((a,0,0),(0,b,0),(0,0,1))

### Reflection
Reflection we inverse x or y or both

A reflection of both x and y in an odd number of coordinate axels, left hand coordinates becomes right handed but with even number of axels it is the same as a rotation in 180^o^

The inverse reflection is the transpose of the reflection matrix. So we can flip back and forth by always tansposing the current reflection matrix.

### Shearing in 2d
Shearing is done by scaling x by a factor of y or y by a factor of x.
Hxy or Hyx where 

Hxy = ((1,a,0),(0,1,0),(0,0,1))

Hyx = ((1,0,0),(a,1,0),(0,0,1))

### Rotation in 2d
R(a) rotates a point counter clockwise by an angle of a

R(a) = ((cosa,-sina,0),(sina,cosa,0),(0,0,1))

The inverse rotation is R^T^

### Concatenation of transformations
We want to do multiple transformations one after another, We can multiply all transformations one efter another to get a resulting matrix that does all the transformations at the same time.

Remember: Matrix multiplication is not comutative, so the order of the transformations matter.

Example: To rotate about a point that is not at the origin. We have the rotation matrix but it can only rotate about origin. So we must first move all points so the point of rotation is at the origin and then rotate and then move the points back.

The inverse of multiple transformations is inverse transformations in the reverse order

transformation ABC has the inverse C^-1 B^-1 A^-1