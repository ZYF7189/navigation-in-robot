#include<ros/ros.h>
#include<face_rec/recognition_results.h>
using namespace std;
int main(int argc,char** argv)
{
  ros::init(argc,argv,"face_rec_ex");
  ros::NodeHandle n;
  ros::ServiceClient face_rec_client = n.serviceClient<face_rec::recognition_results>("face_recognition_results");
  face_rec::recognition_results face_rec_data;
  ros::service::waitForService("face_recognition_results");
  while(ros::ok())
  {
     face_rec_data.request.mode = 1;
     face_rec_client.call(face_rec_data);
     if(face_rec_data.response.success)
        {
           int face_num = face_rec_data.response.result.num;
           cout<<"我看到了"<<face_num<<"个人脸,他们分别是："<<endl;
           for(int i=0;i<face_num;i++)
           {
            string rec_name = face_rec_data.response.result.face_data[i].name;
            cout<<rec_name<<" ";
           }
           cout<<endl;
        }
  }
  return true;
}