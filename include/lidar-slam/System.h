#ifndef SYSTEM_H
#define SYSTEM_H
#include <string>
#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <sensor_msgs/PointCloud.h>
#include <nav_msgs/Path.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/MultiEchoLaserScan.h>
#include <Eigen/Core>
#include "lidar-slam/Tools.h"
#include <tf/tf.h>
#include <geometry_msgs/Point32.h>
#include <boost/foreach.hpp>
#include "lidar-slam/OccupanyMapper.h"
#include <csm/csm_all.h>
#include <sensor_msgs/PointCloud.h>
class System
{
private:
    /* data */
    //相关的话题名称
    std::string m_topic_scan_i_;
    std::string m_topic_map_o_;
    std::string m_topic_pcl_o_;
    std::string m_topic_path_o_;

    //相关的话题发布者和订阅者
    ros::Subscriber m_scan_sub_;
    ros::Publisher m_map_pub_;
    ros::Publisher m_pcl_pub_;
    ros::Publisher m_path_pub_;

    //ros句柄
    ros::NodeHandle m_nh_;

    //是否使用bag包测试系统及bag包地址
    bool m_is_test_ = false;
    std::string m_bag_file_;

    bool m_is_first_frame_;

    //保存前一帧的局部坐标点
    std::vector<Eigen::Vector2d> m_local_pts_;
    //保存前一帧的世界坐标点
    std::vector<Eigen::Vector2d> m_pts_;
    //保存全局坐标系下的位姿向量
    std::vector<Eigen::Vector3d> m_poses_;
    //当前帧的序列号
    int m_scan_id_;
    //路径
    nav_msgs::Path m_path_;

    //imls匹配器
    //Matcher* pt_matcher_;

    //地图
    OccupanyMapper* pt_mapper_;

    //NDT匹配器
    //NDTMatecher m_NDT_matcher_;

    //PLICP匹配器
    //PLMatcher m_pl_matcher_;

    LDP m_prevLDP;
  	sm_params m_PIICPParams;
  	sm_result m_OutputResult;

    //PLICP预测值
    Eigen::Vector3d m_pose_predict_;

public:
    System(std::string scan_topic_i,std::string map_topic_o,std::string pcl_topic_o,std::string path_topic_o,ros::NodeHandle& nh );
    ~System();
    void SetTestMode(std::string bagfile);
    void Run();
    void LaserScanCallback(sensor_msgs::MultiEchoLaserScanConstPtr& msg);
    void PubPath(Eigen::Vector3d& pose, nav_msgs::Path &path, ros::Publisher &mcu_path_pub_);
    void PubPcl(Eigen::Vector3d& pose,std::vector<Eigen::Vector2d> & pts,ros::Publisher &mcu_pcl_pub_);
    void SetPIICPParams();
    void LaserScanToLDP(sensor_msgs::MultiEchoLaserScanConstPtr& pScan,LDP& ldp);
    Eigen::Vector3d  PIICPBetweenTwoFrames(LDP& currentLDPScan,
                                                  Eigen::Vector3d tmprPose);
};









#endif