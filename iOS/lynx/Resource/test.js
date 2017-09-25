
var colorGold = "#FFD700";
var colorGreen = "#32CD32";
var colorRed = "#FF7F50";
var colorGray = '#666';
var colorBlue = '#00f';
var colorBlack = '#000';
var colorWhite = '#fff';

var view = document.createElement("view");
view.setStyle({
              flexDirection: 'column',
              alignItems: 'center',
              backgroundColor: '#eeeeee',
              width: screen.width,
              paddingTop: 5
              });

// title
var title = document.createElement("label");
title.setStyle({
               paddingLeft: 5,
               paddingTop: 5,
               paddingBottom: 5,
               paddingRight: 5
               });
title.setText("view");
view.appendChild(title);

// line
var line = document.createElement("view");
line.setStyle({
              height: 2,
              width: 100,
              marginTop: 5,
              marginBottom: 5,
              backgroundColor: '#000000'
              });
view.appendChild(line);

// Image
{
    var title = document.createElement("label");
    title.setStyle({
                   paddingLeft: 5,
                   paddingTop: 5,
                   paddingBottom: 5,
                   paddingRight: 5
                   });
    title.setText("Image From Net");
    view.appendChild(title);
    
    var image = document.createElement("img");
    image.setStyle({
                   width:200,
                   height:300,
                   });
    image.setAttribute("src", "https://timgsa.baidu.com/timg?image&quality=100&size=b9999_400&sec=1505643836917&di=3f58e600ebe903f0333f639dcc265676&imgtype=0&src=http%3A%2F%2Fimgsrc.baidu.com%2Fimage%2Fc0%253Dshijue1%252C0%252C0%252C294%252C40%2Fsign%3D047228819716fdfacc61ceaddce6e621%2Fae51f3deb48f8c543773168530292df5e1fe7ffc.jpg");
    view.appendChild(image);
}

document.body.appendChild(view);

