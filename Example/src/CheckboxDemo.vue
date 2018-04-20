<template>
  <view class="container1" id="app">
    <checkbox-group :bind-change="changeHandler" ref="checkboxGroup" v-model="checkBoxValue">
      <view v-for="item of items">
        <checkbox class="checkbox"
          :name="item.name"
          :checked="item.checked"
          :pic-array="picArray"
          :disabled="item.disabled">
        </checkbox>
        <view class="viewText">
          <label>{{item.text}}</label>
        </view>
      </view>
    </checkbox-group>
   </view>
</template>
<style>
  .container1 {
  flex-direction: column;
  justify-content: center;
  align-items: center;
}

  .checkbox {
    margin-left: 20;
  }
  .viewText {
    justify-content: center;
    align-items: center;
    margin-left: 20
  }
  .big {
    width: 50;
    height: 50;
  }
  .test2 {
    flex-direction: column
  }
</style>
<script>
const items =  [
  {text:"杭州",name:"HZ",checked:false,disabled:true},
  {text:"上海",name:"SH",checked:false,disabled:false},
  {text:"北京",name:"BJ",checked:true,disabled:false}
]

const picArray = [
  'checkbox-check.png',
  'checkbox-uncheck.png',
  'checkbox-disable.png']

import {Checkbox,CheckboxGroup}  from 'lynx-components'

export default {
  components: {
    Checkbox,
    CheckboxGroup
  },
  data () {
    return {
      items,
      picArray,
      checkBoxValue: [],
    }
  },
  methods: {
    changeHandler (item) {
      var y = this.$refs.checkboxGroup.getCheckedName();
      console.log(this.checkBoxValue)
    }
  }
}
</script>
