#include <memory>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>

class TwistRelayNode : public rclcpp::Node {
public:
  TwistRelayNode()
  : Node("twist_relay_node") {
    // Initialize subscriber
    subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "/spacenav/twist",
      10,
      std::bind(&TwistRelayNode::twist_callback, this, std::placeholders::_1)
    );

    // Initialize publisher
    publisher_ = this->create_publisher<geometry_msgs::msg::TwistStamped>(
      "/servo_node/delta_twist_cmds",
      10
    );
  }

private:
  void twist_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
    auto twist_stamped_msg = geometry_msgs::msg::TwistStamped();
    twist_stamped_msg.twist = *msg;
    twist_stamped_msg.header.stamp = this->get_clock()->now();
    twist_stamped_msg.header.frame_id = "base_link"; // Replace with actual frame ID

    publisher_->publish(twist_stamped_msg);
    RCLCPP_INFO(this->get_logger(), "Publishing TwistStamped message to /servo_node/delta_twist_cmds");
  }

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
  rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr publisher_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TwistRelayNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}