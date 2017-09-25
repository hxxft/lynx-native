<template>
  <view class="container" id="app">
    <Hello></Hello>
  </view>
</template>

<script>
import Hello from './components/Hello.vue'
export default {
  components: {
    Hello,
  }
}
</script>

<style>
.container {
  flex-direction: column;
  justify-content: center;
  align-items: center;
}
</style>
