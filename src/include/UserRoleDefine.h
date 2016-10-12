#ifndef USERROLE_DEFINE_H
#define USERROLE_DEFINE_H

//用户、角色操作返回值说明
#define RTN_OK               0 //执行正确
#define RTN_USER_NOT_EXIST  -1 //用户不存在
#define RTN_SAVE_FAILURE    -2 //保存失败
#define RTN_OTHER_REASION   -3 //其它错误
#define RTN_PSW_ERROR       -4 //口令错误
#define RTN_USERNAME_EMPTY  -5 //用户名为空
#define RTN_USER_IS_EXIST   -6 //用户已存在
#define RTN_USER_PWD_DIFF   -7 //两个口令不相同
#define RTN_ROLENAME_EMPTY  -8 //角色名为空
#define RTN_ROLE_IS_EXIST   -9 //角色已经存在
#define RTN_ROLE_NOT_EXIST  -10 //角色不存在
#define RTN_NAME_CONTAIN_BLANK_SPACE  -11 //名称包含了空格
#define RTN_PSW_CONTAIN_BLANK_SPACE	  -14 //密码包含了空格
#define RTN_PSW_EMPTY		-12 //口令为空

//权限定义
#define MAX_PRIVILEGES_NUM              10  //最大权限数
#define CREATE_SECURITY_MEASURES_TICKET  0 //生成安措票
#define QUERY_SECURITY_MEASURES_TICKET   1 //查询安措票
#define TICKET_TEMPLATE_EDIT             2 //模板编辑
#define MANGER_MAINTAIN_PROCESS          3 //管理检修流程
#define MANGER_USER_AND_ROLE             4 //管理用户及角色
#define MODIFY_PASSWORD                  5 //修改口令
#define CONFIG_TICKET_DEST_STATE         6 //配置安措票的状态转换规则
#define CONFIG_TICKET_CREATE_STRATEGY    7 //配置安措票的生成策略
#define MANGER_SCL_MODEL                 8 //管理模型
#define MANGER_TICKET_ID_MODEL           9 //管理安措票ID模板


#define CREATE_SECURITY_MEASURES_TICKET_DESC  "生成安措票"     //生成安措票描述
#define QUERY_SECURITY_MEASURES_TICKET_DESC   "查询安措票"     //查询安措票描述
#define TICKET_TEMPLATE_EDIT_DESC             "票模板编辑"     //模板编辑
#define MANGER_MAINTAIN_PROCESS_DESC          "管理检修流程"   //管理检修流程描述
#define MANGER_USER_AND_ROLE_DESC             "管理用户及角色" //管理用户描述
#define MODIFY_PASSWORD_DESC                  "修改口令"       //修改口令描述
#define CONFIG_TICKET_DEST_STATE_DESC         "配置安措票的状态转换规则" //配置安措票的状态转换规则描述
#define CONFIG_TICKET_CREATE_STRATEGY_DESC    "配置安措票的生成策略" //配置安措票的生成策略描述
#define MANGER_SCL_MODEL_DESC                 "管理模型"       //模型管理描述
#define MANGER_TICKET_ID_MODEL_DESC        "管理安措票ID模板" //管理安措票ID模板



 
#endif