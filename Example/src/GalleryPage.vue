<template>
  <view
    class="container"
    id="app">
    <listview
      class="scrollview">
      <view class="placeholder">
        <view class="img-wrap">
          <img
            class="img-show"
            :src="firstBgImg"
            coordinator-affinity="responderAffinity"
            coordinator-tag="firstBgImg"
            coordinator-command="touch:onTouchEventForBg"
            ></img>
          <img
            class="img-show"
            :src="secondBgImg"
            coordinator-affinity="responderAffinity"
            coordinator-tag="secondBgImg"
            coordinator-command="touch:onTouchEventForBg"
            ></img>
          <img
            class="img-show"
            :src="thirdBgImg"
            coordinator-affinity="responderAffinity"
            coordinator-tag="thirdBgImg"
            coordinator-command="touch:onTouchEventForBg"
            ></img>
          <img
            class="img-show"
            src="pic_mask.png"></img>
        </view>
        <view
          class="scroll-wrap"
          coordinator-affinity="sponsorAffinity"
          coordinator-tag="scrollWrap"
          coordinator-type="touch"
          @onpagechanged="onPageChanged"
          coordinator-command="touch:onTouchEventForWrap">
          <img
            v-for="(image, index) in images"
            class="scroll-item-image"
            coordinator-affinity="responderAffinity"
            :coordinator-tag="'item' + index"
            coordinator-command="touch:onTouchEventForImg"
            :src="image"></img>
        </view>
      </view>
      <view class="intro-wrap">
        <label class="intro-name">{{animals[curPage].name}}</label>
        <label class="intro-content">{{animals[curPage].intro}}</label>
        <img class="intro-img" :src="animals[curPage].pic"></img>
      </view>
    </listview>

  </view>

</template>

<script>
import animationAction from './coordinator_3d_scroll.lepus'
export default {
  components: {
  },
  data() {
    return {
      firstBgImg: 'pic_lynx_vertical.png',
      secondBgImg: 'pic_cat_vertical.png',
      thirdBgImg: 'pic_tiger_vertical.png',
      curPage: 0,
      images: [
        'pic_lynx_vertical.png',
        'pic_cat_vertical.png',
        'pic_tiger_vertical.png',
        'pic_lion_vertical.png'
      ],
      bgImages: [
        'pic_lynx_vertical.png',
        'pic_cat_vertical.png',
        'pic_tiger_vertical.png'
      ],
      animals: [
        {
          name: 'Lynx',
          intro: 'Lynx have a short tail, characteristic tufts of black hair on the tips of their ears, large, padded paws for walking on snow and long whiskers on the face. Under their neck, they have a ruff which has black bars resembling a bow tie although this is often not visible.',
          pic: 'pic_lynx.png'
        },
        {
          name: 'Cat',
          intro: 'The domestic cat (Felis silvestris catus or Felis catus) is a small, typically furry, carnivorous mammal. They are often called house cats when kept as indoor pets or simply cats when there is no need to distinguish them from other felids and felines. Cats are often valued by humans for companionship and for their ability to hunt vermin. There are more than 70 cat breeds, though different associations proclaim different numbers according to their standards.',
          pic: 'pic_cat.png'
        },
        {
          name: 'Tiger',
          intro: 'The tiger (Panthera tigris) is the largest cat species, most recognizable for their pattern of dark vertical stripes on reddish-orange fur with a lighter underside. The species is classified in the genus Panthera with the lion, leopard, jaguar, and snow leopard. Tigers are apex predators, primarily preying on ungulates such as deer and bovids. They are territorial and generally solitary but social animals, often requiring large contiguous areas of habitat that support their prey requirements. This, coupled with the fact that they are indigenous to some of the more densely populated places on Earth, has caused significant conflicts with humans.',
          pic: 'pic_tiger.png'
        },
        {
          name: 'Lion',
          intro: 'The lion (Panthera leo) is one of the big cats in the Felidae family and a member of genus Panthera. It has been listed as Vulnerable on the IUCN Red List since 1996, as populations in African range countries declined by about 43% since the early 1990s. Lion populations are untenable outside designated protected areas. Although the cause of the decline is not fully understood, habitat loss and conflicts with humans are the greatest causes of concern. The West African lion population is listed as Critically Endangered since 2016. The only lion population in Asia survives in and around Indias Gir Forest National Park and is listed as Endangered since 1986.',
          pic: 'pic_lion.png'
        }
      ]
    }
  },
  mounted() {
    LynxCoordinatorRegister.registeAction("sponsorAffinity", "responderAffinity", animationAction)
  },
  methods: {
    onPageChanged(e) {
      var curIndex = e.detail
      this.curPage = curIndex
      var curShowBgIndex = curIndex % 3
      if (curShowBgIndex == 0) { // FirstBgImg show
        this.firstBgImg = this.images[curIndex]
        if (curIndex + 1 <= this.images.length - 1) {
          this.secondBgImg = this.images[curIndex + 1]
        }
        if (curIndex - 1 >= 0) {
          this.thirdBgImg = this.images[curIndex - 1]
        }
      } else if (curShowBgIndex == 1) { // SecondBgImg show
        this.firstBgImg = this.images[curIndex - 1]
        this.secondBgImg = this.images[curIndex]
        if (curIndex + 1 <= this.images.length - 1) {
          this.thirdBgImg = this.images[curIndex + 1]
        }
      } else if (curShowBgIndex == 2) { // ThirdBgImg show
        this.secondBgImg = this.images[curIndex - 1]
        this.thirdBgImg = this.images[curIndex]
        if (curIndex + 1 <= this.images.length - 1) {
          this.firstBgImg = this.images[curIndex + 1]
        }
      }
    }
  }
}
</script>

<style>
.container {
  flex-direction: row;
}
.scrollview {
  flex-direction: column;
  width: 750;
}
.placeholder {
  height: 900;
  width: 750;
}
.img-wrap {
  position: absolute;
  width: 750;
  height: 750;
}
.img-show {
  position: absolute;
  width: 750;
  height: 750;
}
.scroll-wrap {
  position: absolute;
  flex-direction: row;
  top: 200;
  width: 750;
  height: 700;
}
.scroll-item-image {
  border-radius: 20;
  width: 450;
  height: 700;
  margin-left: 40;
}

.intro-wrap {
  margin-top: 50;
  margin-bottom: 50;
  width: 600;
  align-self: center;
  flex-direction: column;
}
.intro-name {
  font-size: 70;
  font-weight: bold;
  width: 200;
}
.intro-content {
  margin-top: 40;
  font-size: 40;
  width: 600;
}
.intro-img {
  margin-top: 40;
  width: 600;
  height: 300;
}
</style>
