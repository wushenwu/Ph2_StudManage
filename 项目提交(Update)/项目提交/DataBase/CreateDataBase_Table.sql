IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'Ph2_Project_wbxu')
	DROP DATABASE [Ph2_Project_wbxu]
GO

/*
Log:
正在删除数据库文件 'C:\Program Files\Microsoft SQL Server\MSSQL\data\Ph2_Project_wbxu_Log.LDF'。
正在删除数据库文件 'C:\Program Files\Microsoft SQL Server\MSSQL\data\Ph2_Project_wbxu_Data.MDF'。
*/

CREATE DATABASE [Ph2_Project_wbxu]  ON (NAME = N'Ph2_Project_wbxu_Data', 
		          FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL.2\MSSQL\data\Ph2_Project_wbxu_Data.MDF' ,
			 SIZE = 3, FILEGROWTH = 10%)
		 LOG ON (NAME = N'Ph2_Project_wbxu_Log', 
			FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL.2\MSSQL\data\Ph2_Project_wbxu_Log.LDF' , 
			SIZE = 3,
			 FILEGROWTH = 10%)
 COLLATE Chinese_PRC_CI_AS
GO

/*
CREATE DATABASE 进程正在磁盘 'Ph2_Project_wbxu_Data' 上分配 1.00 MB 的空间。
CREATE DATABASE 进程正在磁盘 'Ph2_Project_wbxu_Log' 上分配 1.00 MB 的空间。
*/

exec sp_dboption N'Ph2_Project_wbxu', N'autoclose', N'true'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'bulkcopy', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'trunc. log', N'true'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'torn page detection', N'true'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'read only', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'dbo use', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'single', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'autoshrink', N'true'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'ANSI null default', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'recursive triggers', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'ANSI nulls', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'concat null yields null', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'cursor close on commit', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'default to local cursor', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'quoted identifier', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'ANSI warnings', N'false'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'auto create statistics', N'true'
GO

exec sp_dboption N'Ph2_Project_wbxu', N'auto update statistics', N'true'
GO

if( (@@microsoftversion / power(2, 24) = 8) and (@@microsoftversion & 0xffff >= 724) )
	exec sp_dboption N'Ph2_Project_wbxu', N'db chaining', N'false'
GO

/*
学生表 t_stud：
学号(PK), 姓名，性别，生日，籍贯，班级号（FK),老师号(FK)

班级表t_class:
班级号(PK),班级名称

课程表 t_course:
课程号(PK),课程名称,课时

老师表t_teacher:
   老师号(PK),姓名，性别，生日，职称

选课表 t_sel_cours:
学号(PK,FK), 课程号(PK, FK), 成绩

授课表 t_tch_cours:
老师号(PK, FK), 课程号(PK,FK)
*/
use [Ph2_Project_wbxu]
GO

/*
清理相关外键信息
*/
---学生表 t_stud：
---学号(PK), 姓名，性别，生日，籍贯，班级号（FK),老师号(FK)
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_stud_t_class]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
   ALTER TABLE [dbo].[t_stud] DROP CONSTRAINT FK_t_stud_t_class
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_stud_t_teacher]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
   ALTER TABLE [dbo].[t_stud] DROP CONSTRAINT FK_t_stud_t_teacher
GO

---选课表 t_sel_cours:
---学号(PK,FK), 课程号(PK, FK), 成绩
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_sel_cours_t_stud]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[t_sel_cours] DROP CONSTRAINT FK_t_sel_cours_t_stud
GO

---选课表 t_sel_cours:
---学号(PK,FK), 课程号(PK, FK), 成绩
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_sel_cours_t_course]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[t_sel_cours] DROP CONSTRAINT FK_t_sel_cours_t_course
GO

---授课表 t_tch_cours:
---老师号(PK, FK), 课程号(PK,FK)
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_tch_cours_t_teacher]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[t_tch_cours] DROP CONSTRAINT FK_t_tch_cours_t_teacher
GO

---授课表 t_tch_cours:
---老师号(PK, FK), 课程号(PK,FK)
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_tch_cours_t_course]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[t_tch_cours] DROP CONSTRAINT FK_t_tch_cours_t_course
GO

/*
清理相关表
*/
---学生表
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_stud]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_stud]
GO

---班级表
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_class]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_class]
GO

---课程表
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_course]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_course]
GO

---老师表
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_teacher]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_teacher]
GO

---选课表
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_sel_cours]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_sel_cours]
GO

---授课表
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_tch_cours]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_tch_cours]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_alias]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_alias]
GO

/*
新建表
*/

---学生表 t_stud：
---学号(PK), 姓名，性别，生日，籍贯，班级号（FK), 老师号(FK)
CREATE TABLE [dbo].[t_stud] (
	[sno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[sname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[gender] [char] (4) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[birthday] [datetime] NOT NULL,
	[city]   [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL,
	[clsno]  [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL,
	[tno]    [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

---班级表t_class:
---班级号(PK),班级名称
CREATE TABLE [dbo].[t_class] (
	[clsno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[clsname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

--课程表 t_course:
--课程号(PK),课程名称,课时
CREATE TABLE [dbo].[t_course] (
	[cno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[cname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL,
	[chour] int NOT NULL 
) ON [PRIMARY]
GO

---老师表t_teacher:
---老师号(PK),姓名，性别，生日，职称
CREATE TABLE [dbo].[t_teacher] (
	[tno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[tname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[gender] [char] (4) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[birthday] [datetime] NOT NULL,
	[ttile]   [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL
) ON [PRIMARY]
GO

---选课表 t_sel_cours:
---学号(PK,FK), 课程号(PK, FK), 成绩
CREATE TABLE [dbo].[t_sel_cours] (
	[sno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[cno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[score] [float] NOT NULL 
) ON [PRIMARY]
GO

---授课表 t_tch_cours:
---老师号(PK, FK), 课程号(PK,FK)
CREATE TABLE [dbo].[t_tch_cours] (
	[tno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[cno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO


/*
设置主键信息
*/
ALTER TABLE [dbo].[t_stud] ADD 
	CONSTRAINT [PK_t_stud] PRIMARY KEY  CLUSTERED 
	(
		[sno]
	)  ON [PRIMARY] 
	
ALTER TABLE [dbo].[t_class] ADD 
	CONSTRAINT [PK_t_class] PRIMARY KEY  CLUSTERED 
	(
		[clsno]
	)  ON [PRIMARY] 
	
ALTER TABLE [dbo].[t_course] ADD 
	CONSTRAINT [PK_t_course] PRIMARY KEY  CLUSTERED 
	(
		[cno]
	)  ON [PRIMARY]

ALTER TABLE [dbo].[t_teacher] ADD 
	CONSTRAINT [PK_t_teacher] PRIMARY KEY  CLUSTERED 
	(
		[tno]
	)  ON [PRIMARY] 	

ALTER TABLE [dbo].[t_sel_cours] ADD 
	CONSTRAINT [PK_t_sel_cours] PRIMARY KEY  CLUSTERED 
	(
		[sno],
		[cno]
	)  ON [PRIMARY] 
	
ALTER TABLE [dbo].[t_tch_cours] ADD 
	CONSTRAINT [PK_t_tch_cours] PRIMARY KEY  CLUSTERED 
	(
		[tno],
		[cno]
	)  ON [PRIMARY] 

/*
设置外键信息
选课表 t_sel_cours:
学号(PK,FK), 课程号(PK, FK), 成绩

授课表 t_tch_cours:
老师号(PK, FK), 课程号(PK,FK)

指导表 t_teach_stud:
老师号(PK,FK), 学生号(PK,FK)
*/
ALTER TABLE [dbo].[t_stud] ADD 
	CONSTRAINT [FK_t_stud_t_class] FOREIGN KEY 
	(
		[clsno]
	) REFERENCES [dbo].[t_class] (
		[clsno]
	)

ALTER TABLE [dbo].[t_stud] ADD 
	CONSTRAINT [FK_t_stud_t_teacher] FOREIGN KEY 
	(
		[tno]
	) REFERENCES [dbo].[t_teacher] (
		[tno]
	)

ALTER TABLE [dbo].[t_sel_cours] ADD 
	CONSTRAINT [FK_t_sel_cours_t_stud] FOREIGN KEY 
	(
		[sno]
	) REFERENCES [dbo].[t_stud] (
		[sno]
	),
	
	CONSTRAINT [FK_t_sel_cours_t_course] FOREIGN KEY 
	(
		[cno]
	) REFERENCES [dbo].[t_course] (
		[cno]
	)

ALTER TABLE [dbo].[t_tch_cours] ADD 
	CONSTRAINT [FK_t_tch_cours_t_teach] FOREIGN KEY 
	(
		[tno]
	) REFERENCES [dbo].[t_teacher] (
		[tno]
	),
	
	CONSTRAINT [FK_t_tch_cours_t_course] FOREIGN KEY 
	(
		[cno]
	) REFERENCES [dbo].[t_course] (
		[cno]
	)