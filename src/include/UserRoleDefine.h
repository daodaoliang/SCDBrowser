#ifndef USERROLE_DEFINE_H
#define USERROLE_DEFINE_H

//�û�����ɫ��������ֵ˵��
#define RTN_OK               0 //ִ����ȷ
#define RTN_USER_NOT_EXIST  -1 //�û�������
#define RTN_SAVE_FAILURE    -2 //����ʧ��
#define RTN_OTHER_REASION   -3 //��������
#define RTN_PSW_ERROR       -4 //�������
#define RTN_USERNAME_EMPTY  -5 //�û���Ϊ��
#define RTN_USER_IS_EXIST   -6 //�û��Ѵ���
#define RTN_USER_PWD_DIFF   -7 //���������ͬ
#define RTN_ROLENAME_EMPTY  -8 //��ɫ��Ϊ��
#define RTN_ROLE_IS_EXIST   -9 //��ɫ�Ѿ�����
#define RTN_ROLE_NOT_EXIST  -10 //��ɫ������
#define RTN_NAME_CONTAIN_BLANK_SPACE  -11 //���ư����˿ո�
#define RTN_PSW_CONTAIN_BLANK_SPACE	  -14 //��������˿ո�
#define RTN_PSW_EMPTY		-12 //����Ϊ��

//Ȩ�޶���
#define MAX_PRIVILEGES_NUM              10  //���Ȩ����
#define CREATE_SECURITY_MEASURES_TICKET  0 //���ɰ���Ʊ
#define QUERY_SECURITY_MEASURES_TICKET   1 //��ѯ����Ʊ
#define TICKET_TEMPLATE_EDIT             2 //ģ��༭
#define MANGER_MAINTAIN_PROCESS          3 //�����������
#define MANGER_USER_AND_ROLE             4 //�����û�����ɫ
#define MODIFY_PASSWORD                  5 //�޸Ŀ���
#define CONFIG_TICKET_DEST_STATE         6 //���ð���Ʊ��״̬ת������
#define CONFIG_TICKET_CREATE_STRATEGY    7 //���ð���Ʊ�����ɲ���
#define MANGER_SCL_MODEL                 8 //����ģ��
#define MANGER_TICKET_ID_MODEL           9 //������ƱIDģ��


#define CREATE_SECURITY_MEASURES_TICKET_DESC  "���ɰ���Ʊ"     //���ɰ���Ʊ����
#define QUERY_SECURITY_MEASURES_TICKET_DESC   "��ѯ����Ʊ"     //��ѯ����Ʊ����
#define TICKET_TEMPLATE_EDIT_DESC             "Ʊģ��༭"     //ģ��༭
#define MANGER_MAINTAIN_PROCESS_DESC          "�����������"   //���������������
#define MANGER_USER_AND_ROLE_DESC             "�����û�����ɫ" //�����û�����
#define MODIFY_PASSWORD_DESC                  "�޸Ŀ���"       //�޸Ŀ�������
#define CONFIG_TICKET_DEST_STATE_DESC         "���ð���Ʊ��״̬ת������" //���ð���Ʊ��״̬ת����������
#define CONFIG_TICKET_CREATE_STRATEGY_DESC    "���ð���Ʊ�����ɲ���" //���ð���Ʊ�����ɲ�������
#define MANGER_SCL_MODEL_DESC                 "����ģ��"       //ģ�͹�������
#define MANGER_TICKET_ID_MODEL_DESC        "������ƱIDģ��" //������ƱIDģ��



 
#endif