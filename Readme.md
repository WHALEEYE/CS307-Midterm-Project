## 项目介绍

```
     _____          ___                       ___                         ___           ___           ___     
    /  /::\        /  /\          ___        /  /\         _____         /  /\         /  /\         /  /\    
   /  /:/\:\      /  /::\        /  /\      /  /::\       /  /::\       /  /::\       /  /:/_       /  /:/_   
  /  /:/  \:\    /  /:/\:\      /  /:/     /  /:/\:\     /  /:/\:\     /  /:/\:\     /  /:/ /\     /  /:/ /\  
 /__/:/ \__\:|  /  /:/~/::\    /  /:/     /  /:/~/::\   /  /:/~/::\   /  /:/~/::\   /  /:/ /::\   /  /:/ /:/_ 
 \  \:\ /  /:/ /__/:/ /:/\:\  /  /::\    /__/:/ /:/\:\ /__/:/ /:/\:| /__/:/ /:/\:\ /__/:/ /:/\:\ /__/:/ /:/ /\
  \  \:\  /:/  \  \:\/:/__\/ /__/:/\:\   \  \:\/:/__\/ \  \:\/:/~/:/ \  \:\/:/__\/ \  \:\/:/~/:/ \  \:\/:/ /:/
   \  \:\/:/    \  \::/      \__\/  \:\   \  \::/       \  \::/ /:/   \  \::/       \  \::/ /:/   \  \::/ /:/ 
    \  \::/      \  \:\           \  \:\   \  \:\        \  \:\/:/     \  \:\        \__\/ /:/     \  \:\/:/  
     \__\/        \  \:\           \__\/    \  \:\        \  \::/       \  \:\         /__/:/       \  \::/   
                   \__\/                     \__\/         \__\/         \__\/         \__\/         \__\/    
      ___         ___           ___         ___          ___           ___                                    
     /  /\       /  /\         /  /\       /  /\        /  /\         /  /\          ___                      
    /  /::\     /  /::\       /  /::\     /  /:/       /  /:/_       /  /:/         /  /\                     
   /  /:/\:\   /  /:/\:\     /  /:/\:\   /__/::\      /  /:/ /\     /  /:/         /  /:/                     
  /  /:/~/:/  /  /:/~/:/    /  /:/  \:\  \__\/\:\    /  /:/ /:/_   /  /:/  ___    /  /:/                      
 /__/:/ /:/  /__/:/ /:/___ /__/:/ \__\:\    \  \:\  /__/:/ /:/ /\ /__/:/  /  /\  /  /::\                      
 \  \:\/:/   \  \:\/:::::/ \  \:\ /  /:/     \__\:\ \  \:\/:/ /:/ \  \:\ /  /:/ /__/:/\:\                     
  \  \::/     \  \::/~~~~   \  \:\  /:/      /  /:/  \  \::/ /:/   \  \:\  /:/  \__\/  \:\                    
   \  \:\      \  \:\        \  \:\/:/      /__/:/    \  \:\/:/     \  \:\/:/        \  \:\                   
    \  \:\      \  \:\        \  \::/       \__\/      \  \::/       \  \::/          \__\/                   
     \__\/       \__\/         \__\/                    \__\/         \__\/                                                                

    __             ____     _    __                     __                    __          __                   
   / /_  __  __   / __/__  (_)  / /______  ____  ____ _/ /_____ _   _      __/ /_  ____ _/ /__  ___  __  _____ 
  / __ \/ / / /  / /_/ _ \/ /  / //_/ __ \/ __ \/ __ `/ __/ __ `/  | | /| / / __ \/ __ `/ / _ \/ _ \/ / / / _ \
 / /_/ / /_/ /  / __/  __/ /  / ,< / /_/ / / / / /_/ / /_/ /_/ /   | |/ |/ / / / / /_/ / /  __/  __/ /_/ /  __/
/_.___/\__, /  /_/  \___/_/  /_/|_|\____/_/ /_/\__,_/\__/\__,_/    |__/|__/_/ /_/\__,_/_/\___/\___/\__, /\___/ 
      /____/                                                                                      /____/       
```



## 更新记录

##### 2021/3/25 whaleeye

* 进行了先修课的处理

  + 将给定的先修课表达式转换为了后序表达式
  + `或`关系使用`-1`表示，`与`关系使用`-2`表示
  + 最后生成一个字符串，由数字和空格组成，待保存

* 对项目文件进行一定调整

  * 将Java文件的格式调整为了**Google style**

    + tab缩进由4空格变为2空格
    + import文件的顺序进行了调整
    + 一些杂项调整，代码格式更加合规了

  * 添加了有效的`.gitignore`文件

    和项目无关紧要的配置文件（如`.idea`文件夹内的配置文件）今后将不再被Git同步了

  + 添加了Readme文件用于记录更新

* 待解决问题

  * 如何处理先修课字符串
    * 先修课字符串在数据库中以`varchar`保存
    * 拿出字符串后，需要`String---split-->String[]---parse-->int`，是否存在更简单方法

  * **Task3**的规划



##### 2021/3/30 konata

* 对上次更新的代码部分进行数据库连接测试

  + connect文件 250行 288行 出现数组越界错误
  + 250行已经修复
  + 288行勉强修复能够运行但是存在疑点
  + 字符串去重算法有误，类似“-1 -1 -1”的字段在先修课栏出现

* 对task2全部代码进行整合测试
  + 全部代码测试成功可以使用

* 待解决问题
  + task2还剩两张表尚未处理，需要讨论决定
    + classList表 json处理存在难度
    + 学生选课名单 需讨论决定使用方式



##### 2021/4/2 whaleeye

+ 对`select_course.json`文件进行了预处理

  + 处理后的文件为**`output.json`**

  + 将课名中出现的上下括号由`()`更换为了`（）`

    避免基于括号分割时将课名中的括号错误识别

  + 删除了中文课程名和课程等级之间的空格

    统一了中文课程名的格式

  + 先修课的分隔符由`空格`改为了`|`

    避免因为课名中自带空格而进行的错误分割

  + 去除了`teacher`，`courseName`，`className`，`location`等字段中的多余空格和制表符(`\t`)

  + 详见**JSON文件预处理**

+ 修复了`connect.java`中先修课处理中存在的bug

+ 优化了`connect.java`中先修课处理的结构

  + 删除了不必要的`String`和`Array`的多次转换
  + 对`()`加空格的预处理已不再需要了

+ 仍旧存在的问题

  + 先修课字段中，有些是None，有些是空字符串，是否需要统一

  + 先修课后序表达式中，出现了与或运算符超出合理数量的情况

    推测是因为某个括号内的课程全部无效所致，如当正常情况下应为

    ```
    (A or B) and (C or D) --postorder--> A B or C D or and
    (A or B) and (C or D) and (E or F) --postorder--> A B or C D or and E F or and
    ```

    此时若A，B均无效，则会转化为

    ```
    () and (C or D) --postorder--> C D or and
    () and (C or D) and (E or F) --postorder--> C D or and E F or and
    ```

    ##### 给出两个解决方案：

    1. 不管多余的运算符，在后序运算时只要发现**栈顶没有足够元素**就抛弃该运算符。

       此种方案得出的结果在嵌套层数（即布尔表达式中的括号层数）大于等于两层时即会出现问题。

       但由于给定的课程数据均是SOP形式，嵌套层数均小于等于一层，因此可以使用。

    2. 在进行先修课的后序转换后，再进行一次表达式合法性检测，删除多余的运算符。

       此方案原理和第一种大同小异，只不过此方案在存储之前先将后序表达式合法化了。

       但该方案存在过处理的危险，即有数据的损失，如果后续发现问题难以恢复原表达式。

  * **Task3**的规划

    4.6之前大体完成**Task3**



## JSON文件预处理

### 1. 不规范数据的处理

在对给定的json文件进行分析之后，发现主要存在以下数据不规范现象。

#### (1) 中文课程名和课程等级之间的空格

##### 问题描述：

在中文课程中，课程名和课程等级名之间有些存在空格，有些则不存在，给之后的字段分割带来一定难度。如：

```
大学物理 B(下)
化学原理 A
```

##### 解决方案：

分析发现课程等级只存在`A`，`B`，`C`，`D`，`I`，`II`，`III`的情况，因此可以使用正则表达式`\s[ABCDI]`进行匹配并替换。

#### (2) 中文课程名中的括号存在英文括号

##### 问题描述：

在中文课程名中，存在英文括号，给先修课字段中基于括号的优先级判断带来难度。如：

```
大学物理 B(下)
```

##### 解决方案：

分析发现只有在`(上)`，`(下)`的表示中存在此类情况，因此使用正则表达式`\(.\)`进行匹配并替换。

#### (3) 字段中存在空白符和制表符

##### 问题描述：

此类问题主要出现在`teacher`，`courseName`，`className`，`location`这四个字段中。

在字段的开头和末尾可能存在空格或制表符，导致录入数据库的数据中存在多余空白符。如：

```
\tHisao Ishibuchi
 全球生物多样性保护
```

##### 解决方案：

对于`className`和`location`字段，由于字段内本身不含空白符，因此可以直接使用正则表达式`\s+`进行匹配并删除。

对于`teacher`和`courseName`字段，由于字段本身可能含有空格，因此使用正则表达式`\A\s`匹配开头的空格并删除。



### 2. 先修课字段的处理

在将数据规范化后，将先修课字段中表示分割的`空格`转换为`|`，以便在后续处理时将分割符和课程名中本身的空格区分。



## 先修课字段的处理

后序表达式，相较于其余处理方式的优点：高扩展性和兼容性。