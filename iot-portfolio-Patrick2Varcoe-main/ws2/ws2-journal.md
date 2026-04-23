# UFCFVK-15-2 Internet of Things  - Worksheet 2 Journal

TODO: Add at least three entries on **three different dates** while working on your worksheet and detailing your progress:

Each entry should include:

* What you worked on that day (e.g., understanding HAL, creating sensor, display , etc)
* What issues did you face and how did you solve them? (e.g., debugging, researching, asking tutors for help, etc) Add links to any resources you found useful and as much detail as possible. Explore different approaches.
* What did you learn from the issues/solutions?
* After the second entry, reflect on how your approach to problem-solving has evolved and what you would do differently next time.

After creating one entry, commit and push your changes to your repository. **You should commit your journal entries no later than a week written.**

## 2026-04-15

* Initial work on WS2
To start on this project I essentially moved over all the relevant work I had done from WS1 and the preperation WS2 as a base to start working on this part of the project, modifiying them where necessary to fit this particular brief. And then modified the makefile to produce the correct amount of data for this part of the project.
I then changed the config file to use my designated port and updated the client program to include UDP server interaction and sending messages to the server.
Similar to WS1 my reasearch included online syntax reasearch and exmaples of things like converting strings to char arrays however this day mainly leaned on previous work, reading and understanding the provided utilites and libraries from their documentations.


## 2026-04-16

* Server Interaction improvement with threads.
I started off working on ensuring that both web and UDP servers could run simultaniously, to do this I reasearched and implemented the use of threads.
Then I worked on the client being able to recieve a response from the UDP server and providing each unique client with their own port.
Finally I worked on setting up a thread for each communication so that multiple clients contacting the server didnt interact and block one another.
In addition to the regular reasearch I was doing, this day included reasearch into threads and their use. To do this I mainly utilized stack overflow for other programmers examples and similar issues I ran into.
If I were to do this project again with hindsight I would of spent time reasearching ways of implementing these simultanious actions earlier and of implemented it from the start saving time overall. My reasearch on this day mainly changed focus from day one from the provided documentations and general cpp reasearch into more specific function reasearch.
## 2026-04-22

* 
On this day I was working on JSON formatting and message parsing as well as working on the new information provided in this project compared to WS1, the accelerations of the EBikes.
Json formatting is a new concept to me and as such I spent time researching and understanding the use and format of it, although I still ran into many errors along the way.
After formatting the regular transmissions of the ebikes and handling them, I moved onto producing and handling other types of messages. First I started with an intial message from the ebike client starting communication with the server and obtaining the requested data interval.
I then started work on the management client, reading JSON files, implementing the correct timestamp and sending these messages to the server to be handled.
Finsishing the day with completing both the COMMAND and STATUS management implementations and reimplementing the messagehandler object into threads to be able to track the changed data interval.
This days reasearch mainly relied on reasearch into JSON files, formatting and the use of the Poco library.