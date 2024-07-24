#include"ros/ros.h"//包含ROS头文件  
#include <move_base_msgs/MoveBaseAction.h>//包含move_base action头文件  
#include <actionlib/client/simple_action_client.h>//包含simple_aciton客户端头文件  
// 新加入的头文件
#include <robot_audio/robot_iat.h>
#include <robot_audio/Collect.h>
#include <robot_audio/robot_tts.h>
#include <iostream>
#include <string> //#include "std_msgs/String.h"

// 和语音有关的几个东西
using namespace std;
struct Point   //定义一个名为Student的结构体
{
    float x;  //x坐标
    float y;  //y坐标
    float z;  //姿态z
    float w;   //姿态w
	
    string name; //地点名字
    string present; //介绍语
};
	struct Point m_point[5]={{2.537,0.136,0.038,0.998,"北京","北京，中国的首都"},{2.48,1.077,-0.026,1.000,"广州","广州，自古以来都是中国的商都"},{2.48,2.120,-0.032,0.999,"吉林","吉林，位于中国的东北是人参之都"},{1.026,1.109,-0.012,1.000,"深圳","深圳，中国的科创中心"},{1.073,2.120,0.013,1.000,"上海","上海，中国的经济中心"}};
        /*
        北京：{2.48,0.13,0.03,0.99,"北京","北京，中国的首都"}
        广州：{2.48,1.077,-0.026,1.000,"广州","广州，自古以来都是中国的商都"}
        吉林：{2.48,2.120,-0.032,0.999,"吉林","吉林，位于中国的东北是人参之都"}
        深圳：{1.026,1.109,-0.012,1.000,"深圳","深圳，中国的科创中心"}
        上海：{1.073,2.120,0.013,1.000,"上海","上海，中国的经济中心"}
        */
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> AC;//定义用到的action客户端的别名

class interaction{
    public:
        interaction();
        string voice_collect(); //语音采集
        string voice_dictation(const char* filename); //语音听写
        string voice_tts(const char* text); //语音合成
        void goto_nav(struct Point* point); //导航到目标位置
    private:
        ros::NodeHandle n; //创建一个节点句柄
        actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>* ac; //创建action客户端对象指针
        ros::ServiceClient collect_client,dictation_client,tts_client; //创建客户端
};
interaction::interaction(){
    collect_client = n.serviceClient<robot_audio::Collect>("voice_collect"); //定义语音采集客户端
    dictation_client = n.serviceClient<robot_audio::robot_iat>("voice_iat"); //定义语音听写客户端
    tts_client = n.serviceClient<robot_audio::robot_tts>("voice_tts"); //定义语音合成客户端
}
string interaction::voice_collect(){
    //请求"voice_collect"服务，返回音频保存位置
    ros::service::waitForService("voice_collect");
    robot_audio::Collect srv;
    srv.request.collect_flag = 1;
    collect_client.call(srv);
    return srv.response.voice_filename;
}
string interaction::voice_dictation(const char* filename){
    //请求"voice_dictation"服务，返回听写出的文本
    ros::service::waitForService("voice_iat");
    robot_audio::robot_iat srv;
    srv.request.audiopath = filename;
    dictation_client.call(srv);
    return srv.response.text;
}
string interaction::voice_tts(const char* text){
    //请求"voice_tts"服务，返回合成的文件目录
    ros::service::waitForService("voice_tts");
    robot_audio::robot_tts srv;
    srv.request.text = text;
    tts_client.call(srv);
    string cmd= "play "+srv.response.audiopath;
    system(cmd.c_str());
    sleep(1);
    return srv.response.audiopath;
}

void interaction::goto_nav(struct Point* point){ //导航到目标
    ac = new AC("move_base",true);
    ROS_INFO("Waiting for action server to start.");
    ac->waitForServer();//一直等待move_base Action服务开启
    ROS_INFO("Action server started, sending goal.");
    
            //定义一个导航目标
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now(); //设置时间戳
            //导航点位置信息
    goal.target_pose.pose.position.x = point->x; 
    goal.target_pose.pose.position.y = point->y;
    goal.target_pose.pose.orientation.z = point->z;
    goal.target_pose.pose.orientation.w = point->w;
    ac->sendGoal(goal); //发送导航目标
    ac->waitForResult(); //等待导航结果
    if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) //判断导航状态
       ROS_INFO("Goal succeeded!");
     ac->cancelGoal(); //取消动作
     delete ac;
}




int main(int argc,char **argv){
    ros::init(argc,argv,"my_voice_nav");
    interaction audio; //创建一个交互实例
    string dir,text,path; //创建两个字符串变量
    ros::NodeHandle nh; 
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true);//定义一个acition客户端  
    while(ros::ok()){
        dir = audio.voice_collect(); //采集语音
        text = audio.voice_dictation(dir.c_str()).c_str(); //语音听写
        if(text.find("要到") != string::npos){ //识别到“导航”关键词
            for(int i=0;i<5;i++){ //遍历所有参数
                if(text.find(m_point[i].name.c_str()) != string::npos){ //查找所有导航点是否有匹配的导航点
                    string text1 = "好的，这就带您去";
                    text1 += m_point[i].name;
                    text1 += "馆";
                    audio.voice_tts(text1.c_str());
                    audio.goto_nav(&m_point[i]); //导航到匹配的导航点
                    audio.voice_tts(m_point[i].present.c_str()); //介绍导航语


                    
                    // 回到终点
                    move_base_msgs::MoveBaseGoal goal6_pre;//定义第六个目标点
                    goal6_pre.target_pose.header.frame_id = "map";  
                    goal6_pre.target_pose.header.stamp = ros::Time::now();  
                    goal6_pre.target_pose.pose.position.x =0.275;//目标点x  
                    goal6_pre.target_pose.pose.position.y = 0.006;    //目标点y  
                    goal6_pre.target_pose.pose.orientation.z = 0.00;
                    goal6_pre.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示
                    // move_base_msgs::MoveBaseGoal goal6;//定义第六个目标点 ，上海馆 
                    // goal6.target_pose.header.frame_id = "map";  
                    // goal6.target_pose.header.stamp = ros::Time::now();  
                    // goal6.target_pose.pose.position.x =0.013;//目标点x  
                    // goal6.target_pose.pose.position.y = 0.001;    //目标点y  
                    // goal6.target_pose.pose.orientation.z =-0.002;
                    // goal6.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示
                    move_base_msgs::MoveBaseGoal goal6;//定义第六个目标点 终点
                    goal6.target_pose.header.frame_id = "map";  
                    goal6.target_pose.header.stamp = ros::Time::now();  
                    goal6.target_pose.pose.position.x =0.042;//目标点x  
                    goal6.target_pose.pose.position.y = -0.039;    //目标点y  
                    goal6.target_pose.pose.orientation.z =0.003;
                    goal6.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示
                    // // 终点
                    // ac.sendGoal(goal6_pre);  
                    // while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                    //     {  
                    //     usleep(1000*20);  
                    //     }  
                    // ROS_INFO("机器人到达目标点1");  
                    ac.sendGoal(goal6);  
                    while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                        {  
                        usleep(1000*20);  
                        }  
                    ROS_INFO("机器人到达目标点5");  

                    break;
                }
            }
        }
        
    }
    return 0;
}