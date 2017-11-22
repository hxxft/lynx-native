<template>
  <view
    class="container"
    id="app">

    <scrollview
      ref="vScrollView"
      class="vertical-scrollview"
      coordinator-tag="vScrollView"
      coordinator-type="scroll"
      coordinator-affinity="sponsorAffinity">
      <scrollview
        class="scrollview"
        page-enable="true"
        coordinator-tag="hScrollView"
        coordinator-type="scroll"
        coordinator-affinity="sponsorAffinity">
        <view
          v-for="item in contentList"
          class="scrollview-item">
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
        </view>
      </scrollview>
    </scrollview>

    <view
      class="anim-bg"
      coordinator-affinity="scroll"
      coordinator-command="scroll:onScrollEventForBg">
      <template v-for="(anim, index) in animList">
        <view
          class="anim-card"
          @click="onClick(index)"
          :style="{
            marginTop: index * 310 + ''
          }"
          coordinator-affinity="responderAffinity"
          :coordinator-tag="'card' + index"
          coordinator-command="scroll:onScrollEventForCard">
          <img
            class="anim-card-img"
            :src="anim.imgUrl"
            coordinator-affinity="responderAffinity"
            :coordinator-tag="'img' + index"
            coordinator-command="scroll:onScrollEventForImg"></img>
        </view>
        <label
          class="anim-label"
          :style="{
            marginTop: index * 310 + ''
          }"
          coordinator-affinity="responderAffinity"
          :coordinator-tag="'label' + index"
          coordinator-command="scroll:onScrollEventForLabel">{{anim.title}}</label>
        <view
          class="anim-line"
          :style="{
            marginTop: index * 310 + ''
          }"
          coordinator-affinity="responderAffinity"
          :coordinator-tag="'line' + index"
          coordinator-command="scroll:onScrollEventForLine"></view>
      </template>
    </view>

    <img
      class="back-btn"
      src="icon_back.png"></img>
  </view>

</template>

<script>
import animationCommand from './coordinator_anim_bar.lepus'
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
      ],
      animList: [
        {
          title: "Lynx",
          imgUrl: "pic_lynx.png",
        },
        {
          title: "Cat",
          imgUrl: "pic_cat.png",
        },
        {
          title: "Tiger",
          imgUrl: "pic_tiger.png",
        },
        {
          title: "Lion",
          imgUrl: "pic_lion.png",
        }

      ]
    }
  },
  mounted() {
    LynxCoordinatorRegister.registeAction("sponsorAffinity", "responderAffinity", animationCommand)
  },
  methods: {
    onClick(index) {
      console.log("click")
      this.$refs.vScrollView.scrollTop = 1240 - 120
    }
  }
}
</script>

<style>
.container {
  flex-direction: row;
}
.vertical-scrollview {
  flex-direction: column;
}
.scrollview {
  padding-top: 1240;
  flex-direction: row;
  width: 750;
}
.scrollview-item {
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
  width: 200;
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
.back-btn {
  position: absolute;
  height: 50;
  width: 50;
  top: 30;
  left: 30;
  object-fit: fill;
}
.anim-bg {
  position: absolute;
  width: 750;
  height: 1240;
  background-color: #66ccff;
}
.anim-card {
  position: absolute;
  top: 0;
  right: 0;
  height: 300;
  width: 500;
}
.anim-card-img {
  height: 400;
  width: 600;
  margin-top: -50;
  margin-left: -50;
  z-index: 0;
}
.anim-label {
  position: absolute;
  color: white;
  font-size: 60;
  line-height: 60;
  left: 60;
  top: 120;
  font-weight: bold;
  z-index: 100;
}
.anim-line {
  position: absolute;
  background-color: white;
  border-radius: 4;
  height: 8;
  width: 80;
  right: 40;
  top: 250;
  z-index: 100;
}
</style>
