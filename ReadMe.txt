This is the StudManagement Project for Ph2 in KR.

As for more details, you can see the documents.

And giving thanks to Robert Pickering's blog
http://strangelights.com/blog/archive/2013/05/20/using-the-android-gridview-as-a-datagrid.aspx

/////////////////////////////////////////////////////////////////

学校管理系统是在KR学习期间，为巩固第二阶段所学知识，由授课教师提出，学生需按既定时间完成的项目。

该系统是对第一阶段二叉树版学生管理系统的升级，采用数据库进行数据的管理。为减少各模块间的耦合，采用三层结构进行设计。

定义
DataBase <---> Server <---> M Server <---> Client

其中，
DataBase: 相关数据存储，这里一般指SQL Server
Server   : 与DataBase交互，操控数据处理，回传数据给M Server
M Server : 逻辑层，根据传入的UI数据，构造相关SQL 语句，传给Server,
           也可进行数据安全性处理。
Client   : Window或者Android客户端


目标
结合数据库，实现网络版的学校信息管理系统，包括：
1）对学生、老师、课程、班级的管理

       正确梳理数据关系，建立相关数据库

2）分层设计，各司其职，减小修改所带来的影响

       SQL Server <-----> Server <----> M Server (Logic) <---->Client (UI)

3）利用本地缓存，实现高效的查询效果

      借助修改时间，两次修改之间的查询数据进行本地缓存。

4）利用连接池，重用数据库连接，处理Server向SQL Server频繁的、短时的操作

5）利用线程池，实现Server与多M Server, M Server 与 多Client间的交互。

6）同时提供PC客户端和Android客户端，方便用户使用

