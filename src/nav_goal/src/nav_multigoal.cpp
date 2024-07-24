#include"ros/ros.h"//包含ROS头文件  
#include <move_base_msgs/MoveBaseAction.h>//包含move_base action头文件  
#include <actionlib/client/simple_action_client.h>//包含simple_aciton客户端头文件  
// 新加入的头文件
#include <robot_audio/robot_iat.h>
#include <robot_audio/Collect.h>
#include <robot_audio/robot_tts.h>
#include <iostream>
#include <std_msgs/String.h> //#include "std_msgs/String.h"

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
	struct Point m_point[5]={{2.48,0.13,0.03,0.99,"北京","北京，中国的首都"},{2.48,1.077,-0.026,1.000,"广州","广州，自古以来都是中国的商都"},{2.48,2.120,-0.032,0.999,"吉林","吉林，位于中国的东北是人参之都"},{1.026,1.109,-0.012,1.000,"深圳","深圳，中国的科创中心"},{1.073,2.120,0.013,1.000,"上海","上海，中国的经济中心"}};
        /*
        北京：{2.48,0.13,0.03,0.99,"北京","北京，中国的首都"}
        广州：{2.48,1.077,-0.026,1.000,"广州","广州，自古以来都是中国的商都"}
        吉林：{2.48,2.120,-0.032,0.999,"吉林","吉林，位于中国的东北是人参之都"}
        深圳：{1.026,1.109,-0.012,1.000,"深圳","深圳，中国的科创中心"}
        上海：{1.073,2.120,0.013,1.000,"上海","上海，中国的经济中心"}
        */
// typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> AC;//定义用到的action客户端的别名

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



int main(int argc,char **argv){
    ros::init(argc,argv,"nav_goal");
    interaction audio; //创建一个交互实例
    string dir,text,path; //创建两个字符串变量
    ros::NodeHandle nh; 
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true);//定义一个acition客户端  
    while(ros::ok()){
        dir = audio.voice_collect(); //采集语音
        text = audio.voice_dictation(dir.c_str()).c_str(); //语音听写
        if(text.find("参观一圈") != string::npos){ //识别到“导航”关键词
            audio.voice_tts("好的");
            move_base_msgs::MoveBaseGoal goal1_pre;//定义第一个目标点 北京馆 
            goal1_pre.target_pose.header.frame_id = "map";  
            goal1_pre.target_pose.header.stamp = ros::Time::now();  
            goal1_pre.target_pose.pose.position.x = 2.28;//目标点x 
            goal1_pre.target_pose.pose.position.y = 0.13;//目标点y  
            goal1_pre.target_pose.pose.orientation.z = 0.03;
            goal1_pre.target_pose.pose.orientation.w = 0.99;//目标点姿态四元数表示 
            move_base_msgs::MoveBaseGoal goal1;//定义第一个目标点 北京馆 
            goal1.target_pose.header.frame_id = "map";  
            goal1.target_pose.header.stamp = ros::Time::now();  
            goal1.target_pose.pose.position.x = 2.537;//目标点x 
            goal1.target_pose.pose.position.y = 0.136;//目标点y  
            goal1.target_pose.pose.orientation.z =0.038;
            goal1.target_pose.pose.orientation.w = 0.998;//目标点姿态四元数表示

            move_base_msgs::MoveBaseGoal goal2_pre;//定义第二个目标点 ，广州馆 
            goal2_pre.target_pose.header.frame_id = "map";  
            goal2_pre.target_pose.header.stamp = ros::Time::now();  
            goal2_pre.target_pose.pose.position.x = 2.28;//目标点x  
            goal2_pre.target_pose.pose.position.y = 1.077; //目标点y  
            goal2_pre.target_pose.pose.orientation.z = -0.026;  
            goal2_pre.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示  
            move_base_msgs::MoveBaseGoal goal2;//定义第二个目标点 ，广州馆 
            goal2.target_pose.header.frame_id = "map";  
            goal2.target_pose.header.stamp = ros::Time::now();  
            goal2.target_pose.pose.position.x = 2.527;//目标点x  
            goal2.target_pose.pose.position.y =1.158; //目标点y  
            goal2.target_pose.pose.orientation.z =  -0.026;  
            goal2.target_pose.pose.orientation.w =1.000;//目标点姿态四元数表示   

            move_base_msgs::MoveBaseGoal goal3_pre;//定义第三个目标点 ，吉林馆 
            goal3_pre.target_pose.header.frame_id = "map";  
            goal3_pre.target_pose.header.stamp = ros::Time::now();  
            goal3_pre.target_pose.pose.position.x = 2.28;//目标点x  
            goal3_pre.target_pose.pose.position.y = 2.120;    //目标点y  
            goal3_pre.target_pose.pose.orientation.z = -0.032;  
            goal3_pre.target_pose.pose.orientation.w = 0.999;//目标点姿态四元数表示
            move_base_msgs::MoveBaseGoal goal3;//定义第三个目标点 ，吉林馆 
            goal3.target_pose.header.frame_id = "map";  
            goal3.target_pose.header.stamp = ros::Time::now();  
            goal3.target_pose.pose.position.x = 2.500;//目标点x  
            goal3.target_pose.pose.position.y = 2.136;    //目标点y  
            goal3.target_pose.pose.orientation.z = 0.039;  
            goal3.target_pose.pose.orientation.w =0.999;//目标点姿态四元数表示

            move_base_msgs::MoveBaseGoal goal4_pre;//定义第四个目标点 ，深圳馆 
            goal4_pre.target_pose.header.frame_id = "map";  
            goal4_pre.target_pose.header.stamp = ros::Time::now();  
            goal4_pre.target_pose.pose.position.x =0.826;//目标点x  
            goal4_pre.target_pose.pose.position.y = 1.109;    //目标点y  
            goal4_pre.target_pose.pose.orientation.z = -0.012;  
            goal4_pre.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示
            move_base_msgs::MoveBaseGoal goal4;//定义第四个目标点 ，深圳馆 
            goal4.target_pose.header.frame_id = "map";  
            goal4.target_pose.header.stamp = ros::Time::now();  
            goal4.target_pose.pose.position.x =1.059;//目标点x  
            goal4.target_pose.pose.position.y =  1.113;    //目标点y  
            goal4.target_pose.pose.orientation.z = -0.039;  
            goal4.target_pose.pose.orientation.w =0.999;//目标点姿态四元数表示


            move_base_msgs::MoveBaseGoal goal5_pre;//定义第五个目标点 ，上海馆 
            goal5_pre.target_pose.header.frame_id = "map";  
            goal5_pre.target_pose.header.stamp = ros::Time::now();  
            goal5_pre.target_pose.pose.position.x =0.873;//目标点x  
            goal5_pre.target_pose.pose.position.y = 2.120;    //目标点y  
            goal5_pre.target_pose.pose.orientation.z =0.013;  
            goal5_pre.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示
            move_base_msgs::MoveBaseGoal goal5;//定义第五个目标点 ，上海馆 
            goal5.target_pose.header.frame_id = "map";  
            goal5.target_pose.header.stamp = ros::Time::now();  
            goal5.target_pose.pose.position.x =1.069;//目标点x  
            goal5.target_pose.pose.position.y = 2.099;    //目标点y  
            goal5.target_pose.pose.orientation.z =-0.028;  
            goal5.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示

            // 回到终点
            move_base_msgs::MoveBaseGoal goal6_pre;//定义第六个目标点
            goal6_pre.target_pose.header.frame_id = "map";  
            goal6_pre.target_pose.header.stamp = ros::Time::now();  
            goal6_pre.target_pose.pose.position.x =0.275;//目标点x  
            goal6_pre.target_pose.pose.position.y = 0.006;    //目标点y  
            goal6_pre.target_pose.pose.orientation.z = 0.00;
            goal6_pre.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示
            move_base_msgs::MoveBaseGoal goal6;//定义第六个目标点 终点
            goal6.target_pose.header.frame_id = "map";  
            goal6.target_pose.header.stamp = ros::Time::now();  
            goal6.target_pose.pose.position.x =0.042;//目标点x  
            goal6.target_pose.pose.position.y = -0.039;    //目标点y  
            goal6.target_pose.pose.orientation.z =0.003;
            goal6.target_pose.pose.orientation.w = 1.000;//目标点姿态四元数表示



            // 转角点（防止撞墙）
            move_base_msgs::MoveBaseGoal turn;//定义第六个目标点
            turn.target_pose.header.frame_id = "map";  
            turn.target_pose.header.stamp = ros::Time::now();  
            turn.target_pose.pose.position.x =1.937;//目标点x  
            turn.target_pose.pose.position.y = 0.006;    //目标点y  
            turn.target_pose.pose.orientation.z = -0.101;
            turn.target_pose.pose.orientation.w = 0.995;//目标点姿态四元数表示
            

            int Num = 1;//定义巡检的圈数  
            for(int i=0;i<Num;i++)  
            {  


            


            // // 转角点1
            // ac.sendGoal(turn);  
            // while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
            //     {  
            //     usleep(1000*20);  
            //     }  
            // ROS_INFO("机器人到达目标点1");  
            
            // // 转角点2
            // ac.sendGoal(goal6_pre);  
            // while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
            //     {  
            //     usleep(1000*20);  
            //     }  
            // ROS_INFO("机器人到达目标点1");  


            // 上海馆
            ac.sendGoal(goal5_pre);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                { 
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点1");  
            ac.sendGoal(goal5);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点5");  
            audio.voice_tts(m_point[4].present.c_str()); //介绍导航语
            // 深圳馆
            ac.sendGoal(goal4_pre);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点1");  
            ac.sendGoal(goal4);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点4"); 
            audio.voice_tts(m_point[3].present.c_str()); //介绍导航语


            // 北京馆
            ac.sendGoal(goal1_pre);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                } 
            ROS_INFO("机器人到达目标点1");  
            ac.sendGoal(goal1);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点1");  
            audio.voice_tts(m_point[0].present.c_str()); //介绍导航语

             // 吉林馆
            ac.sendGoal(goal3_pre);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点1");  
            ac.sendGoal(goal3);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点3"); 
            audio.voice_tts(m_point[2].present.c_str()); //介绍导航语
            

            // 广州馆
            ac.sendGoal(goal2_pre);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点1");  
            ac.sendGoal(goal2);  
            while(!(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED))  
                {  
                usleep(1000*20);  
                }  
            ROS_INFO("机器人到达目标点2"); 
            audio.voice_tts(m_point[1].present.c_str()); //介绍导航语
            

           
            

            


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
            }  

            }
        
    }
    return 0;
}