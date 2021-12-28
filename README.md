# Operator-System-Curriculum-Design 

湖南科技大学操作系统课程设计实验报告存档

## 特点

利用pandoc软件，使用markdown语言现代化编写实验报告并转换成latex文档，优雅的渲染成pdf。

## 目录结构

```
.
├── build         实验报告构建位置
├── doc           所有报告分日期存放位置
├── header        构建文档所需的头信息
├── LICENSE
├── makefile
├── README.md
├── ref			  课程设计指导书
├── src			  源代码
├── submit		  上传相关
└── test		  自动测试相关内容
```
## 构建

本仓库已提供了构建完成后的实验报告`build/final.pdf`，也可用户自行构建。

请提供Linux环境，安装`make`,`pandoc`,`xelatex`,`ctex`软件包，安装Pandoc模板[Eisvogel](https://github.com/Wandmalfarbe/pandoc-latex-template)。

在根目录`make`，最终实验报告`final.pdf`即位于`build/final.pdf`

也可`make clean`清除构建文件
