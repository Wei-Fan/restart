#!/usr/bin/env python


# Import required Python code.
import rospy
import random, sys, os, json, base64, urllib, httplib

# Import custom message data.
# from node_example.msg import NodeExampleData
from std_msgs.msg import String

#def callback(data):
#    '''
#    Callback function for the subscriber.
#    '''
#    # Simply print out values in our custom message.
#    rospy.loginfo(rospy.get_name() + " I heard %s", data.message +
#                  ", a + b = %d" % (data.a + data.b))

#def listener():
#    '''
#    Main function.
#    '''
#    # Create a subscriber with appropriate topic, custom message and name of
#    # callback function.
#    rospy.Subscriber('example', NodeExampleData, callback)
#    # Wait for messages on topic, go to callback function when new messages
#    # arrive.
#    rospy.spin()

def talker():
    pub = rospy.Publisher('chatter', String, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) #10Hz

    while not rospy.is_shutdown():
        hello_str = "hello world %s" %rospy.get_time()
        #rospy.loginfo(hello_str)
        pub.publish(hello_str)

        #num = random.randint(1,11)
        #rospy.loginfo(sys.argv[0])
        #rospy.loginfo(os.name)
        #data = {"spam" : "foo", "parrot" : 42}
        #in_json = json.dumps(data)
        #rospy.loginfo(in_json)
        s = 'I am string'
        a = base64.b64encode(s)
        rospy.loginfo(a)
        rate.sleep()

# Main function.
if __name__ == '__main__':
#    # Initialize the node and name it.
#    rospy.init_node('pylistener')
#    # Go to the main loop.
#    listener()
    try:
        talker()
    except rospy.ROSInterruptException:
        pass