PImage img;

void setup() {
  size(400,600);
}  


void draw() {
  img = loadImage("01.jpg");
  image(img, 0, 0);
  
}
void reset(){
img = loadImage("01.jpg");
image(img, 0, 0);
}
