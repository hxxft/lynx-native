<template>
  <view class="container" id="app">
    <view class="card-wrap">
      <img class="card" ref="img1" src="pic_lynx.png"/>
      <img class="card" ref="img2" :style="{zIndex:100}" src="pic_cat.png"/>
      <img class="card" ref="img3" src="pic_tiger.png"/>
      <img class="card" ref="img4" src="pic_lion.png"/>
    </view>
    <view class="btn_container">
      <button
          class="btn"
          v-for="action in actions"
          :key="action"
          :text="action"
          background="#2198f2"
          foreground="#fff"
          :font-size="30"
          font-weight="bold"
          @click="clickHandler(action)">
      </button>
    </view>
  </view>
</template>

<style>
.card {
  height: 250;
  width: 350;
  margin-top: 16;
  margin-left: 16;
}

.card-wrap {
  width: 750;
  flex-wrap: wrap;
}

.btn {
  margin-top: 16;
  height: 100;
  width: 300;
}

.btn_container {
  width: 750;
  justify-content: space-around;
  align-items: center; 
  flex-wrap: wrap;
}

.container {
  flex-direction: column;
  justify-content: center;
  align-items: center;
}
</style>

<script>
import { Button } from 'lynx-components'
export default{
  components: {
    Button,
  },
  data() {
    return {
      actions: ['SPIN', 'TRANSLATE', 'SCALE', 'OPACITY']
    }
  },
  methods: {
    clickHandler(action) {
      switch(action) {
        case 'SPIN':
          this.spin()
          break;
        case 'TRANSLATE':
          this.translate()
          break;
        case 'SCALE':
          this.scale();
          break;
        case 'OPACITY':
          this.opacity();
          break;
        default: break;
      }
    },
    spin() {
      this.$refs.img1.animate([
        {
          offset: 0,
          transform: {
            rotateY: 0,
            perspective: 500
          }
        },
        {
          offset: 1,
          transform: {
            rotateY: 360,
            perspective: 500
          }
        }
      ], {
        duration: 2000,
        easing: 'linear'
      });
    },
    translate() {
      this.$refs.img3.animate([
        {
          offset: 0,
          transform: {
            translateX: 0
          },
          easing: 'ease-out'
        },
        {
          offset: 0.5,
          transform: {
            // https://hxxft.github.io/lynx-book/animation/animation.html
            // 需要做屏幕适配运算
            translateX: (350 + 16) * screen.width / 750
          },
          easing: 'ease-out'
        },
        {
          offset: 1,
          transform: {
            translateX: 0
          }
        }
      ], {
        duration: 2000
      });
      this.$refs.img4.animate([
        {
          offset: 0,
          transform: {
            translateX: 0
          },
          easing: 'ease-out'
        },
        {
          offset: 0.5,
          transform: {
            translateX: -(350 + 16) * screen.width / 750
          },
          easing: 'ease-out'
        },
        {
          offset: 1,
          transform: {
            translateX: 0
          }
        }
      ], {
        duration: 2000
      });
    },
    opacity() {
      this.$refs.img4.animate([
        {
          offset: 0,
          opacity: 1
        },
        {
          offset: 0.5,
          opacity: 0
        },
        {
          offset: 1,
          opacity: 1
        }
      ], {
        duration: 2000
      });
    },
    scale() {
      this.$refs.img2.animate([
        {
          offset: 0,
          transform: {
            translateX: 0,
            translateY: 0,
            scaleX: 1,
            scaleY: 1
          },
          easing: 'ease-out'
        },
        {
          offset: 0.5,
          transform: {
            translateX: -(350 + 16) / 2 * screen.width / 750,
            translateY: (250 + 16) / 2 * screen.width / 750,
            scaleX: 1,
            scaleY: 1
          },
          easing: 'ease-out'
        },
        {
          offset: 1,
          transform: {
            translateX: -(350 + 16) / 2 * screen.width / 750,
            translateY: (250 + 16) / 2 * screen.width / 750,
            scaleX: 2,
            scaleY: 2
          },
          easing: 'ease-out'
        }
      ], {
        duration: 2000,
        iterations: 2,
        direction: 'alternate',
        fill: 'both'
      });
    }
  }
}
</script>
