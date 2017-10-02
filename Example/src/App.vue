<template>
  <view class="container" id="app">
    <drawer-layout>
      <label class="name">Lynx</label>
      <view class="line"></view>
      <label class="setting">Setting</label>
    </drawer-layout>
    <hello></hello>
  </view>
</template>

<script>
import Hello from './components/Hello.vue'
import DrawerLayout from './components/DrawerLayout.vue'
export default {
  components: {
    Hello,
    DrawerLayout
  }
}
</script>

<style>
.container {
  flex-direction: column;
  justify-content: center;
  align-items: center;
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
