IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'Ph2_Project_wbxu')
	DROP DATABASE [Ph2_Project_wbxu]
GO

/*
Log:
����ɾ�����ݿ��ļ� 'C:\Program Files\Microsoft SQL Server\MSSQL\data\Ph2_Project_wbxu_Log.LDF'��
����ɾ�����ݿ��ļ� 'C:\Program Files\Microsoft SQL Server\MSSQL\data\Ph2_Project_wbxu_Data.MDF'��
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
CREATE DATABASE �������ڴ��� 'Ph2_Project_wbxu_Data' �Ϸ��� 1.00 MB �Ŀռ䡣
CREATE DATABASE �������ڴ��� 'Ph2_Project_wbxu_Log' �Ϸ��� 1.00 MB �Ŀռ䡣
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
ѧ���� t_stud��
ѧ��(PK), �������Ա����գ����ᣬ�༶�ţ�FK),��ʦ��(FK)

�༶��t_class:
�༶��(PK),�༶����

�γ̱� t_course:
�γ̺�(PK),�γ�����,��ʱ

��ʦ��t_teacher:
   ��ʦ��(PK),�������Ա����գ�ְ��

ѡ�α� t_sel_cours:
ѧ��(PK,FK), �γ̺�(PK, FK), �ɼ�

�ڿα� t_tch_cours:
��ʦ��(PK, FK), �γ̺�(PK,FK)
*/
use [Ph2_Project_wbxu]
GO

/*
������������Ϣ
*/
---ѧ���� t_stud��
---ѧ��(PK), �������Ա����գ����ᣬ�༶�ţ�FK),��ʦ��(FK)
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_stud_t_class]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
   ALTER TABLE [dbo].[t_stud] DROP CONSTRAINT FK_t_stud_t_class
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_stud_t_teacher]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
   ALTER TABLE [dbo].[t_stud] DROP CONSTRAINT FK_t_stud_t_teacher
GO

---ѡ�α� t_sel_cours:
---ѧ��(PK,FK), �γ̺�(PK, FK), �ɼ�
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_sel_cours_t_stud]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[t_sel_cours] DROP CONSTRAINT FK_t_sel_cours_t_stud
GO

---ѡ�α� t_sel_cours:
---ѧ��(PK,FK), �γ̺�(PK, FK), �ɼ�
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_sel_cours_t_course]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[t_sel_cours] DROP CONSTRAINT FK_t_sel_cours_t_course
GO

---�ڿα� t_tch_cours:
---��ʦ��(PK, FK), �γ̺�(PK,FK)
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_tch_cours_t_teacher]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[t_tch_cours] DROP CONSTRAINT FK_t_tch_cours_t_teacher
GO

---�ڿα� t_tch_cours:
---��ʦ��(PK, FK), �γ̺�(PK,FK)
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_t_tch_cours_t_course]') 
	and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[t_tch_cours] DROP CONSTRAINT FK_t_tch_cours_t_course
GO

/*
������ر�
*/
---ѧ����
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_stud]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_stud]
GO

---�༶��
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_class]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_class]
GO

---�γ̱�
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_course]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_course]
GO

---��ʦ��
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_teacher]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_teacher]
GO

---ѡ�α�
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_sel_cours]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_sel_cours]
GO

---�ڿα�
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_tch_cours]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_tch_cours]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[t_alias]') 
	and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[t_alias]
GO

/*
�½���
*/

---ѧ���� t_stud��
---ѧ��(PK), �������Ա����գ����ᣬ�༶�ţ�FK), ��ʦ��(FK)
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

---�༶��t_class:
---�༶��(PK),�༶����
CREATE TABLE [dbo].[t_class] (
	[clsno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[clsname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

--�γ̱� t_course:
--�γ̺�(PK),�γ�����,��ʱ
CREATE TABLE [dbo].[t_course] (
	[cno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[cname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL,
	[chour] int NOT NULL 
) ON [PRIMARY]
GO

---��ʦ��t_teacher:
---��ʦ��(PK),�������Ա����գ�ְ��
CREATE TABLE [dbo].[t_teacher] (
	[tno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[tname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[gender] [char] (4) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[birthday] [datetime] NOT NULL,
	[ttile]   [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL
) ON [PRIMARY]
GO

---ѡ�α� t_sel_cours:
---ѧ��(PK,FK), �γ̺�(PK, FK), �ɼ�
CREATE TABLE [dbo].[t_sel_cours] (
	[sno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[cno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[score] [float] NOT NULL 
) ON [PRIMARY]
GO

---�ڿα� t_tch_cours:
---��ʦ��(PK, FK), �γ̺�(PK,FK)
CREATE TABLE [dbo].[t_tch_cours] (
	[tno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[cno] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO


/*
����������Ϣ
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
���������Ϣ
ѡ�α� t_sel_cours:
ѧ��(PK,FK), �γ̺�(PK, FK), �ɼ�

�ڿα� t_tch_cours:
��ʦ��(PK, FK), �γ̺�(PK,FK)

ָ���� t_teach_stud:
��ʦ��(PK,FK), ѧ����(PK,FK)
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