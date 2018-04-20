<template>
  <view class="container" id="app">
    <radio-group :bind-change="changeHandle" ref="radioGroup">
      <template v-for="item in items">
        <radio  :checked="item.checked" :name="item.name" :pic-array="picArray" :disabled="item.disabled"></radio>
        <view class="itemLabel">
          <label>{{item.value}}</label>
        </view>
      </template>
    </radio-group>
  </view>
</template>
<style>

.container {
  flex-direction: column;
  background-color: white;
  width: 750
}
  .group {
    flex-direction: column;
  }
  .itemLabel {
    justify-content: center;
    align-items: center
  }
</style>
<script>
const items = [
    {
      name:'HZ',
      value:'杭州',
      checked:true,
      disabled:true
    },
    {
      name:'SH',
      value:'上海',
      checked:false,
      disabled:false
    },
    {
      name:'BJ',
      value:'北京',
      checked:false,
      disabled:false
    }
  ]

const picArray = [
  'radio-uncheck.png',
  'radio-checked.png',
  'radio-disable.png']

import {Radio, RadioGroup} from 'lynx-components'

export default {
  components: {
    Radio,
    RadioGroup
  },
  data() {
    return {
      items,
      picArray
    }
  },
  methods: {
    changeHandle (item) {
      console.log(item.picArray)
      console.log(item.name)
    }
  }
}
</script>