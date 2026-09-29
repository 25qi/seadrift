let result;
function preload() {
   result = loadStrings('data/test.txt');
  
}

function setup() {
  
 //background(200);
  
  
}

function draw(){
 
  text(result, 20, 20, 200, 200);
  
//    lines = loadStrings("list.txt");
//println("there are " + lines.length + " lines");
//for (int i = 0 ; i < lines.length; i++) {
//  println(lines[i]);
redraw();
  
//}
}
function resetSketch() {
  //function preload(){
    result = loadStrings('data/test.txt');
  
  //text(result, 10, 10, 80, 80);
}
