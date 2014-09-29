PGraphics pg;
PImage img, mask;
float fv;
String serverAddress = "http://www.thiagohersan.com/scripts/";
long nextFlash;
int stayWhiteCount;

void setup() {
  size(600, 480);
  frameRate(60);
  pg = createGraphics(400, 480);
  serverAddress = "";
  img = loadImage(serverAddress+"FIESP00_00.png");
  mask = loadImage(serverAddress+"FIESP00_00_mask.png");
  fv = 0;
  nextFlash = millis()+2000;
  stayWhiteCount = 0;
}

void flash() {
  fv = 1.0;
  stayWhiteCount = 0;
}

void draw() {
  if (millis() > nextFlash) {
    nextFlash = millis()+4000;
    flash();
  }

  pg.beginDraw();
  pg.background(0);
  pg.pushMatrix();
  pg.translate(pg.width/1.4, pg.height/2);
  pg.background(abs(fv));
  pg.popMatrix();
  pg.endDraw();

  pushMatrix();
  scale(0.8);

  if (fv > 0.0) {
    fv = min(fv+180.0, 255.0);
    if ((fv >= 255) && (stayWhiteCount>4)) fv = -255.0;
    else if (fv >= 255) stayWhiteCount++;
  }
  else if (fv < 0.0) {
    fv = min(fv+60.0, 0.0);
  }

  background(0);
  pushMatrix();
  translate(width/2, height/2);
  image(pg, -pg.width/2, -pg.height/2.15);
  tint(255, 128);
  image(img, -img.width/2, -img.height/2.15);
  tint(255, 255);
  image(mask, -mask.width/2, -mask.height/2.15);
  popMatrix();

  popMatrix();
}

