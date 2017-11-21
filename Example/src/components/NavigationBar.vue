<template>
  <view class="container">
    <img class="icon" src="icon_back_white.png"></img>
    <label class="title">{{title}}</label>
  </view>
</template>

<script>
export default {
  name: 'NavigationBar',
  props: {
  },
  data () {
    return {
      title:'Lynx Demo'
    }
  },
  methods: {

  }
}
</script>

<style>
.container {
  width: 750;
  flex-direction: column;
}
.header {
  flex-direction: row;
  position: absolute;
  background-color: #42b983;
  z-index: 10;
  height: 100;
  align-items: center;
}
.container {
  flex-direction: row;
  align-items: center;
  width: 750;
  height: 120;
  background-color: #515151;
}
.icon {
  width: 100;
  height: 100;
  padding-top: 30;
  padding-left: 30;
  padding-bottom: 30;
  padding-right: 30;
}
.title {
  color: white;
  font-size: 40;
  font-weight: bold;
  margin-left: 20;
}
</style>
