<template>
  <view class="view-container">
    <label class="title">{{hello}}</label>
  </view>
</template>

<script>
export default {
  name: 'show',
  data () {
    return {
      show: true,
      hello:'hello world'
    }
  },
}
</script>

<style>
.title {
  color:#ff5577;
  font-size: 70
}
.view-container {
  width: 700;
  margin-top: 20;
  margin-bottom: 20;
  margin-left: 10;
  margin-right: 10;
  padding-top: 20;
  padding-bottom: 20;
  border-width: 2;
  border-color: #ccc;
  border-radius: 8;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}
</style>