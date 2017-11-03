<template>
  <view class="view-container">
    <canvas class="canvas" ref="lcanvas"></canvas>
  </view>
</template>

<script>
var WIDTH = 360;
var HEIGHT = 620;
var fishW = 250;             //fish width
var fishH = 255;             //fish height
var velocity = 100;
var fish = [];
var ctx = null;
var ctxbg = null;
var ctxfps = null;
var imageStrip = null;
var fpsMeter = null;
function FpsMeter(initialScore, scoreLabel) {
  this.Reset();
}
/**
 * Class properties
 */
FpsMeter.prototype.fps = 0;                            //how many frames per second are calculated.
FpsMeter.prototype.displayedFps = 0;          //the displayed FPS. This is updated to match fps according to fpsDisplayUpdateFrequency
FpsMeter.prototype.lastFrameTime = new Date();         //time of the last frame
FpsMeter.prototype.timeDelta = .001;                      //milliseconds since the last frame
FpsMeter.prototype.timeDeltaS = .1;                     //seconds since the last frame
FpsMeter.prototype.currentSecond = 0;
FpsMeter.prototype.framesThisSecond = 0;
FpsMeter.prototype.timeDeltaSinceLastFrame = 0;
FpsMeter.prototype.timeFpsDisplayLastChanged = 0;
FpsMeter.prototype.fpsDisplayUpdateFrequency = 500;
FpsMeter.prototype.frames = 0;

FpsMeter.prototype.meterPercent = -1;
FpsMeter.prototype.meterPercentGoal = -1;

/**
 * Reset meter
 */
FpsMeter.prototype.Reset = function() {
  this.fpsResetTime = new Date();
  this.frames = 0;
}

/**
 * Draw meter with updated values
 * @param {Object} score
 */
FpsMeter.prototype.Draw = function(score) {

  var now = new Date();

  /*  Calculate time delta since last time this method was called ---------*/
  this.timeDeltaSinceLastFrame = .001;
  if (this.lastFrameTime != 0)
    this.timeDeltaSinceLastFrame = now - this.lastFrameTime;

  this.timeDeltaS = this.timeDeltaSinceLastFrame / 1000;
  this.lastFrameTime = now;

  /*  Calculate frame rate, since last time this method was called ---------*/
  if (now.getSeconds() == this.currentSecond) {
    this.framesThisSecond++;
  }
  else {
    this.currentSecond = now.getSeconds();
    this.fps = this.framesThisSecond;
    this.framesThisSecond = 1;

    var timingDelayReached = ((now.getTime() - this.timeFpsDisplayLastChanged) > this.fpsDisplayUpdateFrequency);
    var fpsNotChangedYet = (this.timeFpsDisplayLastChanged == 0);

    if (timingDelayReached || fpsNotChangedYet) {
      this.timeFpsDisplayLastChanged = now.getTime();
      this.displayedFps = (this.fps > 60 ? 60 : this.fps);
    }
  }
  if ((now - this.fpsResetTime < 1000)) {
    this.frames++;
    // calculate frames live
    this.fps = Math.min(this.frames / ((now - this.fpsResetTime) / 1000), 60);
    this.displayedFps = this.fps;

  }
  //-----draw the FPS-o-meter--------------------------------------------------------------------------------------------
  // ctxfps.clearRect(0, 0, 110, 110);

  //draw translucent background
  ctxfps.save();
  ctxfps.beginPath();
  // ctxfps.fillStyle = "rgba(75,175,225,.75)";

  this.globalAlpha = .9;
  ctxfps.arc(55, 55, 50, Math.PI, Math.PI * 2, false);
  ctxfps.fill();
  ctxfps.restore();

  //draw meter
  if (this.displayedFps > 1) {
    this.meterPercentGoal = (this.displayedFps > 60 ? 60 : this.displayedFps) / 60; //a maximum of 60fps are drawn so cap the gauge at 60fps.
    if (this.meterPercent == -1) {
      this.meterPercent = .01;
    }

    var delta = Math.abs(this.meterPercent - this.meterPercentGoal);

    if (this.meterPercent < this.meterPercentGoal) {
      this.meterPercent *= 1 + delta / 3;
      if (this.meterPercent > this.meterPercentGoal) {
        this.meterPercent = this.meterPercentGoal;
      }
    }
    else {
      if (this.meterPercent > this.meterPercentGoal) {
        this.meterPercent *= 1 - delta;

        if (this.meterPercent < this.meterPercentGoal) {
          this.meterPercent = this.meterPercentGoal;
        }
      }
    }
    ctxfps.fillStyle = "#FFFF00";
    ctxfps.beginPath();
    ctxfps.arc(55, 55, 50, Math.PI, (Math.PI + Math.PI * (this.meterPercent)), false);
    ctxfps.arc(55, 55, 18, (Math.PI + Math.PI * (this.meterPercent)), Math.PI, true);
    // ctxfps.lineTo(5, 55);
    ctxfps.strokeStyle = "#000000";
    ctxfps.fill();
  }

  //draw line along bottom of the meter
  ctxfps.save();
  ctxfps.beginPath();
  ctxfps.moveTo(5, 55);
  ctxfps.lineWidth = 1;
  ctxfps.lineTo(37, 55);
  ctxfps.arc(55, 55, 18, Math.PI, Math.PI * 2, false);
  ctxfps.lineTo(105, 55);
  ctxfps.globalAlpha = .5;
  ctxfps.strokeStyle = "#000000";
  ctxfps.fillStyle = "#000000";
  ctxfps.stroke();
  this.globalAlpha = 1;
  ctxfps.fill();
  ctxfps.restore();

  //draw needle
  if (this.meterPercent != -1) {
    ctxfps.save();
    ctxfps.globalAlpha = .8;
    ctxfps.beginPath();
    ctxfps.lineWidth = 4;
    ctxfps.arc(55, 55, 3, 0, Math.PI * 2, false);
    ctxfps.moveTo(55, 55);

    ctxfps.lineTo(55 + 50 * -Math.cos(Math.PI * this.meterPercent), 55 + 50 * -Math.sin(Math.PI * this.meterPercent));
    ctxfps.strokeStyle = "#ffffff";
    ctxfps.stroke();
    ctxfps.restore();
  }

  //draw fps
  ctxfps.save();
  ctxfps.fillStyle = "#ffffff";
  ctxfps.globalAlpha = .5;
  ctxfps.font = "15px arial";

  //var fpsString = this.displayedFps+" FPS";
  var fpsString = "";
  var displayedFps = Math.round((this.meterPercent * 60));
  if (this.meterPercent > 0) {
    fpsString = displayedFps + " FPS";
  }
  this.meterFps = displayedFps;

  ctxfps.fillText(fpsString, 0, 80);
  ctxfps.fillText(score + " Fish", 60, 80);
  ctxfps.restore();
}
export default {
  name: 'show',
  mounted() {
    global.Image = Object;
  imageStrip = new Image();
    imageStrip.src = 'https://s10.mogucdn.com//mlcdn/c45406/170921_7j1561gl44h1el99d9fb225ea1008_4000x774.png';
    ctx = this.$refs.lcanvas.getContext("2d")
    ctxfps = ctx
    // this.$refs.canvasfps.initCtx(ctxfps);
    fpsMeter = new FpsMeter(0, "fish");

    this.createFish(100);
    setInterval(
      () => {
        this.draw()
      }, 16.7);
  },
  methods: {
    draw() {
      //clear the canvas
      // ctx.clearRect(0, 0, WIDTH, HEIGHT);
      ctx.fillStyle = "#000000";
      ctx.fillRect(0, 0, WIDTH, HEIGHT);
      var power = Math.min(5, 60);
      if (isNaN(power)) power = 1;
      //velocity = 100 + 100 * (power * power / 3600); //exponential curve between 100-200
      velocity = Math.floor((power * power * .5) / 3) < 1 ? 1 : Math.floor((power * power * .5) / 3);  //exponential curve between 1 and 600.


      // Draw each fish
      for (var fishie in fish) {
        fish[fishie].swim();
      }
      // ctx.postRender();
      fpsMeter.Draw(fish.length);
      ctx.render();
    },
    createFish(num) {
      if (fish.length < num) {
        //add fish
        for (var i = fish.length; i < num; i++) {
          fish.push(new Fish());
        }
      }
      else {
        //remove fish
        fish.splice(num, fish.length - num);
      }
    }
  },
  data() {
    return {
      earth: null,
      sun: null,
      moon: null,
    }
  },
}
function Fish() {

  var angle = Math.PI * 2 * Math.random();                            //set the x,y direction this fish swims
  var xAngle = Math.cos(angle);                                       //set the x value of the angle
  var yAngle = Math.sin(angle);                                       //set the y value of the angle
  var zAngle = 1 + -2 * Math.round(Math.random());                        //set if the fish is swimming toward us or away. 1 = toward us; -1 = away from us
  var x = Math.floor(Math.random() * (WIDTH - fishW) + fishW / 2);    //set the starting x location  
  var y = Math.floor(Math.random() * (HEIGHT - fishH) + fishH / 2);   //set the starting y location  
  var zFar = 100;                                                     //set how far away can a fish go
  var zFarFactor = 1;                                                 //set the max size the fish can be. 1=100%
  var zClose = 0;                                                     //set how near a fish can come
  var z = Math.floor(Math.random() * ((zFar - zClose)));              //set the starting z location 
  var scale = .1;                                                     //set the rate of scaling each frame
  var flip = 1;                                                       //set the direction of the fish. 1=right; -1=left
  var cellCount = 16;                                                 //set the number of cells (columns) in the image strip animation
  var cell = Math.floor(Math.random() * (cellCount - 1));               //set the first cell (columns) of the image strip animation
  var cellReverse = -1;                                               //set which direction we go through the image strip
  var species = Math.floor(Math.random() * 3);                        //set which species of fish this fish is. each species is a row in the image strip 

  // stop fish from swimming straight up or down
  if (angle > Math.PI * 4 / 3 && angle < Math.PI * 5 / 3 || angle > Math.PI * 1 / 3 && angle < Math.PI * 2 / 3) {
    angle = Math.PI * 1 / 3 * Math.random();
    xAngle = Math.cos(angle);
    yAngle = Math.sin(angle);
  }
  // face the fish the right way if angle is between 6 o'clock and 12 o'clock
  if (angle > Math.PI / 2 && angle < Math.PI / 2 * 3) {
    flip = -1;
  }

  // draw the fish each frame -------------------------------------------------------------------------------            
  function swim() {

    // Calculate next position of fish
    var nextX = x + xAngle * velocity * 1;
    var nextY = y + yAngle * velocity * 1;
    var nextZ = z + zAngle * .1 * velocity * 1;
    var nextScale = Math.abs(nextZ) / (zFar - zClose);

    // If fish is going to move off right side of screen
    if (nextX + fishW / 2 * scale > WIDTH) {
      // If angle is between 3 o'clock and 6 o'clock
      if ((angle >= 0 && angle < Math.PI / 2)) {
        angle = Math.PI - angle;
        xAngle = Math.cos(angle);
        yAngle = Math.sin(angle) * Math.random();
        flip = -flip;
      }
      // If angle is between 12 o'clock and 3 o'clock
      else if (angle > Math.PI / 2 * 3) {
        angle = angle - (angle - Math.PI / 2 * 3) * 2
        xAngle = Math.cos(angle);
        yAngle = Math.sin(angle) * Math.random();
        flip = -flip;
      }
    }

    // If fish is going to move off left side of screen
    if (nextX - fishW / 2 * scale < 0) {
      // If angle is between 6 o'clock and 9 o'clock
      if ((angle > Math.PI / 2 && angle < Math.PI)) {
        angle = Math.PI - angle;
        xAngle = Math.cos(angle);
        yAngle = Math.sin(angle) * Math.random();
        flip = -flip;
      }
      // If angle is between 9 o'clock and 12 o'clock
      else if (angle > Math.PI && angle < Math.PI / 2 * 3) {
        angle = angle + (Math.PI / 2 * 3 - angle) * 2
        xAngle = Math.cos(angle);
        yAngle = Math.sin(angle) * Math.random();
        flip = -flip;
      }
    }

    // If fish is going to move off bottom side of screen
    if (nextY + fishH / 2 * scale > HEIGHT) {
      // If angle is between 3 o'clock and 9 o'clock
      if ((angle > 0 && angle < Math.PI)) {
        angle = Math.PI * 2 - angle;
        xAngle = Math.cos(angle);
        yAngle = Math.sin(angle) * Math.random();
      }
    }

    // If fish is going to move off top side of screen
    if (nextY - fishH / 2 * scale < 0) {
      // If angle is between 9 o'clock and 3 o'clock
      if ((angle > Math.PI && angle < Math.PI * 2)) {
        angle = angle - (angle - Math.PI) * 2;
        xAngle = Math.cos(angle);
        yAngle = Math.sin(angle);
      }
    }

    // If fish is going too far (getting too small)
    if (nextZ <= zClose && zAngle < 0) {
      zAngle = -zAngle;

    }
    // If fish is getting to close (getting too large)
    if (((WIDTH / fishW) * 10) < ((fishW * fish.length) / WIDTH)) {
      zFarFactor = .3
    }
    else if (((WIDTH / fishW) * 2) < ((fishW * fish.length) / WIDTH)) {
      zFarFactor = .5
    }
    else { zFarFactor = 1 }

    if (nextZ >= zFar * zFarFactor && zAngle > 0) {
      zAngle = -zAngle;

    }
    if (scale < .1) { scale = .1 }; //don't let fish get too tiny

    //draw the fish
    //locate the fish
    ctx.save();
    ctx.translate(x, y);
    ctx.scale(scale, scale); // make the fish bigger or smaller depending on how far away it is.
    ctx.transform(flip, 0, 0, 1, 0, 0); //make the fish face the way he's swimming.
    ctx.drawImage(imageStrip.src, fishW * cell, fishH * species, fishW, fishH, -fishW / 2, -fishH / 2, fishW, fishH); //draw the fish

    // ctx.drawImage(imageStrip,  fishW * cell/6.55, fishH * species/6.55, fishW/6.55, fishH/6.55, -fishW / 2, -fishH / 2, fishW, fishH); //draw the fish
    ctx.save();
    scale = nextScale // increment scale for next time 
    ctx.restore();
    ctx.restore();

    //increment to next state
    x = nextX;
    y = nextY;
    z = nextZ;
    if (cell >= cellCount - 1 || cell <= 0) { cellReverse = cellReverse * -1; } //go through each cell in the animation
    cell = cell + 1 * cellReverse; //go back down once we hit the end of the animation
  }
  return {
    swim: swim
  }
}

</script>

<style>
.title {
  color: #ff5577;
  font-size: 70
}

.view-container {
  width: 750;
  padding-top: 20;
  padding-bottom: 20;
  border-width: 2;
  border-color: #ccc;
  border-radius: 8;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}

.canvas {
  width: 750;
  height: 1300;
}
</style>