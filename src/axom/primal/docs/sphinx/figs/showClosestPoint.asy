// To turn this Asymptote source file into an image for inclusion in
// Axom's documentation,
// 1. run Asymptote:
//    asy -f png showClosestPoint.asy
// 2. Optionally, use ImageMagick to convert the white background to transparent:
//    convert showClosestPoint.png -transparent white showClosestPoint.png

// preamble
settings.render = 6;
import three;
size(6cm, 0);

// axes
draw(-4.5X -- 1.7X, arrow=Arrow3(DefaultHead2), L=Label("$x$", position=EndPoint, align=W));
draw(O -- 2.4Y, arrow=Arrow3(), L=Label("$y$", position=EndPoint));
draw(O -- 2Z, arrow=Arrow3(), L=Label("$z$", position=EndPoint));

// triangle
path3 tri = (1,0,0)--(0,1,0)--(0,0,1)--cycle;

// triangle
triple pto = (0,0,0);
triple pta = (-1,2,1);
triple cpto = (0.333333,0.333333,0.333333);
triple cpta = (0,1,0);
triple ppta = (-1,2,0);
triple pcpto = (0.333333,0.333333,0);
triple pcpta = (0,1,0);

// draw triangle then points and projections
draw(tri);
dot(pto, blue);
label("$o$", pto, align=W);
dot(cpto, mediumblue);
label("$o'$", cpto, align=N);
draw(cpto--pcpto, dotted);
dot(pta, lightolive);
label("$a$", pta, align=W);
draw(pta--ppta, dotted);
dot(cpta, yellow);
label("$a'$", cpta, align=NE);
draw(cpta--pcpta, dotted);
