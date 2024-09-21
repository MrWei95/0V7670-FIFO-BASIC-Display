# OV7670简易显示

<img src="/Docs/demo.png" class="" title="成品展示" style="zoom:40%;" >

很多嵌入式项目涉及中文会使用GB2312编码，但我习惯使用UTF-8编码，所以本项目代码全部为UTF-8编码。OV7670驱动代码中有申明一些不存在的引脚的定义（应该是为22Pin的模块设计的），可用性未知。

据反应，22Pin摄像头使用本项目代码会出现LCD反色，原因未知。
本项目显示刷新速度还是太慢，如有更好的方案请教授！

效果演示视频：https://www.bilibili.com/video/BV1fBhpePEE1/

如果不想去代码里找接线定义，可以参考下图：

<img src="/Docs/pin.png" class="" title="接线说明" style="zoom:100%;" >

Docs目录下存有相关资料文档，可查阅。
