String[] lines;
int index = 0;

void setup() {



}

void draw() {
  lines = loadStrings("list.txt");
println("there are " + lines.length + " lines");
for (int i = 0 ; i < lines.length; i++) {
  println(lines[i]);
}
}
void reset(){
lines = loadStrings("list.txt"); //<>//
}
