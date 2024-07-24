// #include<ros/ros.h>
// #include<face_rec/recognition_results.h>
// #include <robot_audio/robot_iat.h>
// #include <robot_audio/Collect.h>
// #include <robot_audio/robot_tts.h>
// #include <actionlib/client/simple_action_client.h>
// #include "move_base_msgs/MoveBaseAction.h"
// #include <iostream>
// #include <string>
// using namespace std;
// typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> AC;//定义用到的action客户端的别名

// class interaction {
// public:
//     interaction();
//     string voice_collect(); //语音采集
//     string voice_dictation(const char* filename); //语音听写
//     string voice_tts(const char* text); //语音合成

// private:
//     ros::NodeHandle n; //创建一个节点句柄
//     actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>* ac; //创建action客户端对象指针
//     ros::ServiceClient collect_client, dictation_client, tts_client; //创建客户端
// };

// interaction::interaction() {
//     collect_client = n.serviceClient<robot_audio::Collect>("voice_collect"); //定义语音采集客户端
//     dictation_client = n.serviceClient<robot_audio::robot_iat>("voice_iat"); //定义语音听写客户端
//     tts_client = n.serviceClient<robot_audio::robot_tts>("voice_tts"); //定义语音合成客户端
// }

// string interaction::voice_collect() {
//     //请求"voice_collect"服务，返回音频保存位置
//     ros::service::waitForService("voice_collect");
//     robot_audio::Collect srv;
//     srv.request.collect_flag = 1;
//     collect_client.call(srv);
//     return srv.response.voice_filename;
// }

// string interaction::voice_dictation(const char* filename) {
//     //请求"voice_dictation"服务，返回听写出的文本
//     ros::service::waitForService("voice_iat");
//     robot_audio::robot_iat srv;
//     srv.request.audiopath = filename;
//     dictation_client.call(srv);
//     return srv.response.text;
// }

// string interaction::voice_tts(const char* text) {
//     //请求"voice_tts"服务，返回合成的文件目录
//     ros::service::waitForService("voice_tts");
//     robot_audio::robot_tts srv;
//     srv.request.text = text;
//     tts_client.call(srv);
//     string cmd = "play " + srv.response.audiopath;
//     system(cmd.c_str());
//     sleep(1);
//     return srv.response.audiopath;
// }

// int main(int argc, char** argv) {
//     ros::init(argc, argv, "my_face_rec");
//     interaction audio; //创建一个交互实例
//     string dir, text, path; //创建两个字符串变量
//     while (ros::ok()) {
//         dir = audio.voice_collect(); //采集语音
//         text = audio.voice_dictation(dir.c_str()).c_str(); //语音听写
//         if (text.find("是谁") != string::npos) {
//             audio.voice_tts("好的，让我看一看");
//             ros::NodeHandle n;
//             ros::ServiceClient face_rec_client = n.serviceClient<face_rec::recognition_results>("face_recognition_results");
//             face_rec::recognition_results face_rec_data;
//             // 等待人脸识别服务启动
//             ros::service::waitForService("face_recognition_results");
//             // ROS_INFO("听到了");
//             while (ros::ok()) {
//                 face_rec_data.request.mode = 1;
//                 face_rec_client.call(face_rec_data);
//                 if (face_rec_data.response.success) {
//                     int face_num = face_rec_data.response.result.num;
//                     cout << "我看到了" << face_num << "个人脸,他们分别是：" << endl;
//                     // "我看到了" + to_string(face_num) + 
//                     string names = "你是：";
//                     for (int i = 0; i < face_num; i++) {
//                         string rec_name = face_rec_data.response.result.face_data[i].name;
//                         cout << rec_name << " ";
//                         names += rec_name + ",";
//                     }
//                     cout << endl;
//                     // 去掉最后一个多余的逗号
//                     names = names.substr(0, names.length() - 3);
//                     audio.voice_tts(names.c_str());
//                 }
//             }
//             return 0;
//         }
    
//     }

//     return 0;
// }
// #include<ros/ros.h>
// #include<face_rec/recognition_results.h>
// #include <robot_audio/robot_iat.h>
// #include <robot_audio/Collect.h>
// #include <robot_audio/robot_tts.h>
// #include <actionlib/client/simple_action_client.h>
// #include "move_base_msgs/MoveBaseAction.h"
// #include <iostream>
// #include <string>


// using namespace std;
// typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> AC; // 定义用到的action客户端的别名

// class Interaction {
// public:
//     Interaction();
//     string voiceCollect(); // 语音采集
//     string voiceDictation(const string& filename); // 语音听写
//     string voiceTTS(const string& text); // 语音合成
//     void faceRecognition(); // 人脸识别

// private:
//     ros::NodeHandle nh;
//     actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>* ac; // 定义action客户端对象指针
//     ros::ServiceClient collectClient, dictationClient, ttsClient, faceRecClient; // 定义客户端
// };

// Interaction::Interaction() {
//     collectClient = nh.serviceClient<robot_audio::Collect>("voice_collect");
//     dictationClient = nh.serviceClient<robot_audio::robot_iat>("voice_iat");
//     ttsClient = nh.serviceClient<robot_audio::robot_tts>("voice_tts");
//     faceRecClient = nh.serviceClient<face_rec::recognition_results>("face_recognition_results");
// }

// string Interaction::voiceCollect() {
//     ros::service::waitForService("voice_collect");
//     robot_audio::Collect srv;
//     srv.request.collect_flag = 1;
//     if (collectClient.call(srv)) {
//         return srv.response.voice_filename;
//     } else {
//         ROS_ERROR("Failed to call voice_collect service");
//         return "";
//     }
// }

// string Interaction::voiceDictation(const string& filename) {
//     ros::service::waitForService("voice_iat");
//     robot_audio::robot_iat srv;
//     srv.request.audiopath = filename;
//     if (dictationClient.call(srv)) {
//         return srv.response.text;
//     } else {
//         ROS_ERROR("Failed to call voice_iat service");
//         return "";
//     }
// }// #include<ros/ros.h>
#include<ros/ros.h>
#include<face_rec/recognition_results.h>
#include <robot_audio/robot_iat.h>
#include <robot_audio/Collect.h>
#include <robot_audio/robot_tts.h>
#include <actionlib/client/simple_action_client.h>
#include "move_base_msgs/MoveBaseAction.h"
#include <iostream>
#include <string>


using namespace std;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> AC; // 定义用到的action客户端的别名

class Interaction {
public:
    Interaction();
    string voiceCollect(); // 语音采集
    string voiceDictation(const string& filename); // 语音听写
    string voiceTTS(const string& text); // 语音合成
    void faceRecognition(); // 人脸识别

private:
    ros::NodeHandle nh;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>* ac; // 定义action客户端对象指针
    ros::ServiceClient collectClient, dictationClient, ttsClient, faceRecClient; // 定义客户端
};

Interaction::Interaction() {
    collectClient = nh.serviceClient<robot_audio::Collect>("voice_collect");
    dictationClient = nh.serviceClient<robot_audio::robot_iat>("voice_iat");
    ttsClient = nh.serviceClient<robot_audio::robot_tts>("voice_tts");
    faceRecClient = nh.serviceClient<face_rec::recognition_results>("face_recognition_results");
}

string Interaction::voiceCollect() {
    ros::service::waitForService("voice_collect");
    robot_audio::Collect srv;
    srv.request.collect_flag = 1;
    if (collectClient.call(srv)) {
        return srv.response.voice_filename;
    } else {
        ROS_ERROR("Failed to call voice_collect service");
        return "";
    }
}

string Interaction::voiceDictation(const string& filename) {
    ros::service::waitForService("voice_iat");
    robot_audio::robot_iat srv;
    srv.request.audiopath = filename;
    if (dictationClient.call(srv)) {
        return srv.response.text;
    } else {
        ROS_ERROR("Failed to call voice_iat service");
        return "";
    }
}

string Interaction::voiceTTS(const string& text) {
    ros::service::waitForService("voice_tts");
    robot_audio::robot_tts srv;
    srv.request.text = text;
    if (ttsClient.call(srv)) {
        string cmd = "play " + srv.response.audiopath;
        system(cmd.c_str());
        sleep(1);
        return srv.response.audiopath;
    } else {
        ROS_ERROR("Failed to call voice_tts service");
        return "";
    }
}

void Interaction::faceRecognition() {
    ros::service::waitForService("face_recognition_results");
    face_rec::recognition_results srv;
    srv.request.mode = 1;
    if (faceRecClient.call(srv)) {
        if (srv.response.success && srv.response.result.num > 0) {
            string recName = srv.response.result.face_data[0].name;
            cout << "我看到了一个人脸，他是：" << recName << endl;
            voiceTTS("你是：" + recName);
        } else {
            ROS_ERROR("Face recognition failed or no faces detected");
        }
    } else {
        ROS_ERROR("Failed to call face_recognition_results service");
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "my_face_rec");
    Interaction audio;
    string dir, text;
    while (ros::ok()) {
        dir = audio.voiceCollect();
        if (dir.empty()) continue; // 如果采集失败，跳过此循环
        text = audio.voiceDictation(dir);
        if (text.empty()) continue; // 如果听写失败，跳过此循环

        

        if (text.find("是谁") != string::npos) {
            audio.voiceTTS("好的，让我看一看");
            audio.faceRecognition();
        }
        
    }
    return 0;
}
