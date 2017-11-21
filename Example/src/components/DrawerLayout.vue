<template>
  <view>
    <view
      class="container"
      ref="container"
      :style="{marginLeft: changedMarginLeft}"
      @touchstart="onTouchStart"
      @touchend="onTouchEnd"
      @touchcancel="onTouchEnd"
      @touchmove="onTouchMove">
      <slot></slot>
    </view>
    <view
      class="gray"
      :style="{opacity: changedOpacity, width: grayMaskWidth}"
      @click="onOutSideClick"
      @touchstart="onTouchStart"
      @touchend="onTouchEnd"
      @touchcancel="onTouchEnd"
      @touchmove="onTouchMove">
    </view>
  </view>
</template>

<script>
export default {
  name: 'DrawerLayout',
  props: {
    defaultCloseStateMarginLeft: {
      type: Number,
      default: -450
    },
    defaultOpenStateMarginLeft: {
      type: Number,
      default: 0
    },
    defaultCloseStateOpacity: {
      type: Number,
      default: 0
    },
    defaultOpenStateOpacity: {
      type: Number,
      default: 0.5,
    },
    openState: {
      type: String,
      default: 'open'
    },
    closeState:  {
      type: String,
      default: 'close'
    }
  },
  data () {
    return {
      changedMarginLeft: this.defaultCloseStateMarginLeft,
      startX: 0,
      changedOpacity: this.defaultCloseStateOpacity,
      state: this.closeState,
      dragEnable: false,
      maskEnabled: false,
      grayMaskMinWidth: '1',
      grayMaskMaxWidth: '750',
      grayMaskWidth: '1'
    }
  },
  methods: {
    openDrawer() {
      this.state = this.openState
      this.changedMarginLeft = this.defaultOpenStateMarginLeft
      this.changedOpacity = this.defaultOpenStateOpacity
      this.maskEnabled = true
    },
    closeDrawer() {
      this.state = this.closeState
      this.changedMarginLeft = this.defaultCloseStateMarginLeft
      this.changedOpacity = this.defaultCloseStateOpacity
      this.maskEnabled = false
      this.grayMaskWidth = this.grayMaskMinWidth
    },
    onTouchStart(e) {
      this.startX = e.touches[0].clientX
      // Check drag enabled
      if (this.startX > 1 && this.state == this.closeState) {
        this.dragEnable = false
      } else {
        this.dragEnable = true
        this.grayMaskWidth = this.grayMaskMaxWidth
      }
    },
    onTouchEnd(e) {
      if (this.changedMarginLeft > this.defaultCloseStateMarginLeft / 2) {
        this.openDrawer()
      } else {
        this.closeDrawer()
      }
    },
    onTouchMove(e) {
      if (!this.dragEnable) return;
      // Change margin left to drag
      var moveX = e.touches[0].clientX - this.startX
      var tempMarginLeft = 0
      if (this.state == this.openState) {
        tempMarginLeft = moveX + this.defaultOpenStateMarginLeft
      } else if (this.state == this.closeState) {
        tempMarginLeft = moveX + this.defaultCloseStateMarginLeft
      }
      if (tempMarginLeft > this.defaultOpenStateMarginLeft) {
        tempMarginLeft = this.defaultOpenStateMarginLeft
      } else if (tempMarginLeft < this.defaultCloseStateMarginLeft) {
        tempMarginLeft = this.defaultCloseStateMarginLeft
      }
      this.changedMarginLeft = tempMarginLeft
      // Change opacity
      this.changedOpacity =
          (1 - tempMarginLeft / this.defaultCloseStateMarginLeft) / 2
    },
    onOutSideClick(e) {
      if (this.state == this.openState) {
        this.closeDrawer()
      }
    }
  }
}
</script>

<style>
.container {
  display: flex;
  position: fixed;
  z-index: 100;
  width: 450;
  top: 0;
  bottom: 0;
  background-color: #ffffff;
  flex-direction: column;
}
.gray {
  position: fixed;
  z-index: 50;
  background-color: #000000;
  opacity: 0.9;
  width: 750;
  top: 0;
  bottom: 0;
}
</style>
