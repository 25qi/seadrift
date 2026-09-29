let thisd = 0.5;
let statu=0;
let expw=500;
let exph=500;
let consist=10.5;
let counter = 1;

let count = 1000;//線條數量1000
var particles_a = [];
var particles_b = [];
var particles_c = [];
var fade = 200;//線條尾部褪色的速度200 越小越快
var radius =1;//顆粒大小5

let w = expw; //長出區域大小300
let h = exph;//長出區域大小300

let noiseScale = 3000; //彎來彎去的大小300,1000
let noiseStrength = 50; //noise的幅度1.2,60

//function setup() {
//  //frameRate(40)
//    createCanvas(windowWidth, windowHeight);
//    //pixelDensity(2);
//    noStroke();
//    fill(0);
  

//}


//function draw() {
  
//}

let Particle = function(loc_, dir_, speed_) {
  this.loc = loc_;
  this.dir = dir_;
  this.speed = speed_;
  this.d = 0.5;//流動速度4
}

Particle.prototype.run = function() {
  this.move();
  this.checkEdges();
  this.update();
}

Particle.prototype.update = function(r) {
  ellipse(this.loc.x, this.loc.y, r);
}

Particle.prototype.checkEdges = function() {
  if (this.loc.x < 50 || this.loc.x > 80 || this.loc.y < 50 || this.loc.y > 80) {
     //if (this.loc.x < 0 || this.loc.x > width || this.loc.y < 0 || this.loc.y > height) {
    //this.loc.x = random(w) + windowWidth*0.5 - w*0.5;
    //this.loc.y = random(h) + windowHeight*0.5 - h*0.5;
    this.loc.x = random(w) + windowWidth*0.65 - w*0.5;
    this.loc.y = random(h) + windowHeight*0.5 - h*0.5;
  }
}

Particle.prototype.move = function() {
  this.angle = noise(this.loc.x/noiseScale, this.loc.y/noiseScale, frameCount/noiseScale) * TWO_PI*noiseStrength;
  this.dir.x = cos(this.angle) + sin(this.angle) - sin(this.angle);
  this.dir.y = sin(this.angle) - cos(this.angle)*sin(this.angle);
  this.vel = this.dir.copy();
  this.vel.mult(this.speed*thisd);
  this.loc.add(this.vel);
}

function keyPressed() {
  if (statu ==0) {statu=1;counter=100;} 
  else {statu=0;}
  
  
}

function change(){
  if (statu==0) {
    thisd += (0.5 - thisd)*0.002;
    radius += (1 - radius)*0.002;
    noiseStrength += (50 - noiseStrength)*0.002;
    noiseScale += (3000 - noiseScale)*0.002;
    w += (expw - w)*0.002;
    h += (exph - h)*0.002;
    consist += (25.5 - consist)*0.002;
    
  } 
  else {
    //thisd += (8 - thisd)*0.2;
    //radius += (1.5 - radius)*0.2;
    //noiseStrength += (80 - noiseStrength)*0.2;
    //noiseScale += (2000 - noiseScale)*0.2;
    //w += (expw*0.65 - w)*0.2;
    //h += (exph*0.65 - h)*0.2;
    //consist += (35 - consist)*0.2;
    
    thisd += (8 - thisd)*0.2;
    radius += (1.5 - radius)*0.2;
    noiseStrength += (80 - noiseStrength)*0.2;
    noiseScale += (2000 - noiseScale)*0.2;
    w += (expw*0.65 - w)*0.2;
    h += (exph*0.65 - h)*0.2;
    consist += (35 - consist)*0.2;
    if (counter>0){counter-=0.1;}
    if (counter<0.1){statu=0;}
  }
  
}
