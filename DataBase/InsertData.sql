USE Ph2_Project
GO

---�༶
insert t_class values('cls001','14���������ѧ�뼼��')
insert t_class values('cls002','14����������Ϣ')
insert t_class values('cls003','13����ѧ����')
insert t_class values('cls004','11����Ϣ��ѧ')
insert t_class values('cls005','12��Ӣ��')

--��ʦ
insert t_teacher values('t001', '��sir', '��', '2014-10-2', '����')
insert t_teacher values('t002', '��sir', 'Ů', '2014-10-2', '�߼�����ʦ')
insert t_teacher values('t003', '��sir', '��', '2014-10-2', '���߼�����ʦ')
insert t_teacher values('t004', '��sir', '��', '2014-10-2', '������')

--ѧ��
insert t_stud values('S001', '��һ', '��', '2014-9-1 11:00:00', '�����人', 'cls001', 't001')
insert t_stud values('S002', '����', '��', '2014-9-2 11:00:00', '�Ĵ��ɶ�', 'cls001', 't001')
insert t_stud values('S003', '����', 'Ů', '2014-9-3 11:00:00', '��������', 'cls005', 't002')
insert t_stud values('S004', '����', '��', '2014-9-3 11:00:00', '��������', 'cls005', 't004')
insert t_stud values('S005', '����', 'Ů', '2014-9-4 11:00:00', '��������', 'cls003', 't002')
insert t_stud values('S006', '��һ', 'δ֪', '2014-5-1 11:00:00', 'ɽ������', 'cls002', 't001')
insert t_stud values('S007', '��һ', '����', '2014-6-1 11:00:00', 'ɽ����ͬ', 'cls004', 't001')

--�γ�
insert t_course values('c101', 'C����', 30)
insert t_course values('c102', 'C++', 30)
insert t_course values('c103', '���ݿ�', 20)
insert t_course values('c104', '���ݽṹ', 25)
insert t_course values('c105', 'OS', 40)



--ѡ��
insert t_sel_course values('S002', 'c101', 60)
insert t_sel_course values('S002', 'c102', 70)
insert t_sel_course values('S003', 'c103', 80)
insert t_sel_course values('S004', 'c101', 90)
insert t_sel_course values('S004', 'c104', 40)
insert t_sel_course values('S005', 'c101', 80)
insert t_sel_course values('S005', 'c103', 60)
insert t_sel_course values('S005', 'c105', 30)

---�ڿα�
insert t_teach_course values('t001', 'c101')
insert t_teach_course values('t001', 'c102')
insert t_teach_course values('t003', 'c101')
insert t_teach_course values('t004', 'c105')

---���������ɳ����Զ�����

/*
--ɾ��
delete t_stud where sex = '��'
delete t_stud where sno = 's001'


--�޸�
update t_stud set sex = 'Ů', birthday='2012-9-9'
 where sno='s001'

update t_stud set sname = '����' where sno='s004'
update t_stud set sname = '�Ŷ�' where sno='s002'
update t_stud set sname = '����' where sno='s003'
update t_stud set sname = 'zhang', sex = '����' where sno='s005'


--��������
insert into t_test select sno from t_stud
select *from t_test

*/

--��ѯ

--*��ʾ�����ֶ�
select *from t_stud

--��ʾָ���ֶ�
select sno,sname from t_stud

--ָ���ֶα���
select sno as 'ѧ��', sname as '����' from t_stud
select 'ѧ��'=sno, '����'=sname from t_stud


--������ѯ
select *from t_stud where sno='S001'
select *from t_stud where sname like 'b%' --ģ����ѯ
select *from t_stud where sname like '__ck%' --ģ����ѯ
select *from t_stud where sname like '%c%' --ģ����ѯ
select *from t_stud where sname like '_��%' --ģ����ѯ
select *from t_stud where sex='��' and sname like '��%'
select *from t_stud where sex='��' or  sex='Ů'
select *from t_stud where not sex='Ů'

--��Χ
---select *from t_test where testid between 10 and 30
select *from t_stud where birthday between '2014-9-1' and '2014-09-03'
select *from t_stud where birthday like '%9%'

--�� in
select *from t_stud where sex in ('��', 'Ů')
select *from t_stud where sex not in ('��', 'Ů')

/*
--null
select *from t_test
select *from t_test where testname is null
select *from t_test wh ere testname is not null
*/

--�ۺϺ���
select * from t_sel_course
select sum(score) as '�ܳɼ�' from t_sel_course
select sum(distinct/*ȥ���ظ�*/ score) as '�ܳɼ�' from t_sel_course
select avg(score) as 'avg' from t_sel_course
select count(distinct sno) as 'count' from t_sel_course
select count(*) as 'count' from t_sel_course

select max(score) as 'max' from t_sel_course
select min(score) as 'min' from t_sel_course
select left('hello', 2) 
select substring('hello', 2, 3) 
select *from t_stud where DatePart(dd, birthday)=3
select *from t_stud where DatePart(dw, birthday)=5

--�Ӳ�ѯ = [not]in [not]exists
select *from t_stud where sno = (select *from t_stud)

--��ʾѡ�˿γ̵�ѧ��
--�Ӳ�ѯ���Ҫ�����һ��
select *from t_stud where sno in 
(select sno from t_sel_course)

--��ʾ����60�ֵ�ѧ��
select *from t_stud where sno in 
(select sno from t_sel_course where score = 60)

--��ʾ����60�ֵ�ѧ��
select *from t_stud where exists 
(select * from t_sel_course where score = 60
 and t_sel_course.sno = t_stud.sno)



--��ʾ����ƽ���ֵļ�¼
--�Ӳ�ѯ���һ��һ��
select *from t_sel_course where score > 
(select avg(score) from t_sel_course)



 