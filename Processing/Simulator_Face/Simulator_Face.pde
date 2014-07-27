PGraphics pg;
PImage img, mask;
PImage pic;

float flashValue = 0.0;
//String serverAddress = "http://www.thiagohersan.com/scripts/";
String serverAddress = "";
long nextFlash;
int stayWhiteCount = 0;

boolean fadePicture = false;

ArrayList<Integer> colorsToFade; 
ArrayList<Integer> colorsToRand;
int[] frame;

State mState = State.WAITING;

void setup() {
  size(900, 683);
  pg = createGraphics(400, 600);
  img = loadImage(serverAddress+"FIESP00_01s.png");
  mask = loadImage(serverAddress+"FIESP00_01s_mask.png");
  pic = loadImage(serverAddress+"foto1.jpg");
  pic.resize(600, 600);

  colorsToFade = new ArrayList<Integer>();
  colorsToRand = new ArrayList<Integer>();

  //find a random color to start
  int rColor = pic.pixels[(int)random(pic.pixels.length)];
  findSimiliarColors(rColor, pic);

  nextFlash = millis()+2000;
}

void draw() {
  // state transitions
  if (mState == State.WAITING) {
    if (millis() > nextFlash) {
      mState = State.FLASHING_IN;
      flashValue = 1.0;
      stayWhiteCount = 0;
    }
  }
  else if (mState == State.FLASHING_IN) {
    flashValue = min(flashValue+20.0, 255.0);
    if ((flashValue >= 255) && (stayWhiteCount>32)) {
      stayWhiteCount = 0;
      flashValue = -255.0;
      mState = State.FLASHING_OUT;
    }
    else if (flashValue >= 255) {
      stayWhiteCount++;
    }
  }
  else if (mState == State.FLASHING_OUT) {
    flashValue = min(flashValue+20.0, 0.0);
    if (flashValue >= 0.0) {
      mState = State.FADING_PICTURE_IN;
    }
  }
  else if (mState == State.FADING_PICTURE_IN) {
    flashValue = min(flashValue+20.0, 255.0);
    if ((flashValue >= 255) && (stayWhiteCount>10)) {
      stayWhiteCount = 0;
      flashValue = -255.0;
      mState = State.FADING_PICTURE_OUT;
    }
    else if (flashValue >= 255) {
      stayWhiteCount++;
    }
  }
  else if (mState == State.FADING_PICTURE_OUT) {
    // do something
    mState = State.CLEARING_PICTURE;
  }
  else if (mState == State.CLEARING_PICTURE) {
    flashValue = min(flashValue+20.0, 0.0);
    if (flashValue >= 0.0) {
      mState = State.WAITING;
      nextFlash = millis()+2000;
    }
  }

  // drawing
  if ((mState == State.WAITING) || (mState == State.FLASHING_IN) || (mState == State.FLASHING_OUT)) {
    pg.beginDraw();
    pg.background(abs(flashValue));
    pg.endDraw();
  }
  else if ((mState == State.FADING_PICTURE_IN) || (mState == State.FADING_PICTURE_OUT) || (mState == State.CLEARING_PICTURE)) {
    pg.beginDraw();
    pg.background(0);
    pg.tint(abs(flashValue));
    pg.image(pic, 0, 0, 600, 600);
    pg.endDraw();
  }


  if (fadePicture) {
    //fadeImage util there is no more colors to random 
    if (fadeImage(pic)) {
      fadePicture = false; //flag back to false so it can flash
      //reload a pic
      pic = loadImage("foto" + int(random(1, 7) )+ ".jpg");
      pic.resize(600, 600);

      // first color to Fade
      int rColor = pic.pixels[(int)random(pic.pixels.length)];
      //find a color to fade
      findSimiliarColors(rColor, pic);
    }
  }


  background(0);
  pushMatrix();
  translate(width/2, height/2);
  tint(255, 255);
  image(pg, -pg.width/2, -pg.height/2);
  popMatrix();
  tint(255, 100);
  image(img, 0, 0);
  tint(255, 255);
  //image(mask, 0, 0);
}

boolean fadeImage(PImage p) {
  boolean randColor = false;

  p.loadPixels();

  for (Integer i : colorsToFade) {
    int pixelColor =  p.pixels[i];
    int r = (pixelColor >> 16) & 0xff;
    int g = (pixelColor >> 8) & 0xff;
    int b = pixelColor & 0xff;

    r = min(r+6, 255);
    g = min(g+6, 255);
    b = min(b+6, 255);
    randColor = (r >= 255 && g >= 255 && b >= 255);
    p.pixels[i] = color(r, g, b);
  }

  if (randColor && colorsToRand.size() > 0) {
    Integer rColor = (Integer)colorsToRand.get((int)random(colorsToRand.size()));
    findSimiliarColors(rColor, pic);
  }

  p.updatePixels();

  return (colorsToRand.size() < 10);
}


void findSimiliarColors(int c, PImage p) {
  colorsToFade.clear();
  colorsToRand.clear();

  p.loadPixels();

  for (int i = 0; i < p.pixels.length; i++) {

    int pixelColor = p.pixels[i];
    int r = (pixelColor >> 16) & 0xff;
    int g = (pixelColor >> 8) & 0xff;
    int b = pixelColor & 0xff;

    pixelColor = c;

    int r2 = (pixelColor >> 16) & 0xff;
    int g2 = (pixelColor >> 8) & 0xff;
    int b2 = pixelColor & 0xff;


    if (sqrt(pow(r2-r, 2) + pow(g2-g, 2) + pow(b2-b, 2) ) < 150) {
      colorsToFade.add(i);
    }
    else {
      if (colorsToRand.contains(p.pixels[i]) == false ) {
        colorsToRand.add(p.pixels[i]);
      }
    }
  }
}

