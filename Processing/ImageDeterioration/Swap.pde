void SwapStripes(PImage p) {
  p.loadPixels();
  // swap Hor
  if (random(2) < 1) {
    SwapHere(p, 0, (int)random(p.height), 0, (int)random(p.height), p.width, (int)random(8,16));
  }
  // swap Ver
  else {
    SwapHere(p, (int)random(p.width), 0, (int)random(p.width), 0, (int)random(8,16), p.height);
  }
  p.updatePixels();
}

void SwapAround(PImage p) {
  p.loadPixels();

  // pick swap width and height
  int sw = (int)random(16, 32);
  int sh = (int)random(16, 32);

  // pick A points
  int ax = (int)random(0, p.width);
  int ay = (int)random(0, p.height);

  // pick B points
  int bx = ax;
  int by = ay;

  if (random(2) < 1) {
    bx = ax + sw;
  }
  else {
    by = ay + sh;
  }

  SwapHere(p, ax, ay, bx, by, sw, sh);

  p.updatePixels();
}

void SwapHere(PImage p, int fromX, int fromY, int toX, int toY, int sW, int sH) {
  for (int i=0; i<sH; i++) {
    for (int j=0; (j<sW)&&(p.width>fromX+j)&&(p.width>toX+j)&&(p.height>fromY+i)&&(p.height>toY+i); j++) {
      color tC = p.pixels[(fromY+i)*p.width+(fromX+j)];
      p.pixels[(fromY+i)*p.width+(fromX+j)] = p.pixels[(toY+i)*p.width+(toX+j)];
      p.pixels[(toY+i)*p.width+(toX+j)] = tC;
    }
  }
}

