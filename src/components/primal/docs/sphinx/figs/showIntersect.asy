// To turn this Asymptote source file into an image for inclusion in
// Axom's documentation,
// 1. run Asymptote:
//    asy -f png showIntersect.asy
// 2. Optionally, use ImageMagick to convert the white background to transparent:
//    convert showIntersect.asy -transparent white showIntersect.asy

// preamble
settings.render = 6;
import three;
size(6cm, 0);

// axes
draw(O -- 1.7X, arrow=Arrow3(DefaultHead2), L=Label("$x$", position=EndPoint));
draw(O -- 2.4Y, arrow=Arrow3(), L=Label("$y$", position=EndPoint));
draw(O -- 2Z, arrow=Arrow3(), L=Label("$z$", position=EndPoint, align=W));

// triangle 1
path3 tri1 = (1.2,0,0)--(0,1.8,0)--(0,0,1.4)--cycle;

// triangle 2
path3 tri2 = (0,0,0.5)--(0.8,0.1,1.2)--(0.8,1.4,1.2)--cycle;

// ray
path3 ray = (0.4,0.4,0)--(0.4,0.4,1.8);

// polygon of intersection between bbox and triangle
path3 pgon = (0.780952,0.5,0.1)--(0.52381,0.5,0.4)--(0.8,0.0857143,0.4)--(0.8,0.471429,0.1)--cycle;

// draw bounding box and other geometry
draw(box((0.1,-0.23,0.1), (0.8,0.5,0.4)), blue);
draw(pgon, deepblue);

draw(ray, arrow=Arrow3(DefaultHead2), red);
dot((0.4,0.4,0.1), red);
dot((0.4,0.4,0.622222), red);
dot((0.4,0.4,0.85), red);
draw(tri1);
draw(tri2, blue);
draw((0.420779,0.0525974,0.868182)--(0.26449,0.462857,0.731429), deepblue);
draw((0.8,1.4,1.2)--(0.8,1.4,0), dotted);
