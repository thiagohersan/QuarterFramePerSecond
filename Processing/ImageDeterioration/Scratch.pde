void ScratchDeet(PImage p) {
  p.loadPixels();

  // where to scratch
  int sx = (int)random(0, p.width);
  int sy = (int)random(0, p.height);

  // while c < minimum number of scratches and
  //       within image bounds (sx<width, sy<height)
  for (int c=0; (sx<p.width)&&(sy<p.height)&&(c<(int)random(48,64)); c++) {
    // pick either H or V scratch
    if ((random(2) > 1)&&(sx<p.width)) {
      VertScratch(p, sx, sy);
      sx++;
    }
    else if (sy<p.height) {
      HorScratch(p, sx, sy);
      sy++;
    }
  }
  p.updatePixels();
}

// add a horizontal scratch to image
//   scratch = replace pixel values by a running average of their colors
void HorScratch(PImage p, int col, int row) {
  float rSum, gSum, bSum;
  rSum = gSum = bSum = 0;

  // random start-finish points for a scratch on this row
  int x0 = col + (int)random(-16,16);  
  int x1 = x0 + (int)random(-64, 64);
  // make sure it's longer than 32 pixels (arbitrary)
  while (abs (x0-x1) < 32) {
    x1 = x0 + (int)random(-64, 64);
  }

  // magia!
  for (int i=max(0,min(x0,x1)),c=1;i<min(p.width,max(x0,x1));i++,c++) {
    rSum += red(p.pixels[row*p.width+i]);
    gSum += green(p.pixels[row*p.width+i]);
    bSum += blue(p.pixels[row*p.width+i]);
    p.pixels[row*p.width+i] = color(rSum/c, gSum/c, bSum/c);
  }
}

// add a vertical scratch to image
//   scratch = replace pixel values by a running average of their colors
void VertScratch(PImage p, int col, int row) {
  float rSum, gSum, bSum;
  rSum = gSum = bSum = 0;

  // random start-finish points for a scratch on this column
  int y0 = row + (int)random(-16,16);
  int y1 = y0 + (int)random(-64, 64);
    // make sure it's longer than 32 pixels (arbitrary)
  while (abs (y0-y1) < 32) {
    y1 = y0 + (int)random(-64, 64);
  }

  // magia!
  for (int i=max(0,min(y0,y1)),c=1;i<min(p.height,max(y0,y1));i++,c++) {
    rSum += red(p.pixels[i*p.width+col]);
    gSum += green(p.pixels[i*p.width+col]);
    bSum += blue(p.pixels[i*p.width+col]);
    p.pixels[i*p.width+col] = color(rSum/c, gSum/c, bSum/c);
  }
}


