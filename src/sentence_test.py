#!/usr/bin/python

import rospy
import re
from datetime import datetime
from datetime import timedelta

if __name__ == '__main__':
    ##-----------------initialize ros node-------------------

    sen = " I want to do my homework at 5 o'clock pm on April 26th."
    # Initialize the node and name it.
    rospy.init_node('pytalker')
    
    now = datetime.now()
    due = datetime.now()

    first = 0
    if "at"  in sen:
        m = re.search(r"at [0-9]+ o'clock", sen)
        temp = sen[m.start() : m.end()]
        first = m.start()
        # print temp
        m = re.search(r"[0-9]+",temp)
        temp = temp[m.start() : m.end()]
        # print int(temp)
        due = datetime(due.year,due.month,due.day,int(temp))
        # print due

    if (("this afternoon" in sen)|("this evening" in sen)|("this night" in sen))&(due.hour<12):
        m = re.search(r"this", sen)
        first = m.start()
        due = due + timedelta(hours = 12)
    elif ("pm" in sen)&(due.hour<12):
        due = due +timedelta(hours = 12)
    elif ("tomorrow morning" in sen):
        due = due +timedelta(days =1)
    elif ("tomorrow afternoon" in sen)|("tomorrow evening"in sen)|("tomorrow night"in sen):
        due = due +timedelta(days = 1)
        if due.hour<12:
            due = due + timedelta(hours = 12)    
    elif ("tomorrow" in sen):
        due = due + timedelta(days =1)

    if ("the day after tomorrow" in sen):
        due = due +timedelta(days =2)

    month_complete = {'January':1,"February":2,"March":3,"April":4,"May":5,"June":6,\
    "July":7,"August":8,"September":9,"October":10,"November":11,"December":12}

    if "on" in sen:
        m = re.search("on",sen)
        if m.start()<first :
            first = m.start()
        temp = "";
        for k,v in month_complete.items():
            if k in sen:
                # print v
                # print now.month
                due = datetime(due.year,v,due.day,due.hour,due.minute)
                m = re.search(k, sen)
                temp = sen[m.end()+1:] 

        if temp != "":
            # print temp
            m =  re.search(r"(\d)*", temp)
            # print "hello"
            temp = temp[m.start() : m.end()]
            # print int(temp)
            due = datetime(due.year,due.month,int(temp),due.hour,due.minute)

    print due

## ------------------divide verb-------------------------------

    temp = sen
    if first > 0:
        temp = sen[:first]
        # print temp

    intend = {"want to","like to"," am going to","would like to"}
    for key in intend:
        if key in sen:
            m = re.search(key, sen)
            last = m.end()+1

    temp = temp[last:]
    print temp

