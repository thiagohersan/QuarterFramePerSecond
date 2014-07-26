void AddGlitch(PImage p) {
  p.loadPixels();

  int init = (int)random(p.height*p.width);
  int numRows = (int)random(5, 10);
  int glitchAmount = 0xAAAAAAAA<<(int)random(8);

  for (int r=0; r<numRows; r++) {
    for (int i=r*p.width+init; (i<r*p.width+init+random(128,256))&&(i<p.height*p.width); i++) { 
      p.pixels[i] += glitchAmount;
    }
  }

  p.updatePixels();
}

