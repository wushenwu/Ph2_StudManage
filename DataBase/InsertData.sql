USE Ph2_Project
GO

---班级
insert t_class values('cls001','14级计算机科学与技术')
insert t_class values('cls002','14级电子与信息')
insert t_class values('cls003','13级化学工程')
insert t_class values('cls004','11级信息数学')
insert t_class values('cls005','12级英语')

--老师
insert t_teacher values('t001', '王sir', '男', '2014-10-2', '教授')
insert t_teacher values('t002', '李sir', '女', '2014-10-2', '高级工程师')
insert t_teacher values('t003', '张sir', '男', '2014-10-2', '副高级工程师')
insert t_teacher values('t004', '朱sir', '男', '2014-10-2', '副教授')

--学生
insert t_stud values('S001', '张一', '男', '2014-9-1 11:00:00', '湖北武汉', 'cls001', 't001')
insert t_stud values('S002', '李四', '男', '2014-9-2 11:00:00', '四川成都', 'cls001', 't001')
insert t_stud values('S003', '王五', '女', '2014-9-3 11:00:00', '陕西西安', 'cls005', 't002')
insert t_stud values('S004', '王五', '男', '2014-9-3 11:00:00', '陕西西安', 'cls005', 't004')
insert t_stud values('S005', '张三', '女', '2014-9-4 11:00:00', '云南昆明', 'cls003', 't002')
insert t_stud values('S006', '张一', '未知', '2014-5-1 11:00:00', '山东济南', 'cls002', 't001')
insert t_stud values('S007', '张一', '其它', '2014-6-1 11:00:00', '山西大同', 'cls004', 't001')

--课程
insert t_course values('c101', 'C语言', 30)
insert t_course values('c102', 'C++', 30)
insert t_course values('c103', '数据库', 20)
insert t_course values('c104', '数据结构', 25)
insert t_course values('c105', 'OS', 40)



--选修
insert t_sel_course values('S002', 'c101', 60)
insert t_sel_course values('S002', 'c102', 70)
insert t_sel_course values('S003', 'c103', 80)
insert t_sel_course values('S004', 'c101', 90)
insert t_sel_course values('S004', 'c104', 40)
insert t_sel_course values('S005', 'c101', 80)
insert t_sel_course values('S005', 'c103', 60)
insert t_sel_course values('S005', 'c105', 30)

---授课表
insert t_teach_course values('t001', 'c101')
insert t_teach_course values('t001', 'c102')
insert t_teach_course values('t003', 'c101')
insert t_teach_course values('t004', 'c105')

---后续数据由程序自动加入

/*
--删除
delete t_stud where sex = '男'
delete t_stud where sno = 's001'


--修改
update t_stud set sex = '女', birthday='2012-9-9'
 where sno='s001'

update t_stud set sname = '张四' where sno='s004'
update t_stud set sname = '张二' where sno='s002'
update t_stud set sname = '张三' where sno='s003'
update t_stud set sname = 'zhang', sex = '其它' where sno='s005'


--批量插入
insert into t_test select sno from t_stud
select *from t_test

*/

--查询

--*表示所有字段
select *from t_stud

--显示指定字段
select sno,sname from t_stud

--指定字段别名
select sno as '学号', sname as '姓名' from t_stud
select '学号'=sno, '姓名'=sname from t_stud


--条件查询
select *from t_stud where sno='S001'
select *from t_stud where sname like 'b%' --模糊查询
select *from t_stud where sname like '__ck%' --模糊查询
select *from t_stud where sname like '%c%' --模糊查询
select *from t_stud where sname like '_二%' --模糊查询
select *from t_stud where sex='男' and sname like '张%'
select *from t_stud where sex='男' or  sex='女'
select *from t_stud where not sex='女'

--范围
---select *from t_test where testid between 10 and 30
select *from t_stud where birthday between '2014-9-1' and '2014-09-03'
select *from t_stud where birthday like '%9%'

--在 in
select *from t_stud where sex in ('男', '女')
select *from t_stud where sex not in ('男', '女')

/*
--null
select *from t_test
select *from t_test where testname is null
select *from t_test wh ere testname is not null
*/

--聚合函数
select * from t_sel_course
select sum(score) as '总成绩' from t_sel_course
select sum(distinct/*去掉重复*/ score) as '总成绩' from t_sel_course
select avg(score) as 'avg' from t_sel_course
select count(distinct sno) as 'count' from t_sel_course
select count(*) as 'count' from t_sel_course

select max(score) as 'max' from t_sel_course
select min(score) as 'min' from t_sel_course
select left('hello', 2) 
select substring('hello', 2, 3) 
select *from t_stud where DatePart(dd, birthday)=3
select *from t_stud where DatePart(dw, birthday)=5

--子查询 = [not]in [not]exists
select *from t_stud where sno = (select *from t_stud)

--显示选了课程的学生
--子查询结果要求多行一列
select *from t_stud where sno in 
(select sno from t_sel_course)

--显示考了60分的学生
select *from t_stud where sno in 
(select sno from t_sel_course where score = 60)

--显示考了60分的学生
select *from t_stud where exists 
(select * from t_sel_course where score = 60
 and t_sel_course.sno = t_stud.sno)



--显示大于平均分的记录
--子查询结果一行一列
select *from t_sel_course where score > 
(select avg(score) from t_sel_course)



 