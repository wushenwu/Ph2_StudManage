This is the StudManagement Project for Ph2 in KR.

As for more details, you can see the documents.

And giving thanks to Robert Pickering's blog
http://strangelights.com/blog/archive/2013/05/20/using-the-android-gridview-as-a-datagrid.aspx

/////////////////////////////////////////////////////////////////

ѧУ����ϵͳ����KRѧϰ�ڼ䣬Ϊ���̵ڶ��׶���ѧ֪ʶ�����ڿν�ʦ�����ѧ���谴�ȶ�ʱ����ɵ���Ŀ��

��ϵͳ�ǶԵ�һ�׶ζ�������ѧ������ϵͳ���������������ݿ�������ݵĹ���Ϊ���ٸ�ģ������ϣ���������ṹ������ơ�

����
DataBase <---> Server <---> M Server <---> Client

���У�
DataBase: ������ݴ洢������һ��ָSQL Server
Server   : ��DataBase�������ٿ����ݴ����ش����ݸ�M Server
M Server : �߼��㣬���ݴ����UI���ݣ��������SQL ��䣬����Server,
           Ҳ�ɽ������ݰ�ȫ�Դ���
Client   : Window����Android�ͻ���


Ŀ��
������ݿ⣬ʵ��������ѧУ��Ϣ����ϵͳ��������
1����ѧ������ʦ���γ̡��༶�Ĺ���

       ��ȷ�������ݹ�ϵ������������ݿ�

2���ֲ���ƣ���˾��ְ����С�޸���������Ӱ��

       SQL Server <-----> Server <----> M Server (Logic) <---->Client (UI)

3�����ñ��ػ��棬ʵ�ָ�Ч�Ĳ�ѯЧ��

      �����޸�ʱ�䣬�����޸�֮��Ĳ�ѯ���ݽ��б��ػ��档

4���������ӳأ��������ݿ����ӣ�����Server��SQL ServerƵ���ġ���ʱ�Ĳ���

5�������̳߳أ�ʵ��Server���M Server, M Server �� ��Client��Ľ�����

6��ͬʱ�ṩPC�ͻ��˺�Android�ͻ��ˣ������û�ʹ��

