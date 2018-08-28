// To turn this Asymptote source file into an image for inclusion in
// Axom's documentation,
// 1. run Asymptote:
//    asy -f png showDistance.asy
// 2. Optionally, use ImageMagick to convert the white background to transparent:
//    convert showDistance.png -transparent white showDistance.png

// preamble
settings.render = 6;
import three;
size(6cm, 0);

// axes
draw(O -- 1.3X, arrow=Arrow3(DefaultHead2), L=Label("$x$", position=EndPoint, align=W));
draw(O -- 1.8Y, arrow=Arrow3(), L=Label("$y$", position=EndPoint));
draw(O -- 1.2Z, arrow=Arrow3(), L=Label("$z$", position=EndPoint, align=W));

// query point
triple q = (0.75,1.2,0.4);
// other primitives
triple boxpt = (0.4,0.3,0.7);
triple p = (0.2,1.4,1.1);
dot(q);    dot(p, blue);
draw((1.1,0,0.2)--(1.1,0.5,0.2), blue);
draw((0.2,-0.3,0.4)--(0.25,-0.1,0.3)--(0.3,-0.3,0.35)--cycle, blue);
draw(box((-0.3,-0.2,0.7), (0.4,0.3,0.9)), blue);

// distances and drop-points
draw(q--p, L=Label("0.8325"));
draw(q--(1.1,0.5,0.2), L=Label("0.6525"));
draw(q--(0.25,-0.1,0.3), L=Label("1.95"));
draw(q--boxpt, L=Label("1.0225"));
draw(q--(0.75,1.2,0), dotted);
draw(p--(0.2,1.4,0), dotted);
draw((1.1,0.5,0.2)--(1.1,0.5,0), dotted);
draw((0.25,-0.1,0.3)--(0.25,-0.1,0), dotted);
draw(boxpt--(0.4,0.3,0), dotted);
