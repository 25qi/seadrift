let bgc = 255;
let cacheline;

function preload() {
  // line = loadStrings('data/list.txt');
  //cacheline = line[0];
}

function setup() {
  createCanvas(displayWidth, displayHeight);
  myFont = loadFont("data/Anonymous_Pro_Minus.ttf");
  //pixelDensity(0.5);

  for (let i = 0; i < count; i++) {
    let loc_a = createVector(
      random(w) + windowWidth * 0.5 - w * 0.5,
      random(h) + windowHeight * 0.5 - h * 0.5,
      2
    );
    let angle_a = random(TWO_PI);
    let dir_a = createVector(cos(angle_a), sin(angle_a));

    let loc_b = createVector(
      random(w) + windowWidth * 0.5 - w * 0.5,
      random(h) + windowHeight * 0.5 - h * 0.5,
      2
    );
    let angle_b = random(TWO_PI);
    let dir_b = createVector(cos(angle_b), sin(angle_b));

    let loc_c = createVector(
      random(w) + windowWidth * 0.5 - w * 0.5,
      random(h) + windowHeight * 0.5 - h * 0.5,
      2
    );
    let angle_c = random(TWO_PI);
    let dir_c = createVector(cos(angle_c), sin(angle_c));

    particles_a[i] = new Particle(loc_a, dir_a, consist); //最後一個參數決定線條的連續分散程度，越小越連續
    particles_b[i] = new Particle(loc_b, dir_b, consist);
    particles_c[i] = new Particle(loc_c, dir_c, consist);
 
  }
}
function draw() {
  loadStrings("data/list.txt", loadtext);

  //background(0);
  //fill(bgc, 255, 255)
  textSize(0.0185 * windowWidth);
  textFont(myFont);
  text(
    "Send time: " + line[0],
    windowWidth * 0.2,
    height * 0.5 - 3.5 * windowWidth * 0.028
  );
  text(
    "Acceleration: " + line[1] + ", " + line[2] + ", " + line[3],
    windowWidth * 0.2,
    height * 0.5 - 2.5 * windowWidth * 0.028
  );
  text(
    "Gyroscope: " + line[4] + ", " + line[5] + ", " + line[6],
    windowWidth * 0.2,
    height * 0.5 - 1.5 * windowWidth * 0.028
  );
  text(
    "Atomospheric Pressure: " + line[7],
    windowWidth * 0.2,
    height * 0.5 - 0.5 * windowWidth * 0.028
  );
  text(
    "Temperature: " + line[8],
    windowWidth * 0.2,
    height * 0.5 + 0.5 * windowWidth * 0.028
  );
  text(
    "Altitude: " + line[9],
    windowWidth * 0.2,
    height * 0.5 + 1.5 * windowWidth * 0.028
  );
  text(
    "Waveheight: " + line[10],
    windowWidth * 0.2,
    height * 0.5 + 2.5 * windowWidth * 0.028
  );
  //text('statu'+statu+' counter'+counter, width*0.2, height*0.5+ 4 *windowWidth*0.028)
  text(
    "Location: " + line[11] + "E " + line[12] + "N ",
    windowWidth * 0.2,
    height * 0.5 + 3.5 * windowWidth * 0.028
  );

  //字行差height*0.05=41.15, font大小=width*0.02=28.8, 差=font大小*1.4=width*0.028
  //字位置：height*0.5- x *width*0.028

  //fill(0,0,0,0)
  //stroke(255);
  //circle(width*0.2+500, height*0.5, height*0.6);

  //if(line[4]!=var1){change();var1=line[4];}else{var1=line[4];}

  fill(0, 10);
  noStroke();
  rect(0, 0, width, height);

  for (let i = 0; i < count; i++) {
    fill(101, 101, 191, fade);
    particles_a[i].move();
    particles_a[i].update(radius);
    particles_a[i].checkEdges();

    fill(66, 143, 166, fade);
    particles_b[i].move();
    particles_b[i].update(radius);
    particles_b[i].checkEdges();

    fill(222, 238, 234, fade);
    particles_c[i].move();
    particles_c[i].update(radius);
    particles_c[i].checkEdges();
  }

  if (line[0] != cacheline) {
    statu = 1;
    counter = 100;
  }
  cacheline = line[0];
  change();
}

function loadtext(result) {
  //if(result!=line){changed();}
  line = result;
}
