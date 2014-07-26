void Deresolute(PImage p) {
  p.loadPixels();

  // pick starting point
  int sx = (int)random(0, p.width);
  int sy = (int)random(0, p.height);

  // pick area size
  int ss = (int)random(32, 64);

  // pick number of pixels to average
  int ps = (int)random(2, 8);

  // double double rainbow!! iteration over the area
  for (int i=sy; i<sy+ss; i+=ps) {
    for (int j=sx; j<sx+ss; j+=ps) {
      float rSum, gSum, bSum;
      rSum = gSum = bSum = 0;

      // sum values over a ps-by-ps pixel window
      for (int ii=0; ii<ps; ii++) {
        for (int jj=0; jj<ps&&(p.height>i+ii)&&(p.width>j+jj); jj++) {
          rSum += red(p.pixels[(i+ii)*p.width+j+jj]);
          gSum += green(p.pixels[(i+ii)*p.width+j+jj]);
          bSum += blue(p.pixels[(i+ii)*p.width+j+jj]);
        }
      }

      // average color for window
      color myC = color(rSum/(ps*ps), gSum/(ps*ps), bSum/(ps*ps));

      // set color over entire window
      for (int ii=0; ii<ps; ii++) {
        for (int jj=0; jj<ps&&(p.height>i+ii)&&(p.width>j+jj); jj++) {
          p.pixels[(i+ii)*p.width+j+jj] = myC;
        }
      }
      ////
    }
  }
  ////
  p.updatePixels();
}


