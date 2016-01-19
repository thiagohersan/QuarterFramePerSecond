import java.awt.image.*; 
import javax.imageio.*;
import java.net.*;
import java.io.*;

// Port we are receiving.
int port = 9100; 
DatagramSocket ds; 
// A byte array to read into (max size of 65536, could be smaller)
byte[] buffer = new byte[50*50*3]; 

PImage video;

void setup() {
  size(500, 500);
  try {
    ds = new DatagramSocket(port);
  } 
  catch (SocketException e) {
    e.printStackTrace();
  } 
  video = createImage(50, 50, RGB);
}

void draw() {
  // checkForImage() is blocking, stay tuned for threaded example!
  checkForImage();

  // Draw the image
  background(0);
  image(video, 0, 0, width, height);
}

void checkForImage() {
  DatagramPacket p = new DatagramPacket(buffer, buffer.length); 
  try {
    ds.receive(p);
    //String str = new String(buffer);
    //if (str.split("<")[0].equals("S")) {

    //  int size = int(str.split("<")[1]);
    //  byte[] nBuff = new byte[size]; 

    //  p = new DatagramPacket(nBuff, nBuff.length); 
    //  try {
    //    ds.receive(p);
    //  } 
    //  catch (IOException e) {
    //    e.printStackTrace();
    //  }
    //}
  } 
  catch (IOException e) {
    e.printStackTrace();
  } 

  byte[] data = p.getData();


  // We need to unpack JPG and put it in the PImage video
  video.loadPixels();
  try {


    for (int i=0; i<video.width; i++)
    {
      for (int j=0; j<video.height; j++)
      {
        int id = (i*3)+(j*video.width*3);        
        int r = data[id];
        int g = data[id+1];
        int b = data[id+3];
        video.pixels[i+video.width*j] = color(r, g, b);
      }
    }
  } 
  catch (Exception e) {
    e.printStackTrace();
  }
  // Update the PImage pixels
  video.updatePixels();
}