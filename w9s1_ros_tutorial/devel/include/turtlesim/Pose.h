// Generated by gencpp from file turtlesim/Pose.msg
// DO NOT EDIT!


#ifndef TURTLESIM_MESSAGE_POSE_H
#define TURTLESIM_MESSAGE_POSE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace turtlesim
{
template <class ContainerAllocator>
struct Pose_
{
  typedef Pose_<ContainerAllocator> Type;

  Pose_()
    : x(0.0)
    , y(0.0)
    , theta(0.0)
    , linear_velocity(0.0)
    , angular_velocity(0.0)  {
    }
  Pose_(const ContainerAllocator& _alloc)
    : x(0.0)
    , y(0.0)
    , theta(0.0)
    , linear_velocity(0.0)
    , angular_velocity(0.0)  {
  (void)_alloc;
    }



   typedef float _x_type;
  _x_type x;

   typedef float _y_type;
  _y_type y;

   typedef float _theta_type;
  _theta_type theta;

   typedef float _linear_velocity_type;
  _linear_velocity_type linear_velocity;

   typedef float _angular_velocity_type;
  _angular_velocity_type angular_velocity;




  typedef boost::shared_ptr< ::turtlesim::Pose_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::turtlesim::Pose_<ContainerAllocator> const> ConstPtr;

}; // struct Pose_

typedef ::turtlesim::Pose_<std::allocator<void> > Pose;

typedef boost::shared_ptr< ::turtlesim::Pose > PosePtr;
typedef boost::shared_ptr< ::turtlesim::Pose const> PoseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::turtlesim::Pose_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::turtlesim::Pose_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace turtlesim

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'turtlesim': ['/home/pi/Desktop/w9s1_ros_tutorial/src/turtlesim/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::turtlesim::Pose_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::turtlesim::Pose_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::turtlesim::Pose_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::turtlesim::Pose_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::turtlesim::Pose_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::turtlesim::Pose_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::turtlesim::Pose_<ContainerAllocator> >
{
  static const char* value()
  {
    return "863b248d5016ca62ea2e895ae5265cf9";
  }

  static const char* value(const ::turtlesim::Pose_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x863b248d5016ca62ULL;
  static const uint64_t static_value2 = 0xea2e895ae5265cf9ULL;
};

template<class ContainerAllocator>
struct DataType< ::turtlesim::Pose_<ContainerAllocator> >
{
  static const char* value()
  {
    return "turtlesim/Pose";
  }

  static const char* value(const ::turtlesim::Pose_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::turtlesim::Pose_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float32 x\n\
float32 y\n\
float32 theta\n\
\n\
float32 linear_velocity\n\
float32 angular_velocity\n\
";
  }

  static const char* value(const ::turtlesim::Pose_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::turtlesim::Pose_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.x);
      stream.next(m.y);
      stream.next(m.theta);
      stream.next(m.linear_velocity);
      stream.next(m.angular_velocity);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Pose_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::turtlesim::Pose_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::turtlesim::Pose_<ContainerAllocator>& v)
  {
    s << indent << "x: ";
    Printer<float>::stream(s, indent + "  ", v.x);
    s << indent << "y: ";
    Printer<float>::stream(s, indent + "  ", v.y);
    s << indent << "theta: ";
    Printer<float>::stream(s, indent + "  ", v.theta);
    s << indent << "linear_velocity: ";
    Printer<float>::stream(s, indent + "  ", v.linear_velocity);
    s << indent << "angular_velocity: ";
    Printer<float>::stream(s, indent + "  ", v.angular_velocity);
  }
};

} // namespace message_operations
} // namespace ros

#endif // TURTLESIM_MESSAGE_POSE_H
