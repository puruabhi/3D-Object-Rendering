# 2D Vector Rendition of 3D Objects

Development of an Inkscape Extension (using Python) or a standalone system for personalized 2D vector form rendition (to .svg files) of a 3D voxelized object (from .obj files) using color/material file information, transformation, different choices of projection, lighting, transparency etc.

## Installation
Make sure you have `Inkscape` installed in your system.
```
sudo apt install inkscape
```
Then copy `polyhedron_3d.py` and `polyhedron_3d.inx` to folder `/usr/share/inkscape/extensions/`.

## Face Color

Color of faces can be specified in a line with starting letter "c" followed by the values of r, g and b before defining faces. 
>For coloring a face with **white** it can be specified as
```
c 255 255 255
```
## Face Transparency

Transparency of faces can be specified in a line with starting letter "t" follwed by the values of fill-transparency and stroke-transparency in percent. It should also be specified before the definition of faces.
>For making a face 20% fill-transparent (80% opaque) and 40% stroke-transparent (60% opaque) it can be specified as
```
t 20 40
```
#### Project given by our instructor [Dr. Ranita Biswas](https://www.facebook.com/ranitabiswas87?fref=ts).
