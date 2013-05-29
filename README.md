#drag2d
Tool for making 2D game GUI editor

##Overview   
This library is a tool for making game editors such as [texture packer](https://github.com/xzrunner/easypacker), [particle editor](https://github.com/xzrunner/easyparticle) and many scene editor of different games.

These editors usually have the same part, such as draw lines or paste images. Drag2D extract these as atomic function. So you can easily and quickly compose editor as you need.

##About
The editor made by drag2d was composed by 4 parts: library, property, stage and toolbar. Library is used for loading images, shapes, fonts and other symbol. Property is used to display and set some property value of selected symbols. Stage is the main window for any edit operation. Toolbar is used to set operation's attribute.

Each part has many candidates, so you can select and compose to certain editor. [Shoot Bubble editor](https://github.com/xzrunner/shootbubble) is a good start.

##Source 
[https://github.com/xzrunner/drag2d](https://github.com/xzrunner/drag2d)

##Example 

[How to create Shoot Bubble editor by drag2d](https://github.com/xzrunner/shootbubble/wiki/Create-by-drag2d)