<template>
  <view
    class="container"
    id="app">

    <swiper>
      <listview
        v-for="item in contentList"
        class="listview">
        <view class="item-wrap">
          <label class="item-title">{{item.title}}</label>
        </view>
        <view class="item-wrap">
          <img class="item-img" :src="item.imgUrl"></img>
        </view>
        <template v-for="i in 7">
          <view class="line"></view>
          <view class="item-wrap">
            <img class="item-img-small" :src="item.tinyImgUrl"></img>
            <label class="item-content">{{item.content}}</label>
          </view>
        </template>
      </listview>
    </swiper>
  </view>

</template>

<script>
export default {
  components: {
  },
  data() {
    return {
      contentList: [
        {
          title: "Lynx",
          imgUrl: "pic_lynx.png",
          tinyImgUrl: "icon_lynx_sit.png",
          content: "Interactive animation support in Lynx"
        },
        {
          title: "Cat",
          imgUrl: "pic_cat.png",
          tinyImgUrl: "icon_cat.png",
          content: "Interactive animation support in Lynx"
        },
        {
          title: "Tiger",
          imgUrl: "pic_tiger.png",
          tinyImgUrl: "icon_tiger.png",
          content: "Interactive animation support in Lynx"
        },
        {
          title: "Lion",
          imgUrl: "pic_lion.png",
          tinyImgUrl: "icon_lion.png",
          content: "Interactive animation support in Lynx"
        }
      ]
    }
  },
  mounted() {
  },
  methods: {
  }
}
</script>

<style>
.container {
  flex-direction: row;
}
.listview {
  flex-direction: column;
  width: 750;
}
.shadow {
  position: absolute;
  left: 0;
  top: 120;
  width: 750;
  height: 16;
  object-fit: fill;
  opacity: 0;
}

.item-wrap {
  flex-direction: row;
  padding-top: 40;
  padding-bottom: 40;
  padding-left: 30;
  padding-right: 30;
  align-items: center;
}
.item-title {
  color: black;
  font-size: 50;
  font-weight: bold;
}
.item-img {
  width: 700;
  height: 380;
}
.item-img-small {
  width: 50;
  height: 50;
}
.item-content {
  color: black;
  font-size: 35;
  margin-left: 20;
}
.line {
  height: 2;
  align-self: stretch;
  background-color: #d0d0d0;
}
</style>
