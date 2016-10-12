#ifndef SCL_MODEL_DEFINE_H
#define SCL_MODEL_DEFINE_H

//节点类型定义
#define NODE_TYPE_VOLTLEVEL 0  //电压等级
#define NODE_TYPE_TRANSSUB  1  //变电站
#define NODE_TYPE_CABINET   2  //柜子
#define NODE_TYPE_IED       3  //智能装置


//装置类型
#define IED_TYPE_RELAY     0 //保护装置
#define IED_TYPE_MU        1 //合并单元
#define IED_TYPE_IT        2 //智能终端
#define IED_TYPE_MC        3 //测控装置
#define IED_TYPE_SWITCH    4 //交换机
#define IED_TYPE_MU_IT     5 //合并单元及智能终端
#define IED_TYPE_RELAY_MC  6 //合并单元及智能终端
#define IED_TYPE_OTHER     7 //其它

#endif