var ctx;
PImage img;

void setup() {
  size(800, 960);
  ctx = externals.context;
  smooth();
  init();
  background(0);
}

void draw() {
  pushMatrix();
  translate(width, 0);
  scale(-1, 1);//mirror the video
  ctx.drawImage(video, 0, 0, 800, 480); //video is defined outside processing code
  popMatrix();
}


void mousePressed() {
    img=get();
    image(img, 0, height/2);
}

