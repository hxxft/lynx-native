<template>
  <view class="container" id="app">
    <view class="btn_container">
      <button
          class="btn"
          text="PRESS ME"
          background="#2198f2"
          foreground="#fff"
          :border-radius="2"
          :font-size="30"
          font-weight="bold"
          border-color="#ef3"
          :onclick="clickHandler">
        </button>
      </view>
  </view>
</template>

<style>
  .btn {
    height: 100;
    flex: 1
  }

  .btn_container {
    width: 750;
    background-color: #f5f8f8;
    justify-content: center;
    align-items: center; 
    padding-left: 20;
    padding-right: 20;
    padding-top: 20;
    padding-bottom: 20
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
  methods: {
    clickHandler (e) {
      console.log(e);
    }
  }
}
</script>
