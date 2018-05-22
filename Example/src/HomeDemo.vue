<template>
  <view
    class="container"
    id="app"
    ref="app">

    <drawer-layout>
      <view class="setting">
        <img class="setting-icon" src="icon_lynx.png"></img>
        <label class="setting-title">Setting</label>
        <template v-for="group in settingList">
          <view class="setting-line"></view>
          <view v-for="setting in group"class="setting-item">
            <img class="setting-item-icon" :src="setting.icon"></img>
            <label class="setting-item-title">{{setting.text}}</label>
          </view>
        </template>
      </view>
    </drawer-layout>

    <listview
      class="listview"
      coordinator-affinity="sponsorAffinity"
      coordinator-type="scroll">
      <view class="placeholder"></view>
      <template v-for="content in contentList">
        <view class="item-group">
          <label class="item-group-title">{{content.groupName}}</label>
          <view v-for="item in content.itemList" class="item" @click="jumpToNewPage(item.title)">
            <label class="item-title">{{item.title}}</label>
            <label class="item-introduction">{{item.introduction}}</label>
          </view>
        </view>
        <view class="line"></view>
      </template>
    </listview>

    <img
      class="background-image"
      src="pic_lynx.png"
      coordinator-affinity="responderAffinity"
      coordinator-command="scroll:onScrollEventForImg"></img>
    <view
      class="blue-mask"
      coordinator-affinity="responderAffinity"
      coordinator-command="scroll:onScrollEvent"></view>
    <label
      class="title"
      coordinator-affinity="responderAffinity"
      coordinator-command="scroll:onScrollEventForTitle">Lynx</label>
    <img
      class="menu-btn"
      src="icon_menu.png"
      @clcik="onMenuClick"></img>
    <img
      class="shadow"
      src="shadow_piece.png"
      coordinator-affinity="responderAffinity"
      coordinator-command="scroll:onScrollEventForShadowLine"></img>
  </view>
</template>

<script>
import Hello from './components/Hello.vue'
import DrawerLayout from './components/DrawerLayout.vue'
import animationCommand from './coordinator_fade_bar.lepus'
export default {
  components: {
    Hello,
    DrawerLayout
  },
  data() {
    return {
      contentList: [
        {
          groupName: "Demos",
          itemList: [
            {
              title: "Gallery",
              introduction: "Gallery Demo"
            },
            {
              title: "Coordinator",
              introduction: "Coordinator Demo"
            }
          ]
        },
        {
          groupName: "Component",
          itemList: [
            {
              title: "Button",
              introduction: "Button Demo"
            },
            {
              title: "Checkbox",
              introduction: "Checkbox Demo"
            },
            {
              title: "Radio",
              introduction: "Radio Demo"
            },
            {
              title: "Canvas",
              introduction: "Canvas Demo"
            },
            {
              title: "Swiper",
              introduction: "Swiper Demo"
            }
          ]
        },
        {
          groupName: "Demos",
          itemList: [
            {
              title: "Animation",
              introduction: "Basic Animation Example"
            },
            {
              title: "Shopping",
              introduction: "Test Shopping"
            }
          ]
        }
      ],
      settingList: [
        [
          {
            icon: "icon_feedback.png",
            text: "Send feedback"
          },
          {
            icon: "icon_lynx_sit.png",
            text: "About Lynx"
          }
        ]
      ]
    }
  },
  mounted() {
    CoordinatorRegister.registerAction("sponsorAffinity", "responderAffinity", animationCommand)
  },
  methods: {
    onMenuClick(e) {
    },
    jumpToNewPage(page) {
      if (this.pageEnable(page)) {
        console.log('jumpToNewPage')
        PageNavigator.startNewPage(page.toLowerCase() + "demo")
      }
    },
    pageEnable(page) {
      if (page.startsWith("Shopping")) {
        return false
      }
      return true
    }
  }
}
</script>

<style>
.container {
  flex-direction: row;
}
.name {
  color:#bbbbbb;
  font-size: 70;
  align-self: center;
  font-weight: bold;
  margin-top: 30;
  margin-bottom: 30;
}
.line {
  background-color:#dddddd;
  height: 1;
  align-self: stretch;
}
.listview {
  flex-direction: column;
  width: 750;
}
.placeholder {
  height: 500;
}
.label {
  height: 100;
}
.blue-mask {
  position: absolute;
  background-color: #66ccff;
  width: 750;
  height: 500;
  opacity: 0;
}
.background-image {
  position: absolute;
  width: 750;
  height: 500;
}
.menu-btn {
  position: absolute;
  height: 30;
  width: 40;
  top: 45;
  left: 35;
  object-fit: fill;
}
.title {
  position: absolute;
  color: white;
  font-size: 70;
  top: 400;
  left: 120;
  font-weight: bold;
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

.setting {
  flex-direction: column;
  align-items: center;
  padding-top: 30;
  padding-bottom: 30;
}
.setting-icon {
  width: 200;
  height: 150;
}
.setting-title {
  font-size: 70;
  color: #707070;
}
.setting-line {
  height: 1;
  align-self: stretch;
  background-color: #d0d0d0;
  margin-top: 10;
  margin-bottom: 10;
}
.setting-item {
  flex-direction: row;
  align-items: center;
  align-self: stretch;
  padding-left: 20;
  padding-right: 20;
  padding-top: 30;
  padding-left: 30;
}
.setting-item-icon {
  width: 45;
  height: 45;
}
.setting-item-title {
  font-size: 35;
  color: #707070;
  margin-left: 20;
  font-weight: bold;
}

.item-group {
  flex-direction: column;
  padding-left: 40;
  padding-right: 40;
}
.item-group-title {
  color: #66ccff;
  font-size: 35;
  font-weight: bold;
  margin-top: 50;
}
.item {
  margin-top: 50;
  flex-direction: column;
}
.item-title-wrap {
  justify-content: space-between;
}
.item-title {
  color: black;
  font-size: 35;
}
.item-not-ready {
  color: #a0a0a0;
  font-size: 35;
}
.item-introduction {
  margin-top: 5;
  color: #a0a0a0;
}
.line {
  height: 2;
  align-self: stretch;
  background-color: #d0d0d0;
  margin-top: 50;
}
</style>
