# 报告二版

## 开发目的

  在高度信息化的社会背景下，人们日常生活少不了各种各样的信息交流，更快捷更高效的沟通需求与日俱增。为了给广大网络用户提供一个快捷方便的沟通方式，本项目拟开发一个轻量级即时通讯软件，与现在流行的通讯软件相比，剔除了一些通讯以外的附加功能，界面更加简洁，保留聊天、传输文件、发送图片等核心简单功能，实现一个专注于通讯的软件。
  

## 角色分析

  本项目的主要服务对象是对快捷沟通方式有需求的用户，认为主流聊天软件附加功能太多，偏社交的元素使得界面臃肿，而只需要简单通讯、分享等核心功能的用户。用户期望使用我们的软件与其他用户实现便利的交流，并分享自己的资源。
  
  
## 需求分析

### 注册与登录

  对于一款即时通讯软件而言，为保证一个用户获得一个完整操作权限账号，注册与登录功能是必要的。
  在注册时，需要用户填写用户名、密码等基础信息，并需要确认密码和填写密保。
  注册成功后，在登录界面输入正确的用户名和密码后，就可以正确登录账号并使用其他功能，后续可以修改密码。
  忘记密码时，需要回答密保问题，回答正确时通过系统提示取回密码。
  

### 聊天室
  
  每个用户都可以设置自己的昵称，根据不同的通讯需求，用户可以对其他用户私聊或者创建、加入聊天室群聊。
  在聊天室或私聊中，用户可以发送聊天信息与其他用户交流，需要文件传输时也可以通过相应的功能实现。
  
### 发送功能

  在用户所使用的客户端界面包括发送信息、图片、文件的按钮，并有对应的界面可以完成这些功能。

### 文件传输
  
  在聊天室中，多人之间的文件传输通过文件分享功能实现，分享者上传文件之后，其他聊天室内用户根据自己的需求选择下载。
  在私聊中，用户发送文件给其他用户时，对方可以选择接收或者拒绝接收，实现了与聊天室中类似的效果。

### 音乐播放
  
  软件拥有音乐播放功能，可以自行添加、删除、播放歌曲。
  
## 开发计划

### Step 1.

  由于即时通讯软件需要客户端与服务端分别开发并能联调运行，需要先架设好服务器，通过服务器实现客户端之间的通讯，转发客户端之间的信息，监听客户端连接状态，处理异常信息及中断。
  
  本项目服务端拟采用控制台模式，通过控制台输出各种提示信息，在此基础上编写与之相匹配的客户端，并在客户端开发各种功能，由服务端配合实现。
