# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "restart: 3 messages, 2 services")

set(MSG_I_FLAGS "-Irestart:/home/wade/catkin_ws/src/restart/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(restart_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv" NAME_WE)
add_custom_target(_restart_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "restart" "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv" ""
)

get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/Num.msg" NAME_WE)
add_custom_target(_restart_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "restart" "/home/wade/catkin_ws/src/restart/msg/Num.msg" ""
)

get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/pos_est.msg" NAME_WE)
add_custom_target(_restart_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "restart" "/home/wade/catkin_ws/src/restart/msg/pos_est.msg" "geometry_msgs/Vector3"
)

get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg" NAME_WE)
add_custom_target(_restart_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "restart" "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg" ""
)

get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv" NAME_WE)
add_custom_target(_restart_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "restart" "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(restart
  "/home/wade/catkin_ws/src/restart/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/restart
)
_generate_msg_cpp(restart
  "/home/wade/catkin_ws/src/restart/msg/pos_est.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/restart
)
_generate_msg_cpp(restart
  "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/restart
)

### Generating Services
_generate_srv_cpp(restart
  "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/restart
)
_generate_srv_cpp(restart
  "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/restart
)

### Generating Module File
_generate_module_cpp(restart
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/restart
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(restart_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(restart_generate_messages restart_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv" NAME_WE)
add_dependencies(restart_generate_messages_cpp _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/Num.msg" NAME_WE)
add_dependencies(restart_generate_messages_cpp _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/pos_est.msg" NAME_WE)
add_dependencies(restart_generate_messages_cpp _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg" NAME_WE)
add_dependencies(restart_generate_messages_cpp _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(restart_generate_messages_cpp _restart_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(restart_gencpp)
add_dependencies(restart_gencpp restart_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS restart_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(restart
  "/home/wade/catkin_ws/src/restart/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/restart
)
_generate_msg_eus(restart
  "/home/wade/catkin_ws/src/restart/msg/pos_est.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/restart
)
_generate_msg_eus(restart
  "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/restart
)

### Generating Services
_generate_srv_eus(restart
  "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/restart
)
_generate_srv_eus(restart
  "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/restart
)

### Generating Module File
_generate_module_eus(restart
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/restart
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(restart_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(restart_generate_messages restart_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv" NAME_WE)
add_dependencies(restart_generate_messages_eus _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/Num.msg" NAME_WE)
add_dependencies(restart_generate_messages_eus _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/pos_est.msg" NAME_WE)
add_dependencies(restart_generate_messages_eus _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg" NAME_WE)
add_dependencies(restart_generate_messages_eus _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(restart_generate_messages_eus _restart_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(restart_geneus)
add_dependencies(restart_geneus restart_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS restart_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(restart
  "/home/wade/catkin_ws/src/restart/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/restart
)
_generate_msg_lisp(restart
  "/home/wade/catkin_ws/src/restart/msg/pos_est.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/restart
)
_generate_msg_lisp(restart
  "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/restart
)

### Generating Services
_generate_srv_lisp(restart
  "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/restart
)
_generate_srv_lisp(restart
  "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/restart
)

### Generating Module File
_generate_module_lisp(restart
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/restart
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(restart_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(restart_generate_messages restart_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv" NAME_WE)
add_dependencies(restart_generate_messages_lisp _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/Num.msg" NAME_WE)
add_dependencies(restart_generate_messages_lisp _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/pos_est.msg" NAME_WE)
add_dependencies(restart_generate_messages_lisp _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg" NAME_WE)
add_dependencies(restart_generate_messages_lisp _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(restart_generate_messages_lisp _restart_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(restart_genlisp)
add_dependencies(restart_genlisp restart_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS restart_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(restart
  "/home/wade/catkin_ws/src/restart/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/restart
)
_generate_msg_nodejs(restart
  "/home/wade/catkin_ws/src/restart/msg/pos_est.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/restart
)
_generate_msg_nodejs(restart
  "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/restart
)

### Generating Services
_generate_srv_nodejs(restart
  "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/restart
)
_generate_srv_nodejs(restart
  "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/restart
)

### Generating Module File
_generate_module_nodejs(restart
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/restart
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(restart_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(restart_generate_messages restart_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv" NAME_WE)
add_dependencies(restart_generate_messages_nodejs _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/Num.msg" NAME_WE)
add_dependencies(restart_generate_messages_nodejs _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/pos_est.msg" NAME_WE)
add_dependencies(restart_generate_messages_nodejs _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg" NAME_WE)
add_dependencies(restart_generate_messages_nodejs _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(restart_generate_messages_nodejs _restart_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(restart_gennodejs)
add_dependencies(restart_gennodejs restart_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS restart_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(restart
  "/home/wade/catkin_ws/src/restart/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart
)
_generate_msg_py(restart
  "/home/wade/catkin_ws/src/restart/msg/pos_est.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart
)
_generate_msg_py(restart
  "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart
)

### Generating Services
_generate_srv_py(restart
  "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart
)
_generate_srv_py(restart
  "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart
)

### Generating Module File
_generate_module_py(restart
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(restart_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(restart_generate_messages restart_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/CountNumber.srv" NAME_WE)
add_dependencies(restart_generate_messages_py _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/Num.msg" NAME_WE)
add_dependencies(restart_generate_messages_py _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/pos_est.msg" NAME_WE)
add_dependencies(restart_generate_messages_py _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/msg/NodeExampleData.msg" NAME_WE)
add_dependencies(restart_generate_messages_py _restart_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wade/catkin_ws/src/restart/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(restart_generate_messages_py _restart_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(restart_genpy)
add_dependencies(restart_genpy restart_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS restart_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/restart)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/restart
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(restart_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(restart_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/restart)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/restart
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(restart_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(restart_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/restart)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/restart
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(restart_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(restart_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/restart)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/restart
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(restart_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(restart_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/restart
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(restart_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(restart_generate_messages_py geometry_msgs_generate_messages_py)
endif()
