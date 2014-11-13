PImage img, imgO;

void setup() {
  size(600, 500);
  frameRate(60);
  imgO = loadImage("obama.jpg");
  imageMode(CENTER);
}

void draw() {
  background(255);
  img = new PImage(imgO.getImage());

  int i=0;
  while (i<img.height*img.width) {
    //img.pixels[i] = transparente(img.pixels[i], color(255, 0, 0), 0);
    //img.pixels[i] = pretoBranco(img.pixels[i], color(255, 0, 0), 0);
    //img.pixels[i] = inverso(img.pixels[i], color(255, 0, 0), 0);
    //img.pixels[i] = preto(img.pixels[i], color(255, 0, 0), 0);
    //img.pixels[i] = branco(img.pixels[i], color(255, 0, 0), 0);
    //img.pixels[i] = substituirCor(img.pixels[i], color(255, 0, 0), color(120,255,120), 0);
    //img.pixels[i] = arcoIris(img.pixels[i], color(20, 20, 20), 16);
    i++;
  }
  img.updatePixels();

  image(img, width/2, height/2);
}
