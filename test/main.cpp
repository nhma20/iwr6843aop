/*****************************************************************************/
// Includes
/*****************************************************************************/

#include <rclcpp/rclcpp.hpp>
#include <rclcpp/qos.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

#include <iwr6843aop.hpp>
#include <cstdio>
#include <chrono>

/*****************************************************************************/
// Implementation
/*****************************************************************************/


class IWR6843_publisher : public rclcpp::Node {
public:
	IWR6843_publisher() : Node("IWR6843_publisher") {

    this->declare_parameter("cfg_path");
    cfg_path_ = this->get_parameter("cfg_path");
    cfg_path_str_ = cfg_path_.as_string();
    //str_param.value_to_string().c_str(),
    
    if (!iwr6843aop_.init())
    {
        RCLCPP_FATAL(this->get_logger(), "Could not initialize IWR6843AOP device");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        rclcpp::shutdown();
    }

    iwr6843aop_.configure(cfg_path_str_);
    iwr6843aop_.start();

    pcl_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/iwr6843_pcl", 10);

    timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&IWR6843_publisher::timer_callback, this));

  }

  ~IWR6843_publisher() {
		RCLCPP_INFO(this->get_logger(),  "Shutting down IWR6843_publisher node..");
    iwr6843aop_.stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

  private:
    rclcpp::TimerBase::SharedPtr timer_;
    iwr6843::Iwr6843aop iwr6843aop_;
    std::vector<radar::RadarPointCartesian> radarData_;
    rclcpp::Parameter cfg_path_;
    std::string cfg_path_str_;


    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pcl_pub_;

    void timer_callback();

};



void IWR6843_publisher::timer_callback() {

  iwr6843aop_.getpointCloud(radarData_); 
  
  //   for (auto& point : radarData_)
  // {
  //     std::cout << "x: " << point.x
  //             << " y: " << point.y
  //             << " z: " << point.z
  //             // << " V:" << point.velocity
  //             // << " S:" << point.snr
  //             // << " N:" << point.noise
  //             // << " M:" << point.magnitude
  //             << "\n";

  // }

  auto pcl2_msg = sensor_msgs::msg::PointCloud2();
  pcl2_msg.header.frame_id = "iwr6843_frame";
  pcl2_msg.header.stamp = this->get_clock()->now();

  pcl2_msg.fields.resize(3);
  pcl2_msg.fields[0].name = 'x';
  pcl2_msg.fields[0].offset = 0;
  pcl2_msg.fields[0].datatype = sensor_msgs::msg::PointField::FLOAT32;
  pcl2_msg.fields[0].count = 1;
  pcl2_msg.fields[1].name = 'y';
  pcl2_msg.fields[1].offset = 4;
  pcl2_msg.fields[1].datatype = sensor_msgs::msg::PointField::FLOAT32;
  pcl2_msg.fields[1].count = 1;
  pcl2_msg.fields[2].name = 'z';
  pcl2_msg.fields[2].offset = 8;
  pcl2_msg.fields[2].datatype = sensor_msgs::msg::PointField::FLOAT32;
  pcl2_msg.fields[2].count = 1;

  const uint32_t POINT_STEP = 12;

  if(radarData_.size() > 0){

      pcl2_msg.data.resize(std::max((size_t)1, radarData_.size()) * POINT_STEP, 0x00);

  } 
  else {

      return; // do not publish empty pointcloud

  }

  pcl2_msg.point_step = POINT_STEP; // size (bytes) of 1 point (float32 * dimensions (3 when xyz))
  pcl2_msg.row_step = pcl2_msg.data.size();//pcl2_msg.point_step * pcl2_msg.width; // only 1 row because unordered
  pcl2_msg.height = 1; // because unordered cloud
  pcl2_msg.width = pcl2_msg.row_step / POINT_STEP; // number of points in cloud
  pcl2_msg.is_dense = false; // there may be invalid points

  uint8_t *pcl2_ptr = pcl2_msg.data.data();


  for (auto& point : radarData_)
  {
      *(reinterpret_cast<float*>(pcl2_ptr + 0)) = point.x;
      *(reinterpret_cast<float*>(pcl2_ptr + 4)) = point.y;
      *(reinterpret_cast<float*>(pcl2_ptr + 8)) = point.z;
      pcl2_ptr += POINT_STEP;
  }

  pcl_pub_->publish(pcl2_msg);

}




int main(int argc, char *argv[])
{

  std::cout << "Starting IWR6843_publisher node..." << std::endl;

  rclcpp::init(argc, argv);

  auto node = std::make_shared<IWR6843_publisher>();

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;

}
