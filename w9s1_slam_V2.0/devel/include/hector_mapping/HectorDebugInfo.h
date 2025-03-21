// Generated by gencpp from file hector_mapping/HectorDebugInfo.msg
// DO NOT EDIT!


#ifndef HECTOR_MAPPING_MESSAGE_HECTORDEBUGINFO_H
#define HECTOR_MAPPING_MESSAGE_HECTORDEBUGINFO_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <hector_mapping/HectorIterData.h>

namespace hector_mapping
{
template <class ContainerAllocator>
struct HectorDebugInfo_
{
  typedef HectorDebugInfo_<ContainerAllocator> Type;

  HectorDebugInfo_()
    : iterData()  {
    }
  HectorDebugInfo_(const ContainerAllocator& _alloc)
    : iterData(_alloc)  {
  (void)_alloc;
    }



   typedef std::vector< ::hector_mapping::HectorIterData_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::hector_mapping::HectorIterData_<ContainerAllocator> >::other >  _iterData_type;
  _iterData_type iterData;




  typedef boost::shared_ptr< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> const> ConstPtr;

}; // struct HectorDebugInfo_

typedef ::hector_mapping::HectorDebugInfo_<std::allocator<void> > HectorDebugInfo;

typedef boost::shared_ptr< ::hector_mapping::HectorDebugInfo > HectorDebugInfoPtr;
typedef boost::shared_ptr< ::hector_mapping::HectorDebugInfo const> HectorDebugInfoConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::hector_mapping::HectorDebugInfo_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace hector_mapping

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'hector_mapping': ['/home/pi/Desktop/w9s1_slam/src/hector_slam/hector_mapping/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "4d33c0696c0c536f5c1447c260756674";
  }

  static const char* value(const ::hector_mapping::HectorDebugInfo_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x4d33c0696c0c536fULL;
  static const uint64_t static_value2 = 0x5c1447c260756674ULL;
};

template<class ContainerAllocator>
struct DataType< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "hector_mapping/HectorDebugInfo";
  }

  static const char* value(const ::hector_mapping::HectorDebugInfo_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "HectorIterData[] iterData\n\
================================================================================\n\
MSG: hector_mapping/HectorIterData\n\
float64[9] hessian\n\
float64 conditionNum\n\
float64 determinant\n\
float64 conditionNum2d\n\
float64 determinant2d\n\
";
  }

  static const char* value(const ::hector_mapping::HectorDebugInfo_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.iterData);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct HectorDebugInfo_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::hector_mapping::HectorDebugInfo_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::hector_mapping::HectorDebugInfo_<ContainerAllocator>& v)
  {
    s << indent << "iterData[]" << std::endl;
    for (size_t i = 0; i < v.iterData.size(); ++i)
    {
      s << indent << "  iterData[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::hector_mapping::HectorIterData_<ContainerAllocator> >::stream(s, indent + "    ", v.iterData[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // HECTOR_MAPPING_MESSAGE_HECTORDEBUGINFO_H
