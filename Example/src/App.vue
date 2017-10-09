<template>
  <view class="container" id="app">
    <xcanvas class="canvas" ref="canvas"></xcanvas>
  </view>
</template>

<script>
var ctx;
export default {
  components: {
  },
  data() {
    return {
      process: 0,
      img:null,
      current: 1,
      data: [{ value: 80 }, { value: 80 }, { value: 80 }, { value: 80 }, { value: 80 }, { value: 80 }]
    }
  },
  mounted() {
    
     ctx = this.$refs.canvas.getContext("2d");
    this.init();



// 先绘制目标图形在原始画布上
ctx.fillStyle="#FF4040";
ctx.fillRect(0,20,75,50);

// 配置混合模式为destination-over，之后绘制的图形会在另一块临时的画布上
ctx.globalCompositeOperation="xor";

// 在临时画布上绘制我们的图形
ctx.fillStyle="#7D9EC0";
ctx.fillRect(30,50,75,50);
console.log(ctx.fillStyle)

// 将属性置为默认的source-over，此时会将临时画布上图形通过destination-over混合到原始画布上，之后的图形就是正常绘制了，没有配置那么图像混合将会无效
ctx.globalCompositeOperation="source-over";

ctx.draw()
setTimeout(() => {
  console.log("kaishi")
  var data = ctx.getImageData(200,200,100,100);

  console.log(data.data.length)
}, 3000);
    
  },
  methods:{
    init(){
      if (typeof global.Image === 'undefined') {
      global.Image = function() {
        Object.defineProperty(this, "src", {
          get: function() {
            return this._src;
          },
          set: function(newVal) {
            this._src = newVal;
            if (typeof this.onload !== 'undefined') {
              this.onload();
            }
          }
        });
      };
      global.Image.prototype = {
        constructor: global.Image,
      }
    }
    ctx.draw = () => {
          ctx.render();
      };
      ctx.update = () => {
          ctx.appendUpdate();
      };
      ctx.drawImage = function() {
        var len = arguments.length;
        if (len == 5) {
          ctx.loadImage(arguments[0].src, arguments[1], arguments[2], arguments[3], arguments[4])
        } else if (len == 9) {
          ctx.loadImage(arguments[0].src, arguments[1], arguments[2], arguments[3], arguments[4],
            arguments[5], arguments[6], arguments[7], arguments[8])
        }
      }
    },
  startDraw() {
      ctx.clearRect(0,0,750,1080)
      ctx.lineWidth = 18;
      ctx.strokeStyle = "#ccc9c9";
      // ctx.drawImage(this.img, 0, 0,100,100);
      // ctx.drawImage(this.img, 200, 0,200,200);
      // ctx.drawImage(this.img, 400, 0,200,200);
      // ctx.drawImage(this.img, 600), height + 450),200),200));
      ctx.drawImage(this.img, 0, 0, 10, 10, 0, 0 ,100,100);
      ctx.draw();
    }
  }
}
</script>

<style>
.canvas {
  width: 750;
  height: 1800;
}
.container {
  flex-direction: column;
  justify-content: center;
  align-items: center;
  width: 750;
  height: 1800;
}
.name {
  color:#bbbbbb;
  font-size: 70;
  align-self: center;
  font-weight: bold;
  margin-top: 30;
  margin-bottom: 30;
}
.setting {
  color:#000000;
  font-size: 40;
  margin-left: 20;
  margin-top: 20;
  margin-bottom: 20;
}
.line {
  background-color:#dddddd;
  height: 1;
  align-self: stretch;
}
</style>
