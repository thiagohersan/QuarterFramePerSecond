PImage myImage;

void setup() {
  background(0);
  //myImage = loadImage("sea.jpg");
  //myImage = loadImage("lethalweapon.png");
  myImage = loadImage("deusdiabo02.jpg");
  size(myImage.width, myImage.height);
}

void draw() {
  //Deresolute(myImage);
  //ScratchDeet(myImage);
  //SwapAround(myImage);
  //SwapStripes(myImage);
  //AddGlitch(myImage);
  image(myImage, 0, 0);

  if (frameCount%100 == 1) {
    println(frameRate);
  }
}

void keyReleased() {
  switch(key) {
  case '1' : 
    {
      Deresolute(myImage);
      break;
    } 
  case '2' : 
    {
      ScratchDeet(myImage);
      break;
    }
  case '3' :
    {
      SwapAround(myImage);
      break;
    }
  case '4' :
    {
      SwapStripes(myImage);
      break;
    }
  case '5' :
    {
      AddGlitch(myImage);
      break;
    }
  }
}

