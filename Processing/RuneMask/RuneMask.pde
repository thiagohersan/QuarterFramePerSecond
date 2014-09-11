BuildingMask mMask;
PGraphics mCanvas;

void setup() {
  size(1024, 768);
  mMask = new BuildingMask("mask.png");

  PImage t = createImage(214, 167, ARGB);
  for (int y=0; y<t.height; y++) {
    int cy = (y/16)%2;
    for (int x=0; x<t.width; x++) {
      int cx = (x/16+cy)%2;
      t.pixels[y*t.width+x] = color(cx*255);
    }
  }

  mCanvas = createGraphics(t.width, t.height);
  mCanvas.beginDraw();
  mCanvas.background(1);
  mCanvas.image(t, 0, 0);
  mCanvas.endDraw();
}

void draw() {
  background(1);
  rect(37, 259, mMask.maske.width, mMask.maske.height);
  image(mMask.maske, 37, 259);
  image(mCanvas, 400, 14);
  mMask.applyCanvas(mCanvas);
  image(mMask.maskeCanvas, 400, 259);
}

